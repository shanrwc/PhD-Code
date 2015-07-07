#ifndef _WCANDIDATE_HPP_
#define _WCANDIDATE_HPP_

#include <iostream>
#include <fstream>
#include <string>

#include "MTree/Electron.h"
#include "MTree/EventInfo.h"
#include "MTree/MissingET.h"
#include "MTree/Muon.h"

class WCandidate
{

 private:
  MTree::EventInfo eventInfo;
  MTree::Electron electron;
  MTree::MissingET missingEt;
  MTree::Muon muon;

  bool isMuon;

  double deltaPhi;
  double mT;

 public:
  //constructors, one for each flavor of decay and one default.  While I found the default
  //necessary to avoid weird coding gymnastics in pulling these objects out of vectors, I
  //did not want to have one.  So avoid using it.
  WCandidate(MTree::EventInfo info, MTree::Electron elec, MTree::MissingET met);
  WCandidate(MTree::EventInfo info, MTree::Muon muon, MTree::MissingET met);
  WCandidate();

  //destructor
  ~WCandidate();

  //Getter, print and assignment functions
  MTree::EventInfo getEventInfo() const;
  MTree::Electron getElectron() const;
  MTree::Muon getMuon() const;
  MTree::MissingET getMissingEt() const;
  bool decayedToMuon() const;

  double getMET();
  double getDPhi() const;
  double getMT() const;
  double getWPt();

  void printInfo(std::ofstream& outfile);

  WCandidate &operator=(const WCandidate &rhs);
};
#endif
