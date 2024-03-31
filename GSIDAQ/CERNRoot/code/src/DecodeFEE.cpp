
#include "DecodeFEE.h"
#include "iostream"
#include "Log.h"
#include "UserDefine.h"

DecodeFEE::DecodeFEE()
{
	for(int i=0; i<NFEEMax; i++)
		for(int j=0; j<FEEChNum; j++)
        {
            RebuildEvent[i][j].clear();
        }
    tStart = 0;
}

DecodeFEE::~DecodeFEE()
{
}

int* DecodeFEE::GetPdata()
{
	return pdata;
}

void DecodeFEE::InitData()
{
	vData.clear();
    ModN = 0;
}


void DecodeFEE::Decode(int* pdata_)
{
	InitData();
	pdata = pdata_;
#if(UsingWhiteRabbit)
    DecodeWhiteRabbit *decodeWR = new DecodeWhiteRabbit();
    decodeWR->Decode(pdata);
    wrd.id = decodeWR->Id;
    wrd.ts = decodeWR->ts/5;
    pdata = decodeWR->GetPdata();
    delete decodeWR;
    if(tStart == 0)
        tStart = wrd.ts;
#endif
    while(1)
    {
        DataFEE* df = (DataFEE*)pdata;
     //   std::cout << std::hex << *pdata << std::dec <<std::endl;
        if(((*pdata >> 28) & 0xc) == int(0xc))
        {
            pdata++;
            DataFEETimeLow* ftl = (DataFEETimeLow*)pdata;
            pdata++;
            if(ftl->flag > 0)
                Log::Output("Err", "Upper 4-bits of AIDA timestamp not 0");
            if(df->mid < 1)
                Log::Output("Err", Form("ADC FEE64 Mod Number less than %d.", 1));
            if(df->ch > 63)
                 Log::Output("Err", Form("ADC FEE64 cannel Number greater than %d.", 63));
			FEEData fd;
			fd.mid = df->mid; 
			fd.ch = df->ch; 
			fd.raw = df->data;;
			fd.ts = (wrd.ts & 0xf0000000) | (ftl->tsl); // need to be operated
            fd.ver = df->vernier;
            fd.range = df->range;
            vData.push_back(fd);
        }
        else if(((*pdata >> 28) & 0xc) == int(0x4))
        {
            pdata++;
           // ("AIDA WAVE is not unpacked at the moment.");
        }
        else if(((*pdata >> 28) & 0xc) == int(0x8))
        {
            DataInfFEE* dif = (DataInfFEE*)pdata;
            pdata++;
        //    if( > Max)
          //       Log::Output("Err", Form("ADC FEE64 cannel Number greater than %d.", 63));
            switch(dif->code)
            {
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case 5:
                    break;
                case 6:
                    break;
                case 7:
                    break;
                case 8:
                    break;
                default:
                    Log::Output("Err", Form("AIDA INFO Code %d (with field %05x) is unknown.", dif->code, dif->Field));
                    break;
            }
        }
        else
        {
            Log::Output("Err", Form("Don't know FEE type of %d", (*pdata >> 28) & 0xc));
            pdata++;
            pdata++;
        }
        if(*pdata == 0) break;
    }
}

void DecodeFEE::PrintEvent()
{
    std::cout << "mid \t ch \t raw" << std::endl;
    for(int i=0; i<int(vData.size()); i++)
        std::cout << vData.at(i).mid << " \t" << vData.at(i).ch << " \t" << vData.at(i).raw  << "   " << RebuildEvent[vData.at(i).mid][vData.at(i).ch].size() << std::endl;
}
