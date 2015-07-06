#include "TMath.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

double eff_pos_err(double m, double n) {
  return ( 
	  ( n*(1+2*m) + sqrt(n*n*(1+2*m)*(1+2*m) - 4*n*(1+n)*m*m) ) / 
	  ( 2*n*(1+n) ) 
	  ) 
    - m/n;
}
double eff_neg_err(double m, double n) {
  return m/n - ( 
		( n*(1+2*m) - sqrt(n*n*(1+2*m)*(1+2*m) - 4*n*(1+n)*m*m) ) / 
		( 2*n*(1+n) ) 
		);
}

double poisson_up(double y) {
  double y1 = y + 1;
  double d = 1.0 - 1.0/(9.0*y1) + 1.0/(3*TMath::Sqrt(y1));
  return y1*d*d*d - y;
}

double poisson_down(double y) {
  if (y == 0.0) return 0.0;
  double d = 1.0 - 1.0/(9.0*y) - 1.0/(3.0*TMath::Sqrt(y));
  return y - y*d*d*d;
}
  
double ave_sig_eff(int Nee, int Nem, int Nmm, double effE, double effM)
{
  double answer = (Nee*effE*effE + Nem*effE*effM + Nmm*effM*effM)/(Nee + Nem + Nmm);
  return(answer);
} 

double ave_back_eff(double Nee, double Nem, double Nme, double Nmm, 
		    double effESig, double effMSig, double effEBack, double effMBack)
{
  double numerator = (Nee*effESig*effEBack + Nem*effESig*effMBack + Nme*effEBack*effMSig + Nmm*effMSig*effMBack);
double denominator = (Nee + Nem + Nme + Nmm);
  return(numerator/denominator);
}

double ave_sig_eff_err(double Nee, double errNee, double Nem, double errNem, 
		       double Nmm, double errNmm,
		       double effE, double effEerr, double effM, double effMerr)
{
  double esig = ave_sig_eff(Nee, Nem, Nmm, effE, effM);

  double denom = 1/(Nee*effE*effE + Nem*effE*effM + Nmm*effM*effM);
  double num1 = (Nee*effE*effE)*(errNee/Nee + 2*effEerr/effE);
  double num2 = (Nem*effE*effM)*(errNem/Nem + effEerr/effE + effMerr/effM);
  double num3 = (Nmm*effM*effM)*(errNmm/Nmm + 2*effMerr/effM);

  double term2 = (errNee + errNem + errNmm)/(Nee + Nem + Nmm);

  double error = esig*(denom*(num1 + num2 + num3) - term2);
  return(error);
}

double ave_back_eff_err(double Nee, double errNee, double Nem, double errNem, 
			double Nme, double errNme, double Nmm, double errNmm,
			double effESig, double effESigerr, double effEBack, double effEBackerr,
			double effMSig, double effMSigerr, double effMBack, double effMBackerr)
{
  double eback = ave_back_eff(Nee, Nem, Nme, Nmm, effESig, effMSig, effEBack, effMBack);

  double denom = 1/(Nee*effESig*effEBack + Nem*effESig*effMBack + Nme*effMSig*effEBack + Nmm*effMSig*effMBack);
  double num1 = (Nee*effESig*effEBack)*(errNee/Nee + effESigerr/effESig + effEBackerr/effEBack);
  double num2 = (Nem*effESig*effMBack)*(errNem/Nem + effESigerr/effESig + effMBackerr/effMBack);
  double num3 = (Nme*effMSig*effEBack)*(errNme/Nme + effMSigerr/effMSig + effEBackerr/effEBack);
  double num4 = (Nmm*effMSig*effMBack)*(errNmm/Nmm + effMSigerr/effMSig + effMBackerr/effMBack);

  double term2 = (errNee + errNem + errNme + errNmm)/(Nee + Nem + Nme + Nmm);

  double error = eback*(denom*(num1 + num2 + num3 + num4) - term2);
  return(error);
}



void errorMM(bool applyScale = false)
{
  //Set all the necessary constants
//   double NeeL = 6;
//   double NeeT = 0;
//   double NemL = 1;
//   double NemT = 1;
//   double NmeL = 2;
//   double NmeT = 1;
//   double NmmL = 3;
//   double NmmT = 3;

  double NeeL = 77;
  double NeeT = 24;
  double NemL = 53;
  double NemT = 17;
  double NmeL = 36;
  double NmeT = 15;
  double NmmL = 122;
  double NmmT = 86;

  //just 'cause I'm lazy . . .
  double NL = NeeL + NemL + NmeL + NmmL;
  double NT = NeeT + NemT + NmeT + NmmT;

  double sigEL = 222;
  double sigET = 151;
  double backEL = 29140;
  double backET = 1835;

  double sigML = 358;
  double sigMT = 334;
  double backML = 11754;
  double backMT = 825;

  //Calculate efficiencies and their errors
  double effESig = sigET/sigEL;
  double effESigerrP = eff_pos_err(sigET, sigEL);
  double effESigerrN = eff_neg_err(sigET, sigEL);

  double effEBack = backET/backEL;
  double effEBackerrP = eff_pos_err(backET, backEL);
  double effEBackerrN = eff_neg_err(backET, backEL);

  double effMSig = sigMT/sigML;
  double effMSigerrP = eff_pos_err(sigMT, sigML);
  double effMSigerrN = eff_neg_err(sigMT, sigML);

  double effMBack = backET/backEL;
  double effMBackerrP = eff_pos_err(backMT, backML);
  double effMBackerrN = eff_neg_err(backMT, backML);

  double sigEff = ave_sig_eff(NeeL, NemL+NmeL, NmmL, effESig, effMSig);
  std::cout << "sigEff " << sigEff << std::endl;
  double backEff = ave_back_eff(NeeL, NemL, NmeL, NmmL, effESig, effMSig, effEBack, effMBack);
  std::cout << "backEff " << backEff << std::endl;

  double Nsig = (NT - NL*backEff)/(sigEff - backEff);
  double Nback = (NL*sigEff - NT)/(sigEff - backEff);

  //Now for the big mess.  Start by defining a bunch of the common chunks of mess,
  //so you don't have to type them as often.

  double term1 = NeeL*effESig*(effESig - effEBack) + NemL*effESig*(effMSig - effMBack) + 
                  NmeL*effMSig*(effESig - effEBack) + NmmL*(effMSig*effMSig - effMBack);

  double term2 = NeeL*effESig*effESig + (NemL + NmeL)*effESig*effMSig + NmmL*effMSig*effMSig - NT;

  double dFdef = (NL/(term1*term1))*(((term2 + NT)*(effESig*NeeL + effMSig*NmeL))/(term1*term1));

  double dFdmf = (NL/(term1*term1))*(((term2 + NT)*(effESig*NemL + effMSig*NmmL))/(term1*term1));

  double dFder = (NL/(term1*term1))*((NeeL*effESig + NmeL*effMSig)*(2*term1-term2) - term2*(NeeL*(effESig - effEBack)+NemL*(effMSig - effMBack)));
  double dFdmr = (NL/(term1*term1))*((NemL*effESig + NmmL*effMSig)*(2*term1-term2) - term2*(NmeL*(effESig - effEBack)+NmmL*(effMSig - effMBack)));

  double dFdNeet = term2/term1 + (NL/(term1*term1))*(term1*(effESig*effESig - 1) - effESig*(effESig - effEBack)*term2);
  if (NeeT == 0)
  {
    dFdNeet = 0;
  }
  double dFdNemt = term2/term1 + (NL/(term1*term1))*(term1*(effESig*effMSig - 1) - effESig*(effMSig - effMBack)*term2);
  if (NemT == 0)
  {
    dFdNemt = 0;
  }
  double dFdNmet = term2/term1 + (NL/(term1*term1))*(term1*(effMSig*effESig - 1) - effMSig*(effESig - effEBack)*term2);
  if (NmeT == 0)
  {
    dFdNmet = 0;
  }
  double dFdNmmt = term2/term1 + (NL/(term1*term1))*(term1*(effMSig*effMSig - 1) - effMSig*(effMSig - effMBack)*term2);
  if (NmmT == 0)
  {
    dFdNmmt = 0;
  }
  double dFdNeent = term2/term1 + (NL/(term1*term1))*(term1*(effESig*effESig) - effESig*(effESig - effEBack)*term2);
  if ((NeeL-NeeT) == 0)
  {
    dFdNeent = 0;
  }
  double dFdNemnt = term2/term1 + (NL/(term1*term1))*(term1*(effESig*effMSig) - effESig*(effMSig - effMBack)*term2);
  if ((NemL-NemT) == 0)
  {
    dFdNemnt = 0;
  }
  double dFdNment = term2/term1 + (NL/(term1*term1))*(term1*(effMSig*effESig) - effMSig*(effESig - effEBack)*term2);
  if ((NmeL-NmeT) == 0)
  {
    dFdNment = 0;
  }
  double dFdNmmnt = term2/term1 + (NL/(term1*term1))*(term1*(effMSig*effMSig) - effMSig*(effMSig - effMBack)*term2);
  if ((NmmL-NmmT) == 0)
  {
    dFdNmmnt = 0;
  }


  double errorBackP = TMath::Sqrt(dFdef*dFdef*effEBackerrP*effEBackerrP + dFdmf*dFdmf*effMBackerrP*effMBackerrP +
			      dFder*dFder*effESigerrP*effESigerrP + dFdmr*dFdmr*effMSigerrP*effMSigerrP + 
			      dFdNeet*dFdNeet*poisson_up(NeeT)*poisson_up(NeeT) + dFdNemt*dFdNemt*poisson_up(NemT)*poisson_up(NemT) + 
			      dFdNmet*dFdNmet*poisson_up(NmeT)*poisson_up(NmeT) + dFdNmmt*dFdNmmt*poisson_up(NmmT)*poisson_up(NmmT) + 
			      dFdNeent*dFdNeent*poisson_up(NeeL-NeeT)*poisson_up(NeeL-NeeT) + dFdNemnt*dFdNemnt*poisson_up(NemL-NemT)*poisson_up(NemL-NemT) + 
			      dFdNment*dFdNment*poisson_up(NmeL-NmeT)*poisson_up(NmeL-NmeT) + dFdNmmnt*dFdNmmnt*poisson_up(NmmL-NmmT)*poisson_up(NmmL-NmmT));   

  double errorBackN = TMath::Sqrt(dFdef*dFdef*effEBackerrN*effEBackerrN + dFdmf*dFdmf*effMBackerrN*effMBackerrN +
			      dFder*dFder*effESigerrN*effESigerrN + dFdmr*dFdmr*effMSigerrN*effMSigerrN + 
			      dFdNeet*dFdNeet*poisson_down(NeeT)*poisson_down(NeeT) + dFdNemt*dFdNemt*poisson_down(NemT)*poisson_down(NemT) + 
			      dFdNmet*dFdNmet*poisson_down(NmeT)*poisson_down(NmeT) + dFdNmmt*dFdNmmt*poisson_down(NmmT)*poisson_down(NmmT) + 
			      dFdNeent*dFdNeent*poisson_down(NeeL-NeeT)*poisson_down(NeeL-NeeT) + dFdNemnt*dFdNemnt*poisson_down(NemL-NemT)*poisson_down(NemL-NemT) + 
			      dFdNment*dFdNment*poisson_down(NmeL-NmeT)*poisson_down(NmeL-NmeT) + dFdNmmnt*dFdNmmnt*poisson_down(NmmL-NmmT)*poisson_down(NmmL-NmmT)); 

  std::cout << Nback << std::endl;
  std::cout << errorBackP << std::endl;
  std::cout << errorBackN << std::endl;



  double term3 = NT - NL*backEff;

  double sdFdef = -NL*(term1 + term3)*(NeeL*effESig + NmeL*effMSig)/(term1*term1);
  double sdFdmf = -NL*(term1 + term3)*(NemL*effESig + NmmL*effMSig)/(term1*term1);

  double sdFder = -NL*term3*(NeeL*effESig + NmeL*effMSig)/(term1*term1) - NL*(term1*(NeeL*effEBack + NemL*effMBack) + term3*(NeeL*(effESig - effEBack) + NemL*(effMSig-effMBack)));
  double sdFdmr = -NL*term3*(NemL*effESig + NmmL*effMSig)/(term1*term1) - NL*(term1*(NmeL*effEBack + NmmL*effMBack) + term3*(NmeL*(effESig - effEBack) + NmmL*(effMSig-effMBack)));

  double sdFdNeent = NL*(term1*(-effESig*effEBack) - term3*(effESig*(effESig-effEBack)))/(term1*term1) - term3/term1;
  double sdFdNemnt = NL*(term1*(-effESig*effMBack) - term3*(effESig*(effMSig-effMBack)))/(term1*term1) - term3/term1;
  double sdFdNment = NL*(term1*(-effMSig*effEBack) - term3*(effMSig*(effESig-effEBack)))/(term1*term1) - term3/term1;
  double sdFdNmmnt = NL*(term1*(-effMSig*effMBack) - term3*(effMSig*(effMSig-effMBack)))/(term1*term1) - term3/term1;

  double sdFdNeet = NL*(term1*(1-effESig*effEBack) - term3*(effESig*(effESig-effEBack)))/(term1*term1) - term3/term1;
  double sdFdNemt = NL*(term1*(1-effESig*effMBack) - term3*(effESig*(effMSig-effMBack)))/(term1*term1) - term3/term1;
  double sdFdNmet = NL*(term1*(1-effMSig*effEBack) - term3*(effMSig*(effESig-effEBack)))/(term1*term1) - term3/term1;
  double sdFdNmmt = NL*(term1*(1-effMSig*effMBack) - term3*(effMSig*(effMSig-effMBack)))/(term1*term1) - term3/term1;

  double errorSigP = TMath::Sqrt(sdFdef*sdFdef*effEBackerrP*effEBackerrP + sdFdmf*sdFdmf*effMBackerrP*effMBackerrP +
			      sdFder*sdFder*effESigerrP*effESigerrP + sdFdmr*sdFdmr*effMSigerrP*effMSigerrP + 
			      sdFdNeet*sdFdNeet*poisson_up(NeeT)*poisson_up(NeeT) + sdFdNemt*sdFdNemt*poisson_up(NemT)*poisson_up(NemT) + 
			      sdFdNmet*sdFdNmet*poisson_up(NmeT)*poisson_up(NmeT) + sdFdNmmt*sdFdNmmt*poisson_up(NmmT)*poisson_up(NmmT) + 
			      sdFdNeent*sdFdNeent*poisson_up(NeeL-NeeT)*poisson_up(NeeL-NeeT) + sdFdNemnt*sdFdNemnt*poisson_up(NemL-NemT)*poisson_up(NemL-NemT) + 
			      sdFdNment*sdFdNment*poisson_up(NmeL-NmeT)*poisson_up(NmeL-NmeT) + sdFdNmmnt*sdFdNmmnt*poisson_up(NmmL-NmmT)*poisson_up(NmmL-NmmT));

  double errorSigN = TMath::Sqrt(sdFdef*sdFdef*effEBackerrN*effEBackerrN + sdFdmf*sdFdmf*effMBackerrN*effMBackerrN +
			      sdFder*sdFder*effESigerrN*effESigerrN + sdFdmr*sdFdmr*effMSigerrN*effMSigerrN + 
			      sdFdNeet*sdFdNeet*poisson_down(NeeT)*poisson_down(NeeT) + sdFdNemt*sdFdNemt*poisson_down(NemT)*poisson_down(NemT) + 
			      sdFdNmet*sdFdNmet*poisson_down(NmeT)*poisson_down(NmeT) + sdFdNmmt*sdFdNmmt*poisson_down(NmmT)*poisson_down(NmmT) + 
			      sdFdNeent*sdFdNeent*poisson_down(NeeL-NeeT)*poisson_down(NeeL-NeeT) + sdFdNemnt*sdFdNemnt*poisson_down(NemL-NemT)*poisson_down(NemL-NemT) + 
			      sdFdNment*sdFdNment*poisson_down(NmeL-NmeT)*poisson_down(NmeL-NmeT) + sdFdNmmnt*sdFdNmmnt*poisson_down(NmmL-NmmT)*poisson_down(NmmL-NmmT));

//   std::cout << Nsig << std::endl;
//   std::cout << errorSigP << std::endl;
//   std::cout << errorSigN << std::endl;

//   //Get average efficiencies and their errors
//   double sigEff = ave_sig_eff(NeeL, NemL+NmeL, NmmL, effESig, effMSig);
//   double sigEffErrP = ave_sig_eff_err(NeeL, poisson_up(NeeL), (NemL+NmeL), poisson_up((NemL+NmeL)),
// 				      NmmL, poisson_up(NmmL), 
// 				      effESig, effESigerrP, effMSig, effMSigerrP);
//   double sigEffErrN = ave_sig_eff_err(NeeL, poisson_down(NeeL), (NemL+NmeL), 
// 				      poisson_down((NemL+NmeL)), NmmL, poisson_down(NmmL), 
// 				      effESig, effESigerrN, effMSig, effMSigerrN);

//   double backEff = ave_back_eff(NeeL, NemL, NmeL, NmmL, effESig, effMSig, effEBack, effMBack);
//   double backEffErrP = ave_back_eff_err(NeeL, poisson_up(NeeL), NemL, poisson_up(NemL), 
// 					NmeL,poisson_up(NmeL), NmmL, poisson_up(NmmL),
// 					effESig, effESigerrP, effEBack, effEBackerrP, 
// 					effMSig, effMSigerrP, effMBack, effMBackerrP);
//   double backEffErrN = ave_back_eff_err(NeeL, poisson_down(NeeL), NemL, poisson_down(NemL), 
// 					NmeL, poisson_down(NmeL), NmmL, poisson_down(NmmL),
// 					effESig, effESigerrN, effEBack, effEBackerrN, 
// 					effMSig, effMSigerrN, effMBack, effMBackerrN);

//   //Get Matrix Method Estimates and their errors, what all this mess was merely gearing up to compute.
//   //Kind of sickening, isn't it?
//   double Nsig = (NT - NL*backEff)/(sigEff - backEff);
//   double NsigPerr = Nsig*(((poisson_up(NT) - backEff*poisson_up(NL) - NL*backEffErrP)/(NT - NL*backEff)) - ((sigEffErrP - backEffErrP)/(sigEff - backEff)));
//   double NsigNerr = Nsig*(((poisson_down(NT) - backEff*poisson_down(NL) - NL*backEffErrN)/(NT - NL*backEff)) - ((sigEffErrN - backEffErrN)/(sigEff - backEff)));

//   double Nback = (NL*sigEff - NT)/(sigEff - backEff);

//   double numP = sigEff*poisson_up(NL) + NL*sigEffErrP - poisson_up(NT);
//   double denomP = NL*sigEff - NT;
//   double term2P = (sigEffErrP - backEffErrP)/(sigEff - backEff); 
//   std::cout << numP << std::endl;
//   std::cout << denomP << std::endl;
//   std::cout << term2P << std::endl;

//   double numN = sigEff*poisson_down(NL) + NL*sigEffErrN - poisson_down(NT);
//   double denomN = NL*sigEff - NT;
//   double term2N = (sigEffErrN - backEffErrP)/(sigEff - backEff);

//   double NBackPerr = Nback*(numP/denomP - term2P);
//   double NBackNerr = Nback*(numN/denomN - term2N);

//   //Make a nice text file to store all the numbers, 'cause you know you don't want to write them yourself.
//   //More than what it takes to get the numbers on my slides, anyway.
  std::string filename;
  if (applyScale)
  {
    filename = "macros/textfiles/mm_errors_scaled.txt";
  }
  else
  {
    filename = "macros/textfiles/mm_errors.txt";
  }
  ofstream outfile;
  outfile.open(filename.c_str());

  outfile << "effESig " << effESig << " +" << effESigerrP << ", -" << effESigerrN << std::endl;
  outfile << "effEBack " << effEBack << " +" << effEBackerrP << ", -" << effEBackerrN << std::endl;
  outfile << "effMSig " << effMSig << " +" << effMSigerrP << ", " << effMSigerrN << std::endl;
  outfile << "effMBack " << effMBack << " +" << effMBackerrP << ", " << effMBackerrN <<std::endl;

  outfile << "dFdef " << dFdef <<std::endl;
  outfile << "dFdef error " << effEBackerrP << ", " << effEBackerrN << std::endl;
  outfile << "dFdmf " << dFdmf << std::endl;
  outfile << "dFdmf error" << effMBackerrP << ", " << effMBackerrN << std::endl;
  outfile << "dFder " << dFder << std::endl;
  outfile << "dFder error " << effESigerrP << ", " << effESigerrN << std::endl;
  outfile << "dFdmr " << dFdmr << std::endl;
  outfile << "dFdmr error " << effMSigerrP << ", " << effMSigerrN << std::endl;
  outfile << "dFdNeet " << dFdNeet << std::endl;
  outfile << "dFdNeet error " << poisson_up(NeeT) <<", " << poisson_down(NeeT) << std::endl;
  outfile << "dFdNemt " << dFdNemt << std::endl;
  outfile << "dFdNemt error " << poisson_up(NemT) << ", " << poisson_down(NemT) << std::endl;
  outfile << "dFdNmet " << dFdNmet << std::endl;
  outfile << "dFdNmet error " << poisson_up(NmeT) << ", " << poisson_down(NmeT) << std::endl;
  outfile << "dFdNmmt " << dFdNmmt << std::endl;
  outfile << "dFdNmmt error " << poisson_up(NmmT) << ", " << poisson_down(NmmT) << std::endl;
  outfile << "dFdNeent " << dFdNeent << std::endl;
  outfile << "dFdNeent error " << poisson_up(NeeL - NeeT) << ", " << poisson_down(NeeL-NeeT) << std::endl;
  outfile << "dFdNemnt " << dFdNemnt << std::endl;
  outfile << "dFdNemnt error " << poisson_up(NemL-NemT) << ", " << poisson_down(NemL-NemT) << std::endl;
  outfile << "dFdNment " << dFdNment << std::endl;
  outfile << "dFdNment error " << poisson_up(NmeL-NmeT) << ", " << poisson_down(NmeL-NmeT) << std::endl;
  outfile << "dFdNmmnt " << dFdNmmnt << std::endl;
  outfile << "dFdNmmnt error " << poisson_up(NmmL-NmmT) << ", " << poisson_down(NmmL-NmmT) << std::endl;

//   outfile << "Loose sample = " << NL << " + " << poisson_up(NL) << ", - " << poisson_down(NL) << std::endl;
//   outfile << "Tight sample = " << NT << " + " << poisson_down(NT) << ", - " << poisson_down(NT) << std::endl << std::endl;

//   outfile << "Ave. signal eff = " << sigEff << " + " << sigEffErrP << ", - " << sigEffErrN << std::endl;
//   outfile << "Ave. back eff = " << backEff << " + " << backEffErrP << ", - " << backEffErrN << std::endl << std::endl;

//   outfile << "Estimated Signal = " << Nsig << " + " << NsigPerr << ", - " << NsigNerr << std::endl;
//   outfile << "Estimated Background = " << Nback << " + " << NBackPerr << ", - " << NBackNerr << std::endl << std::endl;

  if (applyScale)
  {
    double scaleL = 5539;
    double scaleT = 142;
    double factor = scaleT/scaleL;
    double factorErrP = eff_pos_err(scaleT, scaleL);
    double factorErrN = eff_neg_err(scaleT, scaleL);

    double NbackNew = Nback*factor;
    double NBackNewErrP = NbackNew*(errorBackP/Nback + factorErrP/factor);
    double NBackNewErrN = NbackNew*(errorBackN/Nback + factorErrN/factor);

    std::cout << "Scaled Background = " << NbackNew << " + " << NBackNewErrP << ", - " << NBackNewErrN << std::endl << std::endl;
  }

  outfile.close();
}
