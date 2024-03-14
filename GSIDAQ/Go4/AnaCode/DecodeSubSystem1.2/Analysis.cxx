//-----------------------------------------------------------------------
//       The GSI Online Offline Object Oriented (Go4) Project
//         Experiment Data Processing at EE department, GSI
//-----------------------------------------------------------------------
// Copyright (C) 2000- GSI Helmholtzzentrum für Schwerionenforschung GmbH
//                     Planckstr. 1, 64291 Darmstadt, Germany
// Contact:            http://go4.gsi.de
//-----------------------------------------------------------------------
// This software can be used under the license agreements as stated
// in Go4License.txt file which is part of the distribution.
//-----------------------------------------------------------------------

#include "Analysis.h"

#include <stdlib.h>
#include "Riostream.h"

#include "Go4EventServer.h"
#include "TGo4StepFactory.h"
#include "TGo4AnalysisStep.h"
#include "TGo4Version.h"

//***********************************************************
Analysis::Analysis(){}

//***********************************************************
// this constructor is called by go4analysis executable
Analysis::Analysis(int argc, char** argv) : TGo4Analysis(argc, argv)
{
   std::cout << "**** Create Analysis " << argv[0] << std::endl;

   if (!TGo4Version::CheckVersion(__GO4BUILDVERSION__)) {
      std::cout << "****  Go4 version mismatch" << std::endl;
      exit(-1);
   }

   TGo4StepFactory* factory = new TGo4StepFactory("Factory");
   TGo4AnalysisStep* step = new TGo4AnalysisStep("Unpack", factory, 0, 0);
   AddAnalysisStep(step);

// More settings are done in macro setup.C

   gROOT->ProcessLine(".x setup.C()");
   Print(); // print setup

   // Define custom passwords for analysis server
   DefineServerPasswords("XXXadmin", "XXXcontrol", "XXXview");

}

//***********************************************************
Analysis::~Analysis()
{
   std::cout << "**** Analysis: Delete instance" << std::endl;
}

//-----------------------------------------------------------
Int_t Analysis::UserPreLoop()
{
   // called after start analysis before first event
   std::cout << "**** Analysis: PreLoop" << std::endl;
   Print(); // print setup
   return 0;
}
//-----------------------------------------------------------
Int_t Analysis::UserPostLoop()
{
   // called after close analysis after last  event
   // all this is optional:
   std::cout << "**** Analysis: PostLoop" << std::endl;
   return 0;
}

//-----------------------------------------------------------
Int_t Analysis::UserEventFunc()
{
   // all this is optional:
   // This function is called once for each event after all steps.
   return 0;
}
