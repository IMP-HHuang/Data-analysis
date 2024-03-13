#ifndef TUNPACKPROCESSOR_H
#define TUNPACKPROCESSOR_H

#include "TGo4EventProcessor.h"
#include "TGo4MbsEvent.h"
#include "TFile.h"
#include "TTree.h"

class Event;
class Param;

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


   ClassDef(Proc,1)
};
#endif //TUNPACKPROCESSOR_H


//----------------------------END OF GO4 SOURCE FILE ---------------------
