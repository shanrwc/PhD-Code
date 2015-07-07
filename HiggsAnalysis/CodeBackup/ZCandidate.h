#ifndef _ZCANDIDATE_HPP_
#define _ZCANDIDATE_HPP_

#include <iostream>
#include <fstream>
#include <string>

#include "MTree/Electron.h"
#include "MTree/EventInfo.h"
#include "MTree/Muon.h"

class ZCandidate
{
 private:
  MTree::EventInfo eventInfo;
  MTree::Electron electronL;
  MTree::Electron electronS;
  MTree::Muon muonL;
  MTree::Muon muonS;

  bool isMuon;

  double ptZ;
  double massZ;

 public:
  //constructors, one for each flavor of decay
  ZCandidate(MTree::EventInfo info, MTree::Electron elec1, MTree::Electron elec2);
  ZCandidate(MTree::EventInfo info, MTree::Muon mu1, MTree::Muon mu2);

  //destructor
  ~ZCandidate();

  //Getter and print functions
  MTree::EventInfo getEventInfo() const;
  MTree::Electron getLeadElectron() const;
  MTree::Electron getSubElectron() const;
  MTree::Muon getLeadMuon() const;
  MTree::Muon getSubMuon() const;

  bool decayedtoMuons() const;

  double getZpt() const;
  double getZMass() const;

  void printInfo(std::ofstream& outfile);
};

#endif
