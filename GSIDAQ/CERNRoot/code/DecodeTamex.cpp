#include "DecodeTamex.h"
#include "iostream"
#include "fstream"
#include "UserDefine.h"
#include "Log.h"

DecodeTamex::DecodeTamex(TString CalName, TString TamexFlag_)
{
	TrailerCode = 187;
	TamexN = 0;
	pdata = nullptr;
    tStart = 0;
    TamexFlag = TamexFlag_;

	for(int i=0; i<NTamexMax; i++)
		for(int j=0; j<TamexChNum; j++)
        {
			CountChId[i][j] = 0.0;
            RebuildEvent[i][j].clear();
            for(int k =0; k<TDCBinMax; k++)
			EdgeFineCal[i][j][k] = 0.0;
        }

    if(DoTamexFineTimeCali)
    {
	    for(int i=0; i<NTamexMax; i++)
		    for(int j=0; j<TamexChNum; j++)
            {
                ht[i][j] = new TH1I(Form("%sMod%dch%d", CalName.Data(), i, j), Form("%sMod%dch%d", CalName.Data(), i, j), TDCBinMax, 0, TDCBinMax);
                ht[i][j]->SetDirectory(0);
            }
    }
    else
    {
        CalName.ReplaceAll(" ",""); //replace // with /
	    std::ifstream calipf(Form("./Par/%sTamexFineTDCCal.txt", CalName.Data()));
	    if(!calipf.is_open())
		    Log::Output("Err", Form("Cannot open TDC Cal parameter file named %s", Form("./Par/%sTamexFineTDCCal.txt", CalName.Data())));
	    else
	    {
		    std::string ss;
		    std::getline(calipf, ss);
		    int cid, ch, bin;
	       	    double val;
		    while(calipf >> cid >> ch >> bin >> val)
		    {
			    EdgeFineCal[cid][ch][bin] = val;
		    }
	        calipf.close();
	    }
    }
}

DecodeTamex::~DecodeTamex()
{
    if(DoTamexFineTimeCali)
    {
        for(int i=0; i<NTamexMax; i++)
		    for(int j=0; j<TamexChNum; j++)
                ht[i][j]->Delete();
    }
}

void DecodeTamex::InitEdges()
{
	for(int i=0; i<NTamexMax; i++)
		for(int j=0; j<NTamexMultiMax; j++)
		{
			EdgeCoarse[i][j] = -1;
			EdgeFine[i][j] = -1;
			EdgeChId[i][j] = -1;
		}
	for(int i=0; i<NTamexMax; i++)
		for(int j=0; j<TamexChNum; j++)
			CountChId[i][j] = 0;
}

void DecodeTamex::GetTrigger()
{
	DataPlaceHolder * ph = (DataPlaceHolder*) pdata;
	//New
	if(ph->SixEight != 6) return;

	pdata++;	
	DataTamex *dt = (DataTamex*) pdata;
	TriggerCoarseT[TamexN] = (double)dt->CoarseT;
	TriggerFineT[TamexN] = (double)dt->FineT;
	TriggerChId[TamexN] = (double)dt->ChId;

	pdata++;
}

void DecodeTamex::SkipPadding()
{
	while(1)
	{
		DataPadding *pd = (DataPadding*) pdata;
		if(pd->Add == int(0xadd)) // 2781
			pdata++;
		else
			break;
	}
}

bool DecodeTamex::NoErrorReached()
{
	DataTamexError *te = (DataTamexError*) pdata;
	return te->ErrorValue != int(0xee);  // 238
}

void DecodeTamex::GetEdges()
{
	TamexNN[TamexN] = 0;
	Writter = false;

	bool CheckLF = false;
	while(NoErrorReached())
	{
		DataPlaceHolder * ph = (DataPlaceHolder*) pdata;
		if(ph->SixEight == int(0x6))  // 6
		{
			pdata++;
			continue;
		}
		else
			Writter = false;
		DataTamex *dt = (DataTamex*) pdata;
		if(dt->LeadingE == 1)
		{
			leading_hit = dt->LeadingE;
			EdgeCoarse[TamexN][TamexNN[TamexN]] = (double) dt->CoarseT;
			EdgeFine[TamexN][TamexNN[TamexN]] = (double) dt->FineT;
			EdgeChId[TamexN][TamexNN[TamexN]] = dt->ChId;;

			LeadEdgeCoarse[TamexN][dt->ChId-1][CountChId[TamexN][dt->ChId-1]] = (double) dt->CoarseT;
			LeadEdgeFine[TamexN][dt->ChId-1][CountChId[TamexN][dt->ChId-1]] =  (double) dt->FineT;
            TamexChannelData tcd;
            tcd.mid = TamexN;
            tcd.ch = dt->ChId-1;
            tcd.wrts = Double_t(wrd.ts);
#if(UsingWhiteRabbit)
            RebuildEvent[TamexN][dt->ChId-1].insert(std::make_pair(wrd.ts, tcd));
            auto it = RebuildEvent[TamexN][dt->ChId-1].lower_bound(ULong64_t(wrd.ts - 1E9));
            if(it != RebuildEvent[TamexN][dt->ChId-1].begin())
            {
                std::advance(it, 1);
                RebuildEvent[TamexN][dt->ChId-1].erase(RebuildEvent[TamexN][dt->ChId-1].begin(), it);
            }
       //     if(RebuildEvent[TamexN][dt->ChId-1].size() > 1)
        //        std::cout <<(RebuildEvent[TamexN][dt->ChId-1].rbegin()->first - tStart)/1E9 << " eraser  time " << (RebuildEvent[TamexN][dt->ChId-1].begin()->first - tStart)/1E9 << "  " << (wrd.ts-RebuildEvent[TamexN][dt->ChId-1].begin()->first)/1E9 << "  " << RebuildEvent[TamexN][dt->ChId-1].size()<< std::endl;
#endif
			CheckLF = true;
		}
		if(dt->LeadingE == 0)
		{
			leading_hit = dt->LeadingE;
			EdgeCoarse[TamexN][TamexNN[TamexN]] = (double) dt->CoarseT;
			EdgeFine[TamexN][TamexNN[TamexN]] = (double) dt->FineT;
			EdgeChId[TamexN][TamexNN[TamexN]] = dt->ChId;;
			if(CheckLF)
			{
				TrailEdgeCoarse[TamexN][dt->ChId-1][CountChId[TamexN][dt->ChId-1]] = (double) dt->CoarseT;
				TrailEdgeFine[TamexN][dt->ChId-1][CountChId[TamexN][dt->ChId-1]] = (double) dt->FineT;
				CountChId[TamexN][dt->ChId-1]++;
				CheckLF = false;
			}
		}
		TamexNN[TamexN]++;
		if(TamexNN[TamexN] > NTamexMultiMax)
		{
			Log::Output("Err", "The count of signal for each Tamex (TamexNN) is over");
			exit(0);
		}
		Writter = true;
		pdata++;
	}
}

void DecodeTamex::CheckTrailer()
{
	pdata++;
	DataTamexTrailer * tt = (DataTamexTrailer*) pdata;
	if(tt->Trailer != TrailerCode)  // 187
		Log::Output("Err", "Unknown Tamex trailer format");
}


void DecodeTamex::DecodeTamexData()  //Decode Tamex board No. TamexN
{
	TamexNN[TamexN] = 0;
	NoEdges[TamexN] = false;
	Writter = false;
	//First Tamex signal check triggle window
	if(TamexN == 0)
	{
		// Get Pre and post triggle time
		DataTriggerWindow *tw = (DataTriggerWindow*) pdata;
		PreTriggerTime = tw->PreTrigger;
		PostTriggerTime = tw->PostTrigger;
		pdata++;

		SkipPadding();
	}
	// Get tamex inforamtion
	DataDAQChannelHeader * dch = (DataDAQChannelHeader*) pdata;

	//check end of Tamex MBS
	bool GoOn = (dch->Identify == int(0x34) //52
		        && dch->Identify2 == 0
		        && (dch->SfpId == 1 || dch->SfpId ==0)); 
	if(!GoOn)
	{
		TamexEnd = true;
		return;
	}
	//Ceck Id : Next id shuould larger than last one
	if(TamexN > 0 && dch->TamexId <= TamexId[TamexN-1])
	{
		TamexEnd = true;
		return;
	}
	SfpId[TamexN] = dch->SfpId;
	TriggerType[TamexN] = dch->TriggerType;
	TamexId[TamexN] = dch->TamexId;

	pdata++;
	DataTamexFired * tf = (DataTamexFired*) pdata;
	AmFired[TamexN] = tf->AmFired/4 - 2;
	if(AmFired[TamexN] < 0)
		Log::Output("Err", "Negative Tamex Fired"); 

	pdata++;
	DataTamexBegin* tb = (DataTamexBegin*) pdata; 
	if(tb->aa != int(0xaa)) // 170
		Log::Output("Err", "Error in Tamex format !");

	pdata++;
	GetTrigger();

	if(AmFired[TamexN] > 3)
		GetEdges();
	else
		NoEdges[TamexN] = true;
	CheckTrailer();
}


void DecodeTamex::Decode(int* pdata_)
{
	// Get address of this event
	this->pdata = pdata_;

    vData.clear();

#if(UsingWhiteRabbit)
    DecodeWhiteRabbit *decodeWR = new DecodeWhiteRabbit();
    decodeWR->Decode(pdata);
    wrd.id = decodeWR->Id;
    wrd.ts = decodeWR->ts;
    pdata = decodeWR->GetPdata();
    delete decodeWR;
    if(tStart == 0)
        tStart = wrd.ts;
#endif

	//clear data of last event
	for(int i=0; i<TamexN; i++)
		TamexNN[i] = 0;
	InitEdges();
	TamexEnd = false;
	TamexN = 0;

	while(!TamexEnd)
	{
		DecodeTamexData(); // Decode one board
		if(!TamexEnd)
		{
			if(TamexN > NTamexMultiMax)
			{
				std::cout << "\033[31mThe count of Multi signal (TamexN) is over\033[0m" << std::endl;
				exit(0);
			}
			TamexN++;
		}
		pdata++;
	}
    if(DoTamexFineTimeCali)
    {
        for(int i=0; i<int(TamexN); i++)
            for(int j=0; j<TamexChNum; j++)
                for(int k=0; k<CountChId[i][j]; k++)
                    ht[i][j]->Fill(int(LeadEdgeFine[i][j][k]));
    }
    else
    {
	    CaliFineTime();
        ArrangeData();
    }
}

int* DecodeTamex::GetPdata()
{
	return pdata;
}

void DecodeTamex::CaliFineTime()
{
	for(int i=0; i<TamexN; i++) //card number
		for(int j=0; j<TamexChNum; j++)
		{
			for(int k=0; k<CountChId[i][j]; k++)
			{
				LeadEdgeFine[i][j][k] =  EdgeFineCal[i][j][int(LeadEdgeFine[i][j][k])];
				TrailEdgeFine[i][j][k] =  EdgeFineCal[i][j][int(TrailEdgeFine[i][j][k])];
			}
		}
}

void DecodeTamex::PrintEvent()
{
	std::cout << PreTriggerTime << " Trigger Time   " << PostTriggerTime << "   "<< vData.size() << std::endl;
    /*
	for(int i=0; i<TamexN; i++) //card number
	{
		std::cout  << " Tamex No. "<< i << "  All Data " << std::endl;
		std::cout << TamexNN[i] << " \t" << TamexId[i] << " \t " << TriggerChId[i]<< " \t"<< TriggerCoarseT[i] << " \t " << TriggerFineT[i] << std::endl;
		for(int j=0; j<TamexNN[i]; j++)
			std::cout << EdgeChId[i][j] << " \t" << EdgeCoarse[i][j] << " \t" << EdgeFine[i][j] << std::endl;
	}
    */
	for(int i=0; i<TamexN; i++) //card number
		for(int j=0; j<TamexChNum; j++)
		{
    //        std::cout << RebuildEvent[i][j].size() << std::endl;
			if(RebuildEvent[i][j].size() > 0)
				std::cout  << " Tamex No. "<< i << "     Ch No. " << j << "  " << RebuildEvent[i][j].size() << "/s "  << std::endl;
			for(int k=0; k<CountChId[i][j]; k++)
			{
				std::cout << i << " \t" << j << " \t" << LeadEdgeCoarse[i][j][k] << " \t" << " \t" << LeadEdgeFine[i][j][k] << " \t" << TrailEdgeCoarse[i][j][k] << " \t" << TrailEdgeFine[i][j][k] << std::endl;
				std::cout << i << " \t" << j << " \t" << LeadEdgeCoarse[i][j][k] - LeadEdgeFine[i][j][k] << " \t" << TrailEdgeCoarse[i][j][k] - TrailEdgeFine[i][j][k] << std::endl;
			}
		}
}

void DecodeTamex::ArrangeData()
{
    for(int i=0; i<int(TamexN); i++)
    {
        for(int j=0; j<32; j+=2)
        {
            for(int k=0; k<std::min(CountChId[i][j], CountChId[i][j+1]); k++)
            {
                TamexData td;
                td.mid = i;
                td.ch = j/2;
                td.ts = Calts(LeadEdgeCoarse[i][j+1][k], LeadEdgeFine[i][j+1][k]);
                td.ToTs = CalToT(LeadEdgeCoarse[i][j+1][k] - LeadEdgeFine[i][j+1][k], TrailEdgeCoarse[i][j+1][k] - TrailEdgeFine[i][j+1][k]);
                td.tf = Calts(LeadEdgeCoarse[i][j][k], LeadEdgeFine[i][j][k]);
                td.ToTf = CalToT(LeadEdgeCoarse[i][j][k] - LeadEdgeFine[i][j][k],                                  TrailEdgeCoarse[i][j][k] - TrailEdgeFine[i][j][k]);
                vData.push_back(td);

            }
        }
    }
}

Double_t DecodeTamex::CalToT(Double_t lt, Double_t tt)
{
    Double_t Totv = -1.0E6;
    if(abs(tt-lt) > double(COARSE_CT_RANGE >> 1))
        Totv = tt - lt + COARSE_CT_RANGE;
    else
        Totv = tt - lt;
    return Totv * 5.0; // ns
}

Double_t DecodeTamex::Calts(Double_t ct, Double_t ft)
{
    return (ct - ft) * 5.0; //ns
}

void DecodeTamex::GetCaliFineTimePar(TString name)
{
    name.ReplaceAll(" ",""); //replace // with /
    std::ofstream calopf(Form("./Par/%sTamexFineTDCCal.txt", name.Data()));
	calopf << "TamexId \t ChId \t BinId \t Vlaue (5 ns)" << std::endl;
	for(int i=0; i<NTamexMax; i++)
		for(int j=0; j<TamexChNum; j++)
        {
            int nSum = ht[i][j]->GetEntries();
            if(nSum<=0) continue;
            int n = 0;
            for(int k=0; k<TDCBinMax; k++)
            {
                n += ht[i][j]->GetBinContent(k+1);
                if(n > 0)
                    calopf << i << " \t" << j << " \t" << k << " \t" << double(n)/double(nSum) << std::endl;
            }
        }
    calopf.close();
    Log::Output("Inf", Form("Save Fine Time calibration parameter file of ./Par/%sTamexFineTDCCal.txt", name.Data()));
}

