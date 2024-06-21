//draw.h
#pragma once
#ifndef DRAW_H
#define DRAW_H

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


class draw : public TGTransientFrame
{
	public:
		draw(const TGWindow * p, const TGWindow * main, char *name, TString Fun_);
		virtual ~draw();
		Bool_t ProcessMessage(Long_t msg,Long_t parm1,Long_t parm2);
		Bool_t Cali(Int_t nN, Double_t *Raw, Double_t *Ene, Bool_t *Enable);
	private:
		TGVerticalFrame* DrawFrame;
		TRootEmbeddedCanvas *FrameCanvas;
		TGraph *g;
		TGraph *gR;
		TString Fun;

};
#endif
