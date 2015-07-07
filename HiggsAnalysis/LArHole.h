#ifndef __LARHOLE__
#define __LARHOLE__

/**
   
Purpose: LAr hole treatment

- Version 1.0 (23 June 2011):  based on SUSYTools-00-00-29 implementation

Usage: 
double threshold=40000;   //(40GeV threshold by default)
bool flag = LArHole::IsLArHole(pt,eta,phi,BCH_CORR_JET, BCH_CORR_CELL, isData, threshold);
//Remove event is flag = true 

*/


namespace LArHole
{
  
  
  bool IsLArHole(float eta, float phi,
		 float etamin = -0.1, float etamax = 1.5,
		 float phimin = -0.9, float phimax = -0.5)
  {
    if (eta < etamin || eta > etamax) return false;
    if (phi < phimin || phi > phimax) return false;
    return true;     
  }
  
  //If the function below returns true, event needs to be vetoed.
  bool IsLArHoleVeto(float jet_pt, 
		     float jet_eta,                
		     float jet_phi,                
		     float jet_BCH_CORR_JET, 
		     float jet_BCH_CORR_CELL,
		     bool kIsData,                  //data or mc flag
		     float threshold=25000.)
  {
    if (!IsLArHole(jet_eta,jet_phi)) return false;
    float pt = threshold;
    // Correct threshold cut by missing energy in LAr hole in data.
    // BCH_CORR_CELL is necessary to take into account dead tile module near LAr hole.
    if (kIsData) pt = pt*(1. - jet_BCH_CORR_JET)/(1. - jet_BCH_CORR_CELL);
    if (jet_pt <= pt) return false;
    return true;
  }

}



#endif
