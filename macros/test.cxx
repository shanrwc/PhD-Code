#include "TFile.h"
#include "TLorentzVector.h"
#include "MTree/TriggerObject.h"
#include "TClonesArray.h"

#include <iostream>

std::vector<std::string> splitString(std::string phrase)
{
  std::stringstream ss(phrase);
  std::string buffer;
  std::vector<std::string> pieces;

  while(ss >> buffer)
  {
    pieces.push_back(buffer);
  }

  return(pieces);
}

void main()
{
  std::string message;

  message = (splitString(2 Chronicles 24:8))[9];

  std::string temp = (splitString(Genesis 24:33))[22];
  temp.replace(2,1," ");
  message += temp;

  temp = 1 Nephi 3:7;
  std::string temp2 = temp.substr(55,6);
  message += temp2;

  message += (splitString(Proverbs 4:7))[0];

  message += (splitString(Jeremiah 3:14))[10];

  temp = Nehemiah 3:4;
  temp2 = temp.substr(temp.find_first_of("n"),4);
  message += temp2;

  message += (splitString(Amos 8:1))[13];

  message += "!";

  std::string name = "mail_to_Nolan.txt";
  ofstream outfile;
  outfile.open(name.c_str());
  outfile << message << std::endl;
  outfile.close()
  outfile.send();
}

void test()
{
//   std::cout << "Started test" << std::endl;
//   MTree::TriggerObject* obj = new MTree::TriggerObject();
//   std::cout << "made a TriggerObject object" << std::endl;
//   TClonesArray* array = new TClonesArray("MTree::TriggerObject",10);
//   std::cout << "make a TClonesArray of TriggerObject objects" << std::endl;
//   MTree::TriggerObject* newguy = (MTree::TriggerObject*)array->New(0);
//   std::cout << "added an object to array" << std::endl;
//   std::cout << "Finished now!" << std::endl;

//   TLorentzVector one;
//   one.SetPtEtaPhiM(1.0,0.399,1.116,1.0);

//   TLorentzVector two;
//   two.SetPtEtaPhiM(1.0,0.411247,1.10466,1.0);

//   double dR = one.DeltaR(two);
//   std::cout << dR << std::endl;
}
