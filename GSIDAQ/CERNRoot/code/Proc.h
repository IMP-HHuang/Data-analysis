#ifndef TUNPACKPROCESSOR_H
#define TUNPACKPROCESSOR_H

#include "DecodeTamex.h"
#include "DecodeFebex.h"
#include "DecodeFEE.h"
#include "DecodeWhiteRabbit.h"
#include "DecodeQDC.h"
#include "UserDefine.h"
#include "Data.h"
#include "Log.h"

#include "fstream"
#include "iostream"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TString.h"
#include "vector"
#include "map"
#include "string"
#include "algorithm"
#include "ctime"
#include "HttpServer.h"

extern Bool_t BResetOnlineHis;
extern Bool_t BFillHis;
extern unsigned int RefreshTime;
extern unsigned int DoTamexFineTimeCali;

class Proc{
    public:
        Proc(TTree* opt_);
        virtual ~Proc();
        Bool_t  DecodeSubEvent(int* pdata_, int PrcId, Long64_t nEv);
        Bool_t FillHistograms();
        Bool_t SaveEvent();

    private:
        void ReadPar();
        void InitData();
        void Initopt();
        void InitDecode();
        void CreateOnline();
        void ResetOnlineHis();

        int* pdata;
        int PrcId;  //Subsystem Id
        int post;
        HttpServer *httpServer; 
        static const int nTamexh1 = 2;
        std::vector<std::vector<TH1D*>> vh1Tamex[nTamexh1];
        static const int nTamexh2 = 1;
        std::vector<std::vector<TH2D*>> vh2Tamex[nTamexh2];
        std::vector<std::vector<TH1D*>> vh1FebexRaw;
        std::vector<std::vector<TH1D*>> vh1FEERaw;

        TTree *opt;

        std::map<int, DetectorInf> mDetector;
        std::vector<WhiteRabbitData> vWR;
        std::vector<std::vector<TamexData>> vTamex;
        std::vector<std::vector<FebexData>> vFebex;
        std::vector<std::vector<FEEData>> vFEE;
        std::vector<std::vector<QDCData>> vQDC;

        std::vector<DecodeTamex*> vDecodeT;
        std::vector<DecodeFebex*> vDecodeF;
        std::vector<DecodeFEE*> vDecodeFEE;
        std::vector<DecodeQDC*> vDecodeQDC;

        ULong64_t tNow;
        ULong64_t tStart;
        ULong64_t tUpdate;
        bool bUpdate;

};
#endif

