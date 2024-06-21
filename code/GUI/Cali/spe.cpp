//spe.cpp

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

#include "spe.h"
#include "iostream"

spe::spe(const TGWindow * p, const TGWindow * main, char *name, TString Source_)
{
	Source = Source_;
	SetCleanup(kDeepCleanup);
	SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));

	DrawFrame = new TGVerticalFrame(this, 700, 600);
	AddFrame(DrawFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 10, 10, 5, 5));
	DrawFrame->SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));
	TGHSplitter *hsplitter0 = new TGHSplitter(DrawFrame, 700, 2);
	DrawFrame->AddFrame(hsplitter0, new TGLayoutHints(kLHintsTop | kLHintsExpandX,0,0,10,10));

	TGImageMap* fImage;
	if(Source == "152Eu133Ba")
    	fImage = new TGImageMap(DrawFrame, "./data/152Eu133Ba.png");
	else
		std::cout << "\033[31mCannot open spectrum\033[0m" << std::endl;
	if(fImage != NULL)
	{
//		fImage->Resize(700, 600);
		DrawFrame->AddFrame(fImage,new TGLayoutHints(kLHintsLeft | kLHintsTop, 0, 0, 0, 0));
		fImage->ChangeOptions(fImage->GetOptions() ^ kRaisedFrame);
		fImage->ChangeOptions(fImage->GetOptions() | kFixedSize);
	}

	MapSubwindows();
	Resize();
	CenterOnParent();
	SetWindowName(name);
	MapWindow();

}
spe::~spe(){}

Bool_t spe::ProcessMessage(Long_t msg,Long_t parm1, Long_t parm2)
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


