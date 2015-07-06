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

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "HiggsAnalysis/HWWConstants.h"
#include "atlasstyle-00-03-03/AtlasLabels.C"

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

TGraphAsymmErrors* GetEfficiencyGraph(TH1D* num) {
  int nrBins = num->GetNbinsX();
  //double xmin = num->GetBinCenter(1);
  double width = num->GetBinWidth(1);
  double* x = new double[nrBins];
  double* y = new double[nrBins];
  double* exl = new double[nrBins];
  double* exh = new double[nrBins];
  double* eyl = new double[nrBins];
  double* eyh = new double[nrBins];
  for (int i = 0; i != nrBins; ++i) {
    double m = num->GetBinContent(i+1);
    x[i] = num->GetBinCenter(i+1);
    y[i] = m;//n != 0.0 ? (m / n) : 0.0;
    exl[i] = width/2.0;
    exh[i] = width/2.0;
    eyl[i] = poisson_down(m);

    eyh[i] = poisson_up(m);
    if(y[i] == 0)
      {
        eyl[i] = 0;
        eyh[i] = 0;
      }
  }
  TGraphAsymmErrors* T_data = new TGraphAsymmErrors(nrBins, x, y, exl, exh, eyl, eyh);
//   T_data->SetLineWidth(2);
//   T_data->SetMarkerStyle(20);
//   T_data->SetMarkerColor(kBlack);
  return T_data;
} 

template<typename T>
std::string makeString(T const& value)
{
  std::stringstream sstr;
  sstr << value;
  return sstr.str();
}

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

void plotHWWOther()
{
  const double lumin = 0.163502;

  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);

  const Style_t DATAMARKER = 8;
  const Size_t DATASIZE = 1.5;
  const Width_t DATALINEWIDTH = 2.5;
  const Width_t MCLINEWIDTH = 2;

  TFile *DATA = new TFile("root-files/hww/data_hwwstudy.root");
  TFile *WW = new TFile("root-files/hww/smWW_hwwstudy.root");
  TFile* Z = new TFile("root-files/hww/smZ_hwwstudy.root");
  TFile* W = new TFile("root-files/hww/smW_hwwstudy.root");
  TFile* TTB = new TFile("root-files/hww/ttbar_hwwstudy.root");
  TFile* STOP = new TFile("root-files/hww/sTop_hwwstudy.root");
  TFile *EW = new TFile("root-files/hww/smEW_hwwstudy.root");

  std::vector<std::string> plots;
  plots.push_back("mu_PV");
  plots.push_back("mu_PV2");

//   for (int nP = 0; nP != (int)plots.size(); ++nP)
//   {
//     std::string label = plots[nP];

//     TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,800,600);
//     pad->SetFillColor(0);
//     pad->SetTitle("");

//     TH1D* data = (TH1D*)DATA->Get(label.c_str());
//     data->SetMarkerStyle(DATAMARKER);
//     data->SetMarkerSize(DATASIZE);
//     data->SetLineWidth(DATALINEWIDTH);
//     data->SetTitle("");
//     double total = data->Integral();

//     TH1D* part = (TH1D*)ZPART->Get(label.c_str());
//     part->Scale(total/part->Integral());
//     part->SetTitle("");
//     part->SetLineColor(kRed);
//     part->SetLineWidth(MCLINEWIDTH);

//     TH1D* mc10b = (TH1D*)MC10B->Get(label.c_str());
//     mc10b->Scale(total/mc10b->Integral());
//     mc10b->SetTitle("");
//     mc10b->SetLineColor(kBlue);
//     mc10b->SetLineWidth(MCLINEWIDTH);

// //     data->Draw("P E1 X0");
//     part->Draw("");
//     mc10b->Draw("same");
// //     data->Draw("P E1 X0 same");

//     std::string name = "macros/HWWplots/other/" + label + ".png";
//     pad->Print(name.c_str());
//   }


//   std::vector<TFile*> files;
//   files.push_back(DATA);
//   files.push_back(Z);

//   std::vector<std::string> jetbins;
//   jetbins.push_back("one");
//   jetbins.push_back("two");

//   std::vector<std::string> properties;
//   properties.push_back("met");
//   properties.push_back("mll");

//   for (int nFile = 0; nFile != (int)files.size(); ++nFile)
//   {
//     for (int nJ = 0; nJ != (int)jetbins.size(); ++nJ)
//     {
//       for (int nProp = 0; nProp != (int)properties.size(); ++nProp)
//       {
// 	std::string label = "tt" + properties[nProp] + "_" + jetbins[nJ];
// 	std::string label2 = label;
// 	if (nFile == 0)
// 	{
// 	  label2 += "_data";
// 	}
// 	else
// 	{
// 	  label2 += "z_mc";
// 	}

// 	TCanvas* pad = new TCanvas(label2.c_str(),label2.c_str(),14,33,800,600);
// 	pad->SetFillColor(0);
// 	pad->SetTitle(label2.c_str());
// 	pad->SetRightMargin(0.13);

// 	TH2D* plot= (TH2D*)files[nFile]->Get(label.c_str());
// 	plot->SetTitle("");
// 	if (nFile == 1)
// 	{
// 	  plot->Scale(lumin);
// 	}
// 	plot->Draw("colz");
// 	plot->GetXaxis()->SetTitle("Z#rightarrow#tau#tau Mass");
// 	if (nProp == 0)
// 	{
// 	  plot->GetYaxis()->SetTitle("E_{T,Rel}^{miss} [GeV]");
// 	}
// 	else if (nProp == 1)
// 	{
// 	  plot->GetYaxis()->SetTitle("m_{ll} [GeV]");
// 	}
// 	if (nFile == 0)
// 	{
// 	  plot->GetZaxis()->SetTitle("Data Events");
// 	}
// 	else
// 	{
// 	  plot->GetZaxis()->SetTitle("Expected MC Events");
// 	}

// 	std::string name = "macros/HWWplots/other/" + label + ".png";
// 	pad->Print(name.c_str());
//       }
//     }//Closes loop over jet bins
//   }//Closes loop over files


/////////////////////////////////////////////////////////////////////////////////
////////////////////Make plot of "wiggling" z-window/////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

//     std::vector<std::string> particles;
//     particles.push_back("e");
//     particles.push_back("m");

//     std::vector<std::string> jbins;
//     jbins.push_back("0");
//     jbins.push_back("1");
//     jbins.push_back("2");

//     for (int nP = 0; nP != (int)particles.size(); ++nP)
//     {
//       for (int nJ = 0; nJ != (int)jbins.size(); ++nJ)
//       {
// 	if (nJ > 1) continue;
// 	TH1D* datab = (TH1D*)DATA->Get(("b_" + particles[nP] + jbins[nJ] + "_width").c_str());
// 	TH1D* datad = (TH1D*)DATA->Get(("d_" + particles[nP] + jbins[nJ] + "_width").c_str());

// 	TH1D* zdyb = (TH1D*)Z->Get(("b_" + particles[nP] + jbins[nJ] + "_width").c_str());
// 	TH1D* zdyd = (TH1D*)Z->Get(("d_" + particles[nP] + jbins[nJ] + "_width").c_str());

// 	TH1D* topb = (TH1D*)TOP->Get(("b_" + particles[nP] + jbins[nJ] + "_width").c_str());
// 	TH1D* topd = (TH1D*)TOP->Get(("d_" + particles[nP] + jbins[nJ] + "_width").c_str());

// 	TH1D* wwb = (TH1D*)WW->Get(("b_" + particles[nP] + jbins[nJ] + "_width").c_str());
// 	TH1D* wwd = (TH1D*)WW->Get(("d_" + particles[nP] + jbins[nJ] + "_width").c_str());

// 	TH1D* wb = (TH1D*)W->Get(("b_" + particles[nP] + jbins[nJ] + "_width").c_str());
// 	TH1D* wd = (TH1D*)W->Get(("d_" + particles[nP] + jbins[nJ] + "_width").c_str());

// 	TH1D* ewb = (TH1D*)EW->Get(("b_" + particles[nP] + jbins[nJ] + "_width").c_str());
// 	TH1D* ewd = (TH1D*)EW->Get(("d_" + particles[nP] + jbins[nJ] + "_width").c_str());

// 	int length = 20;
// 	double* ratio = new double[length];
// 	double* error = new double[length];
// 	double* vx = new double[length];
// 	double * vex = new double[length];

// 	for (int j = 1; j <= length; ++j)
// 	{
// 	  double dataRB = 0;
// 	  double dataIB = datab->IntegralAndError(j,j,dataRB);
// 	  double dataRD = 0;
// 	  double dataID = datad->IntegralAndError(j,j,dataRD);

// 	  double topRB = 0;
// 	  double topIB = topb->IntegralAndError(j,j,topRB);
// 	  double topRD = 0;
// 	  double topID = topd->IntegralAndError(j,j,topRD);

// 	  double wwRB = 0;
// 	  double wwIB = wwb->IntegralAndError(j,j,wwRB);
// 	  double wwRD = 0;
// 	  double wwID = wwd->IntegralAndError(j,j,wwRD);

// 	  double wRB = 0;
// 	  double wIB = wb->IntegralAndError(j,j,wRB);
// 	  double wRD = 0;
// 	  double wID = wd->IntegralAndError(j,j,wRD);

// 	  double ewRB = 0;
// 	  double ewIB = ewb->IntegralAndError(j,j,ewRB);
// 	  double ewRD = 0;
// 	  double ewID = ewd->IntegralAndError(j,j,ewRD);

// 	  dataIB = dataIB - topIB - wwIB - wIB - ewIB;
// 	  dataID = dataID - topID - wwID - wID - ewID;

// 	  dataRB = sqrt(dataRB*dataRB + topRB*topRB + wwRB*wwRB + wRB*wRB + ewRB*ewRB);
// 	  dataRD = sqrt(dataRD*dataRD + topRD*topRD + wwRD*wwRD + wRD*wRD + ewRD*ewRD);

// 	  double dataEff = dataIB/(dataIB + dataID);
// 	  double dataErr = sqrt(dataEff*dataEff/(dataIB*dataIB)*(dataRB*dataRB*(1-dataEff)*(1-dataEff) + dataRD*dataRD*dataEff*dataEff));

// 	  double zdyRB = 0;
// 	  double zdyIB = zdyb->IntegralAndError(j,j,zdyRB);
// 	  double zdyRD = 0;
// 	  double zdyID = zdyd->IntegralAndError(j,j,zdyRD);

// 	  double zdyEff = zdyIB/(zdyIB + zdyID);
// 	  double zdyErr = sqrt(zdyEff*zdyEff/(zdyIB*zdyIB)*(zdyRB*zdyRB*(1-zdyEff)*(1-zdyEff) + zdyRD*zdyRD*zdyEff*zdyEff));

// 	  double nom = dataEff/zdyEff;
// 	  double err = sqrt(nom*nom*((dataErr/dataEff)*(dataErr/dataEff) + (zdyErr/zdyEff)*(zdyEff/zdyEff)));

// 	  vx[j - 1] = j;
// 	  vex[j - 1] = 0.0;
// 	  ratio[j - 1] = nom;
// 	  error[j - 1] = err;
//         }

// 	std::string label = particles[nP] + "_" + jbins[nJ] + "jets_varwin";
// 	TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,800,600);
// 	pad->SetFillColor(0);
// 	pad->SetTitle("");
// 	pad->SetTopMargin(0.1);

// 	TGraphErrors* graph = new TGraphErrors(length,vx,ratio,vex,error);
// 	graph->SetLineWidth(2);
// 	graph->Draw("ap");

// 	std::string headtext;
// 	double xcor = 0.0;
// 	if (nP == 0)
// 	{
// 	  headtext = "Electron ";
// 	  xcor = 0.40;
// 	}
// 	else
// 	{
// 	  headtext = "Muon ";
// 	  xcor = 0.45;
// 	}
// 	if (nJ == 0)
// 	{
// 	  headtext += "0 Jet";
// 	}
// 	else if (nJ == 1)
// 	{
// 	  headtext += "1 Jet";
// 	}
// 	else if (nJ == 2)
// 	{
// 	  headtext += "2 Jet";
// 	}
// 	TLatex* header = new TLatex();
// 	header->SetTextSize(0.06);
// 	header->SetNDC();
// 	header->DrawLatex(xcor,0.91,headtext.c_str());

// 	std::string name = "macros/HWWplots/other/" + label + ".png";
// 	pad->Print(name.c_str());
//       }
//     }

    std::vector<std::string> samples;
    samples.push_back("Zee");
    samples.push_back("Zmm");

    std::vector<std::string> properties;
    properties.push_back("ptll");
    properties.push_back("mll");

    std::vector<std::string> xtitles;
    xtitles.push_back("p_{T}^{ll} [GeV]");
    xtitles.push_back("m_{ll}^{truth} [GeV]");

    std::vector<int> maxes;
    maxes.push_back(12000);
    maxes.push_back(4500);
    maxes.push_back(1600);
    maxes.push_back(200);
    maxes.push_back(60);
    maxes.push_back(20);

    for (int nSample = 0; nSample != (int)samples.size(); ++nSample)
    {
      for (int nPar = 0; nPar != 6; ++nPar)
      {
	for (int nProp = 0; nProp != (int)properties.size(); ++nProp)
	{
	  if (nProp == 0) continue;

	  std::string name = samples[nSample] + "Np" + makeString(nPar);

	  TFile* zFile = new TFile(("root-files/hww/nominal/" + name + "_hwwstudy.root").c_str());
	  TFile* dyFile = new TFile(("root-files/hww/nominal/dy" + name + "_hwwstudy.root").c_str());

	  std::string label = "truth_" + properties[nProp] + samples[nSample] + "Np" + makeString(nPar);

	  TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,800,600);
	  pad->SetFillColor(0);
	  pad->SetTitle(label.c_str());
	  pad->SetTopMargin(0.1);
	  
	  std::string name2 = "truth_" + properties[nProp];

	  TH1D* zpart = (TH1D*)zFile->Get(name2.c_str());
	  zpart->SetLineWidth(2);
	  zpart->SetLineColor(kBlue);
	  zpart->Scale(LUMINOSITY);
	  zpart->Rebin(20);

	  TH1D* dypart = (TH1D*)dyFile->Get(name2.c_str());
	  dypart->SetLineWidth(2);
	  dypart->SetLineColor(kRed);
	  dypart->Scale(LUMINOSITY);
	  dypart->Rebin(20);

	  zpart->Draw();
	  if (nProp == 0)
	  {
	    zpart->GetXaxis()->SetRangeUser(0,10);
	  }
	  else
	  {
	    zpart->GetXaxis()->SetRangeUser(10,70);
	    if (nSample == 1 && nPar == 0)
	    {
	      zpart->GetYaxis()->SetRangeUser(0,17500);
	    }
	    else
	    {
	      zpart->GetYaxis()->SetRangeUser(0,maxes[nPar]);
	    }
	  }
	  zpart->GetXaxis()->SetTitle(xtitles[nProp].c_str());
	  zpart->GetYaxis()->SetTitle("Expected Events/2 GeV");
	  std::string header;
	  if (nSample == 0)
	  {
	    header = "ee ";
	  }
	  else
	  {
	    header = "#mu#mu ";
	  }
	  header += makeString(nPar) + " Parton";
	  TLatex* texhead = new TLatex();
	  texhead->SetTextSize(0.06);
	  texhead->SetNDC();
	  texhead->DrawLatex(0.44,0.91,header.c_str());

	  dypart->Draw("same");

	  TLegend* leg = new TLegend(0.6,0.66,0.8,0.76);
	  leg->SetTextSize(0.04);
	  leg->SetFillColor(0);
	  leg->SetBorderSize(0);
	  leg->AddEntry(zpart,"Z (m_{ll} > 40)","f");
	  leg->AddEntry(dypart,"Drell-Yan (10<m_{ll}<40)","f");
	  leg->Draw();

	  ATLASLabel(0.6,0.8,true,kBlack);

	  TLatex* lumtex = new TLatex();
	  lumtex->SetNDC();
	  lumtex->SetTextSize(0.03);
	  lumtex->SetTextColor(1);
	  lumtex->DrawLatex(0.65,0.6,("#int Ldt = " + makeString(LUMINOSITY) + " fb^{-1}").c_str());

	  std::string label2 = "macros/HWWplots/other/" + label + "_close.eps";
	  pad->Print(label2.c_str());
        }
      }
    } 


}//Closes main function
