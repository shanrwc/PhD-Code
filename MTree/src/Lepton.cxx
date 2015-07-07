#include "MTree/Lepton.h"

namespace MTree {

Lepton::Lepton() : TLorentzVector(0.0, 0.0, 0.0, 0.0), _Author(0), _Charge(0), _LeptonPdgID(0), _Track(0) {
}

Lepton::~Lepton() {

}

void Lepton::InitializeLepton(double Pt, double Eta, double Phi, double Mass, int Author, int Charge, 
			      int LeptonPdgID) {
  SetPtEtaPhiM(Pt, Eta, Phi, Mass);
  _Author = Author;
  _Charge = Charge;
  _LeptonPdgID = LeptonPdgID;
}


MTree::Track* Lepton::GetTrack() const {
  if (!_Track.IsValid()) return 0;
  MTree::Track* track = (MTree::Track*) _Track.GetObject();
  return track;
}

void Lepton::PrintLeptonData(std::ostream& s) const {
  s << "Lepton 4-vector [E,px,py,pz]   = [" << this->E() << ", " << this->Px() << ", " << this->Py()
    << ", " << this->Pz() << "]" << std::endl;
  s << "Lepton 4-vector [pT,eta,phi,m] = [" << this->Pt() << ", " << this->Eta() << ", " << this->Phi()
    << ", " << this->M() << "]" << std::endl;
  MTree::Track* track = GetTrack();
  if (track) {
    s << "Track four vector [E,px,py,pz]   = [" << track->E() << ", " << track->Px() << ", " << track->Py()
      << ", " << track->Pz() << "]" << std::endl;
    s << "Track four vector [pT,eta,phi,m] = [" << track->Pt() << ", " << track->Eta() << ", " << track->Phi()
      << ", " << track->M() << "]" << std::endl;    
  }
  s <<   "Author         : " << _Author 
    << "\nCharge         : " << _Charge 
    << "\nLeptonPdgID    : " << _LeptonPdgID 
    << std::endl;
}

}
 
ClassImp(MTree::Lepton)

