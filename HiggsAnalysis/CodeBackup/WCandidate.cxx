#include "WCandidate.h"

WCandidate::WCandidate(MTree::EventInfo info, MTree::Electron elec, MTree::MissingET met)
{
  eventInfo = info;
  electron = elec;
  missingEt = met;

  isMuon = false;

  double lpt = electron.Pt();
  double mpt = missingEt.Pt();
  deltaPhi = electron.DeltaPhi(missingEt);

  mT = sqrt(2*lpt*mpt*(1-cos(deltaPhi)));
}

WCandidate::WCandidate(MTree::EventInfo info, MTree::Muon mu, MTree::MissingET met)
{
  eventInfo = info;
  muon = mu;
  missingEt = met;

  isMuon = true;

  double lpt = muon.Pt();
  double mpt = missingEt.Pt();
  deltaPhi = muon.DeltaPhi(missingEt);

  mT = sqrt(2*lpt*mpt*(1-cos(deltaPhi)));
}

WCandidate::WCandidate()
{
  ;
}

WCandidate::~WCandidate()
{
  ;
}

MTree::EventInfo WCandidate::getEventInfo() const
{
  return(eventInfo);
}

MTree::Electron WCandidate::getElectron() const
{
  if (isMuon)
  {
    std::cout<<"This candidate didn't decay to an electron!"<<std::endl;
  }

  return(electron);
}

MTree::Muon WCandidate::getMuon() const
{
  if (!isMuon)
  {
    std::cout<<"This candidate didn't decay to a muon!"<<std::endl;
  }
  return(muon);
}

MTree::MissingET WCandidate::getMissingEt() const
{
  return(missingEt);
}

bool WCandidate::decayedToMuon() const
{
  return(isMuon);
}

double WCandidate::getMET()
{
  double met = missingEt.MET();
  return(met);
}

double WCandidate::getDPhi() const
{
  return(deltaPhi);
}

double WCandidate::getMT() const
{
  return(mT);
}

double WCandidate::getWPt()
{
  TLorentzVector w = electron + missingEt;
  return(w.Pt());
}

void WCandidate::printInfo(std::ofstream& outfile)
{
  outfile << "W Candidate" << std::endl;
  outfile << "Run Number: " << eventInfo.RunNumber() << std::endl;
  outfile << "Event Number: " << eventInfo.EventNumber() << std::endl;
  outfile << "Lumi Block: " << eventInfo.LumiBlock() << std::endl;

  if (isMuon)
  {
    outfile << "Muon Information" << std::endl;
    outfile << "[E,px,py,pz] = [" << muon.E() << ", " << muon.Px() <<", " << muon.Py() << "," << muon.Pz() << "]" << std::endl;
    outfile << "[pT,eta,phi,m] = [" << muon.Pt() << ", " << muon.Eta() <<", " << muon.Phi() << ", " << muon.M() << "]" << std::endl;
    outfile << "Author:    " << muon.Author() << std::endl;
    outfile << "Charge:    " << muon.Charge() << std::endl;
    outfile << "PTCone40:  " << muon.PTCone40() << std::endl;
    outfile << "PTCone40/Pt:   " << muon.PTCone40()/muon.Pt() << std::endl;
    outfile << "TrackIso40:   " <<muon.TrackIso40() <<std::endl;
    outfile << "TrackIso40/Pt:   " <<muon.TrackIso40()/muon.Pt() << std::endl;
  }
  else
  {
    outfile << "Electron Information" << std::endl;
    outfile << "[E,px,py,pz] = [" << electron.E() << ", " << electron.Px() <<", " << electron.Py() << ", " << electron.Pz() << "]" << std::endl;
    outfile << "[pT,eta,phi,m] = [" << electron.Pt() << ", " << electron.Eta() <<", " << electron.Phi() << ", " << electron.M() << "]" << std::endl;
    outfile << "Author:    " << electron.Author() << std::endl;
    outfile << "Charge:    " << electron.Charge() << std::endl;
    outfile << "ETCone40:  " << electron.ETCone40() << std::endl;
    outfile << "ETCone40/Pt:   " << electron.ETCone40()/electron.Pt() << std::endl;
    outfile << "TrackIso40:   " <<electron.TrackIso40() <<std::endl;
    outfile << "TrackIso40/Pt:   " <<electron.TrackIso40()/electron.Pt() << std::endl;
  }

  outfile << "MissingEt Information" << std::endl;
  outfile << "[Met,Ex,Ey,SumET] = [" << missingEt.MET() <<", "<<missingEt.Ex() <<", "<<missingEt.Ey()<<", " << missingEt.SumET() << "]" << std::endl;
  outfile << "Delta Phi: " << deltaPhi << std::endl;
  outfile << "Transverse Mass: " << mT << std::endl<<std::endl;
}

WCandidate& WCandidate::operator=(const WCandidate &rhs)
{
  eventInfo = rhs.eventInfo;
  electron = rhs.electron;
  missingEt = rhs.missingEt;
  muon = rhs.muon;

  isMuon = rhs.isMuon;
  deltaPhi = rhs.deltaPhi;
  mT = rhs.mT;

  return *this;
}
