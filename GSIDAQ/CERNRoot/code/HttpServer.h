#ifndef HttpServer_HH
#define HttpServer_HH

#include "THttpServer.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TString.h"
#include "TBox.h"
#include "TLatex.h"

#include "vector"

#include "Data.h"
#include "UserDefine.h"


extern Bool_t BResetOnlineHis;
extern Bool_t BFillHis;
extern unsigned int RefreshTime;
extern unsigned int port; 

class HttpServer{
    public:
        HttpServer();
        virtual ~HttpServer();
        void AddHis(TString Path, TH1D *h, TString Name, TString xName="", TString yName="");
        void AddHis(TString Path, TH2D *h, TString Name, TString xName="", TString yName="");
        void AddRate(TString Label, TString Name);
        void InitRateGui(int id, TString flag = "Tamex");
        void UpdateRateGui(int id);
        void Update();
        void Config();
        void SetRate(int id, int mid, int ch, TString flag, int rate);
        THttpServer *httpServer;
    private:
        std::vector<TCanvas*> Rate;
        std::vector<TString> vName;
        int NModPreLine;
        int NCh;
        static const Int_t NRGBs = 5;
        static const Int_t NCont = 255;
        Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
        Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
        Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
        Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
};


#endif
