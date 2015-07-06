#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2.h"
#include "TProfile.h"
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

TH1D* getAllHistos(TFile* file, std::string property, std::string channel, int phase)
{
  //First index corresponds to phase; second to channel (ee/em/mm)
  int book[7][3];
  book[0][0] = 0;
  book[1][0] = 1;
  book[2][0] = 2;
  book[3][0] = 3;
  book[4][0] = 6;
  book[5][0] = 7;
  book[6][0] = 8;

  book[0][1] = 0;
  book[1][1] = 1;
  book[2][1] = 2;
  book[3][1] = 3;
  book[4][1] = 8;
  book[5][1] = 9;
  book[6][1] = 10;

  book[0][2] = 0;
  book[1][2] = 1;
  book[2][2] = 2;
  book[3][2] = 3;
  book[4][2] = 12;
  book[5][2] = 13;
  book[6][2] = 14;

  TH1D* histo = (TH1D*)file->Get((property + "_" + channel + "0_" + makeString(book[phase][0])).c_str());
  histo->Add((TH1D*)file->Get((property + "_" + channel + "1_" + makeString(book[phase][1])).c_str()));
  histo->Add((TH1D*)file->Get((property + "_" + channel + "2_" + makeString(book[phase][2])).c_str()));

  return(histo);
}

///////////////////////////////////////////////////////////////////////////////////////
////         This macro makes pretty plots of MET versus assorted variables.       ////
////         Histograms are made by the ZControlClass.                             ////
///////////////////////////////////////////////////////////////////////////////////////

void plotHWWmet()
{
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);

//   TFile *HWW = new TFile("root-files/hww/h150_hwwstudy.root");
  TFile *DATA = new TFile("root-files/hww/data_hwwstudy.root");
  TFile *WW = new TFile("root-files/hww/smWW_hwwstudy.root");
  TFile* Z = new TFile("root-files/hww/smZ_hwwstudy.root");
  TFile* W = new TFile("root-files/hww/smW_hwwstudy.root");
  TFile* TTB = new TFile("root-files/hww/ttbar_hwwstudy.root");
  TFile* STOP = new TFile("root-files/hww/sTop_hwwstudy.root");
  TFile* EW = new TFile("root-files/hww/smEW_hwwstudy.root");
  TFile* TOTAL = new TFile("root-files/hww/totalBack_hwwstudy.root");

  const double lumin = 1.035;

  const Color_t H130COLOR = kRed;
  const Color_t WWCOLOR = 4;
  const Color_t ZCOLOR = 210;
  const Color_t WCOLOR = 227;
  const Color_t TTCOLOR = 219;
  const Color_t STCOLOR = kOrange - 3;
  const Color_t EWCOLOR = 222;

  const Style_t DATAMARKER = 8;
  const Size_t DATASIZE = 1.5;
  const Width_t DATALINEWIDTH = 2.5;
  const Width_t MCLINEWIDTH = 2;

  const Float_t LEGTEXTSIZE = 0.04;

  const Float_t AXISTITLESIZE = 0.06;

  std::vector<std::string> sources;
  sources.push_back("data");
  sources.push_back("smZ");

  std::vector<std::string> particles;
  particles.push_back("e");
  particles.push_back("m");

  std::vector<std::string> jetbins;
  jetbins.push_back("0");
  jetbins.push_back("1");
  jetbins.push_back("2");

  std::vector<std::string> properties;
  properties.push_back("Npv");
  properties.push_back("Mu");

  std::vector<std::string> xtitles;
  xtitles.push_back("N_{PV}");
  xtitles.push_back("#mu");

  for (int nPart = 0; nPart != (int)particles.size(); ++nPart)
  {
    for (int nJet = 0; nJet != (int)jetbins.size(); ++nJet)
    {
//       std::string label = particles[nPart] + "_" + jetbins[nJet] + "_NpvMet";

//       TH2D* data = (TH2D*)DATA->Get(label.c_str());
//       TH2D* mc = (TH2D*)TOTAL->Get(label.c_str());

//       int length = data->GetNbinsX();
//       double exes[length];
//       double exErr[length];
//       double fracD[length];
//       double errorD[length];
//       double fracMC[length];
//       double errorMC[length];

//       for (int i = 1; i <= data->GetNbinsX(); ++i)
//       {
// 	exes[i - 1] = i - 1;
// 	exErr[i - 1] = 0.0;

// 	double errDT;
// 	double dataT = data->IntegralAndError(i,i,41,100,errDT);
// 	double errDB;
// 	double dataB = data->IntegralAndError(i,i,1,100,errDB);

// 	double effD;
// 	double tempErr;
// 	if (dataB == 0 || dataT == 0)
// 	{
// 	  effD = 0;
// 	  tempErr = 0;
// 	}
// 	else
// 	{
// 	  effD = dataT/dataB;
// 	  tempErr = effD*sqrt((errDT/dataT)*(errDT/dataT) + (errDB/dataB)*(errDB/dataB));
// 	}
// 	fracD[i - 1] = effD;
// 	errorD[i - 1] = tempErr;
// // 	std::cout << i << " " << effD << " " << tempErr << std::endl;

// 	double errMCT;
// 	double mcT = mc->IntegralAndError(i,i,41,100,errMCT);
// 	double errMCB;
// 	double mcB = mc->IntegralAndError(i,i,1,100,errMCB);

// 	double effMC = mcT/mcB;
// 	fracMC[i - 1] = effMC;
// 	errorMC[i - 1] = effMC*sqrt((errMCT/mcT)*(errMCT/mcT) + (errMCB/mcB)*(errMCB/mcB));
//       }

// //       for (int k = 0; k != length; ++k)
// //       {
// // 	std::cout << exes[k] << " " << fracD[k] << " " << exErr[k] << " " << errorD[k] << std::endl; 
// //       }

//       TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,800,600);
//       pad->SetFillColor(0);
//       pad->SetTitle(label.c_str());
//       pad->SetTopMargin(0.1);

// //       data->Draw("colz");

//       TGraphErrors* graphD = new TGraphErrors(length,exes,fracD,exErr,errorD);
//       graphD->SetLineWidth(2);

//       graphD->GetXaxis()->SetTitle("N_{PV}");
//       graphD->GetYaxis()->SetTitle("Fraction of events with E_{T}^{miss} > 40");
//       graphD->Draw("ap");
//       graphD->GetYaxis()->SetRangeUser(0,0.02);
//       std::string header;
//       if (nPart == 0)
//       {
// 	header = "ee ";
//       }
//       else
//       {
// 	header = "#mu#mu ";
//       }
//       header += jetbins[nJet] + " Jet";
//       TLatex* texhead = new TLatex();
//       texhead->SetTextSize(0.06);
//       texhead->SetNDC();
//       texhead->DrawLatex(0.44,0.91,header.c_str());

//       TGraphErrors* graphMC = new TGraphErrors(length,exes,fracMC,exErr,errorMC);
//       graphMC->SetLineWidth(2);
//       graphMC->SetLineColor(kRed);
//       graphMC->SetMarkerColor(kRed);
//       graphMC->Draw("p same");

//       TLegend *leg = new TLegend(0.2,0.75,0.4,0.85);
//       leg->SetTextSize(LEGTEXTSIZE);
//       leg->SetFillColor(0);
//       leg->SetBorderSize(0);
//       leg->AddEntry(graphD,"2011 Data","p");
//       leg->AddEntry(graphMC,"Total Back.","p");
//       leg->Draw();

//       TLatex* lumtex = new TLatex();
//       lumtex->SetNDC();
//       lumtex->SetTextSize(0.04);
//       lumtex->SetTextColor(1);
//       lumtex->DrawLatex(0.5,0.78,("#int Ldt = " + makeString(LUMINOSITY) + " fb^{-1}").c_str());

//       std::string name = "macros/HWWplots/met/" + label + "_frac.eps";
//       pad->Print(name.c_str());

//       pad->Clear();

//       TH1D* dataL = data->ProjectionY("data1-3",2,4,"e");
//       dataL->SetMarkerColor(kBlack);
//       dataL->SetMarkerStyle(4);

//       TH1D* dataM = data->ProjectionY("data4-6",5,7,"e");
//       dataM->SetMarkerColor(kBlack);
//       dataM->SetMarkerStyle(25);

//       TH1D* dataH = data->ProjectionY("data7+",8,-1,"e");
//       dataH->SetMarkerColor(kBlack);
//       dataH->SetMarkerStyle(27);

//       TH1D* mcL = mc->ProjectionY("mc1-3",2,4,"e");
//       mcL->SetLineColor(kRed);
//       mcL->SetLineStyle(1);
//       mcL->Scale(LUMINOSITY);

//       TH1D* mcM = mc->ProjectionY("mc4-6",5,7,"e");
//       mcM->SetLineColor(kRed);
//       mcM->SetLineStyle(2);
//       mcM->Scale(LUMINOSITY);

//       TH1D* mcH = mc->ProjectionY("mc7+",8,-1,"e");
//       mcH->SetLineColor(kRed);
//       mcH->SetLineStyle(5);
//       mcH->Scale(LUMINOSITY);

//       std::vector<TH1D*> histos;
//       histos.push_back(dataL);
//       histos.push_back(dataM);
//       histos.push_back(dataH);
//       histos.push_back(mcL);
//       histos.push_back(mcM);
//       histos.push_back(mcH);

//       std::vector<std::string> notes;
//       notes.push_back("data");
//       notes.push_back("data");
//       notes.push_back("data");
//       notes.push_back("mc");
//       notes.push_back("mc");
//       notes.push_back("mc");


//       for (int i = 0; i != (int)histos.size(); ++i)
//       {
// 	for (int j = i + 1; j != (int)histos.size(); ++j)
// 	{
// 	  if (histos[i]->GetBinContent(histos[i]->GetMaximumBin()) < histos[j]->GetBinContent(histos[j]->GetMaximumBin()))
// 	  {
// 	    swap(histos[i],histos[j]);
// 	    swap(notes[i],notes[j]);
// 	  }
// 	}
//       }

//       for (int i = 0; i != (int)histos.size(); ++i)
//       {
// 	if (i == 0)
// 	{
// 	  histos[i]->GetXaxis()->SetTitle("E_{T}^{miss} [GeV]");
// 	  histos[i]->GetXaxis()->SetRangeUser(0,50);
// 	  histos[i]->GetYaxis()->SetTitle("Events/GeV");
// 	  if (notes[i] == "data")
// 	  {
// 	    histos[i]->Draw();
// 	  }
// 	  else
// 	  {
// 	    histos[i]->Draw("hist");
// 	  }
// 	}
// 	else
// 	{
// 	  if (notes[i] == "data")
// 	  {
// 	    histos[i]->Draw("same");
// 	  }
// 	  else
// 	  {
// 	    histos[i]->Draw("same hist");
// 	  }
// 	}
//       }

//       TLegend* leg2 = new TLegend(0.55,0.55,0.95,0.85);
//       leg2->SetTextSize(LEGTEXTSIZE);
//       leg2->SetFillColor(0);
//       leg2->SetBorderSize(0);
//       leg2->AddEntry(dataL,"Data (N_{PV} < 3)","p");
//       leg2->AddEntry(mcL,"Total Back. (N_{PV} < 3)","f");
//       leg2->AddEntry(dataM,"Data (3 < N_{PV} < 7)","p");
//       leg2->AddEntry(mcM,"Total Back. (3 < N_{PV} < 7)","f");
//       leg2->AddEntry(dataH,"Data (N_{PV} > 6)","p");
//       leg2->AddEntry(mcH,"Total Back. (N_{PV} > 6)","f");
//       leg2->Draw();

//       lumtex->DrawLatex(0.3,0.78,("#int Ldt = " + makeString(LUMINOSITY) + " fb^{-1}").c_str());
//       texhead->DrawLatex(0.44,0.91,header.c_str());

//       name = "macros/HWWplots/met/" + label + "_strip.eps";
//       pad->Print(name.c_str());

    }//closes loop over jet bins
  }//closes loop over particles/channels.  Call them what you will.

  /////////////////////////////////////////////////////////////////////////////////////
  //Now, having played around with Npv vs. Met plot, I'll do a little toying with the// 
  //mll vs Met one.                                                                  //
  /////////////////////////////////////////////////////////////////////////////////////

  for (int nPart = 0; nPart != (int)particles.size(); ++nPart)
  {
    for (int nJet = 0; nJet != (int)jetbins.size(); ++nJet)
    {
      std::string label = particles[nPart] + "_" + jetbins[nJet] + "_MllMet";

      TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,800,600);
      pad->SetFillColor(0);
      pad->SetTitle(label.c_str());
      pad->SetTopMargin(0.1);
//       pad->SetLogy(1);

      TH2D* Data = (TH2D*)DATA->Get(label.c_str());

      std::vector<TFile*> files;
      files.push_back(WW);
      files.push_back(W);
      files.push_back(TTB);
      files.push_back(STOP);
      files.push_back(EW);

      for (int nF = 0; nF != (int)files.size(); ++nF)
      {
	TH2D* temp = (TH2D*)files[nF]->Get(label.c_str());
	temp->Scale(LUMINOSITY);
	Data->Add(temp,-1);
      }

      TH1D* dataL = Data->ProjectionY((label + "_low").c_str(),1,50);
      dataL->SetMarkerColor(kAzure - 6);
      dataL->SetMarkerStyle(DATAMARKER);
      dataL->SetMarkerSize(DATASIZE);
      dataL->SetLineWidth(DATALINEWIDTH);
      
      TH1D* dataM = Data->ProjectionY((label + "_mid").c_str(),51,76);
      dataM->SetMarkerColor(kRed + 2);
      dataM->SetMarkerStyle(DATAMARKER);
      dataM->SetMarkerSize(DATASIZE);
      dataM->SetLineWidth(DATALINEWIDTH);

      TH1D* dataZ = Data->ProjectionY((label + "z").c_str(),77,106);
      dataZ->SetMarkerColor(kSpring - 7);
      dataZ->SetMarkerStyle(DATAMARKER);
      dataZ->SetMarkerSize(DATASIZE);
      dataZ->SetLineWidth(DATALINEWIDTH);

      TH1D* dataH = Data->ProjectionY((label + "_high").c_str(),107,200);
      dataH->SetMarkerColor(kOrange - 3);
      dataH->SetMarkerStyle(DATAMARKER);
      dataH->SetMarkerSize(DATASIZE);
      dataH->SetLineWidth(DATALINEWIDTH);

      TH2D* MC = (TH2D*)TOTAL->Get(label.c_str());

      TH1D* mcL = MC->ProjectionY((label + "_low2").c_str(),1,50);
      mcL->SetLineColor(kAzure - 6);
      mcL->SetLineWidth(2);
      mcL->Scale(LUMINOSITY);

      TH1D* mcM = MC->ProjectionY((label + "_mid2").c_str(),51,76);
      mcM->SetLineColor(kRed + 2);
      mcM->SetLineWidth(2);
      mcM->Scale(LUMINOSITY);

      TH1D* mcZ = MC->ProjectionY((label + "_z2").c_str(),77,106);
      mcZ->SetLineColor(kSpring - 7);
      mcZ->SetLineWidth(2);
      mcZ->Scale(LUMINOSITY);

      TH1D* mcH = MC->ProjectionY((label + "_high2").c_str(),107,200);
      mcH->SetLineColor(kOrange - 3);
      mcH->SetLineWidth(2);
      mcH->Scale(LUMINOSITY);

      //Code for making Ratio plot
      dataL->Divide(mcL);
      dataL->SetLineColor(kAzure - 6);
      dataM->Divide(mcM);
      dataM->SetLineColor(kRed + 2);
      dataZ->Divide(mcZ);
      dataZ->SetLineColor(kSpring - 7);
      dataH->Divide(mcH);
      dataH->SetLineColor(kOrange - 3);

      dataL->Rebin(5);
      dataM->Rebin(5);
      dataZ->Rebin(5);
      dataH->Rebin(5);

      std::vector<TH1D*> histos;
      histos.push_back(dataL);
      histos.push_back(dataM);
      histos.push_back(dataZ);
      histos.push_back(dataH);

      for (int i = 0; i != (int)histos.size(); ++i)
      {
	for (int j = i + 1; j != (int)histos.size(); ++j)
	{
	  if (histos[i]->GetBinContent(histos[i]->GetMaximumBin()) < histos[j]->GetBinContent(histos[j]->GetMaximumBin()))
	  {
	    swap(histos[i],histos[j]);
	  }
	}
      }

      for (int i = 0; i != (int)histos.size(); ++i)
      {
	if (i == 0)
	{
	  histos[i]->GetXaxis()->SetTitle("E_{T}^{miss} [GeV]");
	  histos[i]->GetXaxis()->SetRangeUser(0,100);
	  histos[i]->GetYaxis()->SetTitle("Ratio Z data/MC");
	  histos[i]->Draw("hist");
	}
	else
	{
	  histos[i]->Draw("same hist");
	}
      }

      //Code for making projection plot
//       std::vector<TH1D*> histos;
//       histos.push_back(dataL);
//       histos.push_back(dataM);
//       histos.push_back(dataZ);
//       histos.push_back(dataH);
//       histos.push_back(mcL);
//       histos.push_back(mcM);
//       histos.push_back(mcZ);
//       histos.push_back(mcH);

//       std::vector<std::string> notes;
//       notes.push_back("data");
//       notes.push_back("data");
//       notes.push_back("data");
//       notes.push_back("data");
//       notes.push_back("mc");
//       notes.push_back("mc");
//       notes.push_back("mc");
//       notes.push_back("mc");

//       for (int i = 0; i != (int)histos.size(); ++i)
//       {
// 	for (int j = i + 1; j != (int)histos.size(); ++j)
// 	{
// 	  if (histos[i]->GetBinContent(histos[i]->GetMaximumBin()) < histos[j]->GetBinContent(histos[j]->GetMaximumBin()))
// 	  {
// 	    swap(histos[i],histos[j]);
//  	    swap(notes[i],notes[j]);
// 	  }
// 	}
//       }
//       for (int i = 0; i != (int)histos.size(); ++i)
//       {
// 	if (i == 0)
// 	{
// 	  histos[i]->GetXaxis()->SetTitle("E_{T}^{miss} [GeV]");
// 	  histos[i]->GetXaxis()->SetRangeUser(20,80);
// 	  histos[i]->GetYaxis()->SetTitle("Events/GeV");
// 	  if (notes[i] == "data")
// 	  {
// 	    histos[i]->Draw();
// 	  }
// 	  else
// 	  {
// 	    histos[i]->Draw("hist");
// 	  }
// 	}
// 	else
// 	{
// 	  if (notes[i] == "data")
// 	  {
// 	    histos[i]->Draw("same");
// 	  }
// 	  else
// 	  {
// 	    histos[i]->Draw("same hist");
// 	  }
// 	}
//       }



      std::string header;
      if (nPart == 0)
      {
	header = "ee ";
      }
      else
      {
	header = "#mu#mu ";
      }
      header += jetbins[nJet] + " Jet";
      TLatex* texhead = new TLatex();
      texhead->SetTextSize(0.06);
      texhead->SetNDC();
      texhead->DrawLatex(0.44,0.91,header.c_str());

      TLegend *leg = new TLegend(0.55,0.65,0.75,0.85);
      leg->SetTextSize(LEGTEXTSIZE);
      leg->SetFillColor(0);
      leg->SetBorderSize(0);
//       leg->AddEntry(dataL,"Z Data (mll < 50)","p");
//       leg->AddEntry(mcL,"Z MC (mll < 50)","f");
//       leg->AddEntry(dataM,"Z Data (50<mll<76)","p");
//       leg->AddEntry(mcM,"Z MC (50<mll<76)","f");
//       leg->AddEntry(dataZ,"Z Data (76<mll<106)","p");
//       leg->AddEntry(mcZ,"Z MC (76<mll<106)","f");
//       leg->AddEntry(dataH,"Z Data (106 < mll)","p");
//       leg->AddEntry(mcH,"Z MC (106 < mll)","f");

      leg->AddEntry(dataL,"mll < 50","f");
      leg->AddEntry(dataM,"50 < mll < 76","f");
      leg->AddEntry(dataZ,"76 < mll < 106","f");
      leg->AddEntry(dataH,"106 < mll","f");

      leg->Draw();

      std::string name = "macros/HWWplots/zcontrol/" + label + "_projsratio.eps";
      pad->Print(name.c_str());
    }//closes loop over jet bins, again.
  }//closes loop over particles/channels, again.

///////////////////////////////////////////////////////////////////////////////////////
////          . . . And this part of the macro makes pretty 1D plots of MET        ////
////         variables.  Histograms are made by the HHistoClass.                   ////
///////////////////////////////////////////////////////////////////////////////////////

    //Okay, so write a bit ol' nested loop (like always), but now have one iteration
    //correspond to plot . . . type.  As in, compare data to stacked monte carlo with
    //higgs shape for comparison normal scale, and then the same log scale, and then
    //everything normalized to one for shape comparison.

    properties.clear();
    properties.push_back("met");
    properties.push_back("meteta");
    properties.push_back("metphi");
    properties.push_back("dpmetl");
    properties.push_back("dpmetj");

    xtitles.clear();
    xtitles.push_back("E_{T,Rel}^{miss} [GeV]");
    xtitles.push_back("#eta_{E_{T}^{miss}}");
    xtitles.push_back("#phi_{E_{T}^{miss}}");
    xtitles.push_back("#Delta#phi_{E_{T}^{miss},l}");
    xtitles.push_back("#Delta#phi_{E_{T}^{miss},j}");

    particles.clear();
    particles.push_back("ee");
    particles.push_back("em");
    particles.push_back("mm");

    std::vector<int> rebins;
    rebins.push_back(5);
    rebins.push_back(1);
    rebins.push_back(2);
    rebins.push_back(1);
    rebins.push_back(1);

//     for (int nProp = 0; nProp != (int)properties.size(); ++nProp)
//     {
//       for (int nChan = 0; nChan != (int)particles.size(); ++nChan)
//       {
// 	for (int nPh = 0; nPh != 7; ++nPh)
// 	{
// 	  for (int nType = 0; nType != 3; ++nType)
// 	  {
// 	    if (nPh > 3) continue;
// 	    if (nProp == 1) continue;
// 	    if (nProp > 2) continue;
// 	    if (nProp == 2 && nType != 0) continue;
// 	    if (nType > 1) continue;

// 	    std::string label = properties[nProp] + "_" + particles[nChan] + "_" + makeString(nPh);
// 	    std::string label2;
// 	    if (nType == 0)
// 	    {
// 	      label2 = label + "_norm";
// 	    }
// 	    else if (nType == 1)
// 	    {
// 	      label2 = label + "_log";
// 	    }
// 	    else if (nType == 2)
// 	    {
// 	      label2 = label + "_unit";
// 	    }

// 	    TCanvas* pad = new TCanvas(label2.c_str(),label2.c_str(),14,33,800,600);
// 	    pad->SetFillColor(0);
// 	    pad->SetTitle(label2.c_str());
// 	    if (nType == 1)
// 	    {
// 	      pad->SetLogy(1);
// 	    }

//             TH1D* data = getAllHistos(DATA,properties[nProp],particles[nChan],nPh);
//             data->SetMarkerStyle(DATAMARKER);
//             data->SetMarkerSize(DATASIZE);
//             data->SetLineWidth(DATALINEWIDTH);
//             data->SetTitle("");

// //             TH1D* hww = getAllHistos(HWW,properties[nProp],particles[nChan],nPh);
// //             hww->SetTitle("");
// //             hww->SetLineColor(H130COLOR);
// //             hww->SetLineWidth(MCLINEWIDTH);
// // 	    hww->Scale(lumin);

//             TH1D* smz = getAllHistos(Z,properties[nProp],particles[nChan],nPh);
//             smz->SetTitle("");
//             smz->SetFillColor(ZCOLOR);
//             smz->SetLineWidth(MCLINEWIDTH);
// 	    smz->Scale(lumin);

//             TH1D* smww = getAllHistos(WW,properties[nProp],particles[nChan],nPh);
//             smww->SetTitle("");
//             smww->SetFillColor(WWCOLOR);
//             smww->SetLineWidth(MCLINEWIDTH);
// 	    smww->Scale(lumin);

//             TH1D* smw = getAllHistos(W,properties[nProp],particles[nChan],nPh);
//             smw->SetTitle("");
//             smw->SetFillColor(WCOLOR);
//             smw->SetLineWidth(MCLINEWIDTH);
// 	    smw->Scale(lumin);

//             TH1D* ew = getAllHistos(EW,properties[nProp],particles[nChan],nPh);
//             ew->SetTitle("");
//             ew->SetFillColor(EWCOLOR);
//             ew->SetLineWidth(MCLINEWIDTH);
// 	    ew->Scale(lumin);

// 	    TH1D* ttb = getAllHistos(TTB,properties[nProp],particles[nChan],nPh);
// 	    ttb->SetTitle("");
// 	    ttb->SetFillColor(TTCOLOR);
// 	    ttb->SetLineWidth(MCLINEWIDTH);
// 	    ttb->Scale(lumin);	    

// 	    TH1D* stop = getAllHistos(STOP,properties[nProp],particles[nChan],nPh);
// 	    stop->SetTitle("");  
// 	    stop->SetFillColor(STCOLOR);
// 	    stop->SetLineWidth(MCLINEWIDTH);
// 	    stop->Scale(lumin);

// 	    if (nType == 0)
// 	    {
// 	      data->Rebin(rebins[nProp]);
// 	      smz->Rebin(rebins[nProp]);
// 	      ew->Rebin(rebins[nProp]);
// 	      smw->Rebin(rebins[nProp]);
// 	      smww->Rebin(rebins[nProp]);
// 	      ttb->Rebin(rebins[nProp]);
// // 	      hww->Rebin(rebins[nProp]);
// 	      stop->Rebin(rebins[nProp]);
// 	    }

// 	    THStack* stack = new THStack(label2.c_str(),label2.c_str());
// 	    stack->SetTitle("");
// 	    stack->Add(ew);
// 	    stack->Add(smw);
// 	    stack->Add(stop);
// 	    stack->Add(ttb);
// 	    stack->Add(smz);
// 	    stack->Add(smww);

// 	    double max = data->GetBinContent(data->GetMaximumBin());
// 	    if (max + sqrt(max) > stack->GetMaximum())
// 	    {
// 	      data->Draw("P E1 X0");
// 	      data->SetMinimum(0.1);
// 	      data->GetXaxis()->SetTitle(xtitles[nProp].c_str());

// 	      if (nProp == 2)
// 	      {
// 		data->SetMaximum(1.5*max);
// 	      }

// 	      stack->Draw("same");
// // 	      hww->Draw("same");
// 	      data->Draw("P E1 X0 same");
// 	    }
// 	    else
// 	    {
// 	      stack->Draw();
// 	      stack->SetMinimum(0.1);
// 	      stack->GetXaxis()->SetTitle(xtitles[nProp].c_str());

// 	      if (nProp == 2)
// 	      {
// 		stack->SetMaximum(1.5*stack->GetMaximum());
// 	      }

// // 	      hww->Draw("same");
// 	      data->Draw("P E1 X0 same");
// 	    }

// 	    TLegend *leg;
// 	    leg = new TLegend(0.65,0.66,0.85,0.92);
// 	    leg->SetTextSize(LEGTEXTSIZE);
// 	    leg->SetFillColor(0);
// 	    leg->SetBorderSize(0);
// 	    std::string dataTag;
// 	    if (nChan == 0)
// 	      {
// 		dataTag = "2011 ee Data";
// 	      }
// 	    else if (nChan == 1)
// 	      {
// 		dataTag = "2011 e#mu Data";
// 	      }
// 	    else
// 	      {
// 		dataTag = "2011 #mu#mu Data";
// 	      }
// 	    leg->AddEntry(data,dataTag.c_str(),"p");
// // 	    leg->AddEntry(hww,"150 H#rightarrowWW","f");
// 	    leg->AddEntry(smww,"WW#rightarrowl#nul#nu","f");
// 	    leg->AddEntry(smz,"Z#rightarrowll","f");
//             leg->AddEntry(smw,"W#rightarrowl#nu","f");
// 	    leg->AddEntry(ew,"Diboson","f");
// 	    leg->AddEntry(ttb,"TTbar","f");
// 	    leg->AddEntry(stop,"Single Top","f");
// 	    leg->Draw();

// 	    std::string name = "macros/HWWplots/met/" + label2 + ".eps";
// 	    pad->Print(name.c_str());
// 	  }//Closes loop over plot types
// 	}//Closes loop over phases of the cutflow
//       }//Closes loop over channels/particles
//     }//Closes loop over properties

///////////////////////////////////////////////////////////////////////////////////////
////          . . . And this part of the macro makes pretty 1D plots of Npv        ////
////         variables.  Histograms are made by the ZControlClass.                 ////
///////////////////////////////////////////////////////////////////////////////////////

  particles.clear();
  particles.push_back("e");
  particles.push_back("m");

  properties.clear();
  properties.push_back("npv");
  properties.push_back("lpv");
  properties.push_back("hpv");

  xtitles.clear();
  xtitles.push_back("N_{PV}");
  xtitles.push_back("E_{T}^{miss}(N_{PV} < 6) [GeV]");
  xtitles.push_back("E_{T}^{miss}(N_{PV} >= 6) [GeV]");

  for (int nPart = 0; nPart != (int)particles.size(); ++nPart)
  {
    for (int nJet = 0; nJet != (int)jetbins.size(); ++nJet)
    {
      for (int nProp = 0; nProp != (int)properties.size(); ++nProp)
      {
// 	std::string label = particles[nPart] + "_" + jetbins[nJet] + "_" + properties[nProp];

// 	TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,800,600);
// 	pad->SetFillColor(0);
// 	pad->SetTitle(label.c_str());
// 	pad->SetTopMargin(0.1);

// 	TH1D* data = (TH1D*)DATA->Get(label.c_str());
// 	data->SetMarkerStyle(DATAMARKER);
// 	data->SetMarkerSize(DATASIZE);
// 	data->SetLineWidth(DATALINEWIDTH);
// 	data->SetTitle("");
	
// 	TH1D* smz = (TH1D*)Z->Get(label.c_str());
// 	smz->SetTitle("");
// 	smz->SetFillColor(ZCOLOR);
// 	smz->SetLineWidth(MCLINEWIDTH);
// 	smz->Scale(lumin);
	
// 	THStack* stack = new THStack(label.c_str(),label.c_str());
// 	stack->SetTitle("");
// 	stack->Add(smz);
	  
// 	double max = data->GetBinContent(data->GetMaximumBin());
// 	if (max + sqrt(max) > stack->GetMaximum())
// 	{
// 	  data->Draw("P E1 X0");
// 	  data->SetMinimum(0.1);
// 	  data->GetXaxis()->SetTitle(xtitles[nProp].c_str());

// 	  stack->Draw("hist same");
// // 	      hww->Draw("same");
// 	  data->Draw("P E1 X0 same");
// 	}
// 	else
// 	{
// 	  stack->Draw("hist");
// 	  stack->SetMinimum(0.1);
// 	  stack->GetXaxis()->SetTitle(xtitles[nProp].c_str());

// // 	      hww->Draw("same");
// 	  data->Draw("P E1 X0 same");
// 	}

// 	TLegend *leg;
// 	leg = new TLegend(0.65,0.74,0.85,0.89);
// 	leg->SetTextSize(LEGTEXTSIZE);
// 	leg->SetFillColor(0);
// 	leg->SetBorderSize(0);
// 	std::string dataTag;
// 	if (nPart == 0)
// 	{
// 	  dataTag = "2011 ee Data";
// 	}
// 	else if (nPart == 1)
// 	{
// 	  dataTag = "2011 #mu#mu Data";
// 	}
// 	leg->AddEntry(data,dataTag.c_str(),"p");
// 	leg->AddEntry(smz,"Z#rightarrowll","f");
// 	leg->Draw();
// 	if (nJet == 0)
// 	{
//           TLatex* header = new TLatex();
//           header->SetTextSize(0.06);
//           header->SetNDC();
// 	  header->DrawLatex(0.44,0.91,"0 Jet");
// 	}
// 	else if (nJet == 1)
// 	{
//           TLatex* header = new TLatex();
//           header->SetTextSize(0.06);
//           header->SetNDC();
// 	  header->DrawLatex(0.44,0.91,"1 Jet");
// 	}
// 	else if (nJet == 2)
// 	{
//           TLatex* header = new TLatex();
//           header->SetTextSize(0.06);
//           header->SetNDC();
// 	  header->DrawLatex(0.44,0.91,"2 Jet");
// 	}


// 	std::string name = "macros/HWWplots/met/" + label + ".eps";
// 	pad->Print(name.c_str());

      }//closes loop over properties
    }//closes loop over jetbins
  }//closes loop over particles

}//closes main function
