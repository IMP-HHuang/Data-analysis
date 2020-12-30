#include "tree.h"

bool SortDS(const Detector &a, const Detector &b)
{
	return a.raw > b.raw;
}

tree::tree()
{
	Init();
	std::cout << "No input tree " << std::endl;
	return;
}

tree::tree(TTree *ipt_)
{
	Init();
	GetBranch();
}

void tree::StripCut(std::vector<Detector> &a)
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
	br_data = NULL;
	MWPC_vtmp.clear();
	x_vtmp.clear();
	y_vtmp.clear();
	box_vtmp.clear();
	veto_vtmp.clear();
	ipt = NULL;
	opt = NULL;
	mr.clear();
	run = -1;
	irecoil = -1;
	re = -1.0;
	rex = -1.0;
	rey = -1.0;
	rx = -1.0;
	ry = -1.0;
	rts = -1;
	vrge.clear();
	vrgts.clear();
	vrgid.clear();
	idecay = -1;
	tdecay = -1.0;
	e = -1.0;
	ex = -1.0;
	ey = -1.0;
	x = -1.0;
	y = -1.0;
	boxe = -1.0;
	ts = -1;
	boxts = -1.0;
	vdge.clear();
	vdgts.clear();
	vdgid.clear();
}

void tree::InitData(Data* data)
{
	data->ex = -1.0;
	data->ey = -1.0;
	data->e = -1.0;
	data->x = -1.0;
	data->y = -1.0;
	data->boxe = -1.0;
	data->boxts = -1;
	data->boxid = -1;
	data->ts = -1;
	data->ge.clear();
	data->gts.clear();
	data->gid.clear();
}
void tree::InitDetector(Detector* detector)
{
	detector->id = -1;
	detector->raw = -1;
	detector->ts = -1;
	detector->e = -1.0;
}

void tree::GetBranch()
{
  	if (ipt == NULL)
    	return;
  	ipt->SetBranchAddress("event", &br_data);
}

void tree::GetEvent()
{
  	mr.clear();
  	md.clear();

  	Long64_t nentries = ipt->GetEntriesFast();
  	for (Long64_t jentry = 0; jentry < nentries; jentry++)
  	{
		Data data;
		InitData(&data);
    	ipt->GetEntry(jentry);
		x_vtmp.clear();
		y_vtmp.clear();
		veto_vtmp.clear();
		MWPC_vtmp.clear();
		box_vtmp.clear();
      	for (int i = 0; i < br_data->size(); i++)
		{
			//MWPC----?
			if((*br_data)[i].det == 4)
			{
				Detector detector;
				InitDetector(&detector);
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
				InitDetector(&detector);
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
				InitDetector(&detector);
				if((*br_data)[i].raw < 0)
					continue;
				detector.id = (*br_data)[i].id;
				detector.ts = (*br_data)[i].ts*8;
				detector.raw = (*br_data)[i].raw;
				detector.e = (*br_data)[i].e;
				x_vtmp.push_back(detector);
			}
			
			//Si_Box
            if((*br_data)[i].det==2)
            {
				Detector detector;
				InitDetector(&detector);
                if((*br_data)[i].raw < 0)
                    continue;
				detector.id = (*br_data)[i].id;
				detector.ts = (*br_data)[i].ts*10;
				detector.raw = (*br_data)[i].raw;
				detector.e = (*br_data)[i].e;
				box_vtmp.push_back(detector);
            }

			
			//veto
			if((*br_data)[i].det==3)
			{
				Detector detector;
				InitDetector(&detector);
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
		sort(box_vtmp.begin(), box_vtmp.end(), SortDS);

		for(auto it = MWPC_vtmp.begin(); it != MWPC_vtmp.end(); )
		{
			bool ber = false;
            if(it->id==0)
            {
                ber = ((x_vtmp[0].ts - it->ts) < -1250 || (x_vtmp[0].ts - it->ts) > -1100);
            }
            if(it->id==1)
            {
                ber = ((x_vtmp[0].ts - it->ts) < -1250 || (x_vtmp[0].ts - it->ts) > -1100);
            }
			if(ber)
				it = MWPC_vtmp.erase(it);
			else
				++it;
		}
/*
		for(auto it = veto_vtmp.begin(); it != veto_vtmp.end(); )
		{
			bool ber = false;
            ber = ((x_vtmp[0].ts - it->ts) < -1500 || (x_vtmp[0].ts - it->ts) > 1500);
			if(ber)
				it = veto_vtmp.erase(it);
			else
				++it;
		}
*/
		for(auto it = box_vtmp.begin(); it != box_vtmp.end(); )
		{
			bool ber = false;
            ber = ((x_vtmp[0].ts - it->ts) < -600 || (x_vtmp[0].ts - it->ts) > 200);
			if(ber)
				it = box_vtmp.erase(it);
			else
				++it;
		}

		StripCut(y_vtmp);
		StripCut(x_vtmp);

    	if (x_vtmp.size() == 0 || y_vtmp.size() == 0)
      		continue;
		
    	//front-back correlation
		/*
    	if (xesum < 14000 && abs(xesum - yesum) > 200)
      		continue;
    	if (xesum > 14000 && abs(xesum - yesum) > 500)
      		continue;
		*/	
    	//calculate x,y positions
    	double x = -1.0;
		double y = -1.0;
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
      	data.ex = x_vtmp[0].e;
		data.ey = y_vtmp[0].e;
      	data.x = x;
     	data.y = y;
    	data.ts = x_vtmp[0].ts;
		data.boxe = -1.0;
		data.boxts = -1;
		if(box_vtmp.size()>0)
		{
			data.boxe = box_vtmp[0].raw;
			data.boxts = box_vtmp[0].ts;
		}
    	if (veto_vtmp.size()==0 &&  MWPC_vtmp.size() > 0 &&  xse > 15000 )
    	{//recoil
			mr[data.ts] = data;
      	}
    	if (veto_vtmp.size()==0 &&  MWPC_vtmp.size() == 0  && xse < 10000)
    	{ //decay
      		md[data.ts] = data;
      	}
    	if (jentry % 1000 == 0)
    	{
			std::cout << TString::Format("Process %.2f %%, %lldk / %lldk\r", Double_t(jentry) / nentries * 100.0,
			jentry / 1000, nentries / 1000);
			std::cout << std::flush;
    	}
    }
	std::cout << "The number of recoil/decay : " << mr.size() << "  " << md.size() << std::endl;
}

void tree::Loop1(TTree *opt_, Int_t run_)
{
  	if (opt_ == NULL)
    	return;
  	opt = opt_;
  	run = run_;
  	BranchOpt();
	std::cout << "making recoil/decay evnts..." << std::endl;
  	GetEvent();

  	Long64_t us = 1000;
  	Long64_t ms = 1000 * us;
  	Long64_t sec = 1000 * ms;
  	Long64_t twindow1 = -1 * sec;   //100s
  	Long64_t twindow2 = 1 * sec; //100s
  	irecoil = 0;
	std::cout << "recoil-decay correlation using Method A ..." << std::endl;
	std::cout << TString::Format("Decay Time Window = %.1lld ms ~ %.1lld ms\n", Long64_t(twindow1) / ms, Long64_t(twindow2) / ms) << std::endl;

  	for (auto ir = mr.begin(); ir != mr.end(); ir++)
  	{
    	//recoil
    	rx = ir->second.x;
    	ry = ir->second.y;
    	re = ir->second.e;
		rex = ir->second.ex;
		rey = ir->second.ey;
    	rts = ir->second.ts;
    	vrge.clear();
    	vrgid.clear();
    	vrgts.clear();
		vrge.assign(ir->second.ge.begin(), ir->second.ge.end());
		vrgid.assign(ir->second.gid.begin(), ir->second.gid.end());
    	for (int i = 0; i < vrge.size(); i++)
    	{
     // 		Long64_t tts = Long64_t(ir->second.gts[i] - rts) - 100 / sqrt(vge[i]) - 200;
			Long64_t tts = ir->second.gts[i];
      		vrgts.push_back(tts);
    	}
    	//recoil-decay correlation
    	idecay = 0;
    	auto ib = md.lower_bound(ir->first + twindow1);
    	for (auto ic = ib; ic != md.end(); ic++)
    	{
      		if (ic->first >= ir->first + twindow2)
        		break;
      		double delx = abs(ic->second.x - ir->second.x);
      		double dely = abs(ic->second.y - ir->second.y);
      		if (delx > 0.95 || dely > 0.95)
        		continue;
      		//decay
      		e = ic->second.e;
			ex = ic->second.ex;
			ey = ic->second.ey;
      		boxe = ic->second.boxe;
			boxts = ic->second.boxts;
      		x = ic->second.x;
      		y = ic->second.y;
      		ts = ic->second.ts;
      		tdecay = Double_t(ic->first - ir->first) / Double_t(ms); //ms
      		vdge.clear();
      		vdgid.clear();
      		vdgts.clear();
	  		vdge.assign(ic->second.ge.begin(),ic->second.ge.end());
	  		vdgid.assign(ic->second.gid.begin(),ic->second.gid.end());
      		for (int i = 0; i < vdge.size(); i++)
      		{
      //  		Long64_t tts = Long64_t(ic->second.gts[i] - ts) - 70 / sqrt(vdge[i]) - 226;
	  			Long64_t tts = ic->second.gts[i];
        		vdgts.push_back(tts);
      		}
      		opt->Fill();
      		idecay++;
    	}
    	irecoil++;
    	if (irecoil % 500 == 0)
    	{
			std::cout << TString::Format("Process %.2f %%, %d / %ld\r", Double_t(irecoil) / mr.size() * 100.,
										irecoil, mr.size());
			std::cout << std::flush;
    	}
    }
	std::cout << std::endl;
}

void tree::Loop2(TTree *opt_, Int_t run_)
{
  	if (opt_ == NULL)
    	return;
  	opt = opt_;
  	run = run_;
  	BranchOpt();
	std::cout << "making recoil/decay evnts..." << std::endl;
  	GetEvent();

  	Long64_t us = 1000;
  	Long64_t ms = 1000 * us;
  	Long64_t sec = 1000 * ms;
  	irecoil = 0;
	std::cout << "recoil-decay correlation using Method B ..." << std::endl;
  	for (auto ir = mr.begin(); ir != mr.end(); ir++)
  	{//recoil
    	rx = ir->second.x;
    	ry = ir->second.y;
		re = ir->second.e;
		rex = ir->second.ex;
		rey = ir->second.ey;
    	rts = ir->second.ts;
    	vrge.clear();
    	vrgid.clear();
    	vrgts.clear();
		vrge.assign(ir->second.ge.begin(),ir->second.ge.end());
		vrgid.assign(ir->second.gid.begin(),ir->second.gid.end());
    	for (int i = 0; i < vrge.size(); i++)
    	{
//      		Long64_t tts = Long64_t(ir->second.gts[i] - rts) - 100 / sqrt(vge[i]) - 200;
			Long64_t tts = ir->second.gts[i];
      		vrgts.push_back(tts);
    	}
    	//recoil events
    	idecay = 0;
    	auto ir1 = mr.upper_bound(ir->first);
    	while (ir1 != mr.end())
    	{
      		double delx = abs(ir->second.x - ir1->second.x);
      		double dely = abs(ir->second.y - ir1->second.y);
      		if (delx < 0.95 && dely < 0.95)
      		{
        		break;
      		}
      		ir1++;
    	}
    	//decay events
    	auto id = md.upper_bound(ir->first);
    	while (id != md.end())
    	{
      		if (id->first > ir1->first)
      			break;
      		double delx = abs(id->second.x - ir->second.x);
      		double dely = abs(id->second.y - ir->second.y);
      		if (delx > 0.95 || dely > 0.95)
			{
				id++;
        		continue;
			}
      		//decay
      		e = id->second.e;
			ex = id->second.ex;
			ey = id->second.ey;
      		boxe = id->second.boxe;
			boxts = id->second.boxts;
/*
			if(idecay == 1)
			{
				if(abs(e - 7263) < 50 || abs(e - 7080) < 50)
				{
					std::cout << "\033[33m ***************************************************************" << "\033[0m" << std::endl;
					std::cout << "\033[31m Maybe new nuclei At-190 : " << "\033[0m" << std::endl;
					std::cout << "\033[34m Entry number \t Daughter decay Energy \t" << "\033[0m" << std::endl;
					std::cout << "\033[32m	" << opt->GetEntries() << "\t" << e << "\033[0m" <<std::endl;
					std::cout << "\033[<F3>33m ***************************************************************" << "\033[0m" <<std::endl;
				}
			}
*/
      		x = id->second.x;
      		y = id->second.y;
      		ts = id->second.ts;
      		tdecay = Double_t(id->first - ir->first) / Double_t(ms); //ms
      		vdge.clear();
      		vdgid.clear();
      		vdgts.clear();
	  		vdge.assign(id->second.ge.begin(), id->second.ge.end());
	  		vdgid.assign(id->second.gid.begin(), id->second.gid.end());
      		for (int i = 0; i < vdge.size(); i++)
      		{
 //       		Long64_t tts = Long64_t(id->second.gts[i] - ts) - 70 / sqrt(vce[i]) - 226;
				Long64_t tts = id->second.gts[i];
        		vdgts.push_back(tts);
      		}
      		opt->Fill();
      		idecay++;
      		id++;
    	}
    	irecoil++;
    	if (irecoil % 50 == 0)
    	{
			std::cout << TString::Format("Process %.2f %%, %d / %ld\r", Double_t(irecoil) / mr.size() * 100.,
										irecoil, mr.size());
			std::cout << std::flush;
    	}
	}
	std::cout << std::endl;
}

void tree::BranchOpt()
{
  	opt->Branch("run", &run, "run/I");  //file

  	opt->Branch("irecoil", &irecoil, "irecoil/I");
	opt->Branch("re", &re, "re/D");
	opt->Branch("rex", &rex, "rex/D");
	opt->Branch("rey", &re, "rey/D");
  	opt->Branch("rx", &rx, "rx/D");
  	opt->Branch("ry", &ry, "ry/D");
  	opt->Branch("rts", &rts, "rts/L");
  	opt->Branch("rge", &vrge);
  	opt->Branch("rgts", &vrgts);
  	opt->Branch("rgid", &vrgid);

  	opt->Branch("idecay", &idecay, "idecay/I");
  	opt->Branch("tdecay", &tdecay, "tdecay/D");  //ms
  	opt->Branch("e", &e, "e/D");
  	opt->Branch("ex", &ex, "ex/D");
  	opt->Branch("ey", &ey, "ey/D");
  	opt->Branch("x", &x, "x/D");
  	opt->Branch("y", &y, "y/D");
  	opt->Branch("ts", &ts, "ts/L");
  	opt->Branch("boxe", &boxe, "boxe/D");
  	opt->Branch("boxts", &boxts, "boxts/L");

  	opt->Branch("dge", &vdge);
  	opt->Branch("dgts", &vdgts);
  	opt->Branch("dgid", &vdgid);
}
