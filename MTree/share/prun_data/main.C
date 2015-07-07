#include "ConvertD3PDToMTreeData.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include "TSystem.h"

int main(int argc, char **argv)
{
  gSystem->Load("libConvertD3PDToMTree.so");
  std::string chan   = argv[1];

  // split by ' ' first
  std::ifstream infile("input.txt");
  std::string argStr;
  std::getline(infile, argStr);
  std::vector<std::string> fileList;
  for (size_t i = 0,n; i <= argStr.length(); i = n+1)
  {
    n = argStr.find_first_of(',',i);
    if (n == std::string::npos) n = argStr.length();
    std::string tmp = argStr.substr(i,n-i);
    fileList.push_back(tmp);
  }

  TChain *mChain = new TChain("susy");
  for (int i = 0; i < (int)fileList.size(); i++)
  {
    mChain->Add(fileList[i].c_str());
  }

  int nrEntries = mChain->GetEntries();
  std::cout << "After loading, entries: " << nrEntries << std::endl;

  ConvertD3PDToMTreeData *c = new ConvertD3PDToMTreeData(mChain);
  c->initialize();
  //c->DumpD3PDLeavesToFile("/home/harper/dev/testarea/16.0.2/MTree/share/prun_data/leaves.txt");
  c->run();
  c->finalize();

  /*
  if (chan == "data")
  {
    gSystem->Load("SUSYD3PDClass_Data_C.so");
    std::cout << "Calling ctor of ConvertD3PDToMTreeData" << std::endl;
    ConvertD3PDToMTreeData* c = new ConvertD3PDToMTreeData();
    std::cout << "Initialization start" << std::endl;
    c->initialize(chan, fileList);
    std::cout << "Initialization complete" << std::endl;
    std::cout << "Starting run()" << std::endl;
    c->run();
    std::cout << "Finished run()" << std::endl;
    std::cout << "Finalizing" << std::endl;
    c->finalize();
    std::cout << "Done" << std::endl;
  }
  */
  return 0;
}
