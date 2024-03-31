#include "UcesbSource.h"
#include "Data.h"
#include "UserDefine.h"
#include "Log.h"

#include "TString.h"
#include "TStopwatch.h"

#include "iostream"
#include "fstream"
#include "ctime"
#include "iomanip"
#include <cstdint>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

Bool_t BResetOnlineHis = kFALSE;
Bool_t BFillHis = kTRUE;
unsigned int RefreshTime = 2; //s Refresh rate for online histograms
unsigned int port = 8001; // Port number for online visualisation, e.g. lxgXXXX:8886
unsigned int DoTamexFineTimeCali = 0;

int main(int argc, char** argv)
{   

    Long64_t nEvMax;
    if(argc == 1)
        nEvMax = -1;
    else
        nEvMax = atoll(argv[1]);
    TStopwatch timer;

    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y/%m/%d %H:%M:%S");
    TString stmp = oss.str().data();
    Log::Output("Inf", Form("Run start at %s", stmp.Data()));
    timer.Start();
    TString ipfPath = "";
    TString ipfName;
    TString UcesbPath;
    TString opfName;

    std::ifstream inf("./Par/UserDefine.txt");
    if(inf.is_open())
    {
        std::string sline;
        while(std::getline(inf, sline))
        {
            std::string snametmp, svaluetmp;
            std::istringstream iss(sline);
            if(iss >> snametmp >> svaluetmp) 
            {
                if(snametmp == "DoTamexFineTimeCali")
                    DoTamexFineTimeCali = std::atoi(svaluetmp.data());
                if(snametmp == "RefreshTime")
                    RefreshTime = std::atoi(svaluetmp.data());
                if(snametmp == "port")
                    port = std::atoi(svaluetmp.data());
                if(snametmp == "ipfPath")
                    ipfPath = svaluetmp;
                if(snametmp == "ipfName")
                    ipfName = svaluetmp;
                if(snametmp == "opfName")
                    opfName = svaluetmp;
                if(snametmp == "UcesbPath")
                    UcesbPath = svaluetmp;
            }
        }
    }
    else
    {
        Log::Output("Err",  "Cannot open ./Par/UserDefine.txt");
        return 1;
    }

    TString UcesbRunOptions = "--data --print --allow-errors --input-buffer=200Mi"; // "RAW"? "time=stitch=1000"
    TString UcesbRunCom = UcesbPath + "empty/empty";
    UcesbRunCom.ReplaceAll("//","/"); //replace // with /
    if(access(UcesbRunCom.Data(), R_OK) == -1)
    {
        Log::Output("Err", Form("Cannot Find Ucesb of %s", UcesbRunCom.Data()));
        return 1;
    }
    UcesbSource* source = new UcesbSource(UcesbRunCom, UcesbRunOptions);


    ipfName = Form("%s%s", ipfPath.Data(), ipfName.Data());
    TString ipfNameCheck = ipfName;
    ipfNameCheck.ReplaceAll(" ","");
    if(ipfNameCheck.SubString(0, 5) != "trans" && access(ipfNameCheck.Data(), R_OK) != 0)
    {
        Log::Output("Err", Form("Cannot Find input file of %s", ipfName.Data()));
        return 1;
    }


    TString outputFileName = Form("%s%04d.root", opfName.Data(), 0);
    TFile *opf = new TFile(outputFileName.Data(), "RECREATE");
    TTree *opt = new TTree("tree", "tree");
    Log::Output("Inf", Form("Create %s", "out.root"));

    if(!source->RunUcesb(ipfName, nEvMax, opt))
    {
        Log::Output("Err", "Error at Init Source");
        return 1;
    }
    Log::Output("Inf", Form("Run Ucesb by %s %s %s ", UcesbRunCom.Data(), ipfName.Data(), UcesbRunOptions.Data()));
    source->Process();
    source->Close();
    opf->cd();
    opt->Write();
    opf->Close();

    delete source;

    timer.Stop();
    Double_t rtime = timer.RealTime();
    Double_t ctime = timer.CpuTime();
    Double_t cpuUsage = ctime / rtime;
    Log::Output("Inf", Form("CPU used: %0.2f%%", cpuUsage));
    Log::Output("Inf", Form("Real time: %0.0f s, CPU time:%0.0f s ", rtime, ctime));
    t = std::time(nullptr);
    tm = *std::localtime(&t);
    std::ostringstream osss;
    osss << std::put_time(&tm, "%Y/%m/%d %H:%M:%S");
    stmp = osss.str().data();
    Log::Output("Inf", Form("Run stop at %s", stmp.Data()));

    return 0;
}
