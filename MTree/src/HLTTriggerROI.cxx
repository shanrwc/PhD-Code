#include "MTree/HLTTriggerROI.h"

namespace MTree {

HLTTriggerROI::HLTTriggerROI() : TLorentzVector(0.0, 0.0, 0.0, 0.0), _EtaWidth(0.0), _PhiWidth(0.0),
			       _ROIWord(0) {
  _HLTNames = new TClonesArray("TObjString", 5);
}

HLTTriggerROI::~HLTTriggerROI() {
  delete _HLTNames;
}

void HLTTriggerROI::Initialize(double Pt, double Eta, double Phi, double EtaWidth, double PhiWidth, int ROIWord) 
{
  SetPtEtaPhiM(Pt, Eta, Phi, 0.0);
  _EtaWidth = EtaWidth;
  _PhiWidth = PhiWidth;
  _ROIWord = ROIWord;
}

void HLTTriggerROI::SetROINames(const std::vector<std::string>& HLTNames) {
  int nameCounter = 0;
  std::vector<std::string>::const_iterator i = HLTNames.begin(), e = HLTNames.end();
  for (; i != e; ++i) {
    new((*_HLTNames)[nameCounter++]) TObjString((*i).c_str());
  }  
}

void HLTTriggerROI::AddROIName(const std::string& HLTName) {
  int place = _HLTNames->GetEntriesFast();
  new((*_HLTNames)[place]) TObjString(HLTName.c_str());
}

std::vector<std::string> HLTTriggerROI::GetHLTNames() const {
  std::vector<std::string> HLTNames;
  int NrNames = _HLTNames->GetEntriesFast();
  for (int i = 0; i != NrNames; ++i) 
    HLTNames.push_back(std::string(((TObjString*) _HLTNames->At(i))->GetString().Data())); 
  return HLTNames;
}

//should eventually change implementation to convert std::string to TObjString and do comparison
bool HLTTriggerROI::PassedTrigger(std::string HLTName) const {
  std::vector<std::string> HLTNames = GetHLTNames();
  int NrHLTNames = HLTNames.size();
  for (int i = 0; i != NrHLTNames; ++i) {
    if (HLTName == HLTNames[i]) return true;
  }
  return false;
}

void HLTTriggerROI::PrintData(std::ostream& s) const {
  s << "*****\n* HLTTriggerROI\n*****" << std::endl;
  s << "HLTTriggerROI 4-vector [E,px,py,pz]   = [" << this->E() << ", " << this->Px() << ", " 
    << this->Py() << ", " << this->Pz() << "]" << std::endl;
  s << "HLTTriggerROI 4-vector [pT,eta,phi,m] = [" << this->Pt() << ", " << this->Eta() << ", " 
    << this->Phi() << ", " << this->M() << "]" << std::endl;
  s <<   "EtaWidth: " << _EtaWidth
    << "\nPhiWidth: " << _PhiWidth
    << "\nROIWord : " << _ROIWord << std::endl;
  s << "HLT Trigger Names with this ROI:" << std::endl;
  int NrNames = _HLTNames->GetEntriesFast();
  for (int i = 0; i != NrNames; ++i) s << "  " 
				       << ((TObjString*) _HLTNames->At(i))->GetString().Data() 
				       << std::endl;
  s << "*****\n" << std::endl;
}

}
 
ClassImp(MTree::HLTTriggerROI)
