#ifndef SPAR_H
#define SPAR_H

#include "TGo4Parameter.h"

class Param : public TGo4Parameter {
   public:
      Param();
      Param(const char* name);
      virtual ~Param();
      virtual Int_t  PrintParameter(Text_t * n, Int_t);
      Int_t  PrintParameter();
      virtual Bool_t UpdateFrom(TGo4Parameter *);

      Bool_t   fFill;   // enable filling histograms 
      Bool_t   fOutput; // enable filling  event
      Float_t  fOffset; // offset used for unpack
      Float_t  fFactor; // factor used for unpack

   ClassDef(Param,1)
};

#endif //SPAR_H

//----------------------------END OF GO4 SOURCE FILE ---------------------
