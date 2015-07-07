#ifndef _MUON_STACO_SF_EPS_
#define _MUON_STACO_SF_EPS_

#include <iostream>
#include <stdio.h>
#include <math.h>

const double pi = acos(-1.);

//// Forward declaration of function
double mu_CB_staco_trigger_SF(double eta, double phi, double pt);
double mu_CB_staco_trigger_Eff_data(double eta, double phi, double pt);
double mu_CB_staco_trigger_Eff_MC(double eta, double phi, double pt);
    
double mu_CB_barrel_SF(double eta, double phi);
double mu_CB_endcap_SF(double pt);

double mu_CB_barrel_Eff_MC(double eta, double phi);
double mu_CB_endcap_Eff_MC(double pt);
double mu_CB_barrel_Eff_data(double eta, double phi);
double mu_CB_endcap_Eff_data(double pt);

double mu_Tight_staco_trigger_SF(double eta, double phi, double pt);
double mu_Tight_staco_trigger_Eff_data(double eta, double phi, double pt);
double mu_Tight_staco_trigger_Eff_MC(double eta, double phi, double pt);
    
double mu_Tight_barrel_SF(double eta, double phi);
double mu_Tight_endcap_SF(double pt);

double mu_Tight_barrel_Eff_MC(double eta, double phi);
double mu_Tight_endcap_Eff_MC(double pt);
double mu_Tight_barrel_Eff_data(double eta, double phi);
double mu_Tight_endcap_Eff_data(double pt);

double check_Phi_range(double phi);


/////////////// COMBINED /////////////// 

double mu_CB_staco_trigger_SF(double eta, double phi, double pt){
  const double eta_EC = 1.05;
  double mu_CB_SF = 0.;  
  if(fabs(eta) < eta_EC ) mu_CB_SF = mu_CB_barrel_SF(eta,phi);
  else mu_CB_SF = mu_CB_endcap_SF(pt);
  return mu_CB_SF; 
}
 
double mu_CB_staco_trigger_Eff_data(double eta, double phi, double pt){
  const double eta_EC = 1.05;
  double mu_CB_Eff = 0.;
  if(fabs(eta) < eta_EC ) mu_CB_Eff = mu_CB_barrel_Eff_data(eta,phi);
  else mu_CB_Eff = mu_CB_endcap_Eff_data(pt);
  return mu_CB_Eff;
}

double mu_CB_staco_trigger_Eff_MC(double eta, double phi, double pt){
  const double eta_EC = 1.05;
  double mu_CB_Eff = 0.;
  if(fabs(eta) < eta_EC ) mu_CB_Eff = mu_CB_barrel_Eff_MC(eta,phi);
  else mu_CB_Eff = mu_CB_endcap_Eff_MC(pt);  
  return mu_CB_Eff;
}

double mu_CB_barrel_Eff_MC(double eta, double phi){
    
  double mu_CB_eta=eta;
  double mu_CB_phi=check_Phi_range(phi);  
  int etaI=-1, phiI=-1;      
  if(mu_CB_phi < -2.945) mu_CB_phi+=2*pi; // fix phi range for unusual binning

  const double etabins[6] = {-1.05,-0.908,-0.476,0.,0.476,0.908}; // lower edges of eta bins
  const double phibins[8] = {-2.945,-2.160,-1.374,-0.589,0.196,0.982,1.767,2.553};// lower edges of phi bins
  const double trig_CB_Eff_barrel_matrix[6][8] = {{0.641,0.548,0.422,0.623,0.628,0.625,0.624,0.603},
					       {0.852,0.463,0.597,0.850,0.853,0.811,0.867,0.820},
					       {0.824,0.732,0.683,0.770,0.803,0.794,0.801,0.794},
					       {0.864,0.499,0.632,0.875,0.808,0.859,0.825,0.818},
					       {0.878,0.485,0.708,0.856,0.870,0.838,0.883,0.869},
					       {0.679,0.578,0.557,0.646,0.660,0.626,0.691,0.627}};
  
  for (int i=7; i>=0; i--){    // find phi index
    if ( mu_CB_phi > phibins[i] ) {
      phiI = i;
      break;
    }
  }
  for (int i=5; i>=0; i--){    // find eta index
    if ( mu_CB_eta > etabins[i] ) {
      etaI = i;
      break;
    }
  }
  
  return  trig_CB_Eff_barrel_matrix[etaI][phiI];  
}

double mu_CB_barrel_Eff_data(double eta, double phi){
  
  double mu_CB_eta=eta;
  double mu_CB_phi=check_Phi_range(phi);  
  int etaI=-1, phiI=-1;    
  if(mu_CB_phi < -2.945) mu_CB_phi+=2*pi;  

  const double etabins[6] = {-1.05,-0.908,-0.476,0.,0.476,0.908}; // lower edges of eta bins
  const double phibins[8] = {-2.945,-2.160,-1.374,-0.589,0.196,0.982,1.767,2.553};// lower edges of phi bins
  const double trig_CB_Eff_barrel_matrix[6][8] = {{0.737,0.584,0.611,0.754,0.762,0.789,0.790,0.750},
					       {0.856,0.467,0.710,0.885,0.902,0.868,0.902,0.843},
					       {0.756,0.756,0.699,0.801,0.868,0.806,0.825,0.779},
					       {0.854,0.568,0.674,0.890,0.865,0.834,0.828,0.805},
					       {0.863,0.485,0.674,0.819,0.847,0.876,0.886,0.861},
					       {0.675,0.380,0.563,0.503,0.545,0.682,0.715,0.540}};  
  
  for (int i=7; i>=0; i--){    // find phi index
    if ( mu_CB_phi > phibins[i] ) {
      phiI = i;
      break;
    }
  }
  for (int i=5; i>=0; i--){    // find eta index
    if ( mu_CB_eta > etabins[i] ) {
      etaI = i;
      break;
    }
  }
  
  return  trig_CB_Eff_barrel_matrix[etaI][phiI];  
}


double mu_CB_endcap_Eff_MC(double pt){
  
  double mu_CB_pt=pt;
  int mu_CB_pt_index=-1;
  const double ptbins[8] = {15.,18.,20.,30.,40.,50.,70.,100.};   // lower edges of pT bins
  const double pt_Eff_MC_array[8] = {0.229,0.851,0.869,0.863,0.855,0.851,0.855,0.867};
  
  for (int i=7; i>=0; i--){    // find pt index
    if ( mu_CB_pt > ptbins[i] ) {
      mu_CB_pt_index = i;
      break;
    }
  }
   
  return  pt_Eff_MC_array[mu_CB_pt_index];
}

double mu_CB_endcap_Eff_data(double pt){
  
  double mu_CB_pt=pt;
  int mu_CB_pt_index=-1;
  const double ptbins[8] = {15.,18.,20.,30.,40.,50.,70.,100.};   // lower edges of pT bins
  const double pt_Eff_data_array[8] = {0.230,0.875,0.887,0.888,0.887,0.886,0.872,0.911};
  
  for (int i=7; i>=0; i--){    // find pt index
    if ( mu_CB_pt > ptbins[i] ) {
      mu_CB_pt_index = i;
      break;
    }
  }
  
  return  pt_Eff_data_array[mu_CB_pt_index];
}


// For trigger scale factors
double mu_CB_endcap_SF(double pt){

  double mu_CB_pt=pt;
  int mu_CB_pt_index=-1;
  const double ptbins[8] = {15.,18.,20.,30.,40.,50.,70.,100.};   // lower edges of pT bins
  const double pt_SFarray[8] = {1.0058817,1.0276103,1.0207309,1.0291394,1.0371242,1.0411885,1.0198005,1.0501196};
  
  for (int i=7; i>=0; i--){    // find pt index
    if ( mu_CB_pt > ptbins[i] ) {
      mu_CB_pt_index = i;
      break;
    }
  }
  
  return  pt_SFarray[mu_CB_pt_index];
  
}

double mu_CB_barrel_SF(double eta, double phi){
  
  double mu_CB_eta=eta;
  double mu_CB_phi=check_Phi_range(phi);
  int etaI=-1, phiI=-1;  
  if(mu_CB_phi < -2.945) mu_CB_phi+=2*pi;  

  const double etabins[6] = {-1.05,-0.908,-0.476,0.,0.476,0.908}; // lower edges of eta bins
  const double phibins[8] = {-2.945,-2.160,-1.374,-0.589,0.196,0.982,1.767,2.553};// lower edges of phi bins
  const double trig_CB_SFmatrix[6][8] = {{1.1502,1.0660,1.4482,1.2095,1.2142,1.2623,1.2654,1.2445},  // rows are eta, columns are phi
				      {1.0048,1.0102,1.1890,1.0415,1.0575,1.0710,1.0401,1.0279},
				      {0.9173,1.0329,1.0243,1.0411,1.0814,1.0160,1.0291,0.9801},
				      {0.9886,1.1381,1.0670,1.0174,1.0696,0.9710,1.0045,0.9845},
				      {0.9829,0.9998,0.9519,0.9569,0.9735,1.0454,1.0030,0.9903},
				      {0.9944,0.6583,1.0112,0.7800,0.8263,1.0898,1.0356,0.8622}};
  
  for (int i=7; i>=0; i--){    // find phi index
    if ( mu_CB_phi > phibins[i] ) {
      phiI = i;
      break;
    }
  }
  for (int i=5; i>=0; i--){    // find eta index
    if ( mu_CB_eta > etabins[i] ) {
      etaI = i;
      break;
    }
  }

  return trig_CB_SFmatrix[etaI][phiI]; 
}

/////////////// TIGHT /////////////// 

double mu_Tight_staco_trigger_SF(double eta, double phi, double pt){
  const double eta_EC = 1.05;
  double mu_Tight_SF = 0.;  
  if(fabs(eta) < eta_EC ) mu_Tight_SF = mu_Tight_barrel_SF(eta,phi);
  else mu_Tight_SF = mu_Tight_endcap_SF(pt);
  return mu_Tight_SF; 
}
 
double mu_Tight_staco_trigger_Eff_data(double eta, double phi, double pt){
  const double eta_EC = 1.05;
  double mu_Tight_Eff = 0.;
  if(fabs(eta) < eta_EC ) mu_Tight_Eff = mu_Tight_barrel_Eff_data(eta,phi);
  else mu_Tight_Eff = mu_Tight_endcap_Eff_data(pt);
  return mu_Tight_Eff;
}

double mu_Tight_staco_trigger_Eff_MC(double eta, double phi, double pt){
  const double eta_EC = 1.05;
  double mu_Tight_Eff = 0.;
  if(fabs(eta) < eta_EC ) mu_Tight_Eff = mu_Tight_barrel_Eff_MC(eta,phi);
  else mu_Tight_Eff = mu_Tight_endcap_Eff_MC(pt);  
  return mu_Tight_Eff;
}

double mu_Tight_barrel_Eff_MC(double eta, double phi){
    
  double mu_Tight_eta=eta;
  double mu_Tight_phi=check_Phi_range(phi);  
  int etaI=-1, phiI=-1;      
  if(mu_Tight_phi < -2.945) mu_Tight_phi+=2*pi; // fix phi range for unusual binning

  const double etabins[6] = {-1.05,-0.908,-0.476,0.,0.476,0.908}; // lower edges of eta bins
  const double phibins[8] = {-2.945,-2.160,-1.374,-0.589,0.196,0.982,1.767,2.553};// lower edges of phi bins
  const double trig_Tight_Eff_barrel_matrix[6][8] = {{0.631,0.535,0.410,0.616,0.623,0.611,0.616,0.596},
					       {0.851,0.462,0.596,0.848,0.851,0.808,0.865,0.818},
					       {0.821,0.724,0.678,0.766,0.800,0.791,0.799,0.789},
					       {0.861,0.495,0.630,0.873,0.806,0.856,0.822,0.813},
					       {0.876,0.484,0.706,0.855,0.867,0.836,0.882,0.868},
					       {0.669,0.564,0.543,0.639,0.652,0.612,0.683,0.620}};
  
  for (int i=7; i>=0; i--){    // find phi index
    if ( mu_Tight_phi > phibins[i] ) {
      phiI = i;
      break;
    }
  }
  for (int i=5; i>=0; i--){    // find eta index
    if ( mu_Tight_eta > etabins[i] ) {
      etaI = i;
      break;
    }
  }
  
  return  trig_Tight_Eff_barrel_matrix[etaI][phiI];  
}

double mu_Tight_barrel_Eff_data(double eta, double phi){
  
  double mu_Tight_eta=eta;
  double mu_Tight_phi=check_Phi_range(phi);  
  int etaI=-1, phiI=-1;   
  if(mu_Tight_phi < -2.945) mu_Tight_phi+=2*pi;   

  const double etabins[6] = {-1.05,-0.908,-0.476,0.,0.476,0.908}; // lower edges of eta bins
  const double phibins[8] = {-2.945,-2.160,-1.374,-0.589,0.196,0.982,1.767,2.553};// lower edges of phi bins
  const double trig_Tight_Eff_barrel_matrix[6][8] = {{0.714,0.565,0.567,0.734,0.738,0.733,0.720,0.730},
					       {0.853,0.468,0.707,0.881,0.901,0.854,0.888,0.841},
					       {0.758,0.750,0.694,0.794,0.863,0.794,0.809,0.773},
					       {0.848,0.566,0.666,0.883,0.857,0.820,0.806,0.799},
					       {0.858,0.485,0.671,0.798,0.842,0.865,0.864,0.859},
					       {0.658,0.366,0.519,0.484,0.524,0.637,0.678,0.530}};  
  
  for (int i=7; i>=0; i--){    // find phi index
    if ( mu_Tight_phi > phibins[i] ) {
      phiI = i;
      break;
    }
  }
  for (int i=5; i>=0; i--){    // find eta index
    if ( mu_Tight_eta > etabins[i] ) {
      etaI = i;
      break;
    }
  }
  
  return  trig_Tight_Eff_barrel_matrix[etaI][phiI];  
}


double mu_Tight_endcap_Eff_MC(double pt){
  
  double mu_Tight_pt=pt;
  int mu_Tight_pt_index=-1;
  const double ptbins[8] = {15.,18.,20.,30.,40.,50.,70.,100.};   // lower edges of pT bins
  const double pt_Eff_MC_array[8] = {0.228,0.850,0.867,0.861,0.853,0.849,0.854,0.862};
  
  for (int i=7; i>=0; i--){    // find pt index
    if ( mu_Tight_pt > ptbins[i] ) {
      mu_Tight_pt_index = i;
      break;
    }
  }
   
  return  pt_Eff_MC_array[mu_Tight_pt_index];
}

double mu_Tight_endcap_Eff_data(double pt){
  
  double mu_Tight_pt=pt;
  int mu_Tight_pt_index=-1;
  const double ptbins[8] = {15.,18.,20.,30.,40.,50.,70.,100.};   // lower edges of pT bins
  const double pt_Eff_data_array[8] = {0.229,0.874,0.884,0.883,0.883,0.883,0.870,0.908};
  
  for (int i=7; i>=0; i--){    // find pt index
    if ( mu_Tight_pt > ptbins[i] ) {
      mu_Tight_pt_index = i;
      break;
    }
  }
  
  return  pt_Eff_data_array[mu_Tight_pt_index];
}


// For trigger scale factors
double mu_Tight_endcap_SF(double pt){

  double mu_Tight_pt=pt;
  int mu_Tight_pt_index=-1;
  const double ptbins[8] = {15.,18.,20.,30.,40.,50.,70.,100.};   // lower edges of pT bins
  const double pt_SFarray[8] = {1.0058817,1.0276103,1.0207309,1.0291394,1.0371242,1.0411885,1.0198005,1.0501196};
  
  for (int i=7; i>=0; i--){    // find pt index
    if ( mu_Tight_pt > ptbins[i] ) {
      mu_Tight_pt_index = i;
      break;
    }
  }
  
  return  pt_SFarray[mu_Tight_pt_index];
  
}

double mu_Tight_barrel_SF(double eta, double phi){
  
  double mu_Tight_eta=eta;
  double mu_Tight_phi=check_Phi_range(phi);
  int etaI=-1, phiI=-1;  
  if(mu_Tight_phi < -2.945) mu_Tight_phi+=2*pi;   

  const double etabins[6] = {-1.05,-0.908,-0.476,0.,0.476,0.908}; // lower edges of eta bins
  const double phibins[8] = {-2.945,-2.160,-1.374,-0.589,0.196,0.982,1.767,2.553};// lower edges of phi bins
  const double trig_Tight_SFmatrix[6][8] = {{1.1502,1.0660,1.4482,1.2095,1.2142,1.2623,1.2654,1.2445},  // rows are eta, columns are phi
				      {1.0048,1.0102,1.1890,1.0415,1.0575,1.0710,1.0401,1.0279},
				      {0.9173,1.0329,1.0243,1.0411,1.0814,1.0160,1.0291,0.9801},
				      {0.9886,1.1381,1.0670,1.0174,1.0696,0.9710,1.0045,0.9845},
				      {0.9829,0.9998,0.9519,0.9569,0.9735,1.0454,1.0030,0.9903},
				      {0.9944,0.6583,1.0112,0.7800,0.8263,1.0898,1.0356,0.8622}};
  
  for (int i=7; i>=0; i--){    // find phi index
    if ( mu_Tight_phi > phibins[i] ) {
      phiI = i;
      break;
    }
  }
  for (int i=5; i>=0; i--){    // find eta index
    if ( mu_Tight_eta > etabins[i] ) {
      etaI = i;
      break;
    }
  }

  return trig_Tight_SFmatrix[etaI][phiI]; 
}


// Helper function to deal with possible phi ambiguity
double check_Phi_range(double phi)
{

  double newphi = phi;
  const double pi = acos(-1.);
  
  if (newphi > pi) {
    printf("<muon_SF>: WARNING: Muon phi %4.2f > pi! Using (phi-2*pi) \n", phi);
    newphi -= 2*pi;
  }
  if (newphi < -pi) {
    printf("<muon_SF>: WARNING: Muon phi %4.2f < -pi! Using (phi+2*pi) \n", phi);
    newphi += 2*pi;
  }
  
  return newphi;
}

#endif
