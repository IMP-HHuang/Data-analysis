//projection.h
#pragma once
#ifndef projection_H
#define projection_H

#include "TGWindow.h"
#include "TGFrame.h"
#include "TGTextEntry.h"
#include "TGNumberEntry.h"
#include "TGText.h"
#include "TRootEmbeddedCanvas.h"
#include "TGraph.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TPad.h"
#include "TLatex.h"
#include "TSpectrum.h"
#include "TPolyMarker.h"
#include "map"
#include "vector"
#include <RQ_OBJECT.h>

#include "gate.h"

//class projection : public TGTransientFrame
class projection : public TGMainFrame
{
	RQ_OBJECT("projection")
	ClassDef (projection, 0);
	public:
	//	projection(const TGWindow * p, const TGWindow * main, char *name, TH2D *h2);
		projection(const TGWindow * p, char *name, TH2D *h2);
		virtual ~projection();
		Bool_t ProcessMessage(Long_t msg,Long_t parm1,Long_t parm2);
		bool Click(Int_t event, Int_t px, Int_t py, TObject* selected);
		Bool_t DrawProj();
		Bool_t FindPeak();
		Bool_t Peaks(TH1D *h, Double_t thres, Int_t Smooth, bool bs, TCanvas *DrawCanvas);
		bool SubtractBackground(TH2D* hgg, Int_t Smooth, Double_t dd);

	private:
		TGVerticalFrame* DrawFrame;
		TRootEmbeddedCanvas *FrameCanvas;
		TGNumberEntry *ThreNumber;
		TGNumberEntry *GateThreNumber;
		TGNumberEntry *SmoothNumber;
		TGCheckButton *BackSubButton;

		TGTextButton *GateB;
		TGCheckButton *PickButton;
		TGCheckButton *BackSub2DButton;
		TGNumberEntry *BackGNumber;

		TH2D *h2, *h2b, h2nb;
		TH1D *hp, *hb, hnb, *hg;
		std::vector<TPad*> vp;
		std::vector<TH1D*> vhg;

		Double_t xGateMin, xGateMax;
		bool bxGateMin, bxGateMax, bGate;

		TGraph *line;	
		gate *Gate;
		enum Commands
		{FindPeakButton, GateButton};

};
#endif
