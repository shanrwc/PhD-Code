#include "TROOT.h"
#include "TString.h"
#include "TFile.h"
#include "TH2F.h"
#include <iostream>
#include <iomanip>
using namespace std;

/**
   Script to asses the ID Data Quality (for now pixel B-Layer status) from the (eta/phi)
   of a track. 

   This new version can be compiled (using the + or ++ option), and the safety score is returned,
   so that it can be used in analysis scripts.
   Also, the function names have been changed ("ID" inserted), to avoid possible name conflicts with the
   calorimeter object quality script.

   WARNING: 
   The script is just a rough estimate of the quality of the regions. 
   The disabled regions are well defined in the detector, but the mapping from track_eta(track_phi) to 
   detector_eta(detector_phi) is non-trivial and dependent on various factors.  
   (Eg: Beam spot, Track parameter, ect.)  To be sure that the track passed an inactive region one should use the 
   extrapolator tools in athena.
   
   Nevertheless, one can use this script to get a general feel for the DQ in this region.

   USE:
   copy IDTester.C and IDDQ_152409.root to a local directory

   > root
   > .L IDDQTester.C+
   > float safety = checkIDRegion(0,1,0.1,"152409")

   The region method takes 4 parameter checkIDRegion(eta,phi,deltaR, runNumber)
   
   - eta is the track eta
   - phi is the trac phi (-3.14,3.14)
   - deltaR is the size of region in which the DQ check is performed. 
      eta +/- deltaR    and    phi +/- deltaR
   - runNumber is the run number in question.  It is forseen that the DQ will depend on run number.
      The ID inactive regions have this far been stable and thus parameter is not currently used.

   OUTPUT
   If no cases of tracks crossing an inactive region are found in the region,
   the returned safety is zero (zero probability of inactivity)
   
   If there are cases of tracks cases an inactive reporter are found in the region,
   the saftey score is returned.
   
   The safety score is the fraction of tracks crossing inactive B-Layer modules in the region.
   Thus higher values of safety score, indicate that a track in this region is more likely to cross an
   inactive BLayer module.
*/

// This function checks the inactive B-Layer modules in the region myEta+/-deltaR VS myPhy+/-deltaR
//  If the region is near an inactive B-Layer modules it reports the saftey-score, which is some indication
//  of how likely it is for the track to pass an iactive region.

void checkIDDQ(double myEta, double myPhi, int& nInactive, int& nActive, TString runnumber, TH2F* inactive, TH2F* active);
float checkIDRegion(double myEta, double myPhi, double deltaR=0.1, TString runnumber="15409");

float checkIDRegion(double myEta, double myPhi, double deltaR, TString runnumber) {

  // Get the histograms
  TFile *f = new TFile("IDDQ_152409.root","READ");
  
  // The active/inactive maps (not using the BS)
  TH2F *active;
  TH2F *inactive;
  gFile->GetObject("etaPhiDeadBLayer",inactive);
  gFile->GetObject("etaPhiActiveBLayer",active);
  
  // The raw number of tracks passing active or inactive B-Layer modules
  //  for bins within the region.
  int nInactive = 0;
  int nActive = 0;
  
  if(myPhi < 0) myPhi += 2*3.14;

  // Define the region
  Double_t etaMin = myEta-deltaR;
  Double_t etaMax = myEta+deltaR;  
  Double_t phiMin = myPhi-deltaR;
  Double_t phiMax = myPhi+deltaR;

  // Step size is set by the histogram binning
  Double_t stepSize = active->GetBinWidth(1);
  
  // Number of steps
  Int_t etaSteps = (etaMax-etaMin)/stepSize;
  Int_t phiSteps = (phiMax-phiMin)/stepSize;

  // The points to check
  Double_t theEta;
  Double_t thePhi;

  theEta=etaMin;
  for(int i=0; i<=etaSteps+1; i++){
    thePhi=phiMin;
    for (int k =0; k<=phiSteps+1; k++){
      // Checks the current point. (Updates nInactive and nActive)
      checkIDDQ(theEta,thePhi,nInactive, nActive, runnumber,inactive,active);
      thePhi = thePhi + stepSize;
    }
    theEta = theEta + stepSize;
  }

//   cout << "Eta: " << myEta << "      Phi: " << myPhi << endl;
//   // If we dont see any inactive declare the region as good.
//   if(nInactive == 0)
//     cout << " Not near inactive BLayer region " << endl;
//   
//   // If we see tracks passing inactive regions, 
//   // report it and provide the difference in the  nActive and nInactive
//   // as an indication of how questionable the region is.
//   else{
//     cout << " Near inactive BLayer region " << endl;
//     cout << "  Saftey score: " << std::setprecision(2) << float(nInactive)/(nActive + nInactive) << endl;
//   }
  
  f->Close();

  float safetyscore = 0; // All good unless told otherwise
  if (nActive) {
    safetyscore = float(nInactive)/(nActive + nInactive);
  }

  return safetyscore;
}


// Finds the bin content of the relevant histograms for the eta(phi) and increments the nActive and nInactive counters
void checkIDDQ(double myEta, double myPhi, int& nInactive, int& nActive, TString runnumber, TH2F* inactive, TH2F* active){
    
  //First check: are values inside the histo? If not, go out
  // Eta
  Double_t xmin = 0.;
  xmin = inactive->GetXaxis()->GetXmin();
  Double_t xmax = 0.;
  xmax = inactive->GetXaxis()->GetXmax();
  if(myEta<xmin || myEta>xmax) {
    return;
  }
  
  // Phi
  Double_t ymin = 0.;
  ymin = inactive->GetYaxis()->GetXmin();
  Double_t ymax = 0.;
  ymax = inactive->GetYaxis()->GetXmax();
  
  //Handle wrap around effect
  if(myPhi < ymin)
    myPhi = myPhi + 2*3.14;

  if(myPhi > ymax)
    myPhi = myPhi - 2*3.14;
  
  if((myPhi<ymin || myPhi>ymax) ) {
    cout << " Phi not in the region of interest" << endl;
    return;
  }
  
  // Number of bins
  int theXBin2 = inactive->GetXaxis()->FindBin(myEta);
  int theYBin2 = inactive->GetYaxis()->FindBin(myPhi);

//   int numOfBinsX = 0;
//   int numOfBinsY = 0;
//   numOfBinsX = inactive->GetXaxis()->GetNbins();
//   numOfBinsY = inactive->GetYaxis()->GetNbins();
//   
//   // Find the bin we want
//   int theXBin=0;
//   int theYBin=0;
//   for (int i=0; i<numOfBinsX; i++) {
//     if (myEta> inactive->GetXaxis()->GetBinLowEdge(i)  ) {
//       theXBin=i;
//     }
//   }
//   
//   for (int i=0; i<numOfBinsY; i++) {
//     if (myPhi> inactive->GetYaxis()->GetBinLowEdge(i)  )  {
//       theYBin=i;
//     }
//   }
//   if (theXBin2 != theXBin) cout << "XBins not equal: " << theXBin <<", " << theXBin2 << ", at eta of " << myEta << endl;
//   else cout << "XBins equal: " << theXBin2 << ", at eta of " << myEta << endl;
//   if (theYBin2 != theYBin) cout << "YBins not equal: " << theYBin <<", " << theYBin2 << ", at phi of " << myPhi << endl;
//   else cout << "YBins equal: " << theYBin2 << ", at eta of " << myEta << endl;
  
  nInactive += inactive->GetBinContent(theXBin2,theYBin2);
  nActive += active->GetBinContent(theXBin2,theYBin2);
  
  return;
}



////////////////////////////////////////////////////
