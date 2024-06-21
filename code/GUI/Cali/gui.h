
#pragma once
#include "TGButton.h"
#include "TGMenu.h"
#include "TColor.h"
#include "TGImageMap.h"
#include "TGFrame.h"
#include "TGComboBox.h"
#include "TGNumberEntry.h"
#include "TGSplitter.h"
#include "TString.h"
#include "TFile.h"
#include "TH1D.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TGFileDialog.h"
//#include "TGFileInfo.h"
#include <RQ_OBJECT.h>


#define INITIAL_SIDE_WIDTH 0
#define INITIAL_CONTANT_WIDTH 970
#define INITIAL_HIGHT 680
#define INITIAL_WIDTH (INITIAL_CONTANT_WIDTH+2*INITIAL_SIDE_WIDTH)

enum Commands
{
	FILE_EXIT, 
	Spectrum152Eu133Ba,
	DrawButton,
	FitButton,
	CaliButton,
	LoadButton
};


class gui : public TGMainFrame  //#include "TGFrame.h"
{
		RQ_OBJECT("gui")
	ClassDef (gui,0);
	public:
		gui(const TGWindow * p);
		virtual ~gui();
		bool Draw();
		bool ClickedGet(Int_t event, Int_t px, Int_t py, TObject* selected);
		bool Cali();
		bool CheckEnableButton();
		bool SetRawNumber();
		bool SetEneNumber();
		bool LoadFile();
	private:
		void MainFrameCrate();
		void CreateMenuBar(void);
		void ControlPanel(TGCompositeFrame *TabPanel);
		virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);

		TGPopupMenu *MenuFile;
		TGNumberEntry *ParNumber;
		TGTextEntry *ParText;
		TGTextEntry *ipfNameText;
		TGTextEntry *HistNameText;
		TGCheckButton *FixButton;
		TGTextButton *FitB;
		TGTextEntry *FunText;

	
		TRootEmbeddedCanvas *FrameCanvas;
		TCanvas *can;

		TFile *ipf;
		TH1D *iph;
		Double_t xFitMin, xFitMax;
		bool bxFitMin, bxFitMax, bFit;
		const static Int_t nNumber = 16;
		TGNumberEntry *RawNumber[nNumber];
		TGNumberEntry *EneNumber[nNumber];
		Double_t Raw[nNumber] = {80.9979, 121.7817, 244.6974, 276.3989, 302.8508, 344.28, 356.0129, 383.8485, 411.12, 443.9609, 778.9, 867.380, 964.057, 1085.837, 1112.076, 1408.013};
		Double_t Ene[nNumber] = {80.9979, 121.7817, 244.6974, 276.3989, 302.8508, 344.28, 356.0129, 383.8485, 411.12, 443.9609, 778.9, 867.380, 964.057, 1085.837, 1112.076, 1408.013};
		bool Enable[nNumber];
		TGCheckButton *EnableButton[nNumber];
};
