#include "DecodeFebex.h"
#include "iostream"

DecodeFebex::DecodeFebex()
{
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
}

void DecodeFebex::Decode(int* pdata_, int size, ULong64_t tsHigh)
{
	InitData();
	pdata = pdata_;
	pdata0 = pdata_;
	while(1)
	{
		int type = ((*pdata & 0xC0000000) >> 30);
		if(type == 2)
		{
			int mid = ((*pdata & 0x3F000000) >> 24);
			int infCode = ((*pdata & 0x00F00000) >> 20);
			int Field = (*pdata & 0x000FFFFF);
			pdata++;
			int lowTs = (*pdata & 0x0FFFFFFF);  // 28 bit at low
			pdata++;
			std::cout << " Fabex inf" << std::endl;
		}
		else if(type == 3)
		{
			FebexData fd;
			fd.data = ((*pdata & 0x0000FFFF));
			fd.mid = ((*pdata & 0x0FC00000) >> 22); 
			fd.ch = ((*pdata & 0x0003F0000) >> 16); 
			fd.veto = ((*pdata & 0x10000000) >> 28);
			// veto == 1 is Implantion 
			// veto == 0 is Decay
			pdata++;
			fd.ts = (tsHigh & 0x0000000) | (*pdata & 0x0FFFFFFF); // need to be operated
			vData.push_back(fd);

			pdata++;
		}
		else
		{
			std::cout << "Fabax : Don't know the event type of " << type << std::endl;
		}
//		exit(0);
		if(pdata - pdata0 >= size)	break;
	}
}
