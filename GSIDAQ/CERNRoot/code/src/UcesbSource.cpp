#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <csignal>


#include "Data.h"
#include "UcesbSource.h"
#include "Log.h"


UcesbSource::UcesbSource(const TString UcesbRunCom_, const TString UcesbRunOptions_)
{
    UcesbRunCom = UcesbRunCom_;
    UcesbRunOptions = UcesbRunOptions_;
    Log::Output("Inf", "Build UcesbSource");
}

UcesbSource::~UcesbSource()
{
}

bool UcesbSource::RunUcesb(const TString IpfName_, const Long64_t nEvMax_, TTree *opt_)
{
    proc = new Proc(opt_);
    IpfName = IpfName_;
    nEvMax = nEvMax_;
    nEv = 0;
    for(int i=0; i<int(sizeof(pdata)/sizeof(int)); i++)
        pdata[i] = 0;
    iss.clear();

    //Run ucesb code
   if (nEvMax > 0)
       RunUcesbCode = Form("%s %s %s --max-events=%lld", UcesbRunCom.Data(), IpfName.Data(), UcesbRunOptions.Data(), nEvMax);
   else
       RunUcesbCode = Form("%s %s %s", UcesbRunCom.Data(), IpfName.Data(), UcesbRunOptions.Data());

    Log::Output("Inf", Form(" Run  Ucesb by %s", RunUcesbCode.Data()));
//	system(RunUcesbCode.Data());

   fInfData = popen(RunUcesbCode.Data(), "r");
   if(fInfData == nullptr)
   {
       Log::Output("Err", "Cannot open pip");
       return false;
   }
   return true;
}

void UcesbSource::Process()
{
    while(1)
    {
        if(feof(fInfData) || (nEvMax > 0 && nEv > nEvMax-1))    break;
//        if(feof(fInfData)) break;
        //Process Event
        while(1)
        {
            if(!GetIpfLine())
		        break;
            if(iss >> Name >> EvNo >> Name >> EvType >> EvSubType >> Name >> EvSize >> Name >> EvTrigger)
                break;
//         std::cout <<"\033[31m  Cannot Get information of this event \033[0m" << std::endl;
        }
      //  std::cout << "Enent No." << nEv+1 << "  " << EvNo << "  " << EvType << "  " << EvSubType << "  " << EvSize << "  " << EvTrigger << std::endl;
        EvSize -= 10;
        while(EvSize > 10)
        {
            ProcessSubEvent();
        }
        proc->SaveEvent();
        nEv++;
    }
}

void UcesbSource::ProcessSubEvent()
{
    while(1)
    {
        if(!GetIpfLine())
            break;
        if(iss >> Name >> Name >> PrcId >> Name >> SubEvType >> SubEvSubType >> Name >> SubEvSize >> Name >> SubEvCtrl >> Name >> SubEvCra)
            break;
   //     std::cout <<"\033[31m  Cannot Get information of this subnent \033[0m" << std::endl;
    }
  //  std::cout << "SubEvent : " << PrcId << "   " << SubEvType << "  " << SubEvSubType << "  " << SubEvSize << "   " << SubEvCtrl << "  " << SubEvCra << std::endl;
    EvSize -= 10;
    EvSize -= SubEvSize;

    int fd = fileno(fInfData);
    for(int i=0; i<int(SubEvSize)/4; i++)
    {
     //   if(fscanf(fInfData, "%x", &pdata[i]) != 1)
    //        break;
        fscanf(fInfData, "%x", &pdata[i]);
 //       std::cout << i << "   " << int(SubEvSize)/4 << "  " << std::hex << pdata[i] << std::dec <<std::endl;
    }

    proc->DecodeSubEvent(pdata, PrcId, nEv);

    for(int i=0; i<int(SubEvSize)/4; i++)
        pdata[i] = 0;

}

bool UcesbSource::GetIpfLine()
{
    iss.clear();
    if(fgets(cbuf, sizeof(cbuf), fInfData) != NULL)
    {
        iss.str(cbuf);
//        std::cout << "Line  : " << cbuf << std::endl;
        return true;
    }
//    std::cout <<"\033[31m  Cannot Get Line \033[0m" << std::endl;
    return false;
}

int UcesbSource::GetWorldNumber()
{
    return SubEvSize/4;
}

int UcesbSource::GetPrcId()
{
    return PrcId;
}

bool UcesbSource::Close()
{
//    kill(ProcessId, SIGTERM);
    if(pclose(fInfData) == -1)
    {
        Log::Output("Err", "The pip has not been closed sucessfully");
        return false;
    }
    Log::Output("Err", "The pip has not been closed sucessfully");
    delete proc;
    return true;
}

