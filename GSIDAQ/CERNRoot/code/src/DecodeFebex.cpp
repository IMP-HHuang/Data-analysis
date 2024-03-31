#include "DecodeFebex.h"
#include "iostream"
#include "Log.h"
#include "UserDefine.h"

DecodeFebex::DecodeFebex()
{
	for(int i=0; i<NFebexMax; i++)
		for(int j=0; j<FebexChNum; j++)
        {
            RebuildEvent[i][j].clear();
        }
    tStart = 0;
}

DecodeFebex::~DecodeFebex()
{
}

int* DecodeFebex::GetPdata()
{
	return pdata;
}

void DecodeFebex::InitData()
{
	vData.clear();
    ModN = 0;
}

void DecodeFebex::SkipPadding()
{
	while(1)
	{
		DataPadding *pd = (DataPadding*) pdata;
		if(pd->Add == int(0xadd))
			pdata++;
		else
			break;
	}
}

void DecodeFebex::Decode(int* pdata_)
{
	InitData();
	pdata = pdata_;
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
    SkipPadding();

    while(1)
    {
//        std::cout << std::hex << *pdata << std::dec <<std::endl;
        DataDAQChannelHeader* dch = (DataDAQChannelHeader*) pdata;
	    bool GoOn = (dch->Identify == int(0x34) &&
		         dch->Identify2 == int(0xff));
        if(!GoOn)
            break;
        pdata++;

        DataFebexSize* fs = (DataFebexSize*) pdata;
        pdata++;

        DataFebexEventTimeHigh* feth = (DataFebexEventTimeHigh*) pdata;
        pdata++;

        DataAll* fetl = (DataAll*) pdata;
        pdata++;

        DataFebexHp* hp = (DataFebexHp*) pdata;
        pdata++;

        DataAll* db = (DataAll*) pdata;  //deadbeef
        pdata++;
        if(dch->TriggerType == 1)
        {
            for(int i=0; i<fs->Size/4-1; i++)
            {
                DataChannelFebex* cf = (DataChannelFebex*) pdata;
                pdata++;

                DataAll* ctsl =  (DataAll*) pdata;
                pdata++;

                DataValueFebex* vf = (DataValueFebex*) pdata;
                pdata++;
			    FebexData fd;
			    fd.mid = dch->TamexId; 
			    fd.ch = cf->ch; 
			    fd.raw = vf->raw;;
                fd.ts = ((ULong64_t) ctsl->value)
                        +(((ULong64_t) cf->chtsh) << 32);
			    fd.Pileup = vf->pileup;
                fd.Overf = vf->overf;
                vData.push_back(fd);
          //      std::cout << fd.mid << "   " << fd.ch << std::endl;
#if(UsingWhiteRabbit)
                RebuildEvent[fd.mid][fd.ch].insert(std::make_pair(wrd.ts, fd));
                auto it = RebuildEvent[fd.mid][fd.ch].lower_bound(ULong64_t(wrd.ts - 1E9));
            if(it != RebuildEvent[fd.mid][fd.ch].begin())
            {
                std::advance(it, 1);
                RebuildEvent[fd.mid][fd.ch].erase(RebuildEvent[fd.mid][fd.ch].begin(), it);
            }
        //    if(RebuildEvent[fd.mid][fd.ch].size() > 1)
        //        std::cout <<(RebuildEvent[fd.mid][fd.ch].rbegin()->first - tStart)/1E9 << " eraser  time  Febex " << (RebuildEvent[fd.mid][fd.ch].begin()->first - tStart)/1E9 << "  " << (wrd.ts-RebuildEvent[fd.mid][fd.ch].begin()->first)/1E9 << "  " << RebuildEvent[fd.mid][fd.ch].size()<< std::endl;
#endif
                DataAll* Emptyd = (DataAll*) pdata;
                pdata++;
            }
        }
    }

}

void DecodeFebex::PrintEvent()
{
    std::cout << "mid \t ch \t raw" << std::endl;
    for(int i=0; i<int(vData.size()); i++)
        std::cout << vData.at(i).mid << " \t" << vData.at(i).ch << " \t" << vData.at(i).raw  << "   " << RebuildEvent[vData.at(i).mid][vData.at(i).ch].size() << std::endl;
}
