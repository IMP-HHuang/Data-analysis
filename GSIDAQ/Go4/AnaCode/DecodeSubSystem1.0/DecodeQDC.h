#ifndef DecodeQDC_HH
#define DecodeQDC_HH

#include "TTree.h"
#include "Data.h"

class DecodeQDC
{
	public:
		DecodeQDC();
		virtual ~DecodeQDC();
		int* GetPdata();
		void Decode(int* pdata_, int NModule);
		static const int NModuleMax = 20;
		static const int NChannel = 32;
		static const int NMultiMax = 10;
		double raw[NModuleMax][NChannel][NMultiMax];
		int NQDCCh[NModuleMax][NChannel];
	private:
		int* pdata;
		Int_t geo_ad;
		Int_t EventCounter;

		void Init();
};

#endif
