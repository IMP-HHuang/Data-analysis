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
#include "TTree.h"
#include "vector"
#define CYCLE_TIME  (Double_t) 5000
#define COARSE_CT_RANGE  0x800  // 11 bits

class Event;
class Param;

typedef struct TamexData_
{
	Int_t cid;
	Int_t ch;
	Double_t ts;
	Double_t ToTs;
	Double_t tf;
	Double_t ToTf;
} TamexData;

typedef struct TamexFineTimeCalData_
{
	Int_t cid;
	Int_t ch;
	Double_t ts;
} TamexFineTimeCalData;

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
      void Init();

 private:
      Double_t CalTamexToT(Double_t lt, Double_t tt);
      Double_t CalTamexts(Double_t ct, Double_t ft);
      static const Int_t NMo = 4;
      static const Int_t NCh = 32;
      Float_t raw[NMo][NCh];
      TH1D  *hraw[NMo][NCh];
      TH1I  *hcount[NMo];

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

      std::vector<TamexData> vFATIMA;
      std::vector<TamexData> vBp;
      std::vector<WhiteRabbitData> vWR;


      std::vector<TamexFineTimeCalData> vFATIMACal;
      std::vector<TamexFineTimeCalData> vBpCal;

      bool Used_Systems;
      DecodeTamex *DecodeTFatima;
      DecodeTamex *DecodeTBp;
      DecodeWhiteRabbit *decodeWR;
      DecodeQDC *decodeQDC;
      int nE;

   ClassDef(Proc,1)
};
#endif


//----------------------------END OF GO4 SOURCE FILE ---------------------
