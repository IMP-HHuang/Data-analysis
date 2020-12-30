
#include "tree.h"
#include <TTree.h>
#include <TFile.h>
#include <TMath.h>
#include <TString.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

//using namespace std;

int main(int argc, char* argv[])
{
	if(argc != 2)
    {
      std::cout << "eg. ./xxx  [run number]" << std::endl;
      exit(1);
    }
  
  	int run = atoi(argv[1]);
//  	char wloop = 'a';  // one alpha
  	char wloop = 'b';  // more alpha

	std::cout << "input:" << Form("event_%04d_W8000.root", run) << std::endl;
  	auto ipf = new TFile(Form("../datain/event_%04d_W8000.root", run), "READ");
	if(ipf==0)
		std::cout << "Can not open the input file!" << std::endl;
  	auto ipt = (TTree*)ipf->Get("tree");

  	auto opf = new TFile(Form("../correlation/decay%c%04d.root", wloop, run), "RECREATE");
  	auto opt = new TTree("tree", "tree");
  	std::cout << "output: " << Form("decay%0d%c.root", run, wloop) << std::endl;
  	auto it = new tree(ipt);
  	if(wloop == 'a') 
		it->Loop1(opt,run);
  	if(wloop=='b') 
		it->Loop2(opt,run);
  	opt->Write();
  	opf->Close();
  
  	return 0;
}

