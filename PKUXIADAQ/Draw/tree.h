#include <TTree.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

typedef struct DATAEVENT
{
  	Short_t      det;//探测器类型
  	Short_t      id;//探测器编号

  	Short_t         sr;
  	Long64_t        ts;
  	UShort_t        raw;//原始能量道址
  	double	  e;//刻度能量
  
  	Bool_t          pileup;
  	Bool_t          outofr;
  	Short_t         cfd;
  	Bool_t          cfdft;
  	Short_t         cfds;

  	// Long64_t        tsflag;
  	// Short_t         sid;
  	// Short_t         cid;
  	// Short_t         ch;
  	// UShort_t        evte;
}Dataevent;
typedef struct DETECTOR
{
  	Short_t      id;//探测器编号
	UShort_t     raw;
  	Long64_t     ts;
  	double	     e;//刻度能量
  
//  	Bool_t          pileup;
//  	Bool_t          outofr;

}Detector;

typedef struct DATA
{
  	Double_t xe;
   	Double_t ye;	
   	Double_t e;	
  	Double_t x, y;
	Double_t boxe;
	Long64_t boxts;
  	Short_t boxid;
  	Long64_t ts;
  	vector<Double_t> ge;
  	vector<ULong64_t> gts;
  	vector<Short_t> gid;
}Data;


class tree 
{
 	public:
  	vector<Dataevent>* br_data;

  	vector<Detector> MWPC_vtmp;
  	vector<Detector> x_vtmp;
  	vector<Detector> y_vtmp;
	vector<Detector> veto_vtmp;
 
  	tree()
	{
			ipt = NULL; 
			opf = NULL;
			br_data = NULL;
	}
  	tree(TTree *ipt_, TFile *opf_)
	{
		ipt = NULL; 
		opf = NULL;
		br_data = NULL;
    	ipt = ipt_;
		opf = opf_;
    	Init();
  	}
  	void Init();
  	void GetEvent();

  	TTree *ipt;
	TFile *opf;
	TH1D *hdtxy;
	TH1D *hdtxm0;
	TH1D *hdtxm1;
	TH1D *hdtxv;
	TH1D *hdtxb;
	TH2D *h2exy;
	TH2D *h2rexy;
	TH2D *h2dexy;
	TH2D *h2dexb; 

  	map<Long64_t, Data> mr;//recoil
  	map<Long64_t, Data> md;//decay

  	///tree branches
  	Int_t run;


};
