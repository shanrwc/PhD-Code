#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <stdlib.h>

#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TROOT.h"
#include "TSystem.h"

int main(int argc, char **argv)
{
  std::string type   = argv[1];
  std::transform(type.begin(), type.end(), type.begin(), ::toupper);

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
  std::string className = "SUSYD3PDClass_";
  if (type == "DATA") className += "Data";
  if (type == "MC")   className += "MC";
  std::cout << "| Generating class from tree 'susy'" << std::endl;
  mChain->MakeClass(className.c_str());
  //std::cout << "| Compiling " << className << std::endl;
  //std::string line = ".L " + className + ".C+";
  //gROOT->ProcessLine(line.c_str());

  std::cout << "| Editing " << className << ".h/C, adding 'std::'" << std::endl;
  std::string command;
  command = "sed -i \'s/vector/std::&/g\' " + className + ".h";
  system(command.c_str());
  command = "sed -i \'s/string/std::&/g\' " + className + ".h";
  system(command.c_str());
  command = "sed -i \'s/.*TrackParticleContainer_HLT_InDetTrigParticleCreation_Electron_EFID.*/\\/\\/&/g\' " + className + ".h";
  system(command.c_str());
  command = "sed -i \'s/.*TrackParticleContainer_HLT_InDetTrigParticleCreation_Electron_EFID.*/\\/\\/&/g\' " + className + ".C";
  system(command.c_str());
  /*
  if (chan == "data")
  {
    std::cout << "Calling ctor of ConvertD3PDToMTreeData" << std::endl;
    ConvertD3PDToMTreeData* c = new ConvertD3PDToMTreeData();
    std::cout << "Initialization start" << std::endl;
    c->initialize(fileList);
    std::cout << "Initialization complete" << std::endl;
    //c->DumpD3PDLeavesToFile("/home/harper/dev/testarea/16.0.1/D3PDConverter/SUSYLeavesMC.txt");
    std::cout << "Starting run()" << std::endl;
    c->run();
    std::cout << "Finished run()" << std::endl;
    std::cout << "Finalizing" << std::endl;
    c->finalize();
    std::cout << "Done" << std::endl;
  }
  else
  {
    ConvertD3PDToMTreeMC* c = new ConvertD3PDToMTreeMC();
    c->initialize(fileList);
    //c->DumpD3PDLeavesToFile("/home/harper/dev/testarea/16.0.1/D3PDConverter/SUSYLeavesMC.txt");
    c->run();
    c->finalize();
  }
  */
  std::cout << "| Done with prelim()" << std::endl;
  return 0;
}
