#include "MTree/L1TriggerROI.h"

namespace MTree {

L1TriggerROI::L1TriggerROI() : TLorentzVector(0.0, 0.0, 0.0, 0.0), _EtaWidth(0.0), _PhiWidth(0.0),
			       _ROIWord(0) {
}

L1TriggerROI::~L1TriggerROI() {

}

  void L1TriggerROI::Initialize(double Pt, double Eta, double Phi, double EtaWidth, double PhiWidth, int ROIWord) 
{
  SetPtEtaPhiM(Pt, Eta, Phi, 0.0);
  _EtaWidth = EtaWidth;
  _PhiWidth = PhiWidth;
  _ROIWord = ROIWord;
}

void L1TriggerROI::PrintData(std::ostream& s) const {
  s << "*****\n* L1TriggerROI\n*****" << std::endl;
  s << "L1TriggerROI 4-vector [E,px,py,pz]   = [" << this->E() << ", " << this->Px() << ", " << this->Py()
    << ", " << this->Pz() << "]" << std::endl;
  s << "L1TriggerROI 4-vector [pT,eta,phi,m] = [" << this->Pt() << ", " << this->Eta() << ", " 
    << this->Phi() << ", " << this->M() << "]" << std::endl;
  s <<   "EtaWidth: " << _EtaWidth
    << "\nPhiWidth: " << _PhiWidth
    << "\nROIWord : " << _ROIWord
    << std::endl;
  s << "*****\n" << std::endl;
}

}
 
ClassImp(MTree::L1TriggerROI)
