
#ifndef DecodeFEE_HH
#define DecodeFEE_HH

#include "TTree.h"
#include "Data.h"
#include "vector"
#include "DecodeWhiteRabbit.h"
#include "map"

class DecodeFEE
{
	public:
		DecodeFEE();
		virtual ~DecodeFEE();
		int* GetPdata();
		void Decode(int* pdata_);
		void PrintEvent();
		std::vector<FEEData> vData;
        WhiteRabbitData wrd;
		int ModN;
		static const int NFEEMax = 20;
		static const int FEEChNum = 64;
        std::multimap<ULong64_t, FEEData> RebuildEvent[NFEEMax][FEEChNum];
	private:
		void InitData();
		int *pdata;
        bool EventEnd;
		ULong64_t tStart;
};

#endif
