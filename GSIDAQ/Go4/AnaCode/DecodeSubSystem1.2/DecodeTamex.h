
#ifndef DecodeTamex_HH
#define DecodeTamex_HH

#include "TGo4UserException.h"
#include "TGo4Picture.h"
#include "TGo4MbsEvent.h"
#include "TGo4EventProcessor.h"
#include "Data.h"
#include "TString.h"


class DecodeTamex
{
	public:
		DecodeTamex(TString CalName);
		virtual ~DecodeTamex();
		void InitEdges();
    		void ProcessMBS(TGo4MbsSubEvent* psubevt){};
		void ProcessMBS(int* pdata_);
		void ProcessTamex();
		void SkipPadding();
		void GetTrigger();
		void GetEdges();
		bool NoErrorReached();
		void CheckTrailer();
		void PrintEvent();
		int* GetPdata();
		void CaliFineTime();
//Fast 1 3 5 7 9
//Slow 2 4 6 8 10
		static const int TDCBinMax = 600;
		static const int NTamexMax = 500;
		static const int TamexChNum = 32;
		static const int NTamexMultiMax = 500; 
		static const int NChMultiMax = 10;
		int CountChId[NTamexMax][TamexChNum];
		double EdgeFineCal[NTamexMax][TamexChNum][TDCBinMax];
		double LeadEdgeCoarse[NTamexMax][TamexChNum][NChMultiMax];
		double LeadEdgeFine[NTamexMax][TamexChNum][NChMultiMax];
		double TrailEdgeCoarse[NTamexMax][TamexChNum][NChMultiMax];
		double TrailEdgeFine[NTamexMax][TamexChNum][NChMultiMax];
		int TamexN;
		int TamexNN[NTamexMax];
		double EdgeCoarse[NTamexMax][NTamexMultiMax];
		double EdgeFine[NTamexMax][NTamexMultiMax];
		int EdgeChId[NTamexMax][NTamexMultiMax];
	private:
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
		int TamexIdentifier;

		ULong64_t PreTriggerTime;
		ULong64_t PostTriggerTime;

		int Add;
		int aa;
		int ErrorCode;
		int SixFired;
		ULong64_t TrailerCode;
		bool leading_hit;
};

#endif
