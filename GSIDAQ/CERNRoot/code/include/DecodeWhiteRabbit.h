#ifndef DecodeWhiteRabbit_HH
#define DecodeWhiteRabbit_HH

#include "TTree.h"
#include "Data.h"

class DecodeWhiteRabbit
{
	public:
		DecodeWhiteRabbit();
		virtual ~DecodeWhiteRabbit();
		int* GetPdata();
		void Decode(int* pdata_);
		int Id;
		ULong64_t ts;
	private:
		int* pdata;

};


#endif
