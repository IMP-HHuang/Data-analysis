#include "Param.h"
#include "Riostream.h"

//***********************************************************
Param::Param() : TGo4Parameter("Parameter")
{
  fFill=kTRUE;
  fOutput=kFALSE;
  fOffset=0;
  fFactor=1;
}
//***********************************************************
Param::Param(const char* name) : TGo4Parameter(name)
{
  fFill=kTRUE;
  fOutput=kFALSE;
  fOffset=0;
  fFactor=1;
}
//***********************************************************
Param::~Param()
{
}
//***********************************************************

//-----------------------------------------------------------
Int_t Param::PrintParameter()
{
  std::cout << "Parameter " << GetName() << ":";
  std::cout << " fill="     << fFill;
  std::cout << " output="   << fOutput;
  std::cout << " offset="   << fOffset;
  std::cout << " factor="   << fFactor << std::endl;
  return 0;
}
//-----------------------------------------------------------
Int_t Param::PrintParameter(Text_t * n, Int_t)
{
  PrintParameter();
  return 0;
}
//-----------------------------------------------------------
Bool_t Param::UpdateFrom(TGo4Parameter *pp)
{
  if(pp->InheritsFrom("Param"))
  {
    Param * from;
    from = (Param *) pp;
    std::cout << "**** Param " << GetName() << " updated from auto save file" << std::endl;
    fFill   = from->fFill;
    fOutput = from->fOutput;
    fOffset = from->fOffset;
    fFactor = from->fFactor;
    PrintParameter();
  }
     else
     std::cout << "Wrong parameter object: " << pp->ClassName() << std::endl;
  return kTRUE;
}

//----------------------------END OF GO4 SOURCE FILE ---------------------
