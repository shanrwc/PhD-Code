#include "MTree/TruthParticle.h"
#include <string>
#include <map>

namespace MTree {

TruthParticle::TruthParticle() : TLorentzVector(0.0, 0.0, 0.0, 0.0), _PdgID(0), _ParentPdgID(0), 
				 _GrandParentPdgID(0), _Status(0) {
}

TruthParticle::~TruthParticle() {

}

void TruthParticle::Initialize(double Pt, double Eta, double Phi, double M, int PdgID,
			       int Status, int ParentPdgID, int GrandParentPdgID) {
  SetPtEtaPhiM(Pt, Eta, Phi, M);
  _PdgID = PdgID;
  _ParentPdgID = ParentPdgID;
  _GrandParentPdgID = GrandParentPdgID;
  _Status = Status;
}

MTree::TruthParticle* TruthParticle::GetParent() const {
  if (!_Parent.IsValid()) return 0;
  MTree::TruthParticle* parent = (MTree::TruthParticle*) _Parent.GetObject();
  return parent;
}

MTree::TruthParticle* TruthParticle::GetDaughter(int index) const {
  MTree::TruthParticle* daughter = (MTree::TruthParticle*) _Daughters.At(index);
  return daughter;
}

void TruthParticle::PrintData(std::ostream& s) const {
  std::map<int, std::string> particleNames;
  particleNames[5] = "b quark";
  particleNames[6] = "top quark";
  particleNames[11] = "electron";
  particleNames[12] = "electron neutrino";
  particleNames[13] = "muon";
  particleNames[14] = "muon neutrino";
  particleNames[15] = "tau";
  particleNames[16] = "tau neutrino";
  particleNames[22] = "photon";
  particleNames[23] = "Z boson";
  particleNames[24] = "W boson";
  particleNames[25] = "Higgs boson";
  particleNames[32] = "Z' boson";
  particleNames[33] = "W' boson";
  particleNames[1000022] = "Neutralino_1";
  particleNames[1000039] = "Gravitino";
  s << "*****\n* Truth Particle\n*****" << std::endl;
  s << "Pdg ID             : " << _PdgID << ", which is a " << particleNames[TMath::Abs(_PdgID)] 
    << "." << std::endl;
  s << "Adress             : " << this << std::endl;
  s << "Parent Pdg ID      : " << _ParentPdgID;
  if (_ParentPdgID != 0) {
    s << ", which is a " << particleNames[TMath::Abs(_ParentPdgID)] << "." << std::endl;
    MTree::TruthParticle* parent = GetParent();
    if (parent)
      s << "Parent Address     : " << parent << std::endl;
    else
      s << "ERROR! Should have parent in tree." << std::endl;
  }
  else s << std::endl;
  s << "Grand Parent Pdg ID: " << _GrandParentPdgID;
  if (_GrandParentPdgID != 0) s << ", which is a " << particleNames[TMath::Abs(_GrandParentPdgID)] 
				<< "." << std::endl;
  else s << std::endl;
  s << "Status             : " << _Status << std::endl;

  s << "Truth Particle 4-vector [E,px,py,pz]   = [" << this->E() << ", " << this->Px() << ", " 
    << this->Py() << ", " << this->Pz() << "]" << std::endl;
  s << "Truth Particle 4-vector [pT,eta,phi,m] = [" << this->Pt() << ", " << this->Eta() << ", " 
    << this->Phi() << ", " << this->M() << "]" << std::endl;

  int NrOfDaughters = GetNrOfDaughters();
  for (int i = 0; i != NrOfDaughters; ++i) {
    MTree::TruthParticle* daughter = GetDaughter(i);
    s << "Daughter " << i << " address: " << daughter << std::endl;
  }
  s << "*****\n" << std::endl;  
}
 
}

ClassImp(MTree::TruthParticle)
