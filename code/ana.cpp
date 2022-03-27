//peaks

#include<TH1F.h>
#include<TSpectrum.h>
#include<TPolyMarker.h>
#include<TLatex.h>
#include <TString.h>
#include<vector>
#include<map>
#include<iostream>
#include <sstream>
#include <string>

#include"gaus.h"
//	vector<Double_t> pe;
//	vector<Double_t> sigame;

void peaks(TH1D* h, Double_t thres, int backsub = 0)
{
    TH1D* hb = NULL;
    Int_t nfound;
    Double_t* xpeaks = NULL, *ypeaks = NULL, mean, sigam;
    TSpectrum* s = NULL;
    pe.clear();//清空pe
    sigame.clear();
    std::map<Int_t, Double_t> me;//用于排序

    if (!s)
        s = new TSpectrum(500);

    if (backsub)
    {
        hb = (TH1D*)s->Background(h, 80, "same");//80-本底光滑程度
        h->Add(h, hb, 1, -1);
    }

    nfound = s->Search(h, 2, "", thres);
    TPolyMarker* pm = (TPolyMarker*)  h->GetListOfFunctions()->FindObject("TPolyMarker");
    pm->SetMarkerStyle(32);
    pm->SetMarkerColor(kGreen);
    pm->SetMarkerSize(0.4);
    xpeaks = s->GetPositionX();
    ypeaks = s->GetPositionY();
    TString sout;
    for (int j = 0; j < nfound; j++)
    {
        std::stringstream ss;
        ss << xpeaks[j];
        TString s1 = ss.str();
        TLatex* tex = new TLatex(xpeaks[j], ypeaks[j]+1, s1);
        tex->SetTextFont(13);
        tex->SetTextSize(20);
        tex->SetTextAlign(12);
        tex->SetTextAngle(90);
        tex->SetTextColor(kRed);
		tex->DrawClone("SAME");
    }
}
void peaks(TH1D* h, std::vector<Double_t>& pe, std::vector<Double_t>& sigame, Double_t thres, int backsub, Double_t x_min, Double_t x_max, Double_t y_min, Double_t y_max, Double_t mean_min, Double_t mean_max, Double_t sigam_min = 0.0, Double_t sigam_max = 1.0, Int_t bcout = 1)
{
    TH1D* hb = NULL;
    Int_t nfound;
    Double_t* xpeaks = NULL, *ypeaks = NULL, mean, sigam;
    TSpectrum* s = NULL;
    pe.clear();//清空pe
    sigame.clear();
    std::map<Int_t, Double_t> me;//用于排序

    if (!s)
        s = new TSpectrum(500);

    if (backsub)
    {
        hb = (TH1D*)s->Background(h, 80, "same");//80-本底光滑程度
        h->Add(h, hb, 1, -1);
    }

    nfound = s->Search(h, 2, "", thres);
    TPolyMarker* pm = (TPolyMarker*)  h->GetListOfFunctions()->FindObject("TPolyMarker");
    pm->SetMarkerStyle(32);
    pm->SetMarkerColor(kGreen);
    pm->SetMarkerSize(0.4);
    xpeaks = s->GetPositionX();
    ypeaks = s->GetPositionY();
    TString sout;
	me.clear();
	/*
	for (std::map<Int_t, Double_t>::iterator ie = me.begin(); ie != me.end(); ie++)
	{
		cout<<ie->first << "  " << ie->second << endl;
	}
	*/
	h->Draw("hist");
    for (int j = 0; j < nfound; j++)
    {
        std::stringstream ss;
        ss << xpeaks[j];
        TString s1 = ss.str();
        TLatex* tex = new TLatex(xpeaks[j], ypeaks[j]+1, s1);
        tex->SetTextFont(13);
        tex->SetTextSize(14);
        tex->SetTextAlign(12);
        tex->SetTextAngle(90);
        tex->SetTextColor(kRed);
		tex->DrawClone("SAME");
		if(bcout == 1)
		{
        	gaus(h, mean, sigam, xpeaks[j] - x_min, xpeaks[j] + x_max, ypeaks[j] - y_min, ypeaks[j] + y_max, xpeaks[j] - mean_min, xpeaks[j] + mean_max, sigam_min, sigam_max);
        	pe.push_back(mean);;
        	me.insert(std::make_pair(Int_t(mean), sigam));
			cout<<Form("%2d\t%3.4f\t%2.5f", j, mean, sigam)<<endl;
		}
//		tex->Delete();
    }
	if(bcout == 2)
	{
		sort(pe.begin(),pe.end());
		int i = 0;
//    for (std::map<Int_t, Double_t>::iterator ie = me.begin(); ie != me.end(); ie++)
		for (auto ie = me.begin(); ie != me.end(); ie++)
		{
			cout<<Form("%2d\t%3.4f\t%2.5f", i, pe[i], ie->second)<<endl;
	   		sigame.push_back(ie->second);
			i++;
    	}
	}
}

void peaks(TString hname, std::vector<Double_t>& pe, std::vector<Double_t>& sigame, Double_t thres, int backsub, Double_t x_min, Double_t x_max, Double_t y_min, Double_t y_max, Double_t mean_min, Double_t mean_max, Double_t sigam_min = 0.0, Double_t sigam_max = 1.0, Int_t bcout = 1)
{
	TH1D* h = (TH1D*)gROOT->FindObject(hname);
	peaks(h, pe, sigame, thres, backsub, x_min, x_max, y_min, y_max, mean_min, mean_max, sigam_min, sigam_max, bcout);

}
void peaksgamma(TH1D* h, std::vector<Double_t>& pe, std::vector<Double_t>& sigame, Double_t thres = 0.05, int backsub = 0)
{
	peaks(h, pe, sigame, thres, backsub, 2.0, 2.0, 10.0, 10.0, 2.0, 2.0, 0.0, 3.0);
}
void peaksgamma_CoutE(TH1D* h, std::vector<Double_t>& pe, std::vector<Double_t>& sigame, Double_t thres = 0.05, int backsub = 0, Double_t Emin=2.0, Double_t Emax=2.0)
{
        peaks(h, pe, sigame, thres, backsub, Emin, Emax, 30.0, 30.0, 2.0, 2.0, 0.0, 2.0, 2);
}
void peaksgamma_NoCout(TH1D* h, std::vector<Double_t>& pe, std::vector<Double_t>& sigame, Double_t thres = 0.05, int backsub = 0)
{
	peaks(h, pe, sigame, thres, backsub, 2.0, 2.0, 10.0, 10.0, 2.0, 2.0, 0.0, 3.0, 0);
}

void peaksgamma_NoCout(TString hname, std::vector<Double_t>& pe, std::vector<Double_t>& sigame, Double_t thres = 0.05, int backsub = 0)
{
	peaks(hname, pe, sigame, thres, backsub, 2.0, 2.0, 10.0, 10.0, 2.0, 2.0, 0.0, 3.0, 0);
}
void peaksgamma_Channel(TH1D* h, std::vector<Double_t>& pe, std::vector<Double_t>& sigame, Double_t thres = 0.05, int backsub = 0)
{
	peaks(h, pe, sigame, thres, backsub, 20.0, 30.0, 10000.0, 10000.0, 20.0, 20.0, 0.0, 30.0);
}
;
void peaks_DSSD_e(TH1D* h, std::vector<Double_t>& pe, std::vector<Double_t>& sigame, Double_t thres = 0.05, int backsub = 0)
{
	peaks(h, pe, sigame, thres, backsub, 10.0, 110.0, 60E3, 60.0E3, 40.0, 40.0, 0.0, 50.0);
}

void peaks_DSSD_ee(TH1D* h, std::vector<Double_t>& pe, std::vector<Double_t>& sigame, Double_t thres = 0.05, int backsub = 0)
{
	peaks(h, pe, sigame, thres, backsub, 10.0, 20.0, 500, 500, 20.0, 20.0, 0.0, 30.0);
}

void peaks_sib_e(TH1D* h, std::vector<Double_t>& pe, std::vector<Double_t>& sigame, Double_t thres = 0.05, int backsub = 0)
{
	peaks(h, pe, sigame, thres, backsub, 60.0, 110.0, 100, 100, 40.0, 40.0, 0.0, 50.0);
}

void draw_line(Double_t p1x=0., Double_t p1y=0., Double_t p2x=0., Double_t p2y=0., TString col = "kRed")
{
	TGraph *line = new TGraph;	
	line->SetPoint(0, p1x, p1y);
	line->SetPoint(1, p2x, p2y);
	if(col == "kRed")
		line->SetLineColor(kRed);
	if(col == "kBlue")
		line->SetLineColor(kBlue);
	if(col == "kGreen")
		line->SetLineColor(kGreen);
	if(col == "kBlack")
		line->SetLineColor(kBlack);
	if(col == "kGray")
		line->SetLineColor(kGray);
	if(col == "kCyan")
		line->SetLineColor(kCyan);
	line->DrawClone("sameL");
 }
void draw_tex(TString s, Double_t px=0., Double_t py=0., TString col="kRed")
{
	TLatex* latex = new TLatex();
	latex->SetTextSize(0.05);
	if(col == "kRed")
		latex->SetTextColor(kRed);
	if(col == "kBlue")
		latex->SetTextColor(kBlue);
	if(col == "kGreen")
		latex->SetTextColor(kGreen);
	if(col == "kBlack")
		latex->SetTextColor(kBlack);
	if(col == "kGray")
		latex->SetTextColor(kGray);
	if(col == "kCyan")
		latex->SetTextColor(kCyan);
	latex->SetTextAlign(10);
	latex->DrawLatex(px, py, s);
}
void draw_arrow(Double_t p1x=0., Double_t p1y=0., Double_t p2x=0., Double_t p2y=0.)
{
	TArrow* arr = new TArrow(p1x, p1y, p2x, p2y);
	arr->SetArrowSize(0.01);
	arr->SetLineWidth(2);
	arr->SetLineColor(kBlack);
	arr->SetFillColor(kBlack);
	arr->SetAngle(30);
	arr->DrawClone("same");
}
TH2D* h;
TH1D *hx, *hy;
void zoom(TString name, Double_t xfirst, Double_t xsecond, Double_t yfirst, Double_t ysecond)
{
	h = (TH2D*) gROOT->FindObject(name.Data());
	h->GetXaxis()->SetRange(xfirst, xsecond);
	h->GetYaxis()->SetRange(yfirst, ysecond);
	h->Draw("colz");
//	h->Draw("surf1");
	Int_t bx = h->GetXaxis()->FindBin(xfirst);
	Int_t ex = h->GetXaxis()->FindBin(xsecond);
	Int_t ey = h->GetYaxis()->FindBin(yfirst);
	Int_t by = h->GetYaxis()->FindBin(ysecond);
	hx = h->ProjectionX("hx", bx, ex);
	hy = h->ProjectionY("hy", by, ey);
}
void zoom(TH1D* h1, Double_t xfirst, Double_t xsecond)
{
	h1->GetXaxis()->SetRangeUser(xfirst, xsecond);
	h1->Draw();
}
void gate(Double_t ge, TH2D* hgg, int backsub=0, Double_t thers=0.25, Double_t xmax=1500, Double_t gew=2, Int_t bpeak = 1)
{
    Double_t gea, geb;
    TH1D *hg;
    TString hname;
    hgg->SetAxisRange(0, xmax, "X");
    hgg->SetAxisRange(0, xmax, "Y");
    gea = hgg->GetXaxis()->FindBin(ge-gew);
    geb = hgg->GetXaxis()->FindBin(ge+gew); 
    hname.Form("gated on %d keV", int(ge));
    hg = hgg->ProjectionX(hname, gea, geb);
    hg->Sumw2(0);
    hg->SetAxisRange(0, xmax, "X");
    //peaks(hname,0.02);
	if(bpeak==1)
    	peaksgamma_NoCout(hname, pe, sigame, thers, backsub);
	else
		hg->Draw();
}
void gate(Double_t ge, TH2D* hgg, Double_t gew, int backsub=0, Int_t bpeak = 1)
{
	Double_t thers=0.25;
	Double_t xmax=1500;
    Double_t gea, geb;
    TH1D *hg;
    TString hname;
    hgg->SetAxisRange(0, xmax, "X");
    hgg->SetAxisRange(0, xmax, "Y");
    gea = hgg->GetXaxis()->FindBin(ge-gew);
    geb = hgg->GetXaxis()->FindBin(ge+gew); 
    hname.Form("gated on %d keV", int(ge));
    hg = hgg->ProjectionX(hname, gea, geb);
    hg->Sumw2(0);
    hg->SetAxisRange(0, 1500, "X");
    //peaks(hname,0.02);
	if(bpeak==1)
    	peaksgamma_NoCout(hname, pe, sigame, thers, backsub);
	else
		hg->Draw();
}



void gate(std::vector<Double_t>& ge, TH2D* hgg, int backsub=0, Double_t thers=0.25, Double_t xmax=1500, Double_t gew=2, int bpeak = 1)
{
    Double_t gea, geb;
    TH1D *hg = new TH1D("hg", "hg", 1500, 0.0, 1500.0);
	TH1D *hgt;
    TString hname;
    hgg->SetAxisRange(0, xmax, "X");
    hgg->SetAxisRange(0, xmax, "Y");
	for(auto ie = ge.begin(); ie != ge.end(); ie++)
	{
    	gea = hgg->GetXaxis()->FindBin(*ie - gew);
    	geb = hgg->GetXaxis()->FindBin(*ie + gew); 
    	hname.Form("gated on %d keV", int(*ie));
    	hgt = hgg->ProjectionX(hname, gea, geb);
    	hg->Add(hg, hgt, 1.0, 1.0);
	}
    hg->Sumw2(0);
    hg->SetAxisRange(0, 1500, "X");
    //peaks(hname,0.02);
	if(bpeak==1)
    	peaksgamma_NoCout(hg, pe, sigame, thers, backsub);
	else
		hg->Draw();
}
void gate(TCanvas* c, Double_t ge, TH2D* hgga, TH2D* hggb, int backsub=0, Double_t thers=0.25, Double_t xmax=1500, Double_t gew=2, int bpeak = 1)
{
    Double_t gea, geb;
    TH1D *hga_tmp = new TH1D("hga_tmp", "hga_tmp", 1500, 0.0, 1500.0);
	TH1D *hgb_tmp = new TH1D("hgb_tmp", "hgb_tmp", 1500, 0.0, 1500.0);
    TString hnamea, hnameb;
    hgga->SetAxisRange(0, xmax, "X");
    hgga->SetAxisRange(0, xmax, "Y");
    hggb->SetAxisRange(0, xmax, "X");
    hggb->SetAxisRange(0, xmax, "Y");
    gea = hgga->GetXaxis()->FindBin(ge - gew);
    geb = hggb->GetXaxis()->FindBin(ge + gew); 
    hnamea.Form("gated on %d keV", int(ge));
    hnameb.Form("background gated on %d keV", int(ge));
    hga_tmp = hgga->ProjectionX(hnamea, gea, geb);
    hgb_tmp = hggb->ProjectionX(hnameb, gea, geb);
    hga_tmp->Sumw2(0);
    hga_tmp->SetAxisRange(0, 1500, "X");
    hgb_tmp->Sumw2(0);
    hgb_tmp->SetAxisRange(0, 1500, "X");
	c->cd();
	if(bpeak==1)
    	peaksgamma_NoCout(hga_tmp, pe, sigame, thers, backsub);
	else
		hga_tmp->Draw();
	hgb_tmp->SetLineColor(kRed);
	hgb_tmp->Draw("same");
}
void Multigate(TCanvas *c, std::vector<Double_t>& ge, TH2D* hgg, Double_t thers=0.25, Double_t xmax = 1500, Double_t Bin =1.0, Double_t gew=2, int bpeak = 1)
{
	Int_t Ngate = ge.size();

	Int_t NBin = Int_t(xmax/Bin);
    TString hname;
	c->Divide(1, Ngate, 0, 0);
    hgg->SetAxisRange(0, xmax, "X");
    hgg->SetAxisRange(0, xmax, "Y");

	TH1D *hg[Ngate];
	Double_t gea, geb;
   	for(int i=0; i<Ngate; i++)
	{
    	gea = hgg->GetXaxis()->FindBin(ge[i]-gew);
    	geb = hgg->GetXaxis()->FindBin(ge[i]+gew); 
    	hname.Form("gated on %d keV", int(ge[i]));
    	hg[i] = hgg->ProjectionX(hname, gea, geb);
		if(int(Bin*100)%10 != 0)
			hg[i]->SetTitle(Form("; Energy (keV); Counts / %0.2f keV", Bin));
		else if(int(Bin*10)%10 != 0)
			hg[i]->SetTitle(Form("; Energy (keV); Counts / %0.1f keV", Bin));
		else
			hg[i]->SetTitle(Form("; Energy (keV); Counts / %0.0f keV", Bin));
		hg[i]->GetXaxis()->CenterTitle(true);
		hg[i]->GetXaxis()->SetLabelSize(0.05);
		hg[i]->GetXaxis()->SetTitleSize(0.05);
		hg[i]->GetXaxis()->SetTitleOffset(1.0);
		hg[i]->GetYaxis()->CenterTitle(true);
		hg[i]->GetYaxis()->SetLabelSize(0.05);
		hg[i]->GetYaxis()->SetTitleSize(0.05);
		hg[i]->GetYaxis()->SetRangeUser(-0.2*hg[i]->GetBinContent(hg[i]->GetMaximumBin()), hg[i]->GetBinContent(hg[i]->GetMaximumBin())+0.2*hg[i]->GetBinContent(hg[i]->GetMaximumBin()));
    	hg[i]->Sumw2(0);
		c->cd(i+1);
		gStyle->SetStatW(0.2);
		gStyle->SetStatH(0.3);
		gStyle->SetStatStyle(0000);
		gPad->SetTickx(2);
		hg[i]->Draw("hist");
		if(bpeak==1)
		{
    		peaksgamma_NoCout(hg[i], pe, sigame, thers, 0);
		}
	}
}

TH2D *SubtractBackground(TH2D* hgg, Double_t Emax = 1500, Double_t bin = 1.0, Int_t niter = 10)
{

	Int_t Nbin = Int_t(Emax/bin);
	// Get a smooth background of the projection of g-g matrix
	TH1D *hg=hgg->ProjectionX("hg");
	hg->SetFillColor(kCyan);
	TSpectrum *sa=new TSpectrum(200);
	TH1D *hbg,*hpeak;
	//Generate Smooth Background under peaks.
	hbg=(TH1D*) sa->Background(hg, niter, "nosmoothing same");//background
	hbg->SetFillColor(kWhite);

	vector<Int_t> K;
	K.clear();
	Double_t dd = 5.0;
	for(int i=2; i<=hg->GetNbinsX(); i++)
	{
		// 找坑中心
    	if(hg->GetBinContent(i) > hg->GetBinContent(i-1) || hg->GetBinContent(i) > hg->GetBinContent(i+1))
        	continue;
		//坑的计数和获得的平滑本底的计数相差不大，dd倍的本底标准差之内
    	if(abs(hg->GetBinContent(i)-hbg->GetBinContent(i)) > dd*sqrt(Double_t(hbg->GetBinContent(i)))) continue;

		//坑中心两边找本底
    	for(int j=i-50; j<i+50; j++)
    	{ 
        	if(j<1) j = 1;
        	if(j>hg->GetNbinsX()) break;
        	if(abs(hg->GetBinContent(j)-hbg->GetBinContent(j)) > dd*sqrt(Double_t(hbg->GetBinContent(i)))) continue;
			// 检查一下这个Bin是否已经视为本底
        	bool bb = true;
        	for(int k=0; k<K.size(); k++)
        	{
            	if(K[k]==j)
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
	vector<Double_t> b;
	b.clear();
	//确定每个Bin开门普本底位置的计数和
	for(int i=1; i<=hg->GetNbinsX(); i++)
	{
    	ULong64_t count = 0;
    	for(int j=0; j<K.size(); j++)
    	{
        	count += hgg->GetBinContent(i, K[j]);
    	}
		//bsum g-g matrix 总本底计数 
    	bsum += Double_t(count);
    	b.push_back(Double_t(count));
	}

	for(int i=0; i<K.size(); i++)
	{
    	Kbsum += b[K[i]];
    	Kfsum += Double_t(hbg->GetBinContent(K[i]));
	}
	
	TH2D* hggb1=new TH2D("hggb1", "", 	  Nbin, 0.0, Emax, Nbin, 0.0, Emax);
	TH2D* hggb2=new TH2D("hggb2", "", 	  Nbin, 0.0, Emax, Nbin, 0.0, Emax);
	TH2D* hggmat=new TH2D("hggmat", "",   Nbin, 0.0, Emax, Nbin, 0.0, Emax);
	TH2D* hggmat1=new TH2D("hggmat1", "", Nbin, 0.0, Emax, Nbin, 0.0, Emax);

	for(int i=1; i<=hgg->GetNbinsX(); i++) 
	{
    	for(int j=1; j<=hgg->GetNbinsY(); j++) 
    	{
        	fi = Double_t(hbg->GetBinContent(i));
        	fj = Double_t(hbg->GetBinContent(j));
        	x = hg->GetBinCenter(i);
        	y = hg->GetBinCenter(j);        
        	Bij = fi*b[j-1]/bsum;
        	hggb1->Fill(x, y, Bij);
        	Bij = (fi*b[j-1] + fj*b[i-1] - fi*fj*Kbsum/bsum)/bsum;
        	hggb2->Fill(x, y, Bij);// backgound gg matix
    }
	}
	hggmat->Add(hgg, hggb2, 1.0, -1.0);// background subtracted gg matix.
	hggmat1->Add(hgg, hggb1, 1.0, -1.0);
	return hggmat;
}


void writemat(TH2D *h2, TString FoutName = "out")
{
  	Int_t ix, iy, nx, ny, zi;
	Int_t BinMax = h2->GetNbinsX();
	BinMax = 20;
	cout << "bg  " << BinMax << endl;
	Int_t zmat[3000][3000];  // Jupyter 限制大小　勿增大
	
	
  	if(h2 != NULL)
  	{
		cout << "bg  " << BinMax << endl;
	
    	for(iy=1; iy <= BinMax; iy++)
		{
			cout <<"bg  " << iy  << "  " <<endl; //<<"  "<< h2->GetBinContent(ix, iy) << endl;
      		for(ix=1; ix <= BinMax; ix++)
			{
			cout <<"aa  " << iy  << "  " << ix <<endl; //<<"  "<< h2->GetBinContent(ix, iy) << endl;
        	//	zmat[ix-1][iy-1] = (Int_t)h2->GetBinContent(ix, iy);
			}
		}
	//	FoutName = Form("%s.spn", FoutName.Data());
  	//	FILE *zout;
    //	zout = fopen(FoutName, "wb+");
  //  	for(zi=0; zi<BinMax; zi++)
    //  		fwrite(zmat[zi], 4, BinMax, zout);
    //	fclose(zout);
	} 
  	else 
		cout << "h2 is NULL " << endl;
}


