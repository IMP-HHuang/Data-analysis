#include "Proc.h"
#include "Riostream.h"
#include "TH1.h"
#include "TH2.h"
#include "TCutG.h"
#include "snprintf.h"
#include "TGo4WinCond.h"
#include "TGo4PolyCond.h"
#include "TGo4CondArray.h"
#include "TGo4Picture.h"
#include "Param.h"
#include "iostream"
#include "string"
// if we have this class:
//#include "Event.h"

#include"QDC.h"

//***********************************************************
Proc::Proc() : TGo4EventProcessor("Proc")
{
	std::cout << "**** Proc: Create instance " << std::endl;
}
//***********************************************************
Proc::~Proc()
{
	opf->cd();
	opt->Write();
	opf->Close();
	std::cout << "**** Proc: Delete instance " << std::endl;
}
void Proc::Init()
{
	for(int i=0; i<NMo; i++)
		for(int j=0; j<NCh; j++)
		{
			raw[i][j] = 0.0;
		}
	for(int i=0; i<16; i++)
	{
		YSOx[i] = 0.0;
		YSOy[i] = 0.0;
	}
	for(int i=0; i<8; i++)
	{
		Fiber[i] = 0.0;
		TDC[i] = 0.0;
	}
	px = 0.0;
	py = 0.0;
	Fiberts = 0.0;
	YSOts =0.0;
}
//***********************************************************
// this one is used in standard factory
Proc::Proc(const char* name) : TGo4EventProcessor(name)
{
	Init();
	for(int i=0; i<NCh; i++)
	{
		CalYSO[0][i] = 0.0;
		CalYSO[1][i] = 0.0;
	}
	std::ifstream inf("CalYSO.dat");
	std::string stmp;
	double a0, a1;
	int idtmp;
	if(inf.is_open())
	{
		getline(inf, stmp);
		while(inf >> idtmp >> a0 >> a1)
		{
			CalYSO[0][idtmp] = a0;
			CalYSO[1][idtmp] = a1;
		}
	}
	else
	{	
		std::cout << "Cannot  open the calbration file" << std::endl;
	}
	inf.close();
	std::cout << "**** Proc: Create instance " << name << std::endl;
	opf = new TFile("out.root", "RECREATE");
	opt = new TTree("tree", "tree");
//	opt->Branch("v", raw, "v[4][32]/F");
	opt->Branch("YSOx", YSOx, "YSOx[16]/F");
	opt->Branch("YSOy", YSOy, "YSOy[16]/F");
	opt->Branch("YSOts", &YSOts, "YSOyts/F");
	opt->Branch("Fiber", Fiber, "Fiber[8]/F");
	opt->Branch("TDC", TDC, "TDC[8]/F");
	opt->Branch("Fiberts", &Fiberts, "Fiberts/F");
	opt->Branch("px", &px, "px/F");
	opt->Branch("py", &py, "py/F");
	for(int i=0; i<NMo; i++)
	{
		for(int j=0; j<NCh; j++)
		{
			hraw[i][j] = (TH1D*) MakeTH1('D', Form("raw/Mod%dCh%d", i, j), Form("Mod %d Ch %d", i, j), 5000, 0, 5000, "channels");
		}
	}
	for(int i=0; i<NMo; i++)
		hcount[i] = (TH1I*) MakeTH1('I', Form("Count/Mod%d", i), Form("Mod = %d", i), 32, 0, 32, "ch");
}
//-----------------------------------------------------------
// event function
Bool_t Proc::BuildEvent(TGo4EventElement* target)
{
	Bool_t isValid=kFALSE; // validity of output event
	TGo4MbsEvent* inp_evt = (TGo4MbsEvent* ) GetInputEvent(); // from this
	if (inp_evt == 0) 
	{
		cout << "SCNUnpackProc: no input event !"<< endl;
		return isValid;
	}
	isValid = kTRUE;
	inp_evt->ResetIterator();
	TGo4MbsSubEvent* psubevt(0);
	while ((psubevt = inp_evt->NextSubEvent()) != 0) // subevent loop
	{
		Int_t* pdata = psubevt->GetDataField();
		Int_t lwords = psubevt->GetIntLen();

		Int_t PrcID = psubevt->GetProcid();  //Crate Id
		Init();
		switch(PrcID)
		{
			case 20: //It's the QDC. // 
				Int_t geo_ad;
				Int_t EvCounter;
				for(Int_t i=0; i<4; i++) //Loop module
				{
					Header *h = (Header*)pdata;
					if(h->check2 != 2) 
					{
						cout<<"There's a problem reading the header"<<endl;
						break;
					}
					geo_ad = h->geo;  
					// i      0    1    2    3
					// geo_ad 6    10   8    13
					// modul  QDC2 QDC1 ADC  TDC
					Int_t ChannelNumber = h->channels;
			//		if(geo_ad == 10)
			//		cout <<EvCounter << "  " << i << "   " << geo_ad << "  " << ChannelNumber << "  " << *pdata << endl;
					pdata++;  //13march2020
					for(Int_t j=0; j<ChannelNumber; j++)
					{
						Data *d = (Data*)pdata;
						if(d->check != 0)
						{
							cout<<"There's a problem reading the data"<<endl;
							break;
						}
						if(i < NMo && d->channel < NCh)
						{
							if(i == 1)
								raw[i][d->channel] = CalYSO[1][d->channel] * (d->value - CalYSO[0][d->channel]);
							else
								raw[i][d->channel] = d->value;
						}
						pdata++; 
					}
					End *e = (End*)pdata;
					if(e->check!=4)
					{
						cout<<"There was a problem reading the ending"<<endl;
						break;
					}
					EvCounter = e->evCounter;
					pdata++;
				}
//			case 80: //It's the Tamax 

			default :
				std::cout << " other " << endl;
		}
	}
//	if(isValid)
		FillHistograms();
	return isValid;
}

//-----------------------------------------------------------
// histogramming function
Bool_t Proc::FillHistograms()
{
	Float_t Sumx = 0.0;
	Float_t Sumy = 0.0;
	for(int i=0; i<NMo; i++)
		for(int j=0; j<NCh; j++)
		{
			if(raw[i][j] > 0.0)
			{
				if(i == 1)
				{
					if(j < 16)
					{
						YSOx[j] = raw[i][j];
						px += raw[i][j] * Float_t(j + 1);
						Sumx += raw[i][j];
					}
					else
					{
						YSOy[j-16] = raw[i][j];
						py += raw[i][j] * Float_t(j - 16 + 1);
						Sumy += raw[i][j];
					}
				}
				if(i == 0 && j < 8)
					Fiber[j] = raw[i][j];
				if(i == 3 && j < 8)
					TDC[j] = raw[i][j];
				if(i == 3 && j == 5)
					YSOts = raw[i][j];
				if(i == 3 && j == 6)
					Fiberts = raw[i][j];

				hraw[i][j]->Fill(raw[i][j]);
				hcount[i]->Fill(j);
			}
		}
	px = px / Sumx;
	py = py / Sumy;
	opt->Fill();
	return kTRUE;
}
//----------------------------END OF GO4 SOURCE FILE ---------------------
