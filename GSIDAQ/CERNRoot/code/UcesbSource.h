
#ifndef UcesbSource_H
#define UcesbSource_H


#include <string>
#include "TObjArray.h"
#include "TString.h"
#include "TTree.h"
#include <Rtypes.h>


#include <fstream>
#include <list>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <vector>

#include "Data.h"
#include "Proc.h"

//class UcesbSource : public FairSource
class UcesbSource
{
  public:
    UcesbSource(const TString, const TString);
    virtual ~UcesbSource();
    bool Close();
    bool RunUcesb(const TString, const Long64_t, TTree *opt_);
    void Process();
    void ProcessSubEvent();
    int GetWorldNumber();
    int GetPrcId();


  private:
    int pdata[32768];
    bool GetIpfLine();
    char cbuf[5000];
    std::istringstream iss;
    std::string Name;
    Long64_t EvNo;
    Int_t EvType;
    Int_t EvSubType;
    Int_t EvSize;
    Int_t EvTrigger;
    Int_t PrcId;
    Int_t SubEvType;
    Int_t SubEvSubType;
    Int_t SubEvSize;
    Int_t SubEvCtrl;
    Int_t SubEvCra;

    FILE* fInfData;
    TString RunUcesbCode;
    TString UcesbRunCom;
    TString UcesbRunOptions;
    TString IpfName;
    Long64_t nEvMax;
    Long64_t nEv;

    Proc *proc;
};

#endif /* UcesbSource_H */
