// $Id: Analysis.h 478 2009-10-29 12:26:09Z linev $
//-----------------------------------------------------------------------
//       The GSI Online Offline Object Oriented (Go4) Project
//         Experiment Data Processing at EE department, GSI
//-----------------------------------------------------------------------
// Copyright (C) 2000- GSI Helmholtzzentrum f�r Schwerionenforschung GmbH
//                     Planckstr. 1, 64291 Darmstadt, Germany
// Contact:            http://go4.gsi.de
//-----------------------------------------------------------------------
// This software can be used under the license agreements as stated
// in Go4License.txt file which is part of the distribution.
//-----------------------------------------------------------------------

#ifndef TANALYSIS_H
#define TANALYSIS_H

#include "TGo4Analysis.h"

class TGo4MbsEvent;
class Control;

class Analysis : public TGo4Analysis {
   public:
      Analysis();
      Analysis(int argc, char** argv);
      virtual ~Analysis() ;
      virtual Int_t UserPreLoop();
      virtual Int_t UserEventFunc();
      virtual Int_t UserPostLoop();
   private:

   ClassDef(Analysis,1)
};
#endif //TANALYSIS_H
