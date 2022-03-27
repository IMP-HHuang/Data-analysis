
TH1F *h = NULL,*hb = NULL;
Int_t nfound;
Double_t *xpeaks = NULL, *ypeaks = NULL;
TSpectrum *s = NULL;
void peaks(int n,TString hname, Double_t thres = 0.05, int backsub = 1)
{
		h = (TH1F*)gROOT->FindObject(hname);
		if(!s) s = new TSpectrum(500);
		if(backsub) //取0时，去本低
			{
				hb = (TH1F*)s->Background(h, 80, "same");
				h->Add(h, hb, 1, -1);   
			}
		h->SetMaximum(h->GetMaximum()*1.2);
		h->SetMinimum(1);
		h->SetFillColor(kCyan);
		nfound = s->Search(h, 2, "", thres); 
		TPolyMarker *pm = (TPolyMarker *)
		h->GetListOfFunctions()->FindObject("TPolyMarker");
		pm->SetMarkerStyle(32);
		pm->SetMarkerColor(kGreen);
		pm->SetMarkerSize(0.4);
		xpeaks = s->GetPositionX();
		ypeaks = s->GetPositionY();
		TLatex *tex; 
		
		for(int j=0; j<nfound; j++) 
			{
	//			int j = n;
				stringstream ssx;
				ssx<<xpeaks[j];
				TString s1 = ssx.str();
//				ssx<<xpeaks[j]<<endl;
				tex = new TLatex(xpeaks[j], ypeaks[j], s1);
				cout<<xpeaks[j]<<"\t"<<s1<<endl;
				tex->SetTextFont(13);
				tex->SetTextSize(18);
				tex->SetTextAlign(12);
				tex->SetTextAngle(90);
				tex->SetTextColor(3);
				tex->DrawClone();
				TF1 *fun = new TF1("fit", "gaus(0)", xpeaks[j]-2, xpeaks[j]+2);
				fun->SetParLimits(0, ypeaks[j]-2.0, ypeaks[j]+2.0);
				fun->SetParLimits(1, xpeaks[j]-2.0, xpeaks[j]+2.0);
				fun->SetParLimits(2, 0, 1.5);
				h->Fit("fit", "SQR+");
				tex->Delete();
			}
}
