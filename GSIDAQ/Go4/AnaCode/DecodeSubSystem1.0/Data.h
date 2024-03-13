#ifndef Data_HH
#define Data_HH

#include <iostream>
#include <string>
#include <sstream>

struct QDCHeader
{
  unsigned int empty : 8;
  unsigned int channels:6;
  unsigned int check1:2;
  unsigned int crate:8;
  unsigned int check2:3;
  unsigned int geo:5;
};

struct QDCData
{
  unsigned int value:12;
  unsigned int OF:1;
  unsigned int UF:1;
  unsigned int empty:2;  
  unsigned int channel:5;
  unsigned int check:6;
  unsigned int geo:5;
};

struct QDCEnd
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

struct DataTamexChannelHeader
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
