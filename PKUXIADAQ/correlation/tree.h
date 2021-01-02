#include <TTree.h>
#include <TFile.h>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>
#include <fstream>


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
	Short_t     raw;
  	Long64_t     ts;
  	double	     e;//刻度能量
  
//  	Bool_t          pileup;
//  	Bool_t          outofr;

}Detector;

typedef struct DATA
{
  	Double_t ex;
   	Double_t ey;	
   	Double_t e;	
  	Double_t x, y;
	Double_t boxe;
	Long64_t boxts;
  	Short_t boxid;
  	Long64_t ts;
	std::vector<Double_t> ge;
  	std::vector<ULong64_t> gts;
  	std::vector<Short_t> gid;
}Data;


class tree 
{
 public:

  	tree();
  	tree(TTree *ipt_);
	virtual ~tree();
	void GetBranch();   //SetBranchAddress
  	void Init();  
  	void InitData(Data* data);
  	void InitDetector(Detector* detector);
	void StripCut(std::vector<Detector> &a);	
  	void Loop1(TTree *opt_, Int_t run_);
  	void Loop2(TTree *opt_, Int_t run_);
  	void BranchOpt();
  	void GetEvent();

	std::vector<Dataevent>* br_data;

	std::vector<Detector> MWPC_vtmp;
  	std::vector<Detector> x_vtmp;
  	std::vector<Detector> y_vtmp;
	std::vector<Detector> box_vtmp;
	std::vector<Detector> veto_vtmp;
 

  	TTree *ipt;
  	TTree *opt;

	std::map<Long64_t, Data> mr;//recoil
	std::map<Long64_t, Data> md;//decay

  	///tree branches
  	Int_t run;

  	//recoil
  	Int_t irecoil;
  	Double_t re, rex, rey;
  	Double_t rx, ry;
  	Long64_t rts;
	//recoil gamma
	std::vector<Double_t> vrge;
  	std::vector<Long64_t> vrgts;
	std::vector<Short_t> vrgid;
 
  	//decay-dssd
  	Int_t idecay;
  	Double_t tdecay;  //ms
  	Double_t e, boxe;
  	Double_t ex, ey;
  	Double_t x, y;
  	Long64_t ts;
	Double_t boxts; 

  	//decay-gamma
	std::vector<Double_t> vdge;
  	std::vector<Long64_t> vdgts;
  	std::vector<Short_t> vdgid;

};
