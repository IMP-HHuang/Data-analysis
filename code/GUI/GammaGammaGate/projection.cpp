//projection.cpp

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
#include "projection.h"
#include "iostream"
#include "gate.h"

//ClassImp(projection);

//projection::projection(const TGWindow * p, const TGWindow * main, char *name, TH2D *h2_)
projection::projection(const TGWindow * p, char *name, TH2D *h2_)
{
	h2 = h2_;
	bGate = false;
	bxGateMin = false;
	bxGateMax = false;
	line = new TGraph;	
	vp.clear();
	vhg.clear();

	SetCleanup(kDeepCleanup);
	SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));

	DrawFrame = new TGVerticalFrame(this, 1000, 600);
	AddFrame(DrawFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 10, 10, 5, 5));
	DrawFrame->SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));

	TGHorizontalFrame *ParFrame = new TGHorizontalFrame(DrawFrame, 1000, 40);
	DrawFrame->AddFrame(ParFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 0, 0, 0, 0));
	ParFrame->SetBackgroundColor(TColor::RGB2Pixel(62, 62, 62));


	TGTextEntry *ParNameText = new TGTextEntry(ParFrame, new TGTextBuffer(100), 10000);
	ParNameText->SetText("Threshold");
	ParFrame->AddFrame(ParNameText, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 0, 0, 0, 0));
	ParNameText->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	ParNameText->SetAlignment(kTextCenterX);
	ParNameText->Resize(100, 40);
	ParNameText->SetEnabled(kFALSE);
	ParNameText->SetFrameDrawn(kFALSE);
	ParNameText->ChangeBackground(TColor::RGB2Pixel(62, 62, 62));
	ParNameText->SetTextColor(TColor::RGB2Pixel(131, 131, 131), false);

	ThreNumber = new TGNumberEntry(ParFrame, 0, 0, TGNumberFormat::kNESInteger);
	ParFrame->AddFrame(ThreNumber, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 10, 0, 0, 0));
	ThreNumber->GetNumberEntry()->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	ThreNumber->Resize(80, 30);
	ThreNumber->SetNumber(0.05);
	ThreNumber->Associate(this);

	SmoothNumber = new TGNumberEntry(ParFrame, 0, 0, TGNumberFormat::kNESInteger);
	ParFrame->AddFrame(SmoothNumber, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 10, 0, 0, 0));
	SmoothNumber->GetNumberEntry()->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	SmoothNumber->Resize(80, 30);
	SmoothNumber->SetNumber(80);
	SmoothNumber->Associate(this);

	BackSubButton = new TGCheckButton(ParFrame, "Background", 1);
	BackSubButton->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	BackSubButton->SetOn(kFALSE);
	BackSubButton->Resize(70, 30);
	ParFrame->AddFrame(BackSubButton, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 0, 0, 0, 0));
	BackSubButton->Associate(this);

	TGTextButton *FindPeakB = new TGTextButton(ParFrame, " Peaks ", FindPeakButton);
	ParFrame->AddFrame(FindPeakB, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 20, 0, 0, 0));
	FindPeakB->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	FindPeakB->Resize(40, 40);
	FindPeakB->SetTextColor(TColor::RGB2Pixel(255, 255, 255));
	FindPeakB->SetBackgroundColor(TColor::RGB2Pixel(34, 151, 159));
	FindPeakB->Associate(this);

	GateB = new TGTextButton(ParFrame, "  Gate  ", GateButton);
	ParFrame->AddFrame(GateB, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 20, 0, 0, 0));
	GateB->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	GateB->Resize(40, 40);
	GateB->SetTextColor(TColor::RGB2Pixel(255, 255, 255));
	GateB->SetBackgroundColor(TColor::RGB2Pixel(34, 151, 159));
	GateB->Associate(this);

	PickButton = new TGCheckButton(ParFrame, "Pick", 1);
	PickButton->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	PickButton->SetOn(kTRUE);
	PickButton->Resize(70, 30);
	ParFrame->AddFrame(PickButton, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 5, 0, 0, 0));
	PickButton->Associate(this);

	ParNameText = new TGTextEntry(ParFrame, new TGTextBuffer(100), 10000);
	ParNameText->SetText("Threshold");
	ParFrame->AddFrame(ParNameText, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY, 5, 0, 0, 0));
	ParNameText->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	ParNameText->SetAlignment(kTextCenterX);
	ParNameText->Resize(100, 40);
	ParNameText->SetEnabled(kFALSE);
	ParNameText->SetFrameDrawn(kFALSE);
	ParNameText->ChangeBackground(TColor::RGB2Pixel(62, 62, 62));
	ParNameText->SetTextColor(TColor::RGB2Pixel(131, 131, 131), false);

	GateThreNumber = new TGNumberEntry(ParFrame, 0, 0, TGNumberFormat::kNESInteger);
	ParFrame->AddFrame(GateThreNumber, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 5, 0, 0, 0));
	GateThreNumber->GetNumberEntry()->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	GateThreNumber->Resize(80, 30);
	GateThreNumber->SetNumber(0.2);
	GateThreNumber->Associate(this);

	BackSub2DButton = new TGCheckButton(ParFrame, "2DBackground", 2);
	BackSub2DButton->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	BackSub2DButton->SetOn(kTRUE);
	BackSub2DButton->Resize(70, 30);
	ParFrame->AddFrame(BackSub2DButton, new TGLayoutHints(kLHintsLeft | kLHintsExpandY, 5, 0, 0, 0));
	BackSub2DButton->Associate(this);

	BackGNumber = new TGNumberEntry(ParFrame, 0, 0, TGNumberFormat::kNESInteger);
	ParFrame->AddFrame(BackGNumber, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 10, 0, 0, 0));
	BackGNumber->GetNumberEntry()->SetFont("-adobe-helvetica-bold-r-*-*-20-*-*-*-*-*-iso8859-1", false);
	BackGNumber->Resize(40, 30);
	BackGNumber->SetNumber(5.0);
	BackGNumber->Associate(this);

	TGHSplitter *hsplitter0 = new TGHSplitter(DrawFrame, 800,2);
	DrawFrame->AddFrame(hsplitter0, new TGLayoutHints(kLHintsTop | kLHintsExpandX,0,0,10,10));


	FrameCanvas = new TRootEmbeddedCanvas("Draw", DrawFrame, 1000, 600);
	DrawFrame->AddFrame(FrameCanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 0, 0, 0, 0));
	Int_t wid = FrameCanvas->GetCanvasWindowId();
	TCanvas *can  = new TCanvas("Canvas", 100, 400, wid);
	can->SetGridy(1);
	FrameCanvas->AdoptCanvas(can);
	FrameCanvas->GetCanvas()->Connect("ProcessedEvent(Int_t, Int_t, Int_t, TObject*)", "projection", this, "Click(Int_t, Int_t, Int_t, TObject*)");
	

	MapSubwindows();
	Resize();
//	CenterOnParent();
	SetWindowName(name);
	MapWindow();

}
projection::~projection(){}

Bool_t projection::ProcessMessage(Long_t msg,Long_t parm1, Long_t parm2)
{
	switch (GET_MSG(msg))
	{
		case kC_COMMAND:
			switch (GET_SUBMSG(msg))
			{
				case kCM_BUTTON:
					switch (parm1)
					{
						case FindPeakButton:
						{
							FindPeak();
							break;
						}
						case GateButton:
							{
									if(bGate)
									{
										bGate = false;
										GateB->SetTextColor(TColor::RGB2Pixel(255, 255, 255));
										if(vhg.size() > 0)
										{
											gate *Gate = new gate(fClient->GetRoot(), (char*)"Gate", int(vhg.size()));
											Double_t thres = GateThreNumber->GetNumber();
											Gate->Draw(vhg, thres);
										}
									}
									else
									{
										bGate = true;
										vhg.clear();
										Int_t Smooth = Int_t(SmoothNumber->GetNumber());
										Double_t dd = BackGNumber->GetNumber();
										SubtractBackground(h2, Smooth, dd);
										DrawProj();
										GateB->SetTextColor(TColor::RGB2Pixel(0, 255, 0));
									}
									break;
							}
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

Bool_t projection::DrawProj()
{
	if(h2 == NULL)
	{
		std::cout << "\033[31m Cannot get TH2D \033[0m" << std::endl;
		return false;
	}
	hp = h2->ProjectionX();
	hp->SetStats(0000);
	hp->GetXaxis()->SetTitle("Energy (keV)");
	hp->GetXaxis()->CenterTitle();
	hp->GetYaxis()->SetTitle("Counts");
	hp->GetYaxis()->CenterTitle();
	hp->GetXaxis()->SetTitleFont(22);
	hp->GetYaxis()->SetTitleFont(22);
	hp->GetXaxis()->SetLabelFont(22);
	hp->GetYaxis()->SetLabelFont(22);
	TCanvas *DrawCanvas = FrameCanvas->GetCanvas();
	DrawCanvas->Clear();
	hp->SetTitle("; Energy (keV); Counts");
	DrawCanvas->cd();
	hp->Draw("hist");
	DrawCanvas->Update();
	return true;
}

Bool_t projection::FindPeak()
{
	if(hp == NULL)
	{
		std::cout << "\033[31m Cannot get TH1D \033[0m" << std::endl;
		return false;
	}

	Double_t thres = ThreNumber->GetNumber();
	Int_t Smooth = Int_t(SmoothNumber->GetNumber());
	bool bs = false;
	if(BackSubButton->IsOn())
		bs = true;

	TCanvas *DrawCanvas = FrameCanvas->GetCanvas();
	Peaks(hp, thres, Smooth, bs, DrawCanvas);
	return true;
}

Bool_t projection::Peaks(TH1D *h, Double_t thres, Int_t Smooth, bool bs, TCanvas *DrawCanvas)
{
	if(h == NULL)
	{
		std::cout << "\033[31m Cannot get TH1D in peaks\033[0m" << std::endl;
		return false;
	}

	Int_t nfound;
	Double_t *xpeaks=NULL, *ypeaks=NULL;
	TSpectrum *s = NULL;
	DrawCanvas->Clear();
	h->SetTitle("; Energy (keV); Counts");
	if(!s) 
			s = new TSpectrum(500);
	TPolyMarker *pm;
	if(bs) 
	{
		hb = (TH1D*) s->Background(h, Smooth, "same"); //80-本底光滑程度
		hnb = *hp;
		hnb.Add(hp, hb, 1,-1);   
		DrawCanvas->cd();
		hnb.Draw();
		nfound=s->Search(&hnb, 2, "", thres); 
		pm = (TPolyMarker *) hnb.GetListOfFunctions()->FindObject("TPolyMarker");
	}
	else
	{
		DrawCanvas->cd();
		hp->Draw();
		nfound=s->Search(hp, 2, "", thres); 
		pm = (TPolyMarker *) h->GetListOfFunctions()->FindObject("TPolyMarker");
	}

	pm->SetMarkerStyle(32);
	pm->SetMarkerColor(kGreen);
	pm->SetMarkerSize(0.4);
	xpeaks = s->GetPositionX();
	ypeaks = s->GetPositionY();
	TString sout;
	for(int j=0;j<nfound;j++) 
	{
		TLatex *tex = new TLatex(xpeaks[j], ypeaks[j], Form("%0.1f", xpeaks[j]));
		tex->SetTextFont(13);
		tex->SetTextSize(18);
		tex->SetTextAlign(12);
		tex->SetTextAngle(90);
		tex->SetTextColor(kRed);
		DrawCanvas->cd();
		tex->Draw();
	}

	DrawCanvas->Update();
	return true;
}

bool projection::Click(Int_t event, Int_t px, Int_t py, TObject* selected)
{
	if (event != kButton1Down) return false;
	if(!PickButton->IsOn()) return false;
	if(h2 == NULL)
	{
		std::cout << "\033[31m Cannot get TH2D \033[0m" << std::endl;
		return false;
	}
	if(!bGate) return false;
    
	TCanvas *c = FrameCanvas->GetCanvas();
    // 将像素坐标转换为数据坐标
    Double_t x = c->AbsPixeltoX(px);
   // Double_t y = c->AbsPixeltoY(py);
	if(!bxGateMin || bxGateMax)
	{
		xGateMin = x;
		bxGateMin = true;
		bxGateMax = false;
		line->SetPoint(0, x, -1.0E9);
		line->SetPoint(1, x, 1.0E9);
		line->SetLineColor(Int_t(vhg.size()+1));
		TCanvas *DrawCanvas = FrameCanvas->GetCanvas();
		DrawCanvas->cd();
		line->DrawClone("sameL");
	}
	else
	{
		xGateMax = x;
		line->SetPoint(0, x, -1.0E9);
		line->SetPoint(1, x, 1.0E9);
		line->SetLineColor(Int_t(vhg.size() + 1));
		TCanvas *DrawCanvas = FrameCanvas->GetCanvas();
		DrawCanvas->cd();
		line->DrawClone("sameL");
		bxGateMax = true;
	}
	if(bxGateMin && bxGateMax && h2 != NULL)
	{
	//	vhg.push_back(new TH1D());
		Int_t bx = h2->GetXaxis()->FindBin(xGateMin);
		Int_t ex = h2->GetXaxis()->FindBin(xGateMax);
		if(BackSub2DButton->IsOn())
			vhg.push_back(h2nb.ProjectionX(Form("gate at %d-%d keV", Int_t(xGateMin), Int_t(xGateMax)), bx, ex)); // different titles are nedded!
		else
			vhg.push_back(h2->ProjectionX(Form("gate at %d-%d keV", Int_t(xGateMin), Int_t(xGateMax)), bx, ex)); // different titles are nedded!
		vhg.at(vhg.size()-1)->SetStats(0000);
		vhg.at(vhg.size()-1)->SetTitle(Form("gate at %d-%d keV", Int_t(xGateMin), Int_t(xGateMax)));
		vhg.at(vhg.size()-1)->GetXaxis()->SetTitle("Energy (keV)");
		vhg.at(vhg.size()-1)->GetXaxis()->CenterTitle();
		vhg.at(vhg.size()-1)->GetYaxis()->SetTitle("counts");
		vhg.at(vhg.size()-1)->GetYaxis()->CenterTitle();
		vhg.at(vhg.size()-1)->GetXaxis()->SetTitleFont(22);
		vhg.at(vhg.size()-1)->GetYaxis()->SetTitleFont(22);
		vhg.at(vhg.size()-1)->GetXaxis()->SetLabelFont(22);
		vhg.at(vhg.size()-1)->GetYaxis()->SetLabelFont(22);
		vhg.at(vhg.size()-1)->SetLineColor(Int_t(vhg.size()));
	}
	return true;
}

bool projection::SubtractBackground(TH2D* hgg, Int_t Smooth, Double_t dd)
{
	if(hgg == NULL) return true;
	Double_t Nbin = hgg->GetNbinsX();
	Double_t Emax = hgg->GetXaxis()->GetXmax();
//niter  background
	// Get a smooth background of the projection of g-g matrix
	TH1D *hg = hgg->ProjectionX("hg");
//	hg->SetFillColor(kCyan);
	TSpectrum *sa = new TSpectrum(200);
	TH1D *hbg;
	//Generate Smooth Background under peaks.
	hbg = (TH1D*) sa->Background(hg, Smooth, "nosmoothing same");//background
	hbg->SetFillColor(kCyan);

	std::vector<Int_t> K;
	K.clear();
	for(int i=2; i<=hg->GetNbinsX(); i++)
	{
		// 找坑中心
    	if(hg->GetBinContent(i) > hg->GetBinContent(i-1) || hg->GetBinContent(i) > hg->GetBinContent(i+1))
        	continue;
		//坑的计数和获得的平滑本底的计数相差不大，dd倍的本底标准差之内
    	if(abs(hg->GetBinContent(i) - hbg->GetBinContent(i)) > dd * sqrt(Double_t(hbg->GetBinContent(i)))) continue;

		//坑中心两边找本底
    	for(int j=i-50; j<i+50; j++)
    	{ 
        	if(j<1) j = 1;
        	if(j>hg->GetNbinsX()) break;
        	if(abs(hg->GetBinContent(j) - hbg->GetBinContent(j)) > dd * sqrt(Double_t(hbg->GetBinContent(i)))) continue;
			// 检查一下这个Bin是否已经视为本底
        	bool bb = true;
        	for(int k=0; k<int(K.size()); k++)
        	{
            	if(K[k] == j)
            	{
                	bb = false;
                	break;
            	}
        	}
        	if(bb)
            	K.push_back(j);
    	}
	}

	Double_t Bij, x, y, fi, fj;
	Double_t bsum = 0.0;
	Double_t Kbsum = 0.0;
	Double_t Kfsum = 0.0;
	std::vector<Double_t> b;
	b.clear();
	//确定每个Bin开门普本底位置的计数和
	for(int i=1; i<=hg->GetNbinsX(); i++)
	{
    	ULong64_t count = 0;
    	for(int j=0; j<int(K.size()); j++)
    	{
        	count += hgg->GetBinContent(i, K[j]);
    	}
		//bsum g-g matrix 总本底计数 
    	bsum += Double_t(count);
    	b.push_back(Double_t(count));
	}

	for(int i=0; i<int(K.size()); i++)
	{
    	Kbsum += b[K[i]];
    	Kfsum += Double_t(hbg->GetBinContent(K[i]));
	}
	
	TH2D* hggb1 = new TH2D("hggb1", "", 	  Nbin, 0.0, Emax, Nbin, 0.0, Emax);
	TH2D* hggb2 = new TH2D("hggb2", "", 	  Nbin, 0.0, Emax, Nbin, 0.0, Emax);

	for(int i=1; i<=hgg->GetNbinsX(); i++) 
	{
    	for(int j=1; j<=hgg->GetNbinsY(); j++) 
    	{
        	fi = Double_t(hbg->GetBinContent(i));
        	fj = Double_t(hbg->GetBinContent(j));
        	x = hg->GetBinCenter(i);
        	y = hg->GetBinCenter(j);        
        	Bij = fi * b[j-1]/bsum;
        	hggb1->Fill(x, y, Bij);

        	Bij = (fi*b[j-1] + fj*b[i-1] - fi*fj*Kbsum/bsum)/bsum;
        	hggb2->Fill(x, y, Bij);// backgound gg matix
    	}
	}
	h2nb = *hgg;
	h2nb.Clear();
//	h2nb.Add(hgg, hggb1, 1.0, -1.0);
	h2nb.Add(hgg, hggb2, 1.0, -1.0);// background subtracted gg matix.

	hggb1->Delete();
	hggb2->Delete();

	return true;
}
