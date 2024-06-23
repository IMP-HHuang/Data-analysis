//gate.h
#pragma once
#ifndef gate_H
#define gate_H

#include "TGWindow.h"
#include "TGFrame.h"
#include "TGTextEntry.h"
#include "TGNumberEntry.h"
#include "TGText.h"
#include "TRootEmbeddedCanvas.h"
#include "TGraph.h"
#include "TF1.h"
#include "TPad.h"
#include "TLatex.h"
#include "TGImageMap.h"
#include "TH1D.h"
#include "TSpectrum.h"
#include "TPolyMarker.h"
#include "TStyle.h"
#include "vector"


class gate : public TGTransientFrame
{
	public:
		gate(const TGWindow * p, char *name, int ng);
		virtual ~gate();
		Bool_t ProcessMessage(Long_t msg,Long_t parm1,Long_t parm2);
		bool Draw(std::vector<TH1D*> vhg, Double_t thre);
		Bool_t Peaks(TH1D *h, Double_t thres, Int_t Smooth, bool bs, int nc);
	private:
		TRootEmbeddedCanvas *FrameCanvas;
		TH1D *hb, hnb;
        TLatex *tex;

};
#endif
