#include "MTree/TriggerObject.h"

namespace MTree {

  TriggerObject::TriggerObject() : _Pt(0.0), _Eta(0.0), _Phi(0.0) {

  }

  TriggerObject::~TriggerObject() {

  }

  void TriggerObject::Initialize(double pt, double eta, double phi)
  {
    _Pt = pt;
    _Eta = eta;
    _Phi = phi;
  }

  void TriggerObject::PrintData(std::ostream& s) const {
    s << "*****\n* TriggerObject\n*****" << std::endl;

    s << "[pT,eta,phi]   = [" << _Pt << ", " << _Eta << ", " << _Phi << "]" << std::endl;
    std::set<int>::iterator iter, end = _Authors.end();
    s << "Authors: ";
    for (iter = _Authors.begin(); iter != end; ++iter)
    {
      s << (*iter) << " ";
    }
    s << std::endl;

    s << "Associated trigger strings:" << std::endl;  
    int nrStrings = _triggerNames.size();
    if (!nrStrings)
    {
      s << "---> None" << std::endl;
    }
    std::set<std::string>::iterator itert = _triggerNames.begin();
    std::set<std::string>::iterator itrEnd = _triggerNames.end();
    for (; itert != itrEnd; ++itert)
    {
      s << "---> " << (*itert) << std::endl;
    }

    s << "*****\n" << std::endl;
  }

  bool TriggerObject::PassedTrigger(std::string trigName)
  {
    if (_triggerNames.find(trigName) != _triggerNames.end())
    {
      return(true);
    }
    else
    {
      return(false);
    }
  }

  bool TriggerObject::IsSame(double pt, double eta, double phi)
  {
    double epsilon = 0.001;
    if (pt > 100000) pt = 0.01;
    if ((fabs(_Pt - pt) < epsilon) && (fabs(_Eta - eta) < epsilon) && (fabs(_Phi - phi) < epsilon))
    {
      return true;
    }
    else
    {
      return false;
    }
  }

}
 
ClassImp(MTree::TriggerObject)
