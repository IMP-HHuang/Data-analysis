#include<iostream>

#include<TF1.h>
void gaus(TH1D *h, Double_t &mean, Double_t &sigam, Double_t xmin, Double_t xmax, Double_t c_min, Double_t c_max, Double_t mean_min, Double_t mean_max, Double_t sigam_min, Double_t sigam_max)
{
//	Double_t x = 0;
	TF1 *fun = new TF1("fit", "gaus", xmin, xmax);
	fun->SetParNames("Constan","Mean","Sigma");
//	TF1 *fun = new TF1("fit", "gaus(0)+pol1(3)", xmin, xmax);
//	fun->SetParNames("Constan", "Mean", "Sigma", "b", "k");
	if(c_min != 0)
	{
		fun->SetParLimits(0, c_min, c_max);
	}
//	fun->SetParameter(0, (c_min+c_max)/2);
	fun->SetParLimits(1, mean_min, mean_max);
	fun->SetParLimits(2, sigam_min, sigam_max);
	h->Fit("fit", "SQR+");
	mean = fun->GetParameter("Mean");
	sigam = fun->GetParameter("Sigma");
}

void gaus(TH1D* h, Double_t &me, Double_t &si, Double_t xmin, Double_t xmax, Double_t mean_min, Double_t mean_max)
{
	TF1 *fun = new TF1("fit", "gaus", xmin, xmax);
	fun->SetParNames("Constan","Mean","Sigma");
	fun->SetParLimits(1, mean_min, mean_max);
	TFitResultPtr r;
	r = h->Fit("fit", "R");
	me = fun -> GetParameter(1);
	si = fun -> GetParameter(2); 
}
