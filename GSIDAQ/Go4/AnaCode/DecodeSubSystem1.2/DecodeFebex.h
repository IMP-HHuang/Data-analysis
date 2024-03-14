#ifndef DecodeFebex_HH
#define DecodeFebex_HH

#include "TTree.h"
#include "Data.h"
#include "vector"

class DecodeFebex
{
	public:
		DecodeFebex();
		virtual ~DecodeFebex();
		int* GetPdata();
		void Decode(int* pdata_, int size, ULong64_t tsHigh);
		std::vector<FebexData> vData;
	private:
		int *pdata;
		int *pdata0;
		void InitData();
};

#endif
