#include "gui.h"

#include <iostream>
#include <string.h>
#include "draw.h"
#include "spe.h"

ClassImp(gui);

gui::gui(const TGWindow * p)
{
	MainFrameCrate();
	ipf = NULL;
	bxFitMin = false;
	bxFitMax = false;
	bFit = false;
	for(int i=0; i<nNumber; i++)
			Enable[i] = true;
}

gui::~gui()
{}

void gui::MainFrameCrate()
{
	SetCleanup(kDeepCleanup);
	SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));
	CreateMenuBar();
	ControlPanel(this);
	SetWindowName("GUI test");
	MapSubwindows();
	MapWindow();
	Resize(INITIAL_WIDTH, INITIAL_HIGHT);
}
void gui::CreateMenuBar(void)
{
	TGMenuBar *MenuBar = new TGMenuBar(this, INITIAL_WIDTH, 24, kHorizontalFrame);
	AddFrame(MenuBar, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 0, 0, 0, 0));

	MenuFile = new TGPopupMenu(fClient->GetRoot());
	MenuFile->AddEntry("&Exit", FILE_EXIT,0,gClient->GetPicture("bld_exit.png"));
	MenuFile->Associate(this);
	MenuFile->SetBackgroundColor(TColor::RGB2Pixel(0,128,0));
	MenuBar->AddPopup(" &File  ", MenuFile, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));

	TGPopupMenu *MenuSpe = new TGPopupMenu(fClient->GetRoot());
	MenuSpe->AddEntry("&152Eu133Ba", Spectrum152Eu133Ba, 0, gClient->GetPicture("bld_exit.png"));
	MenuSpe->Associate(this);
	MenuSpe->SetBackgroundColor(TColor::RGB2Pixel(0,128,0));
	MenuBar->AddPopup(" &Spectra", MenuSpe, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
}

void gui::ControlPanel(TGCompositeFrame *TabPanel)
{
	TGGroupFrame *MainFrame = new TGGroupFrame(TabPanel, "");
	MainFrame->SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));
	TabPanel->AddFrame(MainFrame, new TGLayoutHints(kLHintsExpandX|kLHintsTop, INITIAL_SIDE_WIDTH, INITIAL_SIDE_WIDTH, 0, 0));
	MainFrame->SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));

	TGHorizontalFrame *ParFrame = new TGHorizontalFrame(MainFrame, 800, 40);
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
	ipfNameText->SetText("gammacali.root");
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
	HistNameText->SetText("g0");
	HistNameText->SetEditable(kTRUE);
	ParFrame->AddFrame(HistNameText, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 0, 0));
	HistNameText->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	HistNameText->SetAlignment(kTextLeft);
	HistNameText->Resize(100, 40);
	HistNameText->SetEnabled(kTRUE);  //Editable
	HistNameText->SetFrameDrawn(kFALSE);
	HistNameText->Associate(this);

	TGTextButton *DrawB = new TGTextButton(ParFrame, "  Draw  ", DrawButton);
	ParFrame->AddFrame(DrawB, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 30, 0, 0, 0));
	DrawB->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	DrawB->Resize(40, 40);
	DrawB->SetTextColor(TColor::RGB2Pixel(255, 255, 255));
	DrawB->SetBackgroundColor(TColor::RGB2Pixel(34, 151, 159));
	DrawB->Associate(this);

	FitB = new TGTextButton(ParFrame, "  Fit  ", FitButton);
	ParFrame->AddFrame(FitB, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 30, 0, 0, 0));
	FitB->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	FitB->Resize(40, 40);
	FitB->SetTextColor(TColor::RGB2Pixel(255, 255, 255));
	FitB->SetBackgroundColor(TColor::RGB2Pixel(34, 151, 159));
	FitB->Associate(this);
/*

*/

	TGHSplitter *hsplitter = new TGHSplitter(MainFrame, 800,2);
	MainFrame->AddFrame(hsplitter, new TGLayoutHints(kLHintsExpandX | kLHintsTop,0,0,0,0));
	hsplitter->SetBackgroundColor(TColor::RGB2Pixel(0,0,0));

	TGHorizontalFrame *DrawFrame = new TGHorizontalFrame(MainFrame, 800, 540);
	MainFrame->AddFrame(DrawFrame, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
	DrawFrame->SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));

	FrameCanvas = new TRootEmbeddedCanvas("h", DrawFrame, 600, 540);
	DrawFrame->AddFrame(FrameCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
	Int_t wid = FrameCanvas->GetCanvasWindowId();
//	TCanvas *can  = new TCanvas("Canvas", 100, 400, wid);
	can  = new TCanvas("Canvas", 100, 400, wid);
	can->SetGridy(1);
	FrameCanvas->AdoptCanvas(can);
	FrameCanvas->GetCanvas()->Connect("ProcessedEvent(Int_t, Int_t, Int_t, TObject*)", "gui", this, "ClickedGet(Int_t, Int_t, Int_t, TObject*)");

	TGVerticalFrame *CaliParFrame = new TGVerticalFrame(DrawFrame, 300, 570);
	DrawFrame->AddFrame(CaliParFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 10, 10, 5, 5));
	CaliParFrame->SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));

	for(int i=-1; i<nNumber+1; i++)
	{
		TGHorizontalFrame *CaliValueFrame = new TGHorizontalFrame(CaliParFrame, 370, 40);
		CaliValueFrame->SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));
		CaliParFrame->AddFrame(CaliValueFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
		if(i<0)
		{
			ParNameText = new TGTextEntry(CaliValueFrame, new TGTextBuffer(150), 10000);
			ParNameText->SetText("Raw");
			CaliValueFrame->AddFrame(ParNameText, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 0, 0));
			ParNameText->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
			ParNameText->SetAlignment(kTextCenterX);
			ParNameText->Resize(140, 40);
			ParNameText->SetEnabled(kFALSE);
			ParNameText->SetFrameDrawn(kFALSE);
			ParNameText->ChangeBackground(TColor::RGB2Pixel(62, 62, 62));
			ParNameText->SetTextColor(TColor::RGB2Pixel(131, 131, 131), false);

			ParNameText = new TGTextEntry(CaliValueFrame, new TGTextBuffer(150), 10000);
			ParNameText->SetText("Energy");
			CaliValueFrame->AddFrame(ParNameText, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 0, 0));
			ParNameText->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
			ParNameText->SetAlignment(kTextCenterX);
			ParNameText->Resize(140, 40);
			ParNameText->SetEnabled(kFALSE);
			ParNameText->SetFrameDrawn(kFALSE);
			ParNameText->ChangeBackground(TColor::RGB2Pixel(62, 62, 62));
			ParNameText->SetTextColor(TColor::RGB2Pixel(131, 131, 131), false);
			continue;
		}
		if(i==nNumber)
		{
			FunText = new TGTextEntry(CaliValueFrame, new TGTextBuffer(100), 10000);
			FunText->SetText("pol1");
			FunText->SetEditable(kTRUE);
			CaliValueFrame->AddFrame(FunText, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 5, 0, 4, 0));
			FunText->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
			FunText->SetAlignment(kTextLeft);
			FunText->Resize(50, 38);
			FunText->SetEnabled(kTRUE);  //Editable
			FunText->SetFrameDrawn(kFALSE);
			FunText->Associate(this);

			TGTextButton *CaliB = new TGTextButton(CaliValueFrame, "Calibration", CaliButton);
			CaliValueFrame->AddFrame(CaliB, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 45, 0, 0, 0));
			CaliB->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
			CaliB->Resize(40, 40);
			CaliB->SetTextColor(TColor::RGB2Pixel(255, 255, 255));
			CaliB->SetBackgroundColor(TColor::RGB2Pixel(34, 151, 159));
			CaliB->Associate(this);
				break;
		}

		RawNumber[i] = new TGNumberEntry(CaliValueFrame, 0, 0, TGNumberFormat::kNESInteger);
		CaliValueFrame->AddFrame(RawNumber[i], new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 0, 0, 0));
		RawNumber[i]->GetNumberEntry()->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
   		(RawNumber[i]->GetNumberEntry())->Connect("ReturnPressed()", "gui", this, "SetRawNumber()");
		RawNumber[i]->Resize(140, 30);
		RawNumber[i]->SetNumber(Raw[i]);
		RawNumber[i]->Associate(this);

		EneNumber[i] = new TGNumberEntry(CaliValueFrame, 0, 0, TGNumberFormat::kNESInteger);
		CaliValueFrame->AddFrame(EneNumber[i], new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 0, 0, 0, 0));
		EneNumber[i]->GetNumberEntry()->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
   		(EneNumber[i]->GetNumberEntry())->Connect("ReturnPressed()", "gui", this, "SetEneNumber()");
		EneNumber[i]->Resize(140, 30);
		EneNumber[i]->SetNumber(Ene[i]);
		EneNumber[i]->Associate(this);

		EnableButton[i] = new TGCheckButton(CaliValueFrame, "", i);
		EnableButton[i]->SetOn(kTRUE);
		ULong_t largeFont = gClient->GetFontByName("-*-helvetica-bold-r-*-*-30-*-*-*-*-*-*-*");
		EnableButton[i]->SetFont(largeFont, false);
		EnableButton[i]->Resize(70, 30);
		CaliValueFrame->AddFrame(EnableButton[i], new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 0, 0, 0, 0));
		EnableButton[i]->Connect("Toggled(Bool_t)", "gui", this, "CheckEnableButton()");
		EnableButton[i]->Associate(this);
	}
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
						case Spectrum152Eu133Ba:
						{
							spe *Spe = new spe(fClient->GetRoot(), this, (char*)"Spectrum", "152Eu133Ba");
							break;
						}
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
						case FitButton:
							{
									if(bFit)
									{
										bFit = false;
										FitB->SetTextColor(TColor::RGB2Pixel(255, 255, 255));
									}
									else
									{
										bFit = true;
										FitB->SetTextColor(TColor::RGB2Pixel(0, 255, 0));
									}
									break;
							}
						case CaliButton:
							{
									Cali();
									break;
							}
						case LoadButton:
							{
									LoadFile();
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
		delete ipf;
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

		TString hName = HistNameText->GetText();
		iph = (TH1D*)ipf->Get(hName.Data()); 
		iph->SetStats(0000);
		TCanvas *DrawCanvas = FrameCanvas->GetCanvas();
		DrawCanvas->cd();
		iph->Draw("hist");
		DrawCanvas->Update();
		return true;
}

bool gui::ClickedGet(Int_t event, Int_t px, Int_t py, TObject* selected)
{
	if (event != kButton1Down) return false;
	if(!bFit) return false;
    
    // 获取当前画布
	TCanvas *c = FrameCanvas->GetCanvas();
    
    // 将像素坐标转换为数据坐标
    Double_t x = c->AbsPixeltoX(px);
   // Double_t y = c->AbsPixeltoY(py);
	if(!bxFitMin || bxFitMax)
	{
			xFitMin = x;
			bxFitMin = true;
			bxFitMax = false;
	}
	else
	{
			xFitMax = x;
		bxFitMax = true;
	}
	if(bxFitMin && bxFitMax && iph != NULL)
	{
		iph->Fit("gaus", "R", "FUNC", xFitMin, xFitMax);
		iph->SetStats(0000);
		TCanvas *DrawCanvas = FrameCanvas->GetCanvas();
		DrawCanvas->Clear();
		DrawCanvas->cd();
		iph->Draw();
		DrawCanvas->Update();
	}
	return true;
}

bool gui::Cali()
{
	draw *Draw = new draw(fClient->GetRoot(), this, (char*)"Cali", FunText->GetText());
	Draw->Cali(nNumber, Raw, Ene, Enable);
	return true;
}

bool gui::CheckEnableButton()
{
	for(int i=0; i<nNumber; i++)
	{
		if(EnableButton[i]->IsOn())
				Enable[i] = true;
		else
				Enable[i] = false;
	}
	return true;
}

bool gui::SetRawNumber()
{
	for(int i=0; i<nNumber; i++)
			Raw[i] = RawNumber[i]->GetNumber();
	return true;
}

bool gui::SetEneNumber()
{
	for(int i=0; i<nNumber; i++)
			Ene[i] = EneNumber[i]->GetNumber();
	return true;
}
