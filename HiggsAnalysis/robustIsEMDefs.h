#ifndef ROBUSTISEMDEFS_C
#define ROBUSTISEMDEFS_C

#include "egammaPIDdefs.h"
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

/**
   Author John Alison <johnda@sas.upenn.edu>
   
   Redefine the isEM definitions such that the Reta and w2 cuts are loosened at high eta.
   
   Note: 
   Currently the MC mismodels the Reta and w2 distributions at high large eta (|eta| > 1.8).
   In addition the isEM cuts are relatively aggressive in this region, leading to data/MC 
   disagrement in electron efficiency.
   


*/


bool isRobustLoose(long isEM, float eta, float eT, float Reta, float w2 );
bool isRobustMedium(long isEM, float eta, float eT, float Reta, float w2 );
bool isRobusterTight(long isEM, bool expectBLayer, float eta, float eT, float Reta, float w2 );


#endif
