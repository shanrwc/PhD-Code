#include "ZCandidate.h"

ZCandidate::ZCandidate(MTree::EventInfo info, MTree::Electron elec1, MTree::Electron elec2)
{
  eventInfo = info;

  if (elec1.Pt() > elec2.Pt())
  {
    electronL = elec1;
    electronS = elec2;
  }
  else
  {
    electronS = elec1;
    electronL = elec2;
  }

  isMuon = false;

  TLorentzVector cand = electronL + electronS;
  ptZ = cand.Pt();
  massZ = cand.M();
}

ZCandidate::ZCandidate(MTree::EventInfo info, MTree::Muon mu1, MTree::Muon mu2)
{
  eventInfo = info;

  if (mu1.Pt() > mu2.Pt())
  {
    muonL = mu1;
    muonS = mu2;
  }
  else
  {
    muonL = mu2;
    muonS = mu1;
  }

  isMuon = true;

  TLorentzVector cand = muonL + muonS;
  ptZ = cand.Pt();
  massZ = cand.M();

}

ZCandidate::~ZCandidate()
{
  ;
}

MTree::EventInfo ZCandidate::getEventInfo() const
{
  return(eventInfo);
}

MTree::Electron ZCandidate::getLeadElectron() const
{
  if (isMuon)
  {
    std::cout << "This Z candidate did not decay to electrons!" << std::endl;
  }
  return(electronL);
}

MTree::Electron ZCandidate::getSubElectron() const
{
  if (isMuon)
  {
    std::cout << "This Z candidate did not decay to electrons!" << std::endl;
  }
  return(electronS);
}

MTree::Muon ZCandidate::getLeadMuon() const
{
  if (!isMuon)
  {
    std::cout << "This Z candidate did not decay to muons!" << std::endl;
  }
  return(muonL);
}

MTree::Muon ZCandidate::getSubMuon() const
{
  if (!isMuon)
  {
    std::cout << "This Z candidate did not decay to muons!" << std::endl;
  }
  return(muonS);
}

bool ZCandidate::decayedtoMuons() const
{
  return(isMuon);
}

double ZCandidate::getZpt() const
{
  return(ptZ);
}

double ZCandidate::getZMass() const
{
  return(massZ);
}

void ZCandidate::printInfo(std::ofstream& outfile)
{
  outfile << "Z Candidate" << std::endl;
  outfile << "Run Number: " << eventInfo.RunNumber() << std::endl;
  outfile << "Event Number: " << eventInfo.EventNumber() << std::endl;
  outfile << "LumiBlock: " << eventInfo.LumiBlock() << std::endl;

  if (isMuon)
  {
    outfile << "Muon Information" << std::endl;
    outfile << "First Muon" << std::endl;
    outfile << "[E,px,py,pz] = [" << muonL.E() << ", " << muonL.Px() <<", " << muonL.Py() << "," << muonL.Pz() << "]" << std::endl;
    outfile << "[pT,eta,phi,m] = [" << muonL.Pt() << ", " << muonL.Eta() <<", " << muonL.Phi() << ", " << muonL.M() << "]" << std::endl;
    outfile << "Author:    " << muonL.Author() << std::endl;
    outfile << "Charge:    " << muonL.Charge() << std::endl;
    outfile << "Second Muon" << std::endl;
    outfile << "[E,px,py,pz] = [" << muonS.E() << ", " << muonS.Px() <<", " << muonS.Py() << "," << muonS.Pz() << "]" << std::endl;
    outfile << "[pT,eta,phi,m] = [" << muonS.Pt() << ", " << muonS.Eta() <<", " << muonS.Phi() << ", " << muonS.M() << "]" << std::endl;
    outfile << "Author:    " << muonS.Author() << std::endl;
    outfile << "Charge:    " << muonS.Charge() << std::endl;
  }
  else
  {
    outfile << "Electron Information" << std::endl;
    outfile << "First Electron" << std::endl;
    outfile << "[E,px,py,pz] = [" << electronL.E() << ", " << electronL.Px() <<", " << electronL.Py() << ", " << electronL.Pz() << "]" << std::endl;
    outfile << "[pT,eta,phi,m] = [" << electronL.Pt() << ", " << electronL.Eta() <<", " << electronL.Phi() << ", " << electronL.M() << "]" << std::endl;
    outfile << "Author:    " << electronL.Author() << std::endl;
    outfile << "Charge:    " << electronL.Charge() << std::endl;
    outfile << "Second Electron" << std::endl;
    outfile << "[E,px,py,pz] = [" << electronS.E() << ", " << electronS.Px() <<", " << electronS.Py() << ", " << electronS.Pz() << "]" << std::endl;
    outfile << "[pT,eta,phi,m] = [" << electronS.Pt() << ", " << electronS.Eta() <<", " << electronS.Phi() << ", " << electronS.M() << "]" << std::endl;
    outfile << "Author:    " << electronS.Author() << std::endl;
    outfile << "Charge:    " << electronS.Charge() << std::endl;
  }

  outfile << "PT of Z: " << ptZ << std::endl;
  outfile << "Mass of Z: " << massZ << std::endl << std::endl;
}
