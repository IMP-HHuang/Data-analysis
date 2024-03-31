
#include "HttpServer.h"
#include "Log.h"


HttpServer::HttpServer()
{
    Log::Output("Inf", Form("The port is %d, plz login at %s", port, Form("http:%d", port)));
    std::cout <<"\033[32m The port is " << port << ", plz login at " << Form("http:%d", port) << "\033[0m" << std::endl;
    httpServer = new THttpServer(Form("http:%d", port));
    httpServer->SetReadOnly(kTRUE); //kFALSE
//    Config();
    NModPreLine = 5;
    NCh = 16;
    Rate.clear();
}

HttpServer::~HttpServer()
{
    httpServer->Delete();
}

void HttpServer::Config()
{
    httpServer->SetTopName("DESPEC");
    httpServer->SetItemField("/", "_monitoring", Form("%d", RefreshTime*1000)); //monitoring interval in ms
    httpServer->SetItemField("/","_drawopt", "hist");
    httpServer->RegisterCommand("/Reset", "BResetOnlineHis = kTRUE;", "button;rootsys/icons/ed_delete.png");
    httpServer->SetIcon("Reset", "/Reset");
  //  httpServer->RegisterCommand("/Reset", "BResetOnlineHis = kTRUE;", "./.fig/ed_delete.png");
    httpServer->Hide("/Reset");
    httpServer->SetItemField("/Reset", "_fastcmd", "true");
    httpServer->RegisterCommand("/Stop", "BFillHis = kFALSE;", "button;rootsys/icons/ed_interrupt.png");
    httpServer->Hide("/Stop");
    httpServer->SetItemField("/Stop", "_fastcmd", "true");
    httpServer->RegisterCommand("/Start", "BFillHis = kTRUE;", "button;rootsys/icons/ed_execute.png");
    httpServer->Hide("/Start");
    httpServer->Hide("/Canvases");
    httpServer->Hide("/Files");
    gStyle->SetOptStat(0);
    gStyle->SetOptLogz(1);
    gStyle->SetPalette(1);
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
}

void HttpServer::AddHis(TString Path, TH1D *h, TString Name, TString xName, TString yName)
{
    h->SetDirectory(0);
    h->GetXaxis()->SetTitle(xName.Data());
    h->GetXaxis()->CenterTitle(true);
    h->GetYaxis()->SetTitle(yName.Data());
    h->GetYaxis()->CenterTitle(true);
    httpServer->Register(Path.Data(), h);
    httpServer->SetItemField(Path.Data(), "_drawopt", "hist");
//    httpServer->SetItemField(Path.Data(), "_monitoring", Form("%d", RefreshTime*1000)); //monitoring interval in ms
}

void HttpServer::AddHis(TString Path, TH2D *h, TString Name, TString xName, TString yName)
{
    int index = TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    h->SetContour(NCont);
    h->SetFillColor(index);
    h->SetDirectory(0);
    h->GetXaxis()->SetTitle(xName.Data());
    h->GetXaxis()->CenterTitle(true);
    h->GetYaxis()->SetTitle(yName.Data());
    h->GetYaxis()->CenterTitle(true);
    h->SetOption("colz");
    httpServer->Register(Path.Data(), h);
    httpServer->SetItemField(Path.Data(), "_drawopt", "col");
//    httpServer->SetItemField(Path.Data(), "_monitoring", Form("%d", RefreshTime*1000)); //monitoring interval in ms
}

void HttpServer::Update()
{
    gSystem->ProcessEvents();
    gStyle->SetOptLogz(1);
    gStyle->SetOptStat(0);
    gStyle->SetPalette(1);
}

void HttpServer::AddRate(TString Label, TString Name)
{
    Rate.push_back(new TCanvas(Label.Data(), "Rate", 600, 500));
    vName.push_back(Name);
    InitRateGui(Rate.size() - 1);
    httpServer->Register(Form("/%s/Rate", Label.Data()), Rate.at(Rate.size()-1));
 }

void HttpServer::InitRateGui(int id, TString flag)
{
    NModPreLine = 5;
//    std::cout << id << " update  " << std::endl;
    Rate.at(id)->cd();
    Rate.at(id)->Clear();
    Double_t detx = 1.0 / double(NModPreLine);
    Double_t dety = 1.0 / (double(NCh) * 2.0 + 2.0 + 1.0 + 1.0);
    TBox* box = new TBox();
    box->SetLineStyle(1);
	box->SetLineColor(kBlack);
	box->SetLineWidth(3.0);
    TLatex* latex = new TLatex();
    box->SetFillColor(2);
    box->SetX1(0.01 * detx);
    box->SetX2(1.0);
	box->SetY1(1.0);
	box->SetY2(1.0 - 2.0 * dety);
    box->DrawClone();
    latex->SetTextSize(0.040);
    latex->SetTextAlign(22);
    latex->SetTextColor(kBlack);
    latex->DrawLatex(0.5, 1.0 - dety, vName.at(id).Data());
    Double_t px, py;
    for(int i=0; i<10; i++)
    {
        box->SetFillColor(44);
        box->SetX1((Double_t(i%NModPreLine) + 0.01)* detx);
        box->SetX2((Double_t(i%NModPreLine) + 1.0)* detx);
        latex->SetTextSize(0.02);
        latex->SetTextAlign(22);
        if(i<NModPreLine)
        {
		    box->SetY1(1.0 - 2.0 * dety);
		    box->SetY2(1.0 - (2.0+1.0) * dety);
            box->DrawClone();
            latex->DrawLatex((Double_t(i%NModPreLine) + 0.5)* detx, 1.0 - (2.0+0.5) * dety, Form("Mid = %d", i));
        }
        else
        {
		    box->SetY1(1.0 - (19.0) * dety);
		    box->SetY2(1.0 - (19.0+1.0) * dety);
            box->DrawClone();
            latex->DrawLatex((Double_t(i%NModPreLine) + 0.5)* detx, 1.0 - (19.0+0.5) * dety, Form("Mid = %d", i));
        }
        for(int j=0; j<NCh; j++)
        {
            latex->SetTextSize(0.02);
            latex->SetTextAlign(22);
            latex->SetTextColor(kBlack);
            if(i<NModPreLine)
            {
		        box->SetY1(1.0 - (j+3.0+0.00) * dety);
		        box->SetY2(1.0 - (j+3.0+1.0) * dety);
                py = 1.0 - (j + 3.0 + 0.5) * dety;
            }
            else
            {
		        box->SetY1(1.0 - (j + 3.0 + double(NCh) + 1.0 + 0.00) * dety);
		        box->SetY2(1.0 - (j + 3.0 + double(NCh) + 1.0 + 1.0) * dety);
                py = 1.0 - (j + 3.0 + double(NCh) + 1.0 + 0.5) * dety;
            }
            box->SetX1((Double_t(i%5) + 0.01)* detx);
            box->SetX2((Double_t(i%5) + 0.2)* detx);
            box->SetFillColor(kGray + 1);
            box->DrawClone();
            if(i<NModPreLine)
            {
                latex->DrawLatex((Double_t(i%NModPreLine) + 0.1)* detx, 1.0 - (j+3.0+0.5) * dety, Form("%d", j));
            }
            else
            {
                latex->DrawLatex((Double_t(i%NModPreLine) + 0.1)* detx, 1.0 - (j+3.0+double(NCh)+1.0+0.5) * dety, Form("%d", j));
            }
            if(flag == "Tamex")
            {
                //fast
                box->SetX1((Double_t(i%NModPreLine)+ 0.2)* detx);
                box->SetX2((Double_t(i%NModPreLine) + 0.6)* detx);
                box->SetFillColor(30);
                box->DrawClone();
                px = (Double_t(i%NModPreLine) + 0.2 + 0.2)* detx;
                latex->DrawLatex(px, py, Form("%d", 0));
                
                //Slow
                box->SetX1((Double_t(i%NModPreLine)+ 0.6)* detx);
                box->SetX2((Double_t(i%NModPreLine) + 1.0)* detx);
                box->SetFillColor(42);
                box->DrawClone();
                px = (Double_t(i%NModPreLine) + 0.2 + 0.2 + 0.4)* detx;
                latex->DrawLatex(px, py, Form("%d", 0));
            }
            else
            {
                //All
                box->SetX1((Double_t(i%NModPreLine)+ 0.2)* detx);
                box->SetX2((Double_t(i%NModPreLine) + 1.0)* detx);
                box->SetFillColor(30);
                box->DrawClone();
                px = (Double_t(i%NModPreLine) + 0.2 + 0.4)* detx;
                latex->DrawLatex(px, py, Form("%d", 0));
            }

        }
    }
    latex->Delete();
    box->Delete();
}

void HttpServer::SetRate(int id, int mid, int ch, TString flag, int rate)
{
    Rate.at(id)->cd();
//    if(id ==  0)
//    std::cout << id << "  " << mid << "   " << ch << "  " << rate << std::endl;
    Double_t detx = 1.0 / double(NModPreLine);
    Double_t dety = 1.0 / (double(NCh) * 2.0 + 2.0 + 1.0 + 1.0);
    TLatex* latex = new TLatex();
    TBox* box = new TBox();
    latex->SetTextSize(0.02);
    latex->SetTextAlign(22);
    latex->SetTextColor(kBlack);
    Double_t px, py;
    if(mid<NModPreLine)
    {
	    box->SetY1(1.0 - (ch+3.0+0.00) * dety);
	    box->SetY2(1.0 - (ch+3.0+1.0) * dety);
        py = 1.0 - (ch + 3.0 + 0.5) * dety;
    }
    else
    {
	    box->SetY1(1.0 - (ch + 3.0 + double(NCh) + 1.0 + 0.00) * dety);
	    box->SetY2(1.0 - (ch + 3.0 + double(NCh) + 1.0 + 1.0) * dety);
        py = 1.0 - (ch + 3.0 + double(NCh) + 1.0 + 0.5) * dety;
    }

    if(flag == "fast")
    {
        box->SetX1((Double_t(mid%NModPreLine)+ 0.2)* detx);
        box->SetX2((Double_t(mid%NModPreLine) + 0.6)* detx);
        box->SetFillColor(30);
        box->DrawClone();
        px = (Double_t(mid%NModPreLine) + 0.2 + 0.2)* detx;
    }
    else if(flag == "slow")
    {
        box->SetX1((Double_t(mid%NModPreLine)+ 0.6)* detx);
        box->SetX2((Double_t(mid%NModPreLine) + 1.0)* detx);
        box->SetFillColor(42);
        box->DrawClone();
        px = (Double_t(mid%NModPreLine) + 0.2 + 0.2 + 0.4)* detx;
    }
    else
    {
        box->SetX1((Double_t(mid%NModPreLine)+ 0.2)* detx);
        box->SetX2((Double_t(mid%NModPreLine) + 1.0)* detx);
        box->SetFillColor(30);
        box->DrawClone();
        px = (Double_t(mid%NModPreLine) + 0.2 + 0.4)* detx;
    }
    latex->DrawLatex(px, py, Form("%d", rate));
    latex->Delete();
    box->Delete();
}

void HttpServer::UpdateRateGui(int id)
{
    Rate.at(id)->Modified();
    Rate.at(id)->Update();
    gSystem->Sleep(10);
}
