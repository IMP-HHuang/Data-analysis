
#include "tree.h"
#include <TTree.h>
#include <TFile.h>
#include <TMath.h>
#include <TString.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[])
{
	if(argc != 2)
    {
      std::cout << "eg. ./xxx  [run number]" << std::endl;
      exit(1);
    }
  
  	int run = atoi(argv[1]);

	std::cout << "input:" << Form("event_%04d_W8000.root", run) << std::endl;
  	auto ipf = new TFile(Form("../datain/event_%04d_W8000.root", run), "READ");
	if(ipf==0)
		std::cout << "Can not open the input file!" << endl;
  	auto ipt = (TTree*)ipf->Get("tree");

  	auto opf = new TFile(Form("./draw%04d.root", run), "RECREATE");
  	auto it = new tree(ipt, opf);
	it->GetEvent();
  	opf->Close();
  
  	return 0;
}

