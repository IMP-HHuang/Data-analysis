//spe.h
#pragma once
#ifndef Spe_H
#define Spe_H

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


class spe : public TGTransientFrame
{
	public:
		spe(const TGWindow * p, const TGWindow * main, char *name, TString Source);
		virtual ~spe();
		Bool_t ProcessMessage(Long_t msg,Long_t parm1,Long_t parm2);
	private:
		TGVerticalFrame* DrawFrame;
		TString Source;

};
#endif
