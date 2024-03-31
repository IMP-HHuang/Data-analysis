#ifndef DecodeFebex_HH
#define DecodeFebex_HH

#include "TTree.h"
#include "Data.h"
#include "vector"
#include "DecodeWhiteRabbit.h"
#include "map"

class DecodeFebex
{
	public:
		DecodeFebex();
		virtual ~DecodeFebex();
		int* GetPdata();
		void Decode(int* pdata_);
		void DecodeAida(int* pdata_);
		void PrintEvent();
		std::vector<FebexData> vData;
        WhiteRabbitData wrd;
		int ModN;
		static const int NFebexMax = 10;
		static const int FebexChNum = 16;
        std::multimap<ULong64_t, FebexData> RebuildEvent[NFebexMax][FebexChNum];
	private:
		void SkipPadding();
		void InitData();
		int *pdata;
		int *pdata0;
        bool EventEnd;
		ULong64_t tStart;
};

#endif
