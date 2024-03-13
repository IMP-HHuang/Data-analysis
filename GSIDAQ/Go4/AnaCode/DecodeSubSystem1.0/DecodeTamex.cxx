#include "DecodeTamex.h"
#include "iostream"
#include "fstream"
#include "UserDefine.h"

DecodeTamex::DecodeTamex(TString CalName)
{
	Add = 2781;
	TamexIdentifier  = 52;
	ErrorCode = 238;
	aa = 170;
	SixFired = 6;
	TrailerCode = 187;
	TamexN = 0;
	pdata = nullptr;

	for(int i=0; i<NTamexMax; i++)
		for(int j=0; j<TamexChNum; j++)
			CountChId[i][j] = 0.0;

//	std::ifstream ipf("TamexFineTDCCal.txt");
	std::ifstream ipf(CalName.Data());
	if(!ipf.is_open())
		std::cout << "Cannot open TDC Cal parameter file" << std::endl;
	else
	{
		std::string ss;
		std::getline(ipf, ss);
		int cid, ch, bin;
	       	double val;
		while(ipf >> cid >> ch >> bin >> val)
		{
			EdgeFineCal[cid][ch][bin] = val;
//			std::cout << cid << ch << bin << val << std::endl;
		}
	}
	ipf.close();
}

DecodeTamex::~DecodeTamex()
{}

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
//		std::cout << "   padding " << std::endl;
		DataPadding *pd = (DataPadding*) pdata;
		if(pd->Add == Add)
			pdata++;
		else
			break;
	}
}

bool DecodeTamex::NoErrorReached()
{
	DataTamexError *te = (DataTamexError*) pdata;
	return te->ErrorValue != ErrorCode;
}

void DecodeTamex::GetEdges()
{
	TamexNN[TamexN] = 0;
	Writter = false;

	bool CheckLF = false;
	while(NoErrorReached())
	{
		DataPlaceHolder * ph = (DataPlaceHolder*) pdata;
		if(ph->SixEight == SixFired)
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
			std::cout << "The count of signal for each Tamex (TamexNN) is over" << std::endl;
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
	if(tt->Trailer != TrailerCode)
		std::cout << "\033[31m Unknown Tamex trailer format";
}


void DecodeTamex::ProcessTamex()  //Decode Tamex board No. TamexN
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
//	std::cout << "   check ch header " << std::endl;
	// Get tamex inforamtion
	DataTamexChannelHeader * tch = (DataTamexChannelHeader*) pdata;

	//check end of Tamex MBS
	bool GoOn = (tch->Identify == TamexIdentifier &&
		     tch->Identify2 == 0 &&
		     (tch->SfpId == 1 || tch->SfpId ==0)); 
	if(!GoOn)
	{
	//	std::cout << "Tamex GoOn Error "<< tch->Identify << "   " << TamexIdentifier << "   " << tch->Identify2 << "   " << tch->SfpId<< std::endl;
		TamexEnd = true;
		return;
	}
//	std::cout << "   Goon " << std::endl;
	//Ceck Id : Next id shuould larger than last one
	if(TamexN > 0 && tch->TamexId <= TamexId[TamexN-1])
	{
		TamexEnd = true;
		return;
	}
//	std::cout << "   check channel " << std::endl;
	SfpId[TamexN] = tch->SfpId;
	TriggerType[TamexN] = tch->TriggerType;
	TamexId[TamexN] = tch->TamexId;

	pdata++;
	DataTamexFired * tf = (DataTamexFired*) pdata;
	AmFired[TamexN] = tf->AmFired/4 - 2;
	if(AmFired[TamexN] < 0)
		std::cout << "\033[31m Negative Tamex Fired \033[0m" << std::endl; 

	pdata++;
	DataTamexBegin* tb = (DataTamexBegin*) pdata; 
	if(tb->aa != aa)
		std::cout << "\033[31m Error in Tamex format !" << std::endl;

	pdata++;
	GetTrigger();

	if(AmFired[TamexN] > 3)
		GetEdges();
	else
		NoEdges[TamexN] = true;
	CheckTrailer();
}


void DecodeTamex::ProcessMBS(int* pdata_)
{
//	std::cout << " Process MBS " << std::endl;
	// Get address of this event
	this->pdata = pdata_;
	//clear data of last event
	for(int i=0; i<TamexN; i++)
		TamexNN[i] = 0;
	InitEdges();
	TamexEnd = false;
	TamexN = 0;

	while(!TamexEnd)
	{
		ProcessTamex(); // Decode one board
		if(!TamexEnd)
		{
			if(TamexN > NTamexMultiMax)
			{
				std::cout << "The count of Multi signal (TamexN) is over" << std::endl;
				exit(0);
			}
			TamexN++;
		}
		pdata++;
	}
#if(!GetTamexFineTimeCaliPar)
	CaliFineTime();
#endif
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
	std::cout << PreTriggerTime << " Trigger Time   " << PostTriggerTime << std::endl;
	for(int i=0; i<TamexN; i++) //card number
	{
		std::cout  << " Tamex No. "<< i << PostTriggerTime << std::endl;
		std::cout << TamexNN[i] << " \t" << TamexId[i] << " \t " << TriggerChId[i]<< " \t"<< TriggerCoarseT[i] << " \t " << TriggerFineT[i] << std::endl;
		for(int j=0; j<TamexNN[i]; j++)
			std::cout << EdgeChId[i][j] << " \t" << EdgeCoarse[i][j] << " \t" << EdgeFine[i][j] << std::endl;
	}
	for(int i=0; i<TamexN; i++) //card number
		for(int j=0; j<TamexChNum; j++)
		{
			if(CountChId[i][j] > 0)
				std::cout  << " Tamex No. "<< i << "     Ch No."<< std::endl;
			for(int k=0; k<CountChId[i][j]; k++)
			{
				std::cout << i << " \t" << j << " \t" << LeadEdgeCoarse[i][j][k] << " \t" << " \t" << LeadEdgeFine[i][j][k] << " \t" << TrailEdgeCoarse[i][j][k] << " \t" << TrailEdgeFine[i][j][k] << std::endl;
				std::cout << i << " \t" << j << " \t" << LeadEdgeCoarse[i][j][k] - LeadEdgeFine[i][j][k] << " \t" << TrailEdgeCoarse[i][j][k] - TrailEdgeFine[i][j][k] << std::endl;
			}
		}
}
