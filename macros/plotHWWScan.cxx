#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2.h"
#include "THStack.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TMath.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TLine.h"
#include "TF1.h"
#include "TFitResultPtr.h"

#include "HiggsAnalysis/HWWConstants.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

////////////////////////////////////////////////////////////////////////////
///////////////////////////Helper Functions/////////////////////////////////
////////////////////////////////////////////////////////////////////////////

double poisson_up(double y) {
  if (y < 0.0) return 0.0;
  double y1 = y + 1;
  double d = 1.0 - 1.0/(9.0*y1) + 1.0/(3*TMath::Sqrt(y1));
  return (y1*d*d*d - y);
}

double poisson_down(double y) {
  if (y <= 0.0) return 0.0;
  double d = 1.0 - 1.0/(9.0*y) - 1.0/(3.0*TMath::Sqrt(y));
  return (y - y*d*d*d);
}

template<typename T>
std::string makeString(T const& value)
{
  std::stringstream sstr;
  sstr << value;
  return sstr.str();
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

void plotHWWScan()
{
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);

  const double lumin = 0.329602;

//   const Color_t H130COLOR = kRed;
  const Color_t WWCOLOR = 4;
  const Color_t ZCOLOR = 210;
  const Color_t WCOLOR = 227;
  const Color_t TOPCOLOR = 219;
  const Color_t EWCOLOR = 222;

  const Style_t DATAMARKER = 8;
  const Size_t DATASIZE = 1.5;
  const Width_t DATALINEWIDTH = 2.5;
  const Width_t MCLINEWIDTH = 2;

  const Float_t LEGTEXTSIZE = 0.04;

  TFile* DATA = new TFile("root-files/hww/data_hwwstudy.root");
  TFile* SMZ = new TFile("root-files/hww/smZ_hwwstudy.root");
  TFile* SMWW = new TFile("root-files/hww/smWW_hwwstudy.root");
  TFile* SMW = new TFile("root-files/hww/smW_hwwstudy.root");
  TFile* SMEW = new TFile("root-files/hww/smEW_hwwstudy.root");
  TFile* TOP = new TFile("root-files/hww/Top_hwwstudy.root");

  std::vector<std::string> particles;
  particles.push_back("e");
  particles.push_back("m");

  std::vector<std::string> njets;
  njets.push_back("0");
  njets.push_back("1");
  njets.push_back("2");

  std::string filename = "macros/textfiles/zscan.txt";
  std::ofstream outfile;
  outfile.open(filename.c_str());

  for (int nP = 0; nP != (int)particles.size(); ++nP)
  {
    for (int nJ = 0; nJ != (int)njets.size(); ++nJ)
    {
      if (nJ == 2) continue;

      //Get pertinent histograms
      std::string label = "scan_" + particles[nP] + "_" + njets[nJ];

      TH1D* datab = (TH1D*)DATA->Get(("b" + label).c_str()); 
      TH1D* datad = (TH1D*)DATA->Get(("d" + label).c_str()); 

      TH1D* zb = (TH1D*)SMZ->Get(("b" + label).c_str()); 
      TH1D* zd = (TH1D*)SMZ->Get(("d" + label).c_str()); 

      int length = 15;

      //Make vectors to store results
      double* vx = new double[length];
      double* vex = new double[length];
      double* dataRatio = new double[length];
      double* dataErr = new double[length];
      double* mcRatio = new double[length];
      double* mcErr = new double[length];
      double* factors = new double[length];
      double* factErr = new double[length];

      for (int i = 0; i != length; ++i)
      {
	vx[i] = 0;
	vex[i] = 0.0;
	dataRatio[i] = 0.0;
	dataErr[i] = 0.0;
	mcRatio[i] = 0.0;
	mcErr[i] = 0.0;
	factors[i] = 0.0;
	factErr[i] = 0.0;
      }

      //Loop over histos and compute efficiencies and correction factors
      if (nP == 0)
      {
	outfile << "Electrons" << std::endl;
      }
      else
      {
	outfile << "Muons" << std::endl;
      }
      outfile << nJ << " jet bin" << std::endl;
      outfile << "Efficiencies (N_B/(N_B + N_D))" << std::endl;
      for (int k = 1; k <= length; ++k)
      {
	vx[k-1] = k*10;

	double dataEB = 0;
	//for k = 1, get one, for k = 2, get 11
	//10*k - 9
	double dataIB = datab->IntegralAndError(10*k - 9, 10*k,dataEB);
	double dataED = 0;
	double dataID = datad->IntegralAndError(10*k - 9, 10*k,dataED);

	double zEB = 0;
	double zIB = zb->IntegralAndError(10*k - 9, 10*k,zEB);
	double zED = 0;
	double zID = zd->IntegralAndError(10*k - 9, 10*k,zED);

	double effData = dataIB/(dataIB + dataID);
	double errData = sqrt(effData*effData/(dataIB*dataIB)*(dataEB*dataEB*(1-effData)*(1-effData) + dataED*dataED*effData*effData));
	dataRatio[k-1] = effData;
	dataErr[k-1] = errData;

	double effMC = zIB/(zIB + zID);
	double errMC = sqrt(effMC*effMC/(zIB*zIB)*(zEB*zEB*(1-effMC)*(1-effMC) + zED*zED*effMC*effMC));
	if (effMC == 0)
	{
	  errMC = 0;
	}
	mcRatio[k-1] = effMC;
	mcErr[k-1] = errMC;

	outfile << 10*k-9 << " < mll < " << 10*k << ": " << effMC << " +/- " << errMC << std::endl; 

	double nom = effData/effMC;
        double staterr = sqrt(nom*nom*((errData/effData)*(errData/effData) + (errMC/effMC)*(errMC/effMC)));
	factors[k-1] = nom;
	factErr[k-1] = staterr;
      }
      outfile << std::endl;

      //Make graphs
      TGraphErrors* dataEffs = new TGraphErrors(length,vx,dataRatio,vex,dataErr);

      TGraphErrors* mcEffs = new TGraphErrors(length,vx,mcRatio,vex,mcErr);
      mcEffs->GetXaxis()->SetTitle("m_{ll} [GeV]");
      mcEffs->GetYaxis()->SetTitle("N_{B}/(N_{B} + N_{D}) (MC)");

      TF1* func = new TF1("fitfunc","[0]*x + [1]",0,155);
      double guess = (mcEffs->GetMaximum() + mcEffs->GetMinimum())/2;
      func->SetParameter(0,0.001);
      mcEffs->Fit("fitfunc","RS");
      std::cout << "Fit result: p[0] = " << func->GetParameter(0) << " err_p[0] = " << func->GetParError(0) << std::endl << std::endl << std::endl;

      TGraphErrors* graph = new TGraphErrors(length,vx,factors,vex,factErr);

      TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,800,600);
      pad->SetFillColor(0);
      pad->SetTitle(label.c_str());
      pad->SetLogy(1);
      if (nP == 1 && nJ == 0)
      {
	mcEffs->SetMaximum(0.005);
	mcEffs->SetMinimum(-0.005);
      }
      mcEffs->Draw("AP");

      std::string temp;
      if (nP == 0)
      {
	temp = "el_";
      }
      else
      {
	temp = "mu_";
      }
      temp +=makeString(nJ);
      std::string name = "macros/HWWplots/zcontrol/scan_effs_" + temp + "_mc.png";
      pad->Print(name.c_str());

    }//closes loop over jet bins
  }//closes loop particles
  outfile.close();


  /////////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////////And some more plots///////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////

  
//     std::vector<std::string> properties;
//     properties.push_back("met");
//     properties.push_back("mll");

//     std::vector<std::string> xaxes;
//     xaxes.push_back("E_{T,Rel}^{miss} [GeV]");
//     xaxes.push_back("m_{ll} [GeV]");

//     std::vector<std::string> regions;
//     regions.push_back("a");
//     regions.push_back("b");
//     regions.push_back("c");
//     regions.push_back("d");

//     std::vector<std::string> Regions;
//     Regions.push_back("A");
//     Regions.push_back("B");
//     Regions.push_back("C");
//     Regions.push_back("D");

//     particles.clear();
//     particles.push_back("ee");
//     particles.push_back("mm");

//     std::vector<std::string> yaxes;
//     yaxes.push_back("Electron Events");
//     yaxes.push_back("Muon Events");

//     njets.clear();
//     njets.push_back("zero");
//     njets.push_back("one");
//     njets.push_back("two");

//     std::vector<std::string> jetlabels;
//     jetlabels.push_back("0 Jet");
//     jetlabels.push_back("1 Jet");
//     jetlabels.push_back("2 Jet");

//     std::vector<int> rebins;
//     rebins.push_back(5);
//     rebins.push_back(5);

//     for (int nProp = 0; nProp != (int)properties.size(); ++nProp)
//     {
//       for (int nReg = 0; nReg != (int)regions.size(); ++nReg)
//       {
// 	for (int nPart = 0; nPart != (int)particles.size(); ++nPart)
// 	{
// 	  for (int nJ = 0; nJ != (int)njets.size(); ++nJ)
// 	  {
// // 	    if (nProp != 0) continue;
// 	    if (nJ == 2) continue;

// 	    std::string label = properties[nProp] + "_" + regions[nReg] + "_" + particles[nPart] + "_" + njets[nJ];

// 	    TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,800,600);
// 	    pad->SetTitle(label.c_str());
// 	    pad->SetFillColor(0);
// 	    pad->SetTopMargin(0.1);

// 	    TH1D* data = (TH1D*)DATA->Get(label.c_str()); 
// 	    data->SetMarkerStyle(DATAMARKER);
// 	    data->SetMarkerSize(DATASIZE );
// 	    data->SetLineWidth(DATALINEWIDTH);
// // 	    data->GetXaxis()->SetTitle(xaxes[nProp].c_str());
// // 	    data->GetYaxis()->SetTitle(yaxes[nPart].c_str());
// 	    data->Rebin(rebins[nProp]);

// 	    TH1D* smz = (TH1D*)SMZ->Get(label.c_str()); 
// 	    smz->Scale(lumin);
// 	    smz->SetTitle("");
// 	    smz->SetLineWidth(MCLINEWIDTH);
// 	    smz->SetFillColor(ZCOLOR);
// 	    smz->Rebin(rebins[nProp]);

// 	    TH1D* smww = (TH1D*)SMWW->Get(label.c_str());
// 	    smww->Scale(lumin);
// 	    smww->SetTitle("");
// 	    smww->SetLineWidth(MCLINEWIDTH);
// 	    smww->SetFillColor(WWCOLOR);
// 	    smww->Rebin(rebins[nProp]);

// 	    TH1D* smw = (TH1D*)SMW->Get(label.c_str());
// 	    smw->Scale(lumin);
// 	    smw->SetTitle("");
// 	    smw->SetLineWidth(MCLINEWIDTH);
// 	    smw->SetFillColor(WCOLOR);
// 	    smw->Rebin(rebins[nProp]);

// 	    TH1D* smew = (TH1D*)SMEW->Get(label.c_str());
// 	    smew->Scale(lumin);
// 	    smew->SetTitle("");
// 	    smew->SetLineWidth(MCLINEWIDTH);
// 	    smew->SetFillColor(EWCOLOR);
// 	    smew->Rebin(rebins[nProp]);

// 	    TH1D* top = (TH1D*)TOP->Get(label.c_str());
// 	    top->Scale(lumin);
// 	    top->SetTitle("");
// 	    top->SetLineWidth(MCLINEWIDTH);
// 	    top->SetFillColor(TOPCOLOR);
// 	    top->Rebin(rebins[nProp]);

// 	    THStack* stack = new THStack(label.c_str(),label.c_str());
// 	    stack->SetTitle("");
// 	    stack->Add(top);
// 	    stack->Add(smw);
// 	    stack->Add(smew);
// 	    stack->Add(smz);
// 	    stack->Add(smww);

// 	    double max = data->GetBinContent(data->GetMaximumBin());
// 	    if (1.08*max > stack->GetMaximum() && max < 10000)
// 	    {
// 	      data->GetXaxis()->SetTitle(xaxes[nProp].c_str());
// 	      data->GetYaxis()->SetTitle(yaxes[nPart].c_str());
// 	      data->Draw("P E1 X0");
// 	      data->SetMinimum(0.0001);
// 	      stack->Draw("hist same");
// 	      data->Draw("P E1 X0 same");
// 	    }
// 	    else
// 	    {
// 	      stack->Draw("hist");
// 	      stack->GetXaxis()->SetTitle(xaxes[nProp].c_str());
// 	      stack->GetYaxis()->SetTitle(yaxes[nPart].c_str());
// 	      stack->SetMinimum(0.0001);
// 	      data->Draw("P E1 X0 same");
// 	    }

// 	    TLegend *leg = new TLegend(0.68,0.65,0.9,0.85);
// 	    leg->SetTextSize(LEGTEXTSIZE);
// 	    leg->SetFillColor(0);
// 	    leg->SetBorderSize(0);

// 	    leg->AddEntry(data, "2011 Data (B-F)","p");
// 	    leg->AddEntry(smz,"Z#rightarrowll","f");
// 	    leg->AddEntry(smww,"WW#rightarrowl#nul#nu","f");
// 	    leg->AddEntry(smw,"W#rightarrowl#nu","f");
// 	    leg->AddEntry(smew,"Diboson","f");
// 	    leg->AddEntry(top,"Top","f");
// 	    leg->Draw();

// 	    std::string header;
// 	    if (nPart == 0)
// 	    {
// 	      header = "Electron ";
// 	    }
// 	    else
// 	    {
// 	      header = "Muon ";
// 	    }
// 	    header += Regions[nReg];
// 	    header += " ";
// 	    header += jetlabels[nJ];

// 	    if (nPart == 0)
// 	    {
// 	      TLatex* head = new TLatex();
// 	      head->SetTextSize(0.06);
// 	      head->SetNDC();
// 	      head->DrawLatex(0.42,0.91,header.c_str());
// 	    }
// 	    else
// 	    {
// 	      TLatex* head = new TLatex();
// 	      head->SetTextSize(0.06);
// 	      head->SetNDC();
// 	      head->DrawLatex(0.45,0.91,header.c_str());
// 	    }

// 	    std::string name = "macros/HWWplots/zcontrol/" + label + ".png";
// 	    pad->Print(name.c_str());

// 	  }//Closes loop over jet bins
// 	}//Closes loop over particles
//       }//Closes loop over regions
//     }//Closes loop over properties

}//closes macro
