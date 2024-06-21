//draw.cpp

#include "TColor.h"
#include "TLatex.h"
#include "TCanvas.h"
#include "TGSplitter.h"
#include "TG3DLine.h"
#include "TGraphErrors.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TLegend.h"

#include <vector>

#include "draw.h"
#include "iostream"

draw::draw(const TGWindow * p, const TGWindow * main, char *name, TString Fun_)
{
	Fun = Fun_;
	SetCleanup(kDeepCleanup);
	SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));

	DrawFrame = new TGVerticalFrame(this, 1000, 600);
	AddFrame(DrawFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 10, 10, 5, 5));
	DrawFrame->SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));
	TGHSplitter *hsplitter0 = new TGHSplitter(DrawFrame, 800,2);
	DrawFrame->AddFrame(hsplitter0, new TGLayoutHints(kLHintsTop | kLHintsExpandX,0,0,10,10));


	FrameCanvas = new TRootEmbeddedCanvas("Draw", DrawFrame, 1000, 600);
	DrawFrame->AddFrame(FrameCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
	Int_t wid = FrameCanvas->GetCanvasWindowId();
	TCanvas *can  = new TCanvas("Canvas", 100, 400, wid);
	can->SetGridy(1);
	FrameCanvas->AdoptCanvas(can);
	

	MapSubwindows();
	Resize();
	CenterOnParent();
	SetWindowName(name);
	MapWindow();

	g = new TGraph();
	g->SetTitle(";Raw;Energy (keV)");
	g->GetXaxis()->CenterTitle();
	g->GetYaxis()->CenterTitle();
	g->GetXaxis()->SetTitleFont(22);
	g->GetYaxis()->SetTitleFont(22);
	g->GetXaxis()->SetLabelFont(22);
	g->GetYaxis()->SetLabelFont(22);
	gR = new TGraph();
	gR->SetTitle(";Residual (keV);Energy (keV)");
	gR->GetXaxis()->CenterTitle();
	gR->GetYaxis()->CenterTitle();
	gR->GetXaxis()->SetTitleFont(22);
	gR->GetYaxis()->SetTitleFont(22);
	gR->GetXaxis()->SetLabelFont(22);
	gR->GetYaxis()->SetLabelFont(22);
}
draw::~draw(){}

Bool_t draw::ProcessMessage(Long_t msg,Long_t parm1, Long_t parm2)
{
	switch (GET_MSG(msg))
	{
		case kC_COMMAND:
			switch (GET_SUBMSG(msg))
			{
				case kCM_BUTTON:
					switch (parm1)
					{
						default:
							break;
					}
				default:
					break;
			}
		default:
			break;
	}
	return kTRUE;
}

Bool_t draw::Cali(Int_t nN, Double_t *Raw, Double_t *Ene, Bool_t *Enable)
{
	Double_t Emin = 9.0E9;
	Double_t Emax = -9.0E9;
	Double_t Rmin = 9.0E9;
	Double_t Rmax = -9.0E9;
	g->Clear();
	for(int i=0; i<nN; i++)
	{
			if(!Enable[i])  continue;
			g->SetPoint(g->GetN(), Raw[i], Ene[i]);
			if(Ene[i] < Emin)
					Emin = Ene[i];
			if(Ene[i] > Emax)
					Emax = Ene[i];
	}
	Double_t a0, a1, a2;
	if(Fun == "pol1")
	{
		g->Fit("pol1");
		TF1 *f = g->GetFunction("pol1") ;
		a0 = f->GetParameter(0);
		a1 = f->GetParameter(1);
	}
	else if (Fun == "pol2")
	{
		g->Fit("pol2");
		TF1 *f = g->GetFunction("pol2") ;
		a0 = f->GetParameter(0);
		a1 = f->GetParameter(1);
		a2 = f->GetParameter(2);
	}
	else
	{
		std::cout << "\033[31m plz check the function \033[0m" << std::endl;
		return false;
	}
	Double_t RvBar = 0.0;
	Double_t np = 0.0;
	for(int i=0; i<nN; i++)
	{
			if(!Enable[i])  continue;
			Double_t Rv;
			if(Fun == "pol1")
				Rv = Ene[i] - (a0 + a1 * Raw[i]);
			else if (Fun = "pol2")
				Rv = Ene[i] - (a0 + a1 * Raw[i] + a2 * pow(Raw[i], 2.0));
			gR->SetPoint(gR->GetN(), Rv, Ene[i]);
			if(Rv < Rmin)
					Rmin = Rv;
			if(Rv > Rmax)
					Rmax = Rv;
			np += 1.0;
			RvBar += Rv * Rv;
	}
	RvBar = sqrt(RvBar/np);
	TPad *p1 = new TPad("p1", "p1", 0.0, 0.0, 1.0, 1.0);
	p1->SetTopMargin(0.05);
	p1->SetRightMargin(0.0);
	p1->SetLeftMargin(0.1);
	p1->cd();
	g->GetYaxis()->SetRangeUser(Emin - (Emax - Emin) * 0.1, Emax + (Emax - Emin) * 0.1);
	TLatex *tex;
	if(Fun == "pol1")
   		tex = new TLatex(g->GetXaxis()->GetXmin() + (g->GetXaxis()->GetXmax() - g->GetXaxis()->GetXmin()) * 0.05, Emax, Form("#splitline{a0 = %0.3f}{a1 = %0.3f}", a0, a1));
	else if(Fun == "pol2")
   		tex = new TLatex(g->GetXaxis()->GetXmin() + (g->GetXaxis()->GetXmax() - g->GetXaxis()->GetXmin()) * 0.05, Emax, Form("#splitline{a0 = %0.3f}{#splitline{a1 = %0.3f}{a2 = %0.3f}}", a0, a1, a2));
	tex->SetTextFont(22);
	g->Draw("AP*");
	tex->Draw();
	TPad *p2 = new TPad("p2", "p2", 0.0, 0.0, 1.0, 1.0);
	p2->cd();
	p2->SetLeftMargin(0.0);
	p2->SetRightMargin(0.01);
	p2->SetTopMargin(0.05);
	gR->GetYaxis()->SetRangeUser(Emin - (Emax - Emin) * 0.1, Emax + (Emax - Emin) * 0.1);
	gR->GetXaxis()->SetRangeUser(Rmin - (Rmax - Rmin) * 0.1, Rmax + (Rmax - Rmin) * 0.1);
	tex = new TLatex(Rmin, Emax, Form("#sigma = %0.3f", RvBar));
	tex->SetTextFont(22);
	gR->Draw("AP*");
	tex->Draw();
	TCanvas *DrawCanvas = FrameCanvas->GetCanvas();
	DrawCanvas->Clear();
	DrawCanvas->Divide(2, 1, 0.0, 0.15);
	DrawCanvas->cd(1);
	p1->Draw();
	DrawCanvas->cd(2);
	p2->Draw();
	DrawCanvas->Update();
	return true;
}
