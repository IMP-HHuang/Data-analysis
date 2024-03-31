#ifndef Data_HH
#define Data_HH

#include <iostream>
#include <string>
#include <sstream>
#include "TString.h"
//Data structure used in ana

typedef struct DetectorInf_
{
	TString Name;
    TString Label;
    std::string Module;
	int NumberMOfodule;
	int ModuleId;
	int id;
	int WRid;
}DetectorInf;

typedef struct WhiteRabbitData_
{
	Int_t id;
	TString Name;
	ULong64_t ts;
}WhiteRabbitData;


typedef struct FebexData_
{
	Int_t mid;
	Int_t ch;
	Int_t raw;
	ULong64_t ts;
	Int_t Pileup;
	Int_t Overf;
} FebexData;

typedef struct FEEData_
{
	Int_t mid;
	Int_t ch;
	Int_t raw;
	ULong64_t ts;
	Int_t ver;
	Int_t range;
}FEEData;

struct DataAll
{
	unsigned int value   : 32;
};

struct DataFEE
{
	unsigned int data    : 16;
	unsigned int ch      : 6;
	unsigned int mid     : 6;
	unsigned int range   : 1;
	unsigned int vernier : 1;
	unsigned int type    : 2;
};

struct DataFEETimeLow
{
	unsigned int tsl     : 28;
	unsigned int flag    : 4;
};

struct DataInfFEE
{
	unsigned int Field   : 20;
	unsigned int code    : 4;
	unsigned int mid     : 6;
	unsigned int Empty   : 2;
};

struct DataChannelFebex
{
	unsigned int chtsh   : 16;
	unsigned int ch      : 8;
	unsigned int Ident   : 8;
};

struct DataValueFebex
{
	unsigned int raw     : 24;
	unsigned int tsf     : 6;
	unsigned int pileup  : 1;
	unsigned int overf   : 1;
};

struct DataFebexEventTimeHigh
{
	unsigned int tsh     : 16;
	unsigned int Empty   : 16;
};

struct DataFebexSize
{
	unsigned int Empty   : 2;
	unsigned int Size    : 30;
};

struct DataFebexHp
{
	unsigned int hp      : 16;
	unsigned int Empty   : 16;
};

struct DataFebexHP
{
	unsigned int hp      : 16;
	unsigned int Empty   : 16;
};

typedef struct TamexData_
{
	Int_t mid;
	Int_t ch;
	Double_t ts;
	Double_t ToTs;
	Double_t tf;
	Double_t ToTf;
} TamexData;

typedef struct TamexChannelData_
{
	Int_t mid;
	Int_t ch;
	Double_t wrts;
} TamexChannelData;

typedef struct QDCData_
{
	Int_t mid;
	Int_t ch;
	Double_t raw;
} QDCData;

typedef struct TamexFineTimeCalData_
{
	Int_t mid;
	Int_t ch;
	Double_t ts;
} TamexFineTimeCalData;

// Data format of subsystem
struct DataQDCHeader
{
  unsigned int empty : 8;
  unsigned int channels:6;
  unsigned int check1:2;
  unsigned int crate:8;
  unsigned int check2:3;
  unsigned int geo:5;
};

struct DataQDC
{
  unsigned int value:12;
  unsigned int OF:1;
  unsigned int UF:1;
  unsigned int empty:2;  
  unsigned int channel:5;
  unsigned int check:6;
  unsigned int geo:5;
};

struct DataQDCEnd
{
  unsigned int evCounter:24;
  unsigned int check:3;
  unsigned int geo:5;
};

struct DataTriggerWindow
{
	unsigned int PreTrigger  : 16;
	unsigned int PostTrigger : 16;
};

struct DataPadding
{
	unsigned int Empty  : 20;
	unsigned int Add    : 12;
};

struct DataDAQChannelHeader
{
	unsigned int Identify    : 8;
	unsigned int TriggerType : 4;
       	unsigned int SfpId       : 4;
	unsigned int TamexId     : 8;
	unsigned int Identify2   : 8;
};

struct DataTamexHeader
{
	unsigned int Identify    : 8;
	unsigned int TriggerType : 4;
       	unsigned int Empty       : 3;
       	unsigned int Nov         : 1;
	unsigned int LEC         : 16;
};

struct DataTamexTime
{
	unsigned int Identify    : 1;
	unsigned int Empty       : 2;
       	unsigned int Ch          : 7;
       	unsigned int FineT       : 10;
       	unsigned int EdgeFlag    : 1;
	unsigned int CoarseT     : 11;
};

struct DataTamexFired
{
	unsigned int AmFired : 12;
	unsigned int Empty   : 20;
};

struct DataTamexBegin
{
	unsigned long Empty : 24;
	unsigned int  aa    : 8;
};

struct DataPlaceHolder
{
	unsigned long Empty    : 28;
	unsigned int  SixEight : 4;
};

struct DataTamexError
{
	unsigned long Errorcode  : 24;
	unsigned int  ErrorValue : 8;
};

struct DataTamexTrailer
{
	unsigned long Empty   : 24;
	unsigned long Trailer : 8;
};

struct DataTamex
{
	unsigned long CoarseT : 11;
	unsigned int LeadingE : 1;
	unsigned long FineT   : 10;
	unsigned int ChId     : 7;
	unsigned int TDC      : 3;
};

struct DataWhiteRabbit
{
	unsigned int ts       : 16;  //timestamp
	unsigned int Id       : 16;
};
#endif
