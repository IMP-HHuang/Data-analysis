//gate.cpp

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

#include "gate.h"
#include "iostream"

gate::gate(const TGWindow * p, char *name, int ng)
{
	SetCleanup(kDeepCleanup);
	SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));
	FrameCanvas = new TRootEmbeddedCanvas("Draw", this, 1000, 300 * ng);
	AddFrame(FrameCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY));
	Int_t wid = FrameCanvas->GetCanvasWindowId();
	TCanvas *can  = new TCanvas("Canvas", 100, 400, wid);
	can->SetGridy(1);
	FrameCanvas->AdoptCanvas(can);

	MapSubwindows();
	Resize();
	CenterOnParent();
	SetWindowName(name);
	MapWindow();

}
gate::~gate(){}

Bool_t gate::ProcessMessage(Long_t msg,Long_t parm1, Long_t parm2)
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

bool gate::Draw(std::vector<TH1D*> vhg, Double_t thres)
{
	TCanvas *DrawCanvas = FrameCanvas->GetCanvas();
	DrawCanvas->Clear();
	DrawCanvas->Divide(1, Int_t(vhg.size()), 0.15, 0.0);
	for(int i=0; i<int(vhg.size()); i++)
	{
		TPad *pad = new TPad("pad", "pad", 0.0, 0.0, 1.0, 1.0);
		Peaks(vhg.at(i), thres, 80, false, pad);
		DrawCanvas->cd(i+1);
		pad->SetTopMargin(0.0);
		pad->SetBottomMargin(0.0);
		pad->SetLeftMargin(0.1);
		pad->SetRightMargin(0.01);
		pad->SetFillStyle(0);
		if(i == 0)
			pad->SetTopMargin(0.05);
		if(i == int(vhg.size())-1)
		pad->SetBottomMargin(0.1);
		pad->Draw();
//		vhg.at(i)->Draw();
	}
	DrawCanvas->Update();
	return true;
}

Bool_t gate::Peaks(TH1D *h, Double_t thres, Int_t Smooth, bool bs, TPad *pad)
{
	if(h == NULL)
	{
		std::cout << "\033[31m Cannot get TH1D in peaks\033[0m" << std::endl;
		return false;
	}

	Int_t nfound;
	Double_t *xpeaks=NULL, *ypeaks=NULL;
	TSpectrum *s = NULL;
	pad->Clear();
	h->SetTitle(Form("%s;Energy (keV);Couts", h->GetTitle()));
	h->GetYaxis()->SetTitleOffset(1.0);
	h->GetYaxis()->SetLabelOffset(0.015);
	if(!s) 
			s = new TSpectrum(500);
	TPolyMarker *pm;
	if(bs) 
	{
		hb = (TH1D*) s->Background(h, Smooth, "same"); //80-本底光滑程度
		hnb = *h;
		hnb.Add(h, hb, 1,-1);   
    	hnb.Sumw2(0); //cancal error
		pad->cd();
		hnb.Draw("HIST");
		nfound=s->Search(&hnb, 2, "", thres); 
		pm = (TPolyMarker *) hnb.GetListOfFunctions()->FindObject("TPolyMarker");
	}
	else
	{
		pad->cd();
    	h->Sumw2(0);
		(*h).Draw("HIST");
		nfound=s->Search(h, 2, "", thres); 
		pm = (TPolyMarker *) h->GetListOfFunctions()->FindObject("TPolyMarker");
	}

	pm->SetMarkerStyle(32);
	pm->SetMarkerColor(kGreen);
	pm->SetMarkerSize(0.4);
	xpeaks = s->GetPositionX();
	ypeaks = s->GetPositionY();
	TString sout;
	for(int j=0;j<nfound;j++) 
	{
		TLatex *tex = new TLatex(xpeaks[j], ypeaks[j], Form("%0.1f", xpeaks[j]));
		tex->SetTextFont(13);
		tex->SetTextSize(18);
		tex->SetTextAlign(12);
		tex->SetTextAngle(90);
		tex->SetTextColor(kRed);
		pad->cd();
		tex->Draw();
	}

	return true;
}
