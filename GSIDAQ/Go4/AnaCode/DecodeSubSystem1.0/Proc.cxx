#include "Riostream.h"
#include "TH1.h"
#include "TH2.h"
#include "TCutG.h"
#include "snprintf.h"
#include "TGo4WinCond.h"
#include "TGo4PolyCond.h"
#include "TGo4CondArray.h"
#include "TGo4Picture.h"
#include "iostream"
#include "string"
#include "algorithm"
// if we have this class:
//#include "Event.h"

#include "Param.h"
#include "Proc.h"
#include "Data.h"
#include "UserDefine.h"

//***********************************************************
// this one is used in standard factory
Proc::Proc(const char* name) : TGo4EventProcessor(name)
{
    	DecodeTFatima = new DecodeTamex("./Par/FatimaTamexFineTDCCal.txt");
    	DecodeTBp = new DecodeTamex("./Par/BetaPlasticTamexFineTDCCal.txt");
    	decodeWR = new DecodeWhiteRabbit();
    	decodeQDC = new DecodeQDC();
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
#if(GetTamexFineTimeCaliPar)
	opt->Branch("fatimacal", &vFATIMACal);
	opt->Branch("bpcal", &vBpCal);
#else
	opt->Branch("WR", &vWR);
	opt->Branch("fatima", &vFATIMA);
	opt->Branch("bp", &vBp);
#endif
	for(int i=0; i<NMo; i++)
	{
		for(int j=0; j<NCh; j++)
		{
			hraw[i][j] = (TH1D*) MakeTH1('D', Form("raw/Mod%dCh%d", i, j), Form("Mod %d Ch %d", i, j), 5000, 0, 5000, "channels");
		}
	}
	for(int i=0; i<NMo; i++)
		hcount[i] = (TH1I*) MakeTH1('I', Form("Count/Mod%d", i), Form("Mod = %d", i), 32, 0, 32, "ch");
	nE = 0;
}

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

	vFATIMA.clear();
	vBp.clear();
	vWR.clear();

	vFATIMACal.clear();
	vBpCal.clear();
}
//-----------------------------------------------------------
// event function
Bool_t Proc::BuildEvent(TGo4EventElement* target)
{
	Bool_t isValid=kFALSE; // validity of output event
	TGo4MbsEvent* inp_evt = (TGo4MbsEvent* ) GetInputEvent(); // from this
	if (inp_evt == 0) 
	{
		std::cout << "SCNUnpackProc: no input event !"<< std::endl;
		return isValid;
	}
	isValid = kTRUE;
	inp_evt->ResetIterator();
	TGo4MbsSubEvent* psubevt(0);
	while ((psubevt = inp_evt->NextSubEvent()) != 0) // subevent loop
	{
		nE++;
//		if(nE > 4) exit(0);
		int* pdata = psubevt->GetDataField();
		Int_t lwords = psubevt->GetIntLen();

		int PrcID = psubevt->GetProcid();  //Crate Id
		Init();
#if(UsingWhiteRabbit)
		decodeWR->Decode(pdata);
//		std::cout << decodeWR->Id << "  " << decodeWR->Name <<"   " << decodeWR->ts << std::endl;
		WhiteRabbitData wrd;
		wrd.id = decodeWR->Id;
		wrd.Name = decodeWR->Name;
		wrd.ts = decodeWR->ts;
		vWR.push_back(wrd);
		pdata = decodeWR->GetPdata();
#endif

		switch(PrcID)
		{
			case 20: //It's the QDC. // 
				decodeQDC->Decode(pdata, NMo);
				for(Int_t i=0; i<NMo; i++) //Loop module
				{
					for(Int_t j=0; j<NCh; j++)
					{
						if(i == 1)
							raw[i][j] = CalYSO[1][j] * (decodeQDC->raw[i][j][0] - CalYSO[0][j]);
						else
							raw[i][j] = decodeQDC->raw[i][j][0];
					}
				}
				pdata = decodeQDC->GetPdata();
				break;
			case 90: //It's the TAMAX Beta plastic 
				std::cout << "This is AIDA, PrcId = " << PrcID << std::endl;
				break;
			case 80: //It's the TAMAX Beta plastic 
      				DecodeTBp->ProcessMBS(pdata);
//				DecodeTBp->PrintEvent();
				for(int i=0; i<int(DecodeTBp->TamexN); i++)
				{
					for(int j=0; j<32; j+=2)
					{
						for(int k=0; k<std::min(DecodeTBp->CountChId[i][j], DecodeTBp->CountChId[i][j+1]); k++)
						{
							TamexData td;
							td.cid = i;
							td.ch = j/2;
							td.ts = CalTamexts(DecodeTBp->LeadEdgeCoarse[i][j+1][k], DecodeTBp->LeadEdgeFine[i][j+1][k]);
							td.ToTs = CalTamexToT(DecodeTBp->LeadEdgeCoarse[i][j+1][k] - DecodeTBp->LeadEdgeFine[i][j+1][k], DecodeTBp->TrailEdgeCoarse[i][j+1][k] - DecodeTBp->TrailEdgeFine[i][j+1][k]);
							td.tf = CalTamexts(DecodeTBp->LeadEdgeCoarse[i][j][k], DecodeTBp->LeadEdgeFine[i][j][k]);
							td.ToTf = CalTamexToT(DecodeTBp->LeadEdgeCoarse[i][j][k] - DecodeTBp->LeadEdgeFine[i][j][k], DecodeTBp->TrailEdgeCoarse[i][j][k] - DecodeTBp->TrailEdgeFine[i][j][k]);
							vBp.push_back(td);
#if(GetTamexFineTimeCaliPar)
							TamexFineTimeCalData tft;
							tft.cid = i;
							tft.ch = j;
							tft.ts = DecodeTBp->LeadEdgeFine[i][j][k];
							vBpCal.push_back(tft);
							tft.ch = j + 1;
							tft.ts = DecodeTBp->LeadEdgeFine[i][j+1][k];
							vBpCal.push_back(tft);
#endif
						}
					}
				}
      				pdata = DecodeTBp->GetPdata();
				break;
			case 75: //It's the TAMAX FATIMA
   //   			DecodeTFatima->ProcessMBS(psubevt);
      //				DecodeTFatima->ProcessTamexNew(pdata);
      				DecodeTFatima->ProcessMBS(pdata);
      				pdata = DecodeTFatima->GetPdata();
//				DecodeTFatima->PrintEvent();
				for(int i=0; i<int(DecodeTFatima->TamexN); i++)
				{
					for(int j=0; j<32; j+=2)
					{
						for(int k=0; k<std::min(DecodeTFatima->CountChId[i][j], DecodeTFatima->CountChId[i][j+1]); k++)
						{
							TamexData td;
							td.cid = i;
							td.ch = j/2;
							td.ts = CalTamexts(DecodeTFatima->LeadEdgeCoarse[i][j+1][k], DecodeTFatima->LeadEdgeFine[i][j+1][k]);
							td.ToTs = CalTamexToT(DecodeTFatima->LeadEdgeCoarse[i][j+1][k] - DecodeTFatima->LeadEdgeFine[i][j+1][k], DecodeTFatima->TrailEdgeCoarse[i][j+1][k] - DecodeTFatima->TrailEdgeFine[i][j+1][k]);
							td.tf = CalTamexts(DecodeTFatima->LeadEdgeCoarse[i][j][k], DecodeTFatima->LeadEdgeFine[i][j][k]);
							td.ToTf = CalTamexToT(DecodeTFatima->LeadEdgeCoarse[i][j][k] - DecodeTFatima->LeadEdgeFine[i][j][k], DecodeTFatima->TrailEdgeCoarse[i][j][k] - DecodeTFatima->TrailEdgeFine[i][j][k]);
							vFATIMA.push_back(td);
#if(GetTamexFineTimeCaliPar)
							TamexFineTimeCalData tft;
							tft.cid = i;
							tft.ch = j;
							tft.ts = DecodeTFatima->LeadEdgeFine[i][j][k];
							vFATIMACal.push_back(tft);
							tft.ch = j + 1;
							tft.ts = DecodeTFatima->LeadEdgeFine[i][j+1][k];
							vFATIMACal.push_back(tft);
#endif
						}
					}
				}
				break;
			default :
				std::cout << "Don't know PrcId = " << PrcID  << std::endl;
				break;
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
Double_t Proc::CalTamexToT(Double_t lt, Double_t tt)
{
	Double_t Totv = -1.0E6;
	if(abs(tt-lt) > double(COARSE_CT_RANGE >> 1))
		Totv = tt - lt + COARSE_CT_RANGE;
	else
		Totv = tt - lt;
	return Totv * 5.0; // ns
}
Double_t Proc::CalTamexts(Double_t ct, Double_t ft)
{
	return (ct - ft) * 5.0; //ns
}
//----------------------------END OF GO4 SOURCE FILE ---------------------
