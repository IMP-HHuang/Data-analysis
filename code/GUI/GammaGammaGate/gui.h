
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
#include "TH2D.h"
#include "TStyle.h"
#include "TRootEmbeddedCanvas.h"
#include "TCanvas.h"
#include "TGFileDialog.h"
#include <RQ_OBJECT.h>


#define INITIAL_SIDE_WIDTH 0
#define INITIAL_CONTANT_WIDTH 980
#define INITIAL_HIGHT 680
#define INITIAL_WIDTH (INITIAL_CONTANT_WIDTH+2*INITIAL_SIDE_WIDTH)



class gui : public TGMainFrame  //#include "TGFrame.h"
{
		RQ_OBJECT("gui")
	ClassDef (gui,0);
	public:
		gui(const TGWindow * p);
		virtual ~gui();
		bool Draw();
		bool LoadFile();
	private:
		void MainFrameCrate();
		void CreateMenuBar(void);
		void ControlPanel(TGCompositeFrame *TabPanel);
		virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);

        TGMenuBar *MenuBar;
		TGPopupMenu *MenuFile;
	    TGGroupFrame *MainFrame;
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
		TH2D *h2;
		enum Commands
		{FILE_EXIT, DrawButton, LoadButton, ProjButton};
};
