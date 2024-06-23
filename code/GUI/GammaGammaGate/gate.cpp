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
	FrameCanvas->AdoptCanvas(new TCanvas("Canvas", 100, 400, wid));
	TCanvas *DrawCanvas = FrameCanvas->GetCanvas();
	DrawCanvas->Clear();
	DrawCanvas->Divide(1, ng, 0.15, 0.0);
    DrawCanvas->SetGridy(1);

	MapSubwindows();
	Resize();
	CenterOnParent();
	SetWindowName(name);
	MapWindow();

    tex = new TLatex();
	tex->SetTextFont(13);
	tex->SetTextSize(18);
	tex->SetTextAlign(12);
	tex->SetTextAngle(90);
	tex->SetTextColor(kRed);
}
gate::~gate()
{
    FrameCanvas->Delete();
    tex->Delete();
}

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
	for(int i=0; i<int(vhg.size()); i++)
	{
        if(vhg.at(i)->GetEntries() <= 0)
        {
            std::cout << "\033[31m" << vhg.at(i)->GetTitle() << " is empty!\033[0m" << std::endl;
            continue;
        }
		Peaks(vhg.at(i), thres, 80, false, i+1);
	}
	TCanvas *DrawCanvas = FrameCanvas->GetCanvas();
	DrawCanvas->Update();
	return true;
}

Bool_t gate::Peaks(TH1D *h, Double_t thres, Int_t Smooth, bool bs, int nc)
{
	if(h == NULL)
	{
		std::cout << "\033[31m Cannot get TH1D in peaks\033[0m" << std::endl;
		return false;
	}

	Int_t nfound;
	Double_t *xpeaks=NULL, *ypeaks=NULL;
	TSpectrum *s = NULL;
	h->SetTitle(Form("%s;Energy (keV);Couts", h->GetTitle()));
	h->GetYaxis()->SetTitleOffset(1.0);
	h->GetYaxis()->SetLabelOffset(0.015);
	if(!s) 
			s = new TSpectrum(500);
	TPolyMarker *pm;
    pm = NULL;
	TCanvas *DrawCanvas = FrameCanvas->GetCanvas();
	DrawCanvas->cd(nc);
	if(bs) 
	{
		hb = (TH1D*) s->Background(h, Smooth, "same"); //80-本底光滑程度
		hnb = *h;
		hnb.Add(h, hb, 1,-1);   
    	hnb.Sumw2(0); //cancal error
	DrawCanvas->cd(nc);
		hnb.Draw("HIST");
		nfound = s->Search(&hnb, 2, "", thres); 
		pm = (TPolyMarker *) hnb.GetListOfFunctions()->FindObject("TPolyMarker");
	}
	else
	{
    	h->Sumw2(0);
	DrawCanvas->cd(nc);
		(*h).Draw("HIST");
		nfound = s->Search(h, 2, "", thres); 
		pm = (TPolyMarker *) h->GetListOfFunctions()->FindObject("TPolyMarker");
	}

    if(pm != NULL)
    {
	    pm->SetMarkerStyle(32);
	    pm->SetMarkerColor(kGreen);
	    pm->SetMarkerSize(0.4);
    }
	xpeaks = s->GetPositionX();
	ypeaks = s->GetPositionY();
	for(int j=0;j<nfound;j++) 
	{
		DrawCanvas->cd(nc);
        tex->DrawLatex(xpeaks[j], ypeaks[j], Form("%0.1f", xpeaks[j]));
	}
	if(!s) 
    s->Delete();

	return true;
}
