#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "TFile.h"
#include "TH1.h"

void makeHistTxt(std::string sample = "geowiresag")
{
  //Get input from all the files.  Store it in . . . something.
  //For now, blend eta and phi regions but keep charges separate.

  std::vector<std::string> charges;
  charges.push_back("p");
  charges.push_back("n");

  std::vector<std::string> phases;
  phases.push_back("1");
  phases.push_back("2");
  phases.push_back("3");
  phases.push_back("4");
  phases.push_back("5");

  for (int nQ = 0; nQ != (int)charges.size(); ++nQ)
  {
    //Declare thing to store results in
    std::vector<double> means;
    for (int nPhase = 0; nPhase != (int)phases.size(); ++nPhase)
    {
      std::string fileName = sample + "_reso1OP_mean_etaslice_" + phases[nPhase] + charges[nQ] + ".txt";

      std::ifstream myFile;
      myFile.open(fileName.c_str());
      std::string input;
      if (myFile.open())
      {
	while (!myFile.eof())
	{
	  myFile >> input;

	  std::string temp = input.substr(input.find(" "), 7);
	  means.push_back(strtod(temp));
	}
      }
    }
  }

//  ifstream myReadFile;
//  myReadFile.open("text.txt");
//  char output[100];
//  if (myReadFile.is_open()) {
//  while (!myReadFile.eof()) {


//     myReadFile >> output;
//     cout<<output;


//  }
// }
// myReadFile.close();
// return 0;

}
