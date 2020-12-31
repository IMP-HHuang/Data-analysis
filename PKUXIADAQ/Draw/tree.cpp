#include "tree.h"
using namespace std;

bool SortDS(const Detector &a, const Detector &b)
{
	return a.raw > b.raw;
}
void StripCut(vector<Detector> &a)
{
	for(auto it = a.begin(); it != a.end(); )
	{
		if(TMath::Abs(it->id - a[0].id > 1))
			it = a.erase(it);
		else
			++it;
	}
}
void tree::Init()
{
  	if (ipt == NULL)
    	return;
	br_data = NULL;
  	ipt->SetBranchAddress("event", &br_data);
}

void tree::GetEvent()
{
  	mr.clear();
  	md.clear();
	hdtxy = new TH1D("dtxy", "dtxy", 16000, -8000.0, 8000.0);
	hdtxm0 = new TH1D("dtxm0", "dtxm0", 16000, -8000.0, 8000.0);
	hdtxm1 = new TH1D("dtxm1", "dtxm1", 16000, -8000.0, 8000.0);
	hdtxv = new TH1D("dtxv", "dtxv", 16000, -8000.0, 8000.0);
	hdtxb = new TH1D("dtxb", "dtxb", 16000, -8000.0, 8000.0);
	h2rexy = new TH2D("h2rexy", "h2rexy", 5000, 0.0, 150000, 5000, 0.0, 150000.0);
	h2dexy = new TH2D("h2dexy", "h2dexy", 5000, 0.0, 150000, 5000, 0.0, 150000.0);
	h2exy = new TH2D("h2exy", "h2exy", 5000, 0.0, 150000, 5000, 0.0, 150000.0);
	h2dexb = new TH2D("h2dexb", "h2dexb", 300, 0.0, 9000, 300, 0.0, 9000.0);
  	Long64_t nentries = ipt->GetEntriesFast();
  	for (Long64_t jentry = 0; jentry < nentries; jentry++)
  	{
		Data data;
    	ipt->GetEntry(jentry);
		x_vtmp.clear();
		y_vtmp.clear();
		veto_vtmp.clear();
		MWPC_vtmp.clear();
      	for (int i = 0; i < br_data->size(); i++)
		{
			//MWPC----4
			if((*br_data)[i].det == 4)
			{
				Detector detector;
				//MWPC1  threshold
				if((*br_data)[i].id == 0 && (*br_data)[i].raw < 10)
					continue;
				//MWPC2  threshold
				if((*br_data)[i].id == 1 && (*br_data)[i].raw < 10)
					continue;
				detector.id = (*br_data)[i].id;
				detector.ts = (*br_data)[i].ts*10;
				detector.raw = (*br_data)[i].raw;
				MWPC_vtmp.push_back(detector);
			}
			//DSSD y
			if((*br_data)[i].det==0)
			{
				Detector detector;
				if((*br_data)[i].raw < 0)
					continue;
				detector.id = (*br_data)[i].id;
				detector.ts = (*br_data)[i].ts*10;
				detector.raw = (*br_data)[i].raw;
				detector.e = (*br_data)[i].e;
				y_vtmp.push_back(detector);
			}
			// DSSD x
			if((*br_data)[i].det==1)
			{
				Detector detector;
				if((*br_data)[i].raw < 0)
					continue;
				detector.id = (*br_data)[i].id;
				detector.ts = (*br_data)[i].ts*8;
				detector.raw = (*br_data)[i].raw;
				detector.e = (*br_data)[i].e;
				x_vtmp.push_back(detector);
			}
			
			//Si_Box
			data.boxe = -1.0;	
			if((*br_data)[i].det==2)
			{
				if((*br_data)[i].raw < 0 || (*br_data)[i].raw < data.boxe)
					continue;
				data.boxe = (*br_data)[i].raw;  //未刻度
				data.boxts = (*br_data)[i].ts*10;
				data.boxid = (*br_data)[i].id;
			}
		
			
			//veto
			if((*br_data)[i].det==3)
			{
				Detector detector;
				if((*br_data)[i].raw < 0)
					continue;
				detector.id = (*br_data)[i].id;
				detector.ts = (*br_data)[i].ts*10;
				detector.raw = (*br_data)[i].raw;
				veto_vtmp.push_back(detector);
			}
			
			//Ge : plan Clover  HPGe
			/*
			if((*br_data)[i].det == 4)
			{
				data.ge.push_back((*br_data)[i].e);
				data.gts.push_back((*br_data)[i].ts);
				data.gid.push_back((*br_data)[i].id);
			}
			*/
		}
//		cout << MWPC_vtmp.size() << "  "  << x_vtmp.size() << "  " << y_vtmp.size() <<"  "<<veto_vtmp.size() << endl;
		sort(MWPC_vtmp.begin(), MWPC_vtmp.end(), SortDS);
		sort(x_vtmp.begin(), x_vtmp.end(), SortDS);
		sort(y_vtmp.begin(), y_vtmp.end(), SortDS);
		sort(veto_vtmp.begin(), veto_vtmp.end(), SortDS);

		StripCut(y_vtmp);
		StripCut(x_vtmp);

    	if (x_vtmp.size() == 0 || y_vtmp.size() == 0)
      		continue;
		h2exy->Fill(x_vtmp[0].e, y_vtmp[0].e);
		hdtxy->Fill(x_vtmp[0].ts-y_vtmp[0].ts);
		for(int i=0; i< MWPC_vtmp.size(); i++)
		{
			if(MWPC_vtmp[i].id==0)
			{
				hdtxm0->Fill(x_vtmp[0].ts-MWPC_vtmp[i].ts);
			}
			if(MWPC_vtmp[i].id==1)
			{
				hdtxm1->Fill(x_vtmp[0].ts-MWPC_vtmp[i].ts);
			}
		}
		for(int i=0; i < veto_vtmp.size(); i++)
		{
			hdtxv->Fill(x_vtmp[0].ts-veto_vtmp[i].ts);
		}
		if(data.boxts>0)
		{
			hdtxb->Fill(x_vtmp[0].ts-data.boxts);
			h2dexb->Fill(x_vtmp[0].e, data.boxe);
		}
    	//front-back correlation
		/*
    	if (xesum < 14000 && abs(xesum - yesum) > 200)
      		continue;
    	if (xesum > 14000 && abs(xesum - yesum) > 500)
      		continue;
		*/	
    	//calculate x,y positions
    	double x, y;
    	double xeid = 0;
    	double xse = 0;
    	for (int i = 0; i < x_vtmp.size(); i++)
    	{
      		xeid += x_vtmp[i].id * x_vtmp[i].e;
      		xse += x_vtmp[i].e;
    	}
    	x = xeid / xse;
    	double yeid = 0;
    	double yse = 0;
    	for (int i = 0; i < y_vtmp.size(); i++)
    	{
      		yeid += y_vtmp[i].id * y_vtmp[i].e;
      		yse += y_vtmp[i].e;
    	}
    	y = yeid / yse;
		data.e = xse;
      	data.xe = x_vtmp[0].e;
		data.ye = y_vtmp[0].e;
      	data.x = x;
     	data.y = y;
    	data.ts = x_vtmp[0].ts;
//		cout << x_vtmp[0].ts << "   "   << y_vtmp[0].ts << "  " << x_vtmp[0].ts-y_vtmp[0].ts << endl;
    	if (veto_vtmp.size()==0 &&  MWPC_vtmp.size() > 0 && (x_vtmp[0].ts - MWPC_vtmp[0].ts) > 10 && xse > 15000 )
    	{//recoil
			h2rexy->Fill(data.xe, data.ye);
			mr[data.ts] = data;
      	}
    	if (veto_vtmp.size()==0 &&  MWPC_vtmp.size() == 0  && xse < 10000)
    	{ //decay
			h2dexy->Fill(data.xe, data.ye);
      		md[data.ts] = data;
      	}
    	if (jentry % 1000 == 0)
    	{
			std::cout << TString::Format("Process %.2f %%, %lldk / %lldk\r", Double_t(jentry) / nentries * 100.0,
			jentry / 1000, nentries / 1000);
			std::cout << std::flush;
    	}
    }
  	cout << "The number of recoil/decay : " << mr.size() << "  " << md.size() << endl;
	opf->cd();
	hdtxy->Write();
	hdtxy->Delete();
	hdtxm0->Write();
	hdtxm0->Delete();
	hdtxm1->Write();
	hdtxm1->Delete();
	hdtxv->Write();
	hdtxv->Delete();
	hdtxb->Write();
	hdtxb->Delete();
	h2exy->Write();
	h2exy->Delete();
	h2rexy->Write();
	h2rexy->Delete();
	h2dexy->Write();
	h2dexy->Delete();
	h2dexb->Write();
	h2dexb->Delete();
}

