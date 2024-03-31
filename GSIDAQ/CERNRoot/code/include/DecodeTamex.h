#ifndef DecodeTamex_HH
#define DecodeTamex_HH

#include "Data.h"
#include "DecodeWhiteRabbit.h"

#include "TString.h"
#include "TH1I.h"
#include "vector"
#include "map"

#define CYCLE_TIME  (Double_t) 5000
#define COARSE_CT_RANGE  0x800  // 11 bits

extern unsigned int DoTamexFineTimeCali;
class DecodeTamex
{
	public:
		DecodeTamex(TString CalName, TString TamexFlag = "nsTamex");
		virtual ~DecodeTamex();
		void InitEdges();
		void Decode(int* pdata_);
		void DecodeTamexData();
		void GetTrigger();
		void GetEdges();
		bool NoErrorReached();
		void CheckTrailer();
		void PrintEvent();
		int* GetPdata();
		void CaliFineTime();
        void GetCaliFineTimePar(TString name);
//Fast 1 3 5 7 9
//Slow 2 4 6 8 10
		static const int TDCBinMax = 600;
		static const int NTamexMax = 10;
		static const int TamexChNum = 32;
		double EdgeFineCal[NTamexMax][TamexChNum][TDCBinMax];


		static const int NTamexMultiMax = 500; 
		static const int NChMultiMax = 10;
		int CountChId[NTamexMax][TamexChNum];
		double LeadEdgeCoarse[NTamexMax][TamexChNum][NChMultiMax];
		double LeadEdgeFine[NTamexMax][TamexChNum][NChMultiMax];
		double TrailEdgeCoarse[NTamexMax][TamexChNum][NChMultiMax];
		double TrailEdgeFine[NTamexMax][TamexChNum][NChMultiMax];
		int TamexN;
		int TamexNN[NTamexMax];
		double EdgeCoarse[NTamexMax][NTamexMultiMax];
		double EdgeFine[NTamexMax][NTamexMultiMax];
		int EdgeChId[NTamexMax][NTamexMultiMax];
        WhiteRabbitData wrd;
        std::vector<TamexData> vData;
        std::multimap<ULong64_t, TamexChannelData> RebuildEvent[NTamexMax][TamexChNum];
	private:
        void ArrangeData();
		void SkipPadding();
        Double_t CalToT(Double_t lt, Double_t tt);
        Double_t  Calts(Double_t ct, Double_t ft);
        TString TamexFlag;
		int* pdata;

		bool TamexEnd;
		bool NoEdges[NTamexMultiMax];
		int TamexId[NTamexMultiMax];
		int TriggerType[NTamexMultiMax];
		int SfpId[NTamexMultiMax];
		int AmFired[NTamexMultiMax];
		double TriggerCoarseT[NTamexMultiMax];
		double TriggerFineT[NTamexMultiMax];
		unsigned int TriggerChId[NTamexMultiMax];
		bool Writter;	

		ULong64_t PreTriggerTime;
		ULong64_t PostTriggerTime;

		int ErrorCode;
		ULong64_t TrailerCode;
		bool leading_hit;

		ULong64_t tStart;

        TH1I *ht[NTamexMax][TamexChNum];
};

#endif
