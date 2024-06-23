#include "gui.h"

#include <iostream>
#include <string.h>
#include "projection.h"

ClassImp(gui);

gui::gui(const TGWindow * p)
{
	MainFrameCrate();
	ipf = NULL;
	h2 = NULL;
}

gui::~gui()
{
	MainFrame->Delete();
    MenuFile->Delete();
	MenuBar->Delete();
}

void gui::MainFrameCrate()
{
	SetCleanup(kDeepCleanup);
	SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));
	CreateMenuBar();
	ControlPanel(this);
	SetWindowName("Gate test");
	MapSubwindows();
	MapWindow();
	Resize(INITIAL_WIDTH, INITIAL_HIGHT);
}
void gui::CreateMenuBar(void)
{
	MenuBar = new TGMenuBar(this, INITIAL_WIDTH, 24, kHorizontalFrame);
	AddFrame(MenuBar, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 0, 0, 0, 0));

	MenuFile = new TGPopupMenu(fClient->GetRoot());
	MenuFile->AddEntry("&Exit", FILE_EXIT,0,gClient->GetPicture("bld_exit.png"));
	MenuFile->Associate(this);
	MenuFile->SetBackgroundColor(TColor::RGB2Pixel(0,128,0));
	MenuBar->AddPopup(" &File  ", MenuFile, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

}

void gui::ControlPanel(TGCompositeFrame *TabPanel)
{
	MainFrame = new TGGroupFrame(TabPanel, "");
	MainFrame->SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));
	TabPanel->AddFrame(MainFrame, new TGLayoutHints(kLHintsExpandX|kLHintsTop, INITIAL_SIDE_WIDTH, INITIAL_SIDE_WIDTH, 0, 0));
	MainFrame->SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));

	TGHorizontalFrame *ParFrame = new TGHorizontalFrame(MainFrame, 980, 40);
	MainFrame->AddFrame(ParFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
	ParFrame->SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));

	TGTextEntry *ParNameText;
	ParNameText = new TGTextEntry(ParFrame, new TGTextBuffer(100), 10000);
	ParNameText->SetText("File Name : ");
	ParFrame->AddFrame(ParNameText, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 0, 0));
	ParNameText->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	ParNameText->SetAlignment(kTextCenterX);
	ParNameText->Resize(120, 40);
	ParNameText->SetEnabled(kFALSE);
	ParNameText->SetFrameDrawn(kFALSE);
	ParNameText->ChangeBackground(TColor::RGB2Pixel(62, 62, 62));
	ParNameText->SetTextColor(TColor::RGB2Pixel(131, 131, 131), false);

	ipfNameText = new TGTextEntry(ParFrame, new TGTextBuffer(100), 10000);
	ipfNameText->SetText("Clover_g-g_matrix.root");
	ipfNameText->SetEditable(kTRUE);
	ParFrame->AddFrame(ipfNameText, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 0, 0));
	ipfNameText->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	ipfNameText->SetAlignment(kTextLeft);
	ipfNameText->Resize(150, 40);
	ipfNameText->SetEnabled(kTRUE);  //Editable
	ipfNameText->SetFrameDrawn(kFALSE);
	ipfNameText->Associate(this);

	TGTextButton *LoadB = new TGTextButton(ParFrame, "  Load  ", LoadButton);
	ParFrame->AddFrame(LoadB, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 30, 0, 0, 0));
	LoadB->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	LoadB->Resize(40, 40);
	LoadB->SetTextColor(TColor::RGB2Pixel(255, 255, 255));
	LoadB->SetBackgroundColor(TColor::RGB2Pixel(34, 151, 159));
	LoadB->Associate(this);

	ParNameText = new TGTextEntry(ParFrame, new TGTextBuffer(100), 10000);
	ParNameText->SetText("Histogram Name : ");
	ParFrame->AddFrame(ParNameText, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 0, 0));
	ParNameText->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	ParNameText->SetAlignment(kTextCenterX);
	ParNameText->Resize(200, 40);
	ParNameText->SetEnabled(kFALSE);
	ParNameText->SetFrameDrawn(kFALSE);
	ParNameText->ChangeBackground(TColor::RGB2Pixel(62, 62, 62));
	ParNameText->SetTextColor(TColor::RGB2Pixel(131, 131, 131), false);

	HistNameText = new TGTextEntry(ParFrame, new TGTextBuffer(100), 10000);
	HistNameText->SetText("hggAll");
	HistNameText->SetEditable(kTRUE);
	ParFrame->AddFrame(HistNameText, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 0, 0));
	HistNameText->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	HistNameText->SetAlignment(kTextLeft);
	HistNameText->Resize(100, 40);
	HistNameText->SetEnabled(kTRUE);  //Editable
	HistNameText->SetFrameDrawn(kFALSE);
	HistNameText->Associate(this);

	TGTextButton *DrawB = new TGTextButton(ParFrame, "  Draw  ", DrawButton);
	ParFrame->AddFrame(DrawB, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 0, 0, 0));
	DrawB->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	DrawB->Resize(40, 40);
	DrawB->SetTextColor(TColor::RGB2Pixel(255, 255, 255));
	DrawB->SetBackgroundColor(TColor::RGB2Pixel(34, 151, 159));
	DrawB->Associate(this);

	TGTextButton *ProjB = new TGTextButton(ParFrame, "Projection", ProjButton);
	ParFrame->AddFrame(ProjB, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 10, 0, 0, 0));
	ProjB->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	ProjB->Resize(60, 40);
	ProjB->SetTextColor(TColor::RGB2Pixel(255, 255, 255));
	ProjB->SetBackgroundColor(TColor::RGB2Pixel(34, 151, 159));
	ProjB->Associate(this);

	TGHSplitter *hsplitter = new TGHSplitter(MainFrame, 940,2);
	MainFrame->AddFrame(hsplitter, new TGLayoutHints(kLHintsTop | kLHintsExpandX,0,0,10,10));
	hsplitter->SetBackgroundColor(TColor::RGB2Pixel(0,0,0));

	TGHorizontalFrame *DrawFrame = new TGHorizontalFrame(MainFrame, 900, 600);
	MainFrame->AddFrame(DrawFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
	DrawFrame->SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));


	FrameCanvas = new TRootEmbeddedCanvas("h", DrawFrame, 900, 580);
	DrawFrame->AddFrame(FrameCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
	Int_t wid = FrameCanvas->GetCanvasWindowId();
//	TCanvas *can  = new TCanvas("Canvas", 100, 400, wid);
	can  = new TCanvas("Canvas", 100, 600, wid);
	can->SetGridy(1);
	FrameCanvas->AdoptCanvas(can);
//	FrameCanvas->GetCanvas()->Connect("ProcessedEvent(Int_t, Int_t, Int_t, TObject*)", "gui", this, "ClickedGet(Int_t, Int_t, Int_t, TObject*)");

}

Bool_t gui::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
	int ParMsg = 0;
	int ParSubMsg = 0;
	switch (ParMsg = GET_MSG(msg))
	{
		case  kC_HSLIDER:
			switch (ParSubMsg = GET_SUBMSG(msg))
			{
				case  kSL_POS:
					switch(parm1);
					{
					}
					break;
				default:
					break;
			}
			break;
		case kC_COMMAND:
			switch (ParSubMsg = GET_SUBMSG(msg))
			{
				case kCM_MENU:
					switch (parm1)
					{
						case FILE_EXIT:
							CloseWindow();
							break;
						default:
							break;
					}
				case kCM_BUTTON: //Button
					switch (parm1)
					{
						case DrawButton:
						{
							Draw();
							break;
						}
						case LoadButton:
						{
							LoadFile();
							break;
						}
						case ProjButton:
						{
								if(h2 == NULL)
									std::cout << "\033[31m Cannot Get g-g histgram \033[0m" << std::endl;
								else
								{
									projection *Proj = new projection(fClient->GetRoot(), (char*)"Gate", h2);
									Proj->DrawProj();
								}
								break;
						}
						default:
							break;
					}
				default:
					break;
			}
		case kC_TEXTENTRY:
			switch (parm1)
			{
				default:
					break;
			}
		default:
				break;
	}
	return kTRUE;
}

bool gui::LoadFile()
{
	static const char *filetypes[] = { "ROOT files", "*.root", 0, 0 };
    TGFileInfo fileInfo;
    fileInfo.fFileTypes = filetypes;
	new TGFileDialog(gClient->GetRoot(), this, kFDOpen, &fileInfo);

	if(ipf != NULL)
	{
		ipf->Close();
//		delete ipf;
	}
    if (fileInfo.fFilename != nullptr) 
	{
        ipf = TFile::Open(fileInfo.fFilename);
	}
	if(ipf != NULL)
	{
		ipfNameText->SetText(ipf->GetName());
	}
	else
	{
		std::cout << "\033[31m Cannot open root file" << std::endl;
		return false;
	}
	return true;
}

bool gui::Draw()
{
		if(ipf == NULL)
		{
				std::cout << "\033[31m Haven't opened the root file \033[0m" << std::endl;
				return false;
		}
      //  if(h2 != NULL)
        //    h2->Delete();

		TString hName = HistNameText->GetText();
		h2 = (TH2D*)ipf->Get(hName.Data()); 
		h2->SetStats(0000);
		h2->GetXaxis()->SetTitle("Energy (keV)");
		h2->GetXaxis()->CenterTitle();
		h2->GetYaxis()->SetTitle("Energy (keV)");
		h2->GetYaxis()->CenterTitle();
		h2->GetXaxis()->SetTitleFont(22);
		h2->GetYaxis()->SetTitleFont(22);
		h2->GetXaxis()->SetLabelFont(22);
		h2->GetYaxis()->SetLabelFont(22);
		TCanvas *DrawCanvas = FrameCanvas->GetCanvas();
		DrawCanvas->cd();
		DrawCanvas->SetLogz(1);
        gStyle->SetPalette(1);
		h2->Draw("colz");
		DrawCanvas->Update();
		return true;
}

