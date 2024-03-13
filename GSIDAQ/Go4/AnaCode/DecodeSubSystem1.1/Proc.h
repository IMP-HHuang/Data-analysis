#ifndef TUNPACKPROCESSOR_H
#define TUNPACKPROCESSOR_H

#include "DecodeTamex.h"
#include "DecodeWhiteRabbit.h"
#include "DecodeQDC.h"
#include "TGo4MbsEvent.h"
#include "TGo4EventProcessor.h"
#include "TGo4UserException.h"
#include "TGo4Picture.h"
#include "TFile.h"
#include "/cvmfs/eel.gsi.de/debian10-x86_64/root/628-04/include/THttpServer.h"
#include "TTree.h"
#include "vector"
#include "map"
#include "string"

#define CYCLE_TIME  (Double_t) 5000
#define COARSE_CT_RANGE  0x800  // 11 bits

class Event;
class Param;

typedef struct TamexData_
{
	Int_t mid;
	Int_t ch;
	Double_t ts;
	Double_t ToTs;
	Double_t tf;
	Double_t ToTf;
} TamexData;

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

typedef struct DetectorInf_
{
	TString Name;
	TString Label;
	TString Module;
	int ModuleId;
	int WRid;
}DetectorInf;

typedef struct WhiteRabbitData_
{
	Int_t id;
	TString Name;
	ULong64_t ts;
}WhiteRabbitData;

class Proc : public TGo4EventProcessor {
   public:
      Proc() ;
      Proc(const char* name);
      virtual ~Proc() ;
      // event processing function, default name
      Bool_t BuildEvent(TGo4EventElement* target);
      Bool_t FillHistograms();

 private:
      Double_t CalTamexToT(Double_t lt, Double_t tt);
      Double_t CalTamexts(Double_t ct, Double_t ft);
      void ReadPar();
      void InitData();
      void Initopf();
      void InitDecode();
      void CreateOnline();
      static const Int_t NMo = 4;
      static const Int_t NCh = 32;
      Float_t raw[NMo][NCh];

      int post;
      THttpServer *httpServertmp; 
      TH1D  *hraw[NMo][NCh];
      TH1I  *hcount[NMo];
      TH1D  *he[10][10][16]; 

      TFile *opf;
      TTree *opt;
      Float_t CalYSO[2][32];
      Float_t YSOx[16];
      Float_t YSOy[16];
      Float_t YSOts;
      Float_t px; 
      Float_t py;
      Float_t Fiber[8];
      Float_t Fiberts;
      Float_t TDC[8];


      std::map<int, DetectorInf> mDetector;
      std::vector<WhiteRabbitData> vWR;
      std::vector<std::vector<TamexData>> vTamex;
      std::vector<std::vector<QDCData>> vQDC;


      std::vector<std::vector<TamexFineTimeCalData>> vTamexCal;

      std::vector<DecodeTamex*> vDecodeT;
      std::vector<DecodeQDC*> vDecodeQDC;

      DecodeWhiteRabbit *decodeWR;
      int nE;

   ClassDef(Proc,1)
};
#endif


//----------------------------END OF GO4 SOURCE FILE ---------------------
