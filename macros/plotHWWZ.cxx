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
#include "atlasstyle-00-03-03/AtlasLabels.C"

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

void plotHWWZ()
{

  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);

  const double lumin = 1.035;

//   const Color_t H130COLOR = kRed;
//   const Color_t WWCOLOR = 4;
//   const Color_t ZCOLOR = 210;
//   const Color_t WCOLOR = 227;
//   const Color_t TOPCOLOR = 219;
//   const Color_t WZCOLOR = 222;

//   const Style_t DATAMARKER = 8;
//   const Size_t DATASIZE = 1.5;
//   const Width_t DATALINEWIDTH = 2.5;
//   const Width_t MCLINEWIDTH = 2;

//   const Float_t LEGTEXTSIZE = 0.04;

  TFile* file2 = new TFile("root-files/hww/data_hwwstudy.root");
  TFile* file1 = new TFile("root-files/hww/smZ_hwwstudy.root");

  std::vector<TFile*> sources;
  sources.push_back(file2);
  sources.push_back(file1);

  TFile* WW = new TFile("root-files/hww/smWW_hwwstudy.root");
  TFile* W = new TFile("root-files/hww/smW_hwwstudy.root");
  TFile* TT = new TFile("root-files/hww/ttbar_hwwstudy.root");
  TFile* STOP = new TFile("root-files/hww/sTop_hwwstudy.root");
  TFile* EW = new TFile("root-files/hww/smEW_hwwstudy.root"); 

  std::vector<std::string> particles;
  particles.push_back("e");
  particles.push_back("m");

  std::vector<std::string> njets;
  njets.push_back("0");
  njets.push_back("1");
  njets.push_back("2");

  //Make a nice text file to store output.
  std::vector<std::string> zeroNames;
  zeroNames.push_back("Nominal Factor");
  zeroNames.push_back("p_{T}^{ll} > 30");
  zeroNames.push_back("#Delta#phi < 1.3");
  zeroNames.push_back("#Delta#phi < 1.8");

  std::vector<std::string> oneNames;
  oneNames.push_back("Nominal Factor");
  oneNames.push_back("Passes B-tag");
  oneNames.push_back("p_{T}^{tot} < 30");
  oneNames.push_back("Z#rightarrow#tau#tau veto");
  oneNames.push_back("#Delta#phi < 1.3");
  oneNames.push_back("#Delta#phi < 1.8");

  std::vector<std::string> twoNames;
  twoNames.push_back("Nominal Factor");
  twoNames.push_back("eta1*eta2 < 0");
  twoNames.push_back("delta_eta > 3.8");
  twoNames.push_back("mjj > 500");
  twoNames.push_back("no third central jet");
  twoNames.push_back("Passes B-tag");
  twoNames.push_back("pT_tot < 30");
  twoNames.push_back("Ztautau veto");
  twoNames.push_back("#Delta#phi < 1.3");
  twoNames.push_back("#Delta#phi < 1.8");

  std::string filename = "macros/textfiles/zcontrol.txt";
  std::ofstream outfile;
  outfile.open(filename.c_str());

//   for (int nP = 0; nP != (int)particles.size(); ++nP)
//   {
//     if (nP == 0) continue;
    for (int nJ = 0; nJ != (int)njets.size(); ++nJ)
    {
      if (nJ == 2) continue;
      //make arrays of doubles to hold ratios and errors
      int length = 0;
      if (nJ == 0)
      {
	length = (int)zeroNames.size();
      }
      else if (nJ == 1)
      {
	length = (int)oneNames.size();
      }
      else
      {
	length = (int)twoNames.size();
      }
      double* dataRatio = new double[length];
      double* dataErr = new double[length];
      double* mcRatio = new double[length];
      double* mcErr = new double[length];

      for (int k = 0; k != length; ++k)
      {
	dataRatio[k] = 0.0;
	dataErr[k] = 0.0;
	mcRatio[k] = 0.0;
	mcErr[k] = 0.0;
      }

      for (int nSource = 0; nSource != (int)sources.size(); ++nSource)
      {
        TFile* file = sources[nSource];

	if (nSource == 0)
	{
	  outfile << "Data (with MC correction)" << std::endl;
	}
	else
	{
	  outfile << "Monte Carlo Z & DY" << std::endl;
	}
//         if (nP == 0)
//         {
//           outfile << "Electrons" << std::endl << std::endl;
//         }
//         else
//         {
//           outfile << "Muons" << std::endl << std::endl;
//         }

//       if (nP == 0) continue;
        //First, make pretty 2D histogram
//         std::string label = particles[nP] + "_" + njets[nJ] + "_MllMet";
        std::string label = "combem_" + njets[nJ] + "_MllMet";
	std::string label2 = label;
	if (nSource == 0)
	{
	  label2 += "_data";
	}
	else
	{
	  label2 += "_mc";
	}

        TCanvas* pad = new TCanvas(label2.c_str(),label2.c_str(),14,33,800,600);
        pad->SetFillColor(0);
        pad->SetTitle(label2.c_str());
        pad->SetRightMargin(0.2);
	pad->SetTopMargin(0.1);

      if (nSource == 1)
      {
	pad->SetLogz();
      }

//       TH2D* plot = (TH2D*)file->Get(label.c_str());
      TH2D* plot = (TH2D*)file->Get(("e_"+njets[nJ]+"_MllMet").c_str());
        if (plot == 0)
        {
	  std::cout << "couldn't get histogram from file" << std::endl;
        }
	plot->Add((TH2D*)file->Get(("m_"+njets[nJ]+"_MllMet").c_str()));

        plot->SetTitle("");
        if (nSource == 1)
        {
// 	  plot->Scale(LUMINOSITY);
	  double total = plot->Integral();
	  plot->Scale(1/total);
        }
        plot->GetYaxis()->SetTitle("E_{T,Rel}^{miss} [GeV]");
	plot->GetZaxis()->SetTitleOffset(1.2);
	if (nSource == 1)
	{
	  plot->GetZaxis()->SetRangeUser(0.00001,0.01);
	}
        std::string xtitle = "m_{ll} [GeV]";
//         if (nP == 0)
//         {
// 	  xtitle = "m_{ee} [GeV]";
//          }
//         else
//         {
// 	  xtitle = "m_{#mu#mu} [GeV]";
//         }
        plot->GetXaxis()->SetTitle(xtitle.c_str());
        if (nSource == 0)
        {
          plot->GetZaxis()->SetTitle("Data Events");
        }
        else
        {
	  plot->GetZaxis()->SetTitle("Probability");
        }

        plot->Draw("colz");
        plot->GetXaxis()->SetRangeUser(-10,200);
        plot->GetYaxis()->SetRangeUser(0,80);
	if (nJ == 0)
	{
          TLatex* header = new TLatex();
          header->SetTextSize(0.06);
          header->SetNDC();
	  header->DrawLatex(0.44,0.91,"0 Jet");
	}
	else if (nJ == 1)
	{
          TLatex* header = new TLatex();
          header->SetTextSize(0.06);
          header->SetNDC();
	  header->DrawLatex(0.44,0.91,"1 Jet");
	}
	else if (nJ == 2)
	{
          TLatex* header = new TLatex();
          header->SetTextSize(0.06);
          header->SetNDC();
	  header->DrawLatex(0.44,0.91,"2 Jet");
	}

        //Add pretty lines to plot
	//To extend line across plot, go from 0 to 200
//         TLine* hline1 = new TLine(ZMASS - ZWINDOW,METCUTSF,ZMASS + ZWINDOW,METCUTSF);
//         hline1->SetLineColor(kRed + 1);
// 	hline1->SetLineStyle(1);
//         hline1->SetLineWidth(3);
//         hline1->Draw();

//         TLine* hline2 = new TLine(MLLFLOORSF,METCUTSF,50,METCUTSF);
//         hline2->SetLineColor(kRed + 1);
// 	hline2->SetLineStyle(1);
//         hline2->SetLineWidth(3);
//         hline2->Draw();

//         TLine* hline3 = new TLine(ZMASS - ZWINDOW,30,ZMASS + ZWINDOW,30);
//         hline3->SetLineColor(kRed + 1);
// 	hline3->SetLineStyle(1);
//         hline3->SetLineWidth(3);
//         hline3->Draw();

//         TLine* hline4 = new TLine(MLLFLOORSF,30,50,30);
//         hline4->SetLineColor(kRed + 1);
// 	hline4->SetLineStyle(1);
//         hline4->SetLineWidth(3);
//         hline4->Draw();

//         TLine* vline1 = new TLine(ZMASS - ZWINDOW,30,ZMASS - ZWINDOW,81);
//         vline1->SetLineColor(kRed + 1);
// 	vline1->SetLineStyle(1);
//         vline1->SetLineWidth(3);
//         vline1->Draw();

//         TLine* vline2 = new TLine(ZMASS + ZWINDOW,30,ZMASS + ZWINDOW,81);
//         vline2->SetLineColor(kRed + 1);
// 	vline2->SetLineStyle(1);
//         vline2->SetLineWidth(3);
//         vline2->Draw();

//         TLine* vline3 = new TLine(MLLFLOORSF,30,MLLFLOORSF,81);
//         vline3->SetLineColor(kRed + 1);
// 	vline3->SetLineStyle(1);
//         vline3->SetLineWidth(3);
//         vline3->Draw();

//         TLine* vline4 = new TLine(50,30,50,81);
//         vline4->SetLineColor(kRed + 1);
// 	vline4->SetLineStyle(1);
//         vline4->SetLineWidth(3);
//         vline4->Draw();

        //Add region labels
        TLatex* atex = new TLatex();
        atex->SetNDC();
        atex->SetTextSize(0.07);
        atex->SetTextColor(kBlack);
        atex->DrawLatex(0.25,0.6,"A");

        TLatex* ctex = new TLatex();
        ctex->SetNDC();
        ctex->SetTextSize(0.07);
        ctex->SetTextColor(kBlack);
        ctex->DrawLatex(0.44,0.6,"C");

        TLatex* btex = new TLatex();
        btex->SetNDC();
        btex->SetTextSize(0.07);
        btex->SetTextColor(kBlack);
        btex->DrawLatex(0.25,0.35,"B");

        TLatex* dtex = new TLatex();
        dtex->SetNDC();
        dtex->SetTextSize(0.07);
        dtex->SetTextColor(kBlack);
        dtex->DrawLatex(0.44,0.35,"D");

	TLatex* etex = new TLatex();
	etex->SetNDC();
	etex->SetTextSize(0.07);
	etex->SetTextColor(kBlack);
	etex->DrawLatex(0.62,0.6,"E");

	TLatex* ftex = new TLatex();
	ftex->SetNDC();
	ftex->SetTextSize(0.07);
	ftex->SetTextColor(kBlack);
	ftex->DrawLatex(0.62,0.35,"F");

	TLine* vline1 = new TLine(ZMASS - ZWINDOW,0,ZMASS - ZWINDOW,81);
	vline1->SetLineColor(kRed + 1);
	vline1->SetLineWidth(3);
	vline1->Draw();

	TLine* vline2 = new TLine(ZMASS + ZWINDOW,0,ZMASS + ZWINDOW,81);
	vline2->SetLineColor(kRed + 1);
	vline2->SetLineWidth(3);
	vline2->Draw();

	TLine* hline = new TLine(0,METCUTSF,200,METCUTSF);
	hline->SetLineColor(kRed + 1);
	hline->SetLineWidth(3);
	hline->Draw();

        if (nSource == 0)
        {
	  label = label + "_data";
        }
        else
        {
	  label = label + "_mc";
        }

	  ATLASLabel(0.5,0.85,true,kBlack);

	  TLatex* simtex = new TLatex();
	  simtex->SetNDC();
	  simtex->SetTextSize(0.04);
	  simtex->SetTextColor(1);
	  if (nSource == 1)
	  {
	    simtex->DrawLatex(0.57, 0.8, "(simulation)");
	  }

	  if (nSource == 0)
	  {
	    TLatex* lumtex = new TLatex();
	    lumtex->SetNDC();
	    lumtex->SetTextSize(0.03);
	    lumtex->SetTextColor(1);
	    lumtex->DrawLatex(0.55,0.8,("#int Ldt = " + makeString(LUMINOSITY) + " fb^{-1}").c_str());
	  }

        std::string name = "macros/HWWplots/zcontrol/" + label + "_abcdef.png";
        pad->Print(name.c_str());
     
//         //Then, calculate rato.  And, the error on the ratio.
//         TH1D* num = (TH1D*)file->Get((particles[nP] + "_" + njets[nJ] + "_breg").c_str());
//         TH1D* den = (TH1D*)file->Get((particles[nP] + "_" + njets[nJ] + "_dreg").c_str());

//         TH1D* wwnum = (TH1D*)WW->Get((particles[nP] + "_" + njets[nJ] + "_breg").c_str());
//         wwnum->Scale(LUMINOSITY);
//         TH1D* wwden = (TH1D*)WW->Get((particles[nP] + "_" + njets[nJ] + "_dreg").c_str());
//         wwden->Scale(LUMINOSITY);
//         TH1D* wnum = (TH1D*)W->Get((particles[nP] + "_" + njets[nJ] + "_breg").c_str());
//         wnum->Scale(LUMINOSITY);
//         TH1D* wden = (TH1D*)W->Get((particles[nP] + "_" + njets[nJ] + "_dreg").c_str());
//         wden->Scale(LUMINOSITY);
//         TH1D* tnum = (TH1D*)TT->Get((particles[nP] + "_" + njets[nJ] + "_breg").c_str());
//         tnum->Scale(LUMINOSITY);
//         TH1D* tden = (TH1D*)TT->Get((particles[nP] + "_" + njets[nJ] + "_dreg").c_str());
//         tden->Scale(LUMINOSITY);
//         TH1D* stnum = (TH1D*)STOP->Get((particles[nP] + "_" + njets[nJ] + "_breg").c_str());
//         stnum->Scale(LUMINOSITY);
//         TH1D* stden = (TH1D*)STOP->Get((particles[nP] + "_" + njets[nJ] + "_dreg").c_str());
//         stden->Scale(LUMINOSITY);
// 	TH1D* ewnum = (TH1D*)EW->Get((particles[nP] + "_" + njets[nJ] + "_breg").c_str());
// 	ewnum->Scale(LUMINOSITY);
// 	TH1D* ewden = (TH1D*)EW->Get((particles[nP] + "_" + njets[nJ] + "_dreg").c_str());
// 	ewden->Scale(LUMINOSITY);

//         outfile << nJ << " Jet Bin" << std::endl;
//         for (int i = 1; i <= length; ++i)
//         {
//           double errB = 0;
//           double intB = num->IntegralAndError(i,i,errB);

//           double errD = 0;
//           double intD = den->IntegralAndError(i,i,errD);

// 	  double scale = 1.0;
// 	  if (nSource == 1)
// 	  {
// 	    scale = LUMINOSITY;
// 	  }
// 	  outfile << "In region B: " << intB*scale << " and in region D: " << intD*scale << std::endl;

// 	  if (nSource == 0)
// 	  {
// 	    double wwberr = 0;
// 	    double wwbint = wwnum->IntegralAndError(i,i,wwberr);
// 	    double wwderr = 0;
// 	    double wwdint = wwden->IntegralAndError(i,i,wwderr);
// 	    outfile << "WW in region B: " << wwbint << " and in region D: " << wwdint << std::endl;

// 	    double wberr = 0;
// 	    double wbint = wnum->IntegralAndError(i,i,wberr);
// 	    double wderr = 0;
// 	    double wdint = wden->IntegralAndError(i,i,wderr);
// 	    outfile << "W in region B: " << wbint << " and in region D: " << wdint << std::endl;

// 	    double tberr = 0;
// 	    double tbint = tnum->IntegralAndError(i,i,tberr);
// 	    double tderr = 0;
// 	    double tdint = tden->IntegralAndError(i,i,tderr);
// 	    outfile << "TT in region B: " << tbint << " and in region D: " << tdint << std::endl;

// 	    double stberr = 0;
// 	    double stbint = stnum->IntegralAndError(i,i,stberr);
// 	    double stderr = 0;
// 	    double stdint = stden->IntegralAndError(i,i,stderr);
// 	    outfile << "singleT in region B: " << stbint << " and in region D: " << stdint << std::endl;

// 	    double ewberr = 0;
// 	    double ewbint = ewnum->IntegralAndError(i,i,ewberr);
// 	    double ewderr = 0;
// 	    double ewdint = ewden->IntegralAndError(i,i,ewderr);
// 	    outfile << "EW in region B: " << ewbint << " and in region D: " << ewdint << std::endl;

// // 	    intB = intB - wwbint - tbint - stbint;
// // 	    intD = intD - wwdint - tdint - stbint;

// 	    intB = intB - wwbint - tbint - stbint - ewbint - wbint;
// 	    intD = intD - wwdint - tdint - stbint - ewdint - wdint;

// // 	    intB = intB - tbint;
// // 	    intD = intD - tdint;

// // 	    errB = sqrt(errB*errB + wwberr*wwberr + tberr*tberr + stberr*stberr);
// // 	    errD = sqrt(errD*errD + wwderr*wwderr + tderr*tderr + stderr*stderr);

// 	    errB = sqrt(errB*errB + wwberr*wwberr + tberr*tberr + ewberr*ewberr + wberr*wberr + stberr*stberr);
// 	    errD = sqrt(errD*errD + wwderr*wwderr + tderr*tderr + ewderr*ewderr + wderr*wderr + stderr*stderr);

// // 	    errB = sqrt(errB*errB + tberr*tberr);
// // 	    errD = sqrt(errD*errD + tderr*tderr);
// 	  }

// 	  std::string label;
// 	  if (nJ == 0 && (i-1) < (int)zeroNames.size())
// 	  {
// 	    label = zeroNames[i - 1];
// 	  }
// 	  else if (nJ == 1 && (i - 1) < (int)oneNames.size())
// 	  {
// 	    label = oneNames[i - 1];
// 	  }
// 	  else if ((i - 1) < (int)twoNames.size())
// 	  {
// 	    label = twoNames[i - 1];
// 	  }

//           if ((intD + intB )!= 0)
//           {
//             double eff = intB/(intB + intD);
//             double error = sqrt(eff*eff/(intB*intB)*(errB*errB*(1-eff)*(1-eff) + errD*errD*eff*eff));

// 	    if (nSource == 0)
// 	    {
// 	      dataRatio[i-1] = eff;
// 	      dataErr[i-1] = error;
// 	    }
// 	    else
// 	    {
// 	      mcRatio[i-1] = eff;
// 	      mcErr[i-1] = error;
// 	    }
// 	    outfile << label << ": " << eff << " +/- " << error << std::endl;
//           }
// 	  else
// 	  {
// 	    outfile << label << ": B region = " << intB << " D region = " << intD << std::endl;
// 	  }

//         }
//         outfile << std::endl;
      }//Closes loop over source files

//       std::string name = "system_" + makeString(nJ) + "jet";
//       if (nP == 0)
//       {
// 	name += "_elec";
//       }
//       else
//       {
// 	name += "_muon";
//       }
//       TCanvas* Pad = new TCanvas(name.c_str(),name.c_str(),14,33,800,600);
//       Pad->SetFillColor(0);
//       Pad->SetTitle(name.c_str());
//       Pad->SetTopMargin(0.1);
//       Pad->SetLeftMargin(0.1);

//       //calculate ratio, and error on ratio, and put that in new vectors
//       std::vector<double> ratios;
//       std::vector<double> errors;

//       double nom = 0.0;
//       double statErr = 0.0;
//       double sysErr = 0.0;

//       for (int k = 0; k != length; ++k)
//       {
// 	if (k == length - 1) continue;

// 	if (k == 0)
// 	{
// 	  nom = dataRatio[k]/mcRatio[k];
//           statErr = sqrt(nom*nom*((dataErr[k]/dataRatio[k])*(dataErr[k]/dataRatio[k]) + (mcErr[k]/mcRatio[k])*(mcErr[k]/mcRatio[k])));

// 	  ratios.push_back(1.0);
// 	  errors.push_back(statErr/nom);

// // 	  vx[k] = k+1;
// // 	  vxerr[k] = 0.0;
// // 	  vy[k] = 1.0;
// // 	  vyerr[k] = statErr/nom;
// 	}
// 	else
// 	{
// 	  double ratio = (dataRatio[k]/mcRatio[k]);
// 	  double error2 = sqrt(ratio*ratio*((dataErr[k]/dataRatio[k])*(dataErr[k]/dataRatio[k]) + (mcErr[k]/mcRatio[k])*(mcErr[k]/mcRatio[k])))/nom;

// 	  ratios.push_back(ratio/nom);
// 	  errors.push_back(error2);
// // 	  vx[k] = k+1;
// // 	  vxerr[k] = 0.0;
// // 	  vy[k] = ratio/nom;
// // 	  vyerr[k] = error2;

// 	  if (ratio > 100)
// 	  {
// 	    std::cout << ratio << " " << dataRatio[k] << " " << dataErr[k] << " " << mcRatio[k] << " " << mcErr[k] << std::endl; 
// 	  }
// 	}



// // 	vx[k] = k+1;
// // 	vxerr[k] = 0.0;

// // 	double ratio;
// // 	if (mcRatio[k] > 0)
// // 	{ 
// //           ratio = dataRatio[k]/mcRatio[k];
// // 	  vy[k] = ratio;
// // 	  vyerr[k] =  sqrt(ratio*ratio*((dataErr[k]/dataRatio[k])*(dataErr[k]/dataRatio[k]) + (mcErr[k]/mcRatio[k])*(mcErr[k]/mcRatio[k])));

// // 	  if (ratio > 100)
// // 	  {
// // 	    std::cout << ratio << " " << dataRatio[k] << " " << dataErr[k] << " " << mcRatio[k] << " " << mcErr[k] << std::endl; 
// // 	  }
// // 	}
// // 	else
// // 	{
// // 	  ratio = 0.0;
// // 	  vy[k] = 0.0;
// // 	  vyerr[k] = 0.0;
// // 	}

// // 	if (k == 0)
// // 	{
// // 	  nom = ratio;
// // 	  statErr = vyerr[k];
// // 	}
// // 	else if (vyerr[k]/vy[k] < 0.4 && ratio > 0.0)
// // 	{
// // 	  sysErr += (nom - vy[k])*(nom - vy[k]);
// // 	}
//       }
//       outfile << "Correction Factor = " << nom << " +/- " << statErr << " (stat) +/- " << sqrt(sysErr) << " (sys) " << std::endl;

// //       if (nJ == 1)
// //       {
// // 	std::cout << (int)ratios.size() << ": " << ratios[(int)ratios.size() - 1] << std::endl;
// //       }

//       //Now make arrays for real values.
//       for (int temp = 0; temp != (int)ratios.size(); ++temp)
//       {
// 	if (ratios[temp] < 0.01)
// 	{
// 	  if (nJ == 0)
// 	  {
// 	    zeroNames.erase(zeroNames.begin() + temp);
// 	  }
// 	  else if (nJ == 1)
// 	  {
// 	    oneNames.erase(oneNames.begin() + temp);
// 	  }
// 	  else
// 	  {
// 	    twoNames.erase(twoNames.begin() + temp);
// 	  }
// 	  ratios.erase(ratios.begin() + temp);
// 	  --temp;
// 	}
//       }

//       if (nJ == 0)
//       {
// 	length = (int)zeroNames.size();
//       }
//       else if (nJ == 1)
//       {
// 	length = (int)oneNames.size();
//       }
//       else
//       {
// 	length = (int)twoNames.size();
//       }

//       double vx[length];
//       double vxerr[length];
//       double vy[length];
//       double vyerr[length];

//       for (int temp = 0; temp != length; ++temp)
//       {
// 	vx[temp] = double(temp + 1);
// 	vxerr[temp] = 0.0;
// 	vy[temp] = ratios[temp];
// 	vyerr[temp] = errors[temp];
//       }

//       TH1D* hist = new TH1D(name.c_str(),name.c_str(),length,0.5,length+0.5);
//       hist->GetYaxis()->SetTitle("Cor. Factor_{cut}/Cor. Factor_{nom}");

//       for (int k = 0; k != length; ++k)
//       {
// 	if (nJ == 0)
// 	{
// 	  hist->GetXaxis()->SetBinLabel((k+1),zeroNames[k].c_str());
// 	}
// 	else if (nJ == 1)
// 	{
// 	  hist->GetXaxis()->SetBinLabel(k+1,oneNames[k].c_str());
// 	}
// 	else
// 	{
// 	  hist->GetXaxis()->SetBinLabel(k+1,twoNames[k].c_str());
// 	}
//       }
//       TLatex* text = new TLatex();
//       text->SetTextSize(0.06);
//       text->SetNDC();
//       std::string text2;
//       if (nP == 0)
//       {
// 	text2 = "Electron - ";
//       }
//       else
//       {
// 	text2 = "Muon - ";
//       }
//       if (nJ == 0)
//       {
// 	text2 += "Zero Jet";
//       }
//       else if (nJ == 1)
//       {
// 	text2 += "One Jet";
//       }
//       else 
//       {
// 	text2 += "Two Jet";
//       }

//       TGraphErrors* graph = new TGraphErrors(length,vx,vy,vxerr,vyerr);

// //       TF1* func = new TF1("fitfunc","[0]",1.5,length+0.5);
// //       func->SetParameter(0,1.0);
// //       TFitResultPtr r = graph->Fit("fitfunc","RS");

//       hist->SetMaximum(5);
//       hist->Draw();
//       graph->SetLineWidth(2);
//       graph->Draw("p");
//       if (nP == 0)
//       { 
//         text->DrawLatex(0.35,0.91,text2.c_str());
//       }
//       else
//       {
// 	text->DrawLatex(0.4,0.91,text2.c_str());
//       }

// //       outfile << "Fit result: p[0] = " << func->GetParameter(0) << " err_p[0] = " << func->GetParError(0) << std::endl << std::endl << std::endl;

//       name = "macros/HWWplots/zcontrol/" + name + ".eps";
//       Pad->Print(name.c_str());
    }//Close loop over jet bins
//   }//Closes loop over particles
  outfile.close();
}
