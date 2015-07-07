#include "MTree/MissingET.h"

namespace MTree {

  MissingET::MissingET() : TLorentzVector(0.0, 0.0, 0.0, 0.0), _SumET(0.0), _METTerm("none"), 
			   _ExCentralRegion(0.0), _ExEndcapRegion(0.0), _ExForwardRegion(0.0), 
			   _EyCentralRegion(0.0), _EyEndcapRegion(0.0), _EyForwardRegion(0.0), 
			   _SumETCentralRegion(0.0), _SumETEndcapRegion(0.0), _SumETForwardRegion(0.0) {
}

MissingET::~MissingET() {

}

void MissingET::Initialize(double met, double metx, double mety, double SumET, TObjString term) {
  SetPxPyPzE(metx, mety, 0.0, met);
  _SumET = SumET;
  _METTerm = term;
}
 
void MissingET::InitializeRegions(float ExCentralRegion, float ExEndcapRegion, float ExForwardRegion,
				  float EyCentralRegion, float EyEndcapRegion, float EyForwardRegion,
				  float SumETCentralRegion, float SumETEndcapRegion, float SumETForwardRegion) {
  _ExCentralRegion = ExCentralRegion;
  _ExEndcapRegion = ExEndcapRegion;
  _ExForwardRegion = ExForwardRegion;
  _EyCentralRegion = EyCentralRegion;
  _EyEndcapRegion = EyEndcapRegion;
  _EyForwardRegion = EyForwardRegion;
  _SumETCentralRegion = SumETCentralRegion;
  _SumETEndcapRegion = SumETEndcapRegion;
  _SumETForwardRegion = SumETForwardRegion;
}

void MissingET::ClearVariables() {
  SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
  _SumET = 0.0;
  _METTerm = "none";
  _ExCentralRegion = _ExEndcapRegion = _ExForwardRegion = 0.0;
  _EyCentralRegion = _EyEndcapRegion = _EyForwardRegion = 0.0;
  _SumETCentralRegion = _SumETEndcapRegion = _SumETForwardRegion = 0.0;
}

  void MissingET::PrintData(std::ostream& s) const {
    s << "*****\n* Missing ET\n*****" << std::endl;
    s << "Missing ET 4-vector [E,px,py,pz]   = [" << this->E() << ", " << this->Px() << ", " << this->Py()
      << ", " << this->Pz() << "]" << std::endl;
    s << "Missing ET 4-vector [pT,eta,phi,m] = [" << this->Pt() << ", " << this->Eta() << ", " << this->Phi()
      << ", " << this->M() << "]" << std::endl;
    s <<   "Missing ET Term   : " << _METTerm.GetString().Data() 
      << "\nSum ET            : " << _SumET 
      << "\nExCentralRegion   : " << _ExCentralRegion 
      << "\nExEndcapRegion    : " << _ExEndcapRegion 
      << "\nExForwardRegion   : " << _ExForwardRegion 
      << "\nEyCentralRegion   : " << _EyCentralRegion 
      << "\nEyEndcapRegion    : " << _EyEndcapRegion 
      << "\nEyForwardRegion   : " << _EyForwardRegion 
      << "\nSumETCentralRegion: " << _SumETCentralRegion 
      << "\nSumETEndcapRegion : " << _SumETEndcapRegion 
      << "\nSumETForwardRegion: " << _SumETForwardRegion 
      << std::endl;
    s << "*****\n" << std::endl;  
  }


std::string MissingET::GetTerm() const
{
  return std::string(_METTerm.GetString().Data());
}

}
ClassImp(MTree::MissingET)
