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

#include "Param.h"
#include "Proc.h"
#include "Data.h"
#include "UserDefine.h"

//***********************************************************
// this one is used in standard factory
Proc::Proc(const char* name) : TGo4EventProcessor(name)
{
	InitDecode();
	InitData();
	ReadPar();
	Initopf();
	CreateOnline();
	nE = 0;
	std::cout << "**** Proc: Create instance " << name << std::endl;
}

Proc::~Proc()
{
	opf->cd();
	opt->Write();
	opf->Close();
	std::cout << "**** Proc: Delete instance " << std::endl;
}

void Proc::CreateOnline()
{
	//Go4 
	for(int i=0; i<NMo; i++)
	{
		for(int j=0; j<NCh; j++)
		{
			hraw[i][j] = (TH1D*) MakeTH1('D', Form("raw/Mod%dCh%d", i, j), Form("Mod %d Ch %d", i, j), 5000, 0, 5000, "channels");
		}
	}
	for(int i=0; i<NMo; i++)
		hcount[i] = (TH1I*) MakeTH1('I', Form("Count/Mod%d", i), Form("Mod = %d", i), 32, 0, 32, "ch");

	// httpServer
	/*
      	post = 8080;
      	httpServertmp = new THttpServer(Form("http:%d", post));
	for(int i=0; i<1; i++)
		for(int j=0; j<16; j++)
		{
			he[0][i][j] = new TH1D(Form("FATIMA Module%d ch%d", i, j), Form("FATIMA Module%d ch%d", i, j), 10000, 0.0, 1000.0);
        		httpServertmp->Register("Fatima/Slow/", he[0][i][j]);
		}
    	httpServertmp->ProcessRequests();
	*/
}

void Proc::ReadPar()
{
	std::string stmp, swrid;
	TString sname, slab , smod;
	int idtmp, flag;
	std::ifstream inf("./Par/DetectorInf.txt");
	if(inf.is_open())
	{
		getline(inf, stmp);
		while(inf >> sname >> idtmp >> swrid >> slab >> smod >> flag)
		{
			if(!flag)	continue;
			DetectorInf det;
			det.Name = sname;
			det.Label = slab;
			det.Module = smod;
			if(det.Module == "Tamex")
			{
				det.ModuleId = int(vDecodeT.size()); 
    				DecodeTamex* dt = new DecodeTamex(Form("./Par/%sTamexFineTDCCal.txt", sname.Data()));
				vDecodeT.push_back(dt);
				std::vector<TamexData> vt;
				vTamex.push_back(vt);
				std::vector<TamexFineTimeCalData> vtc;
				vTamexCal.push_back(vtc);
				
			}
			else if(det.Module == "Febex")
			{
				det.ModuleId = int(vDecodeF.size()); 
    				DecodeFebex* dq = new DecodeFebex();
				vDecodeF.push_back(dq);
				std::vector<FebexData> vf;
				vFebex.push_back(vf);
			}
			else if(det.Module == "QDC")
			{
				det.ModuleId = int(vDecodeQDC.size()); 
    				DecodeQDC* dq = new DecodeQDC();
				vDecodeQDC.push_back(dq);
				std::vector<QDCData> vq;
				vQDC.push_back(vq);
				std::cout << " QDC" << std::endl;
				
			}
			det.WRid = std::stoi(swrid, nullptr, 16);
			mDetector[idtmp] = det;
		}
	}
	else
	{
		std::cout << "Cannot open ./Par/DetectorInf.txt" << std::endl;
		exit(0);
	}
	inf.close();
	
	for(int i=0; i<NCh; i++)
	{
		CalYSO[0][i] = 0.0;
		CalYSO[1][i] = 0.0;
	}
	inf.open("./par/calyso.dat");
	double a0, a1;
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
		std::cout << "Cannot  open the calbration file of ./par/calyso.dat"  << std::endl;
	}
	inf.close();
}
void Proc::InitDecode()
{
    	decodeWR = new DecodeWhiteRabbit();
	decodef = new DecodeFebex();
}

void Proc::InitData()
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

	vWR.clear();

	for(int i=0; i<int(vTamex.size()); i++)
	{
		vTamex[i].clear();
		vTamexCal[i].clear();
	}
	for(int i=0; i<int(vFebex.size()); i++)
		vFebex[i].clear();
	for(int i=0; i<int(vQDC.size()); i++)
		vQDC[i].clear();
}

void Proc::Initopf()
{
	opf = new TFile("out.root", "RECREATE");
	opt = new TTree("tree", "tree");
#if(GetTamexFineTimeCaliPar)
	for(auto it = mDetector.begin(); it != mDetector.end(); it++)
		if(it->second.Module == "Tamex")
			opt->Branch(it->second.Label.Data(), &vTamexCal[it->second.ModuleId]); 				//the vector must be finished befor se branch 
#else
	opt->Branch("WR", &vWR);
	for(auto it = mDetector.begin(); it != mDetector.end(); it++)
	{
		if(it->second.Module == "Tamex")
			opt->Branch(it->second.Label.Data(), &vTamex[it->second.ModuleId]); 				//the vector must be finished befor se branch 
		if(it->second.Module == "Febex")
			opt->Branch(it->second.Label.Data(), &vFebex[it->second.ModuleId]); 				//the vector must be finished befor se branch 
		if(it->second.Module == "QDC")
			opt->Branch(it->second.Label.Data(), &vQDC[it->second.ModuleId]); 				//the vector must be finished befor se branch 
	}
#endif
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
		if(nE%10000 == 0 && nE !=0)
		{
//			httpServertmp->ProcessRequests();
//			sleep(2);  // second
		}
//		if(nE > 4) exit(0);
		int* pdata = psubevt->GetDataField();
		Int_t lwords = psubevt->GetIntLen();

		int PrcID = psubevt->GetProcid();  //Crate Id
		InitData();
		WhiteRabbitData wrd;
#if(UsingWhiteRabbit)
		decodeWR->Decode(pdata);
		wrd.id = decodeWR->Id;
		for(auto it = mDetector.begin(); it != mDetector.end(); it++)
		{
			if(it->second.WRid == wrd.id)
			{
				wrd.Name = it->second.Label;
			}
		}
		wrd.ts = decodeWR->ts;
		vWR.push_back(wrd);
		pdata = decodeWR->GetPdata();
#endif
		auto it = mDetector.find(PrcID);
		if(it != mDetector.end())
		{
			if(it->second.Module == "Tamex")
			{
      				vDecodeT[it->second.ModuleId]->ProcessMBS(pdata);
//				vDecodeT[it->second.ModuleId]->PrintEvent();
				for(int i=0; i<int(vDecodeT[it->second.ModuleId]->TamexN); i++)
				{
					for(int j=0; j<32; j+=2)
					{
						for(int k=0; k<std::min(vDecodeT[it->second.ModuleId]->CountChId[i][j], vDecodeT[it->second.ModuleId]->CountChId[i][j+1]); k++)
						{
							TamexData td;
							td.mid = i;
							td.ch = j/2;
							td.ts = CalTamexts(vDecodeT[it->second.ModuleId]->LeadEdgeCoarse[i][j+1][k], vDecodeT[it->second.ModuleId]->LeadEdgeFine[i][j+1][k]);
							td.ToTs = CalTamexToT(vDecodeT[it->second.ModuleId]->LeadEdgeCoarse[i][j+1][k] - vDecodeT[it->second.ModuleId]->LeadEdgeFine[i][j+1][k], vDecodeT[it->second.ModuleId]->TrailEdgeCoarse[i][j+1][k] - vDecodeT[it->second.ModuleId]->TrailEdgeFine[i][j+1][k]);
							td.tf = CalTamexts(vDecodeT[it->second.ModuleId]->LeadEdgeCoarse[i][j][k], vDecodeT[it->second.ModuleId]->LeadEdgeFine[i][j][k]);
							td.ToTf = CalTamexToT(vDecodeT[it->second.ModuleId]->LeadEdgeCoarse[i][j][k] - vDecodeT[it->second.ModuleId]->LeadEdgeFine[i][j][k], vDecodeT[it->second.ModuleId]->TrailEdgeCoarse[i][j][k] - vDecodeT[it->second.ModuleId]->TrailEdgeFine[i][j][k]);
							vTamex[it->second.ModuleId].push_back(td);
#if(GetTamexFineTimeCaliPar)
							TamexFineTimeCalData tft;
							tft.mid = i;
							tft.ch = j;
							tft.ts = vDecodeT[it->second.ModuleId]->LeadEdgeFine[i][j][k];
							vTamexCal[it->second.ModuleId].push_back(tft);
							tft.ch = j + 1;
							tft.ts = vDecodeT[it->second.ModuleId]->LeadEdgeFine[i][j+1][k];
#endif
						}
					}
				}
      				pdata = vDecodeT[it->second.ModuleId]->GetPdata();
			}
			else if(it->second.Module == "Febex")
			{
      				vDecodeF[it->second.ModuleId]->Decode(pdata, (psubevt->GetDlen() - 2)/2 -5, wrd.ts >> 28);
				vFebex[it->second.ModuleId].assign(vDecodeF[it->second.ModuleId]->vData.begin(), vDecodeF[it->second.ModuleId]->vData.end());
      				pdata = vDecodeF[it->second.ModuleId]->GetPdata();
			}
			else if(it->second.Module == "QDC")
			{
      				vDecodeQDC[it->second.ModuleId]->Decode(pdata, NMo);
				for(Int_t i=0; i<NMo; i++) //Loop module
				{
					for(Int_t j=0; j<NCh; j++)
					{
						if(i == 1)
							raw[i][j] = CalYSO[1][j] * (vDecodeQDC[it->second.ModuleId]->raw[i][j][0] - CalYSO[0][j]);
						else
							raw[i][j] = vDecodeQDC[it->second.ModuleId]->raw[i][j][0];
						QDCData qd;
						qd.mid = i;
						qd.ch = j;
						qd.raw = vDecodeQDC[it->second.ModuleId]->raw[i][j][0];
						vQDC[it->second.ModuleId].push_back(qd);

					}
				}
				pdata = vDecodeQDC[it->second.ModuleId]->GetPdata();
			}
		}
		else
		{
			switch(PrcID)
			{
//				case 90: //It's the FABAX AIDA
//					std::cout << "This is AIDA, PrcId = " << PrcID << "   " <<  psubevt->GetDlen()-2 << std::endl;
//					decodef->Decode(pdata, (psubevt->GetDlen() - 2)/2 -5);
//					pdata = decodef->GetPdata();
//					break;
				default :
					std::cout << "Don't know PrcId = " << PrcID  << std::endl;
					break;
			}
		}
	}
	opt->Fill();
	
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
