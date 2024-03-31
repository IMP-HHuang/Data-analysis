#include "Proc.h"


//***********************************************************
Proc::Proc(TTree* opt_)
{
    opt = opt_;
    InitDecode();
    InitData();
    ReadPar();
    Initopt();
    CreateOnline();
    tNow = 0;
    tStart = 0;
    tUpdate = std::time(nullptr);
    bUpdate = false;
}

Proc::~Proc()
{
    if(DoTamexFineTimeCali)
    {
        for(auto it = mDetector.begin(); it != mDetector.end(); it++)
            if(it->second.Module.substr(2, 5) == "Tamex")
                vDecodeT[it->second.ModuleId]->GetCaliFineTimePar(it->second.Label);
    }
    delete httpServer;
    for(int i=0; i<int(vDecodeT.size()); i++)
        delete vDecodeT[i];
    for(int i=0; i<nTamexh1; i++)
    {
        for(int j=0; j<int(vh1Tamex[i].size()); j++)
                vh1Tamex[i].at(j).clear();
        vh1Tamex[i].clear();
    }
    for(int i=0; i<nTamexh2; i++)
    {
        for(int j=0; j<int(vh2Tamex[i].size()); j++)
                vh2Tamex[i].at(j).clear();
        vh2Tamex[i].clear();
    }
    for(int i=0; i<int(vh1FebexRaw.size()); i++)
            vh1FebexRaw.at(i).clear();
    vh1FebexRaw.clear();
    for(int i=0; i<int(vh1FEERaw.size()); i++)
            vh1FEERaw.at(i).clear();
    vh1FEERaw.clear();
    for(int i=0; i<int(vDecodeF.size()); i++)
        delete vDecodeF[i];
    for(int i=0; i<int(vDecodeFEE.size()); i++)
        delete vDecodeFEE[i];
    for(int i=0; i<int(vDecodeQDC.size()); i++)
        delete vDecodeQDC[i];
}

void Proc::ReadPar()
{
    std::string stmp, swrid;
    TString sname, slab , smod;
    int idtmp, flag, Nmod;
    std::ifstream inf("./Par/DetectorInf.txt");
    if(inf.is_open())
    {
        getline(inf, stmp);
        while(inf >> sname >> idtmp >> swrid >> slab >> smod >> Nmod >> flag)
        {
            Log::Output("Inf", Form("%d \t %s \t %d \t %s \t %s \t %s \t %d \t %d", int(mDetector.size()), sname.Data(), idtmp, swrid.data(), slab.Data(), smod.Data(), Nmod, flag));
            if(!flag)	continue;
            DetectorInf det;
            det.Name = sname;
            det.Label = slab;
            det.Module = smod;
            det.NumberMOfodule = Nmod;
            det.id = mDetector.size();
            if(det.Module.substr(2, 5) == "Tamex")
            {
                det.ModuleId = int(vDecodeT.size()); 
                vDecodeT.push_back(new DecodeTamex(det.Label, det.Module));
                std::vector<TamexData> vt;
                vTamex.push_back(vt);
                std::vector<TH1D*> vh1;
                vh1Tamex[0].push_back(vh1);
                vh1Tamex[1].push_back(vh1);
                std::vector<TH2D*> vh2;
                vh2Tamex[0].push_back(vh2);

            }
            else if(det.Module == "Febex")
            {
                det.ModuleId = int(vDecodeF.size()); 
                vDecodeF.push_back(new DecodeFebex());
                std::vector<FebexData> vf;
                vFebex.push_back(vf);
                std::vector<TH1D*> vh;
                vh1FebexRaw.push_back(vh);
            }
            else if(det.Module == "FEE")
            {
                det.ModuleId = int(vDecodeFEE.size()); 
                vDecodeFEE.push_back(new DecodeFEE());
                std::vector<FEEData> vf;
                vFEE.push_back(vf);
                std::vector<TH1D*> vh;
                vh1FEERaw.push_back(vh);
            }
            else if(det.Module == "QDC")
            {
                det.ModuleId = int(vDecodeQDC.size()); 
                vDecodeQDC.push_back( new DecodeQDC());
                std::vector<QDCData> vq;
                vQDC.push_back(vq);
            }
            det.WRid = std::stoi(swrid, nullptr, 16);
            mDetector[idtmp] = det;
        }
        inf.close();
    }
    else
    {
        Log::Output("Err",  "Cannot open ./Par/DetectorInf.txt");

        exit(0);
    }

}

void Proc::CreateOnline()
{
    httpServer = new HttpServer(); 
    int id =0;
    for(auto it = mDetector.begin(); it != mDetector.end(); it++)
    {
        if(it->second.Module.substr(2, 5) == "Tamex")
        {
            vh1Tamex[0].at(it->second.ModuleId).clear();
            vh1Tamex[1].at(it->second.ModuleId).clear();
            vh2Tamex[0].at(it->second.ModuleId).clear();
            Int_t nbins, nbinf;
            Double_t smin, smax, fmin, fmax;
            if(it->second.Module == "nsTamex")
            {
                nbins = 2500;
                smin = 0.0;
                smax = 5000.0;
                nbinf = 1000;
                fmin = 0.0;
                fmax = 1000.0;
            }
            else
            {
                nbins = 2000;
                smin = 550.0;
                smax = 750.0;
                nbinf = 1000;
                fmin = 0.0;
                fmax = 100.0;
            }
            for(int i=0; i<it->second.NumberMOfodule; i++)
            {
                for(int j=0; j<16; j++)
                {
                    vh1Tamex[0].at(it->second.ModuleId).push_back(new TH1D(Form("ch %d Mod %d %s ToTs", j, i, it->second.Label.Data()), Form("%s ToTs Mod %d ch %d", it->second.Label.Data(), i, j), nbins, smin, smax));
                    httpServer->AddHis(Form("/%s/ToTs/Mod%d/", it->second.Label.Data(),i), vh1Tamex[0].at(it->second.ModuleId).at(i*16+j), Form("ch%d", j), "ToTs", "Count");
                    vh1Tamex[1].at(it->second.ModuleId).push_back(new TH1D(Form("ch %d Mod %d %s ToTf", j, i, it->second.Label.Data()), Form("%s ToTf Mod %d ch %d", it->second.Label.Data(), i, j), nbinf, fmin, fmax));
                    httpServer->AddHis(Form("/%s/ToTf/Mod%d/", it->second.Label.Data(),i), vh1Tamex[1].at(it->second.ModuleId).at(i*16+j), Form("ch%d", j) , "ToTf", "Count");
                    vh2Tamex[0].at(it->second.ModuleId).push_back(new TH2D(Form("ch %d Mod %d %s ToTf vs. ToTs", j, i, it->second.Label.Data()), Form("%s ToTf vs. ToTs Mod %d ch %d", it->second.Label.Data(), i, j),  nbins, smin, smax, nbinf, fmin, fmax));
                    httpServer->AddHis(Form("/%s/ToTf.vs.ToTs/Mod%d/", it->second.Label.Data(),i), vh2Tamex[0].at(it->second.ModuleId).at(i*16+j), Form("ch%d", j), "ToTs", "ToTf");
                }
            }
        }
        if(it->second.Module == "Febex")
        {
            vh1FebexRaw.at(it->second.ModuleId).clear();
            for(int i=0; i<it->second.NumberMOfodule; i++)
            {
                for(int j=0; j<16; j++)
                {
                    vh1FebexRaw.at(it->second.ModuleId).push_back(new TH1D(Form("ch %d Mod %d %s Raw", j, i, it->second.Label.Data()), Form("%s Raw Mod %d ch %d", it->second.Label.Data(), i, j), 1000, 0.0, 2000E3));
                     httpServer->AddHis(Form("/%s/Raw/Mod%d/", it->second.Label.Data(), i), vh1FebexRaw.at(it->second.ModuleId).at(i*16+j), Form("ch%d", j), "Raw", "Counts");
                }
            }
        }
        if(it->second.Module == "FEE")
        {
            vh1FEERaw.at(it->second.ModuleId).clear();
            for(int i=0; i<it->second.NumberMOfodule; i++)
            {
                for(int j=0; j<64; j++)
                {
                    vh1FEERaw.at(it->second.ModuleId).push_back(new TH1D(Form("ch %d Mod %d %s Raw", j, i, it->second.Label.Data()), Form("%s Raw Mod %d ch %d", it->second.Label.Data(), i, j), 500000, 20000.0, 70000));
                     httpServer->AddHis(Form("/%s/Raw/Mod%d/", it->second.Label.Data(), i), vh1FEERaw.at(it->second.ModuleId).at(i*64+j), Form("ch%d", j), "Raw", "Counts");
                }
            }
        }
        it->second.id = id;
        id++;
        httpServer->AddRate(it->second.Label, it->second.Name);
        Log::Output("Inf", Form("%s \t %d", it->second.Name.Data(), it->second.id));
        //    if(it->second.Module == "QDC")
    }
    httpServer->Config();
}

void Proc::ResetOnlineHis()
{
    for(int i=0; i<nTamexh1; i++)
        for(int j=0; j<int(vh1Tamex[i].size()); j++)
            for(int k=0; k<int(vh1Tamex[i].at(j).size()); k++)
                vh1Tamex[i].at(j).at(k)->Reset();
    for(int i=0; i<nTamexh2; i++)
        for(int j=0; j<int(vh2Tamex[i].size()); j++)
            for(int k=0; k<int(vh2Tamex[i].at(j).size()); k++)
                vh2Tamex[i].at(j).at(k)->Reset();
    for(int i=0; i<int(vh1FebexRaw.size()); i++)
        for(int j=0; j<int(vh1FebexRaw.at(i).size()); j++)
            vh1FebexRaw.at(i).at(j)->Reset();
    for(int i=0; i<int(vh1FEERaw.size()); i++)
        for(int j=0; j<int(vh1FEERaw.at(i).size()); j++)
            vh1FEERaw.at(i).at(j)->Reset();
}

void Proc::InitDecode()
{
}

void Proc::InitData()
{

    vWR.clear();

    for(int i=0; i<int(vTamex.size()); i++)
        vTamex[i].clear();
    for(int i=0; i<int(vFebex.size()); i++)
        vFebex[i].clear();
    for(int i=0; i<int(vFEE.size()); i++)
        vFEE[i].clear();
    for(int i=0; i<int(vQDC.size()); i++)
        vQDC[i].clear();
}

void Proc::Initopt()
{
    opt->Branch("WR", &vWR);
    for(auto it = mDetector.begin(); it != mDetector.end(); it++)
    {
        if(it->second.Module.substr(2, 5) == "Tamex")
            opt->Branch(it->second.Label.Data(), &vTamex[it->second.ModuleId]); 				//the vector must be finished befor se branch 
        if(it->second.Module == "Febex")
            opt->Branch(it->second.Label.Data(), &vFebex[it->second.ModuleId]);
        if(it->second.Module == "FEE")
            opt->Branch(it->second.Label.Data(), &vFEE[it->second.ModuleId]);
        if(it->second.Module == "QDC")
            opt->Branch(it->second.Label.Data(), &vQDC[it->second.ModuleId]);
    }
}
//-----------------------------------------------------------
// event function
Bool_t Proc::DecodeSubEvent(int* pdata_, int PrcId_, Long64_t nEv)
{
    //    Log::Output("Inf",  Form("Event No. %lld", nEv));
    if (pdata_ == NULL) 
    {
        Log::Output("Err", "Proc:::DecodeSubEvent() no input event !");
        return false;
    }
    pdata = pdata_;
    PrcId = PrcId_;  //Subsystem Id
    auto it = mDetector.find(PrcId);
    if(it != mDetector.end())
    {
        if(it->second.Module.substr(2, 5) == "Tamex")
        {
            vDecodeT[it->second.ModuleId]->Decode(pdata);
  //          vDecodeT[it->second.ModuleId]->PrintEvent();
            vTamex[it->second.ModuleId].assign(vDecodeT[it->second.ModuleId]->vData.begin(), vDecodeT[it->second.ModuleId]->vData.end());
      //      std::cout << "Size " << vTamex[it->second.ModuleId].size() << std::endl;
            if(it->second.WRid == vDecodeT[it->second.ModuleId]->wrd.id)
            {
                vDecodeT[it->second.ModuleId]->wrd.Name = it->second.Label;
                vWR.push_back(vDecodeT[it->second.ModuleId]->wrd);
            }
            else
            {
                std::cout << " The id of wr of " << it->second.Name << " (0x" << std::hex  << it->second.WRid << std::dec<<") is not the same as that (0x"<< std::hex << vDecodeT[it->second.ModuleId]->wrd.id << std::dec << ")" << std::endl; 
            }
            pdata = vDecodeT[it->second.ModuleId]->GetPdata();
        }
        else if(it->second.Module == "Febex")
        {
                vDecodeF[it->second.ModuleId]->Decode(pdata);
 //             vDecodeF[it->second.ModuleId]->PrintEvent();
                vFebex[it->second.ModuleId].assign(vDecodeF[it->second.ModuleId]->vData.begin(), vDecodeF[it->second.ModuleId]->vData.end());
            if(it->second.WRid == vDecodeF[it->second.ModuleId]->wrd.id)
            {
                vDecodeF[it->second.ModuleId]->wrd.Name = it->second.Label;
                vWR.push_back(vDecodeF[it->second.ModuleId]->wrd);
            }
            else
            {
                std::cout << " The id of wr of " << it->second.Name << " (0x" << std::hex  << it->second.WRid << std::dec<<") is not the same as that (0x"<< std::hex << vDecodeF[it->second.ModuleId]->wrd.id << std::dec << ")" << std::endl; 
            }
            pdata = vDecodeF[it->second.ModuleId]->GetPdata();
        }
        else if(it->second.Module == "FEE")
        {
                vDecodeFEE[it->second.ModuleId]->Decode(pdata);
    //          vDecodeFEE[it->second.ModuleId]->PrintEvent();
                vFEE[it->second.ModuleId].assign(vDecodeFEE[it->second.ModuleId]->vData.begin(), vDecodeFEE[it->second.ModuleId]->vData.end());
            if(it->second.WRid == vDecodeFEE[it->second.ModuleId]->wrd.id)
            {
                vDecodeFEE[it->second.ModuleId]->wrd.Name = it->second.Label;
                vWR.push_back(vDecodeFEE[it->second.ModuleId]->wrd);
            }
            else
            {
                std::cout << " The id of wr of " << it->second.Name << " (0x" << std::hex  << it->second.WRid << std::dec<<") is not the same as that (0x"<< std::hex << vDecodeFEE[it->second.ModuleId]->wrd.id << std::dec << ")" << std::endl; 
            }
            pdata = vDecodeFEE[it->second.ModuleId]->GetPdata();
        }
        else if(it->second.Module == "QDC")
        {
            vDecodeQDC[it->second.ModuleId]->Decode(pdata, it->second.NumberMOfodule);
            for(Int_t i=0; i<it->second.NumberMOfodule; i++) //Loop module
            {
                for(Int_t j=0; j<32; j++)
                {
                    QDCData qd;
                    qd.mid = i;
                    qd.ch = j;
                    qd.raw = vDecodeQDC[it->second.ModuleId]->raw[i][j][0];
                    vQDC[it->second.ModuleId].push_back(qd);

                }
            }
            pdata = vDecodeQDC[it->second.ModuleId]->GetPdata();
        }
    }
    else
    {
#if(!IgnoreOtherSubsystem)
        Log::Output("", Form("Don't know PrcId = %d", PrcId));
#endif
    }

    return true;
}

// histogramming function
Bool_t Proc::FillHistograms()
{
    if((std::time(nullptr) - tUpdate) ==  RefreshTime)
    {
//        std::cout << std::time(nullptr) << "  " << tUpdate << "  "  << std::endl;
        bUpdate = true;
        tUpdate = std::time(nullptr);
    }
    for(auto it = mDetector.begin(); it != mDetector.end(); it++)
    {
        if(it->second.Module.substr(2, 5) == "Tamex")
        {
            for(int i=0; i<int(vTamex[it->second.ModuleId].size()); i++)
            {
                if(vTamex[it->second.ModuleId].at(i).mid * 16 + vTamex[it->second.ModuleId].at(i).ch < int(vh1Tamex[0].at(it->second.ModuleId).size()))
                {
                       // if(vDecodeT[it->second.ModuleId]->vData.at(i).ToTf > 50)
                        vh1Tamex[0].at(it->second.ModuleId).at(vTamex[it->second.ModuleId].at(i).mid * 16 + vTamex[it->second.ModuleId].at(i).ch)->Fill(vTamex[it->second.ModuleId].at(i).ToTs);
                        vh1Tamex[1].at(it->second.ModuleId).at(vTamex[it->second.ModuleId].at(i).mid * 16 + vTamex[it->second.ModuleId].at(i).ch)->Fill(vTamex[it->second.ModuleId].at(i).ToTf);
                        vh2Tamex[0].at(it->second.ModuleId).at(vTamex[it->second.ModuleId].at(i).mid * 16 + vTamex[it->second.ModuleId].at(i).ch)->Fill(vTamex[it->second.ModuleId].at(i).ToTs, vTamex[it->second.ModuleId].at(i).ToTf);
                }
            }
            if(bUpdate)
            {
                httpServer->InitRateGui(it->second.id);
                for(int i=0; i<vDecodeT[it->second.ModuleId]->NTamexMax; i++)
                    for(int j=0; j<vDecodeT[it->second.ModuleId]->TamexChNum; j++)
                    {
    //            std::cout << i  << "  " << j << "  "  << vDecodeT[it->second.ModuleId]->RebuildEvent[i][j].size() << "   " << vDecodeT[it->second.ModuleId]->vData.size()<< std::endl;
                        if(vDecodeT[it->second.ModuleId]->RebuildEvent[i][j].size() > 0)
                        {
                            if(j%2 ==0)
                                httpServer->SetRate(it->second.id, i, j/2, "fast", int(vDecodeT[it->second.ModuleId]->RebuildEvent[i][j].size()));
                            else
                                httpServer->SetRate(it->second.id, i, j/2, "slow", int(vDecodeT[it->second.ModuleId]->RebuildEvent[i][j].size()));
          //                  std::cout << it->second.Name << "  " << i << "  " << j << "  " << vDecodeT[it->second.ModuleId]->RebuildEvent[i][j].size() << std::endl;
                        }
                    }
                httpServer->UpdateRateGui(it->second.id);
            }
                            
        }

        if(it->second.Module == "Febex")
        {
            for(int i=0; i<int(vDecodeF[it->second.ModuleId]->vData.size()); i++)
                if(vDecodeF[it->second.ModuleId]->vData.at(i).mid * 16 + vDecodeF[it->second.ModuleId]->vData.at(i).ch < int(vh1FebexRaw.at(it->second.ModuleId).size()))
                    vh1FebexRaw.at(it->second.ModuleId).at(vDecodeF[it->second.ModuleId]->vData.at(i).mid * 16 + vDecodeF[it->second.ModuleId]->vData.at(i).ch)->Fill(vDecodeF[it->second.ModuleId]->vData.at(i).raw);
            if(bUpdate)
            {
                httpServer->InitRateGui(it->second.id, "NonTamex");
                for(int i=0; i<vDecodeF[it->second.ModuleId]->NFebexMax; i++)
                    for(int j=0; j<vDecodeF[it->second.ModuleId]->FebexChNum; j++)
                    {
                        if(vDecodeF[it->second.ModuleId]->RebuildEvent[i][j].size() > 0)
                        {
                                httpServer->SetRate(it->second.id, i, j, "NonTamex", int(vDecodeF[it->second.ModuleId]->RebuildEvent[i][j].size()));
                        }
                    }
                httpServer->UpdateRateGui(it->second.id);
            }
        }
        if(it->second.Module == "FEE")
        {
            for(int i=0; i<int(vDecodeFEE[it->second.ModuleId]->vData.size()); i++)
                if(vDecodeFEE[it->second.ModuleId]->vData.at(i).mid * 64 + vDecodeFEE[it->second.ModuleId]->vData.at(i).ch < int(vh1FEERaw.at(it->second.ModuleId).size()))
                    vh1FEERaw.at(it->second.ModuleId).at(vDecodeFEE[it->second.ModuleId]->vData.at(i).mid * 64 + vDecodeFEE[it->second.ModuleId]->vData.at(i).ch)->Fill(vDecodeFEE[it->second.ModuleId]->vData.at(i).raw);
            if(bUpdate)
            {
      //          httpServer->InitRateGui(it->second.id, "NonTamex");
      //          for(int i=0; i<vDecodeF[it->second.ModuleId]->NFebexMax; i++)
      //              for(int j=0; j<vDecodeF[it->second.ModuleId]->FebexChNum; j++)
      //              {
      //                  if(vDecodeF[it->second.ModuleId]->RebuildEvent[i][j].size() > 0)
      //                  {
      //                          httpServer->SetRate(it->second.id, i, j, "NonTamex", int(vDecodeF[it->second.ModuleId]->RebuildEvent[i][j].size()));
       //                 }
       //             }
       //         httpServer->UpdateRateGui(it->second.id);
            }
        }
        //    if(it->second.Module == "QDC")
    }
    bUpdate = false;
    return kTRUE;
}

Bool_t Proc::SaveEvent()
{
    opt->Fill();
    FillHistograms();
    gSystem->ProcessEvents();
    httpServer->Update();
    if(BResetOnlineHis)
    {
        ResetOnlineHis();
        BResetOnlineHis = kFALSE;
    }
    if(vWR.size() > 0)
    {
        if(tStart == 0)
        {
            tStart = vWR.at(0).ts/1E9;
        }
        else
        {
            if((vWR.at(0).ts/1E9 - tStart) > RefreshTime*10 * ((tNow - tStart)/RefreshTime/10 + 1))
            {
                Log::Output("Inf",  Form("Run Time:  %lld s", tNow - tStart));
            }
        }
        if(vWR.at(0).ts/1E9 > tNow)
            tNow = vWR.at(0).ts/1E9;
    }
    InitData();
    return true;
}
