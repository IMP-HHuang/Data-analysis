#include "DecodeQDC.h"

DecodeQDC::DecodeQDC()
{
}

DecodeQDC::~DecodeQDC()
{

}
int* DecodeQDC::GetPdata()
{
	return pdata;
}

void DecodeQDC::Init()
{
	for(int i=0; i<NModuleMax; i++)
		for(int j=0; j<NChannel; j++)
		{
			NQDCCh[i][j] = 0;
			for(int k=0; k<NMultiMax; k++)
			{
				raw[i][j][k] = 0.0;
			}
		}
}

void DecodeQDC::Decode(int* pdata_, int NModule)
{
	this->pdata = pdata_;

	Init();

	for(Int_t i=0; i<NModule; i++) //Loop module
	{
		DataQDCHeader *h = (DataQDCHeader*)pdata;
		if(h->check2 != 2) 
		{
			std::cout<<"There's a problem reading the header of QDC"<< std::endl;
			break;
		}
		geo_ad = h->geo;  
		// i      0    1    2    3
		// geo_ad 6    10   8    13
		// modul  QDC2 QDC1 ADC  TDC
		Int_t ChannelNumber = h->channels;

		pdata++; 
		for(Int_t j=0; j<ChannelNumber; j++)
		{
			DataQDC *d = (DataQDC*)pdata;
			if(d->check != 0)
			{
				std::cout<<"There's a problem reading the data"<< std::endl;
				break;
			}
			if(d->value > raw[i][d->channel][0])
			{
				raw[i][d->channel][NQDCCh[i][d->channel]] = raw[i][d->channel][0];
				raw[i][d->channel][0] = d->value;
			}
			else
				raw[i][d->channel][NQDCCh[i][d->channel]] = d->value;

			NQDCCh[i][j]++;

			pdata++; 
		}
		DataQDCEnd *e = (DataQDCEnd*)pdata;
		if(e->check!=4)
		{
			std::cout << "There was a problem reading the ending"<< std::endl;
			break;
		}
		EventCounter = e->evCounter;

		pdata++;
	}
}
