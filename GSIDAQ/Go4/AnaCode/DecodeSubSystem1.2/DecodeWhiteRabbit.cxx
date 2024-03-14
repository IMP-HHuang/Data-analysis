#include "DecodeWhiteRabbit.h"

DecodeWhiteRabbit::DecodeWhiteRabbit()
{
}
DecodeWhiteRabbit::~DecodeWhiteRabbit()
{
}
int* DecodeWhiteRabbit::GetPdata()
{
	return pdata;
}
void DecodeWhiteRabbit::Decode(int* pdata_)
{
	ts = 0;
	Id = 0;

	this->pdata = pdata_;
	Id = int(*pdata);

	pdata++;
	DataWhiteRabbit* WRL = (DataWhiteRabbit*)pdata;

	pdata++;
	DataWhiteRabbit* WRM = (DataWhiteRabbit*)pdata;

	pdata++;
	DataWhiteRabbit* WRH = (DataWhiteRabbit*)pdata;

	pdata++;
	DataWhiteRabbit* WRX = (DataWhiteRabbit*)pdata;

//	std::cout << WRL->Id << "    " << WRM->Id << "   " << WRH->Id << "   " << WRX->Id  << std::endl;
	if(WRL->Id == int(0x03e1) &&
	   WRM->Id == int(0x04e1) &&
	   WRH->Id == int(0x05e1) &&
	   WRX->Id == int(0x06e1))
	{
		ts = ((ULong64_t) WRL->ts)
            	   + (((ULong64_t) WRM->ts) << 16)
            	   + (((ULong64_t) WRH->ts) << 32)
            	   + (((ULong64_t) WRX->ts) << 48);
	}

	pdata++;
}

