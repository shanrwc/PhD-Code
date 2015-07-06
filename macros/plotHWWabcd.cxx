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
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

////////////////////////////////////////////////////////////////////////////
///////////////////////////Helper Functions/////////////////////////////////
////////////////////////////////////////////////////////////////////////////

double calcError(double sumErr, double partErr)
{
  return(sqrt(sumErr*sumErr - partErr*partErr));
}

double findABCDError(double B, double Berr,
		     double c0, double c0err, double cp, double cperr,
		     double d0, double d0err, double dp, double dperr)
{
  double dAdb0 = (c0+cp)/(d0+dp) * (cp+dp)/(c0+cp+d0+dp);
//   double dAdbp = dAdb0;

  double dAdc0 = (B)*(cp+dp)/((c0+cp+d0+dp)*(c0+dp+d0+dp));
  double dAdcp = (B)/(d0+dp)*(((c0+cp+d0+dp)*(2*cp+c0+dp)-(c0+cp)*(cp+dp))/((c0+dp+d0+dp)*(c0+dp+d0+dp)));

  double dAdd0 = -1*(B)*(c0+cp)*(cp+dp)*(2*d0+2*dp+c0+cp)/((d0+dp)*(d0+dp)*(c0+cp+d0+dp)*(c0+cp+d0+dp));
  double dAddp = (B)*(c0+cp)*(((d0+dp)*(c0+cp+d0+dp))-((cp+dp)*(2*d0+2*dp+c0+cp)))/((d0+dp)*(d0+dp)*(c0+cp+d0+dp)*(c0+cp+d0+dp));

double err2 = dAdb0*dAdb0*Berr*Berr + 
              dAdc0*dAdc0*c0err*c0err + dAdcp*dAdcp*cperr*cperr +
              dAdd0*dAdd0*d0err*d0err + dAddp*dAddp*dperr*dperr;

 return(sqrt(err2));
}

template<typename T>
std::string makeString(T const& value)
{
  std::stringstream sstr;
  sstr << value;
  return sstr.str();
}

////////////////////////////////////////////////////////////////////////////

void plotHWWabcd(double hmass = 150)
{
  const Float_t LEGTEXTSIZE = 0.04;

  std::vector<std::string> particles;
  particles.push_back("e");
  particles.push_back("m");

  std::vector<std::string> njets;
  njets.push_back("0");
  njets.push_back("1");
//   njets.push_back("2");

  std::vector<std::string> metnum;
  metnum.push_back("8");
  metnum.push_back("10");
  metnum.push_back("14");

  std::vector<double> mcscale;
//   mcscale.push_back(0.87886);
//   mcscale.push_back(0.988976);
//   mcscale.push_back(0.837768);
//   mcscale.push_back(0.866344);
  mcscale.push_back(1.0);
  mcscale.push_back(1.0);
  mcscale.push_back(1.0);
  mcscale.push_back(1.0);

  std::vector<std::string> inputs;
  inputs.push_back("data");
  inputs.push_back("ttbar");
  inputs.push_back("sTop");
  inputs.push_back("smWW");
  inputs.push_back("smEW");
  inputs.push_back("smW");
  inputs.push_back("smZ");

  std::string resultname = "macros/textfiles/abcd_table.txt";
  std::ofstream results;
  results.open(resultname.c_str());
  results << "\\begin{tabular}{|c|c|c|c|c|c|}" << std::endl;
  results << "\\hline" << std::endl;
  results << "& $A^{MC}_{pre\\metrel}$ & $B^{MC}\\frac{C^{MC}}{D^{MC}}$  & $A^{ABCD}_{pre\\metrel}$ & $A^{MC}_{final}$ & $A^{ABCD}_{final}$ \\\\" << std::endl;

  for (int nPart = 0; nPart != (int)particles.size(); ++nPart)
  {
    for (int nJet = 0; nJet != (int)njets.size(); ++nJet)
    {
      std::string chan;
      if (nPart == 0)
      {
	chan = "ee ";
	if (nJet == 0)
	{
	  chan += "0 jet";
	}
	else if (nJet == 1)
	{
	  chan += "1 jet";
	}
      }
      else
      {
	chan = "$\\mu\\mu$ ";
	if (nJet == 0)
	{
	  chan += "0 jet";
	}
	else if (nJet == 1)
	{
	  chan += "1 jet";
	}
      }

      //Make list of persistant variables
      double B = 0.0;
      double Berr = 0.0;
      double C = 0.0; 
      double Cerr = 0.0;
      double D = 0.0;
      double Derr = 0.0;

      double Bz = 0.0;
      double Cz = 0.0;
      double Dz = 0.0;
      double Bzerr = 0.0;
      double Czerr = 0.0;
      double Dzerr = 0.0;

      double Cf = 0.0;
      double CfErr = 0.0;
      double Df = 0.0;
      double DfErr = 0.0;

      TH1D* shape = 0;

      std::string filename = "macros/textfiles/abcd_" + particles[nPart] + "_" + njets[nJet] + ".txt";
      std::ofstream outfile;
      outfile.open(filename.c_str());
      outfile << "\\begin{tabular}{|c|c|c|c|c|}" << std::endl;
      outfile << "\\hline" << std::endl;
      outfile << "& A & B & C & D \\\\" << std::endl;
      outfile << "\\hline" << std::endl;

      //Loop over list and calculate B, C, and D for each, updating running counters
      //Also update 1D histogram and write appropriate stuff to file.
      for (int nIn = 0; nIn != (int)inputs.size(); ++nIn)
      {
	TFile* file = new TFile(("root-files/hww/" + inputs[nIn] + "_hwwstudy.root").c_str());
	TH2D* plot = (TH2D*)file->Get((particles[nPart] + "_" + njets[nJet] + "_MllMet").c_str());

	if (inputs[nIn] != "data")
	{
	  plot->Scale(LUMINOSITY);
	}

	std::string getplot = particles[nPart] + "_" + njets[nJet];
	TH1D* mTc = (TH1D*)file->Get((getplot + "_mTc").c_str());
	TH1D* mTd = (TH1D*)file->Get((getplot + "_mTd").c_str());
	
	double aerr = 0;
	double a = plot->IntegralAndError(16,50,41,151,aerr);
	double berr = 0;
	double b = plot->IntegralAndError(16,50,1,40,berr);
	double cerr = 0;
	double c = plot->IntegralAndError(77,106,41,151,cerr);
	double derr = 0;
	double d = plot->IntegralAndError(77,106,1,40,derr);

	double cf = 0;
	double cferr = 0;
	double df = 0;
	double dferr = 0;

	cf = mTc->IntegralAndError(mTc->FindBin(0.75*hmass),mTc->FindBin(hmass),cferr);
	df = mTd->IntegralAndError(mTd->FindBin(0.75*hmass),mTd->FindBin(hmass),dferr);

	if (inputs[nIn] == "smZ")
	{
	  outfile << "\\hline" << std::endl;
	  Bz = b;
	  Cz = c;
	  Dz = d;
	  Bzerr = berr;
	  Czerr = cerr;
	  Dzerr = derr;
	}
	if (nIn == 1)
	{
	  outfile << fixed << setprecision(2);
	}
	outfile << "\\" << inputs[nIn] << " & " << a << " &" << b << " & " << c << " & " << d << "\\\\" << std::endl;

	if (inputs[nIn] == "data")
	{
	  B += b;
	  C += c;
	  D += d;
	  Cf += cf;
	  Df += df;
	  if (nIn == 0)
	  {
	    shape = plot->ProjectionY((inputs[nIn] + particles[nPart] + "_" + njets[nJet] + "_py").c_str(),77,106);
	  }
	  else
	  {
	    shape->Add(plot->ProjectionY((inputs[nIn] + particles[nPart] + "_" + njets[nJet] + "_py").c_str(),77,106));
	  }
	}
	else if (inputs[nIn] != "smZ")
	{
	  B -= b;
	  C -= c;
	  D -= d;
	  Cf -= cf;
	  Df -= df;
	  if (nIn == 0)
	  {
	    shape = plot->ProjectionY((inputs[nIn] + particles[nPart] + "_" + njets[nJet] + "_py").c_str(),77,106);
	    shape->Scale(-1);
	  }
	  else
	  {
	    shape->Add(plot->ProjectionY((inputs[nIn] + particles[nPart] + "_" + njets[nJet] + "_py").c_str(),77,106), -1);
	  }
	}
	if (inputs[nIn] != "smZ")
	{
	  Berr = sqrt(Berr*Berr + berr*berr);
	  Cerr = sqrt(Cerr*Cerr + cerr*cerr);
	  Derr = sqrt(Derr*Derr + derr*derr);
	  CfErr = sqrt(CfErr*CfErr + cferr*cferr);
	  DfErr = sqrt(DfErr*DfErr + dferr*dferr);
	}
      }

      //Calculate estimate.  Make 1D shape prediction.
      TFile* Z = new TFile("root-files/hww/smZ_hwwstudy.root");

      TH2D* zplot = (TH2D*)Z->Get((particles[nPart] + "_" + njets[nJet] + "_MllMet").c_str());
      zplot->Scale(LUMINOSITY);
      double mcestErr = 0.0;
      double mcest = zplot->IntegralAndError(15,50,41,100,mcestErr);

      std::string othername = "met_" + particles[nPart] + particles[nPart] + njets[nJet] + "_3";
      TH1D* comp = zplot->ProjectionY(("mc_" + particles[nPart] + njets[nJet] + "comp_py").c_str(),1,50);
      comp->SetTitle("");
      comp->Scale(LUMINOSITY);
      comp->SetLineColor(kRed);
      comp->GetXaxis()->SetRangeUser(9,100);

      TH1D* othercomp = (TH1D*)Z->Get(("met_" + particles[nPart] + particles[nPart] + njets[nJet] + "_" + metnum[nJet]).c_str());
      othercomp->Scale(LUMINOSITY);
      othercomp->SetTitle("");
      othercomp->SetLineColor(kRed);

      int index = 0;
      if (nPart == 1) index = 2;
      index += nJet;

      double xcheck = Bz*(Cz/Dz);
      double xerr = xcheck*sqrt((Bzerr/Bz)*(Bzerr/Bz) + (Czerr/Cz)*(Czerr/Cz) + (Dzerr/Dz)*(Dzerr/Dz));
      double otherr = B*(C/D)*sqrt((Berr/B)*(Berr/B) + (Cerr/C)*(Cerr/C) + (Derr/D)*(Derr/D));

      results << "\\hline" << std::endl;
      results << chan << " & " << std::fixed << std::showpoint << std::setprecision(3) << mcest<<"$\\pm$"<< mcestErr<< " & " << Bz*(Cz/Dz) <<"$\\pm$"<<xerr<< " & " << B*(C/D) <<"$\\pm$" << otherr << " & " << othercomp->Integral()*mcscale[index] << " & " << B*(C/D)*(Cf+Df)/(C+D) << "$\\pm$" << findABCDError(B,Berr,C-Cf,calcError(Cerr,CfErr),Cf,CfErr,D-Df,calcError(Derr,DfErr),Df,DfErr) << "\\\\" << std::endl;

      shape->Scale((B/D));
      shape->GetXaxis()->SetRangeUser(9,100);

      std::string label = "abcd_" + particles[nPart] + "_" + njets[nJet];
      TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,800,600);
      pad->SetFillColor(0);
      pad->SetTitle(label.c_str());
      pad->SetTopMargin(0.1); 
      pad->SetLogy(1);

      if (comp->GetBinContent(comp->GetMaximumBin()) > shape->GetBinContent(shape->GetMaximumBin()))
      {
	comp->Draw("hist");
	comp->GetXaxis()->SetTitle("E_{T,Rel}^{miss} [GeV]");
	comp->GetYaxis()->SetTitle("Estimated Events");
	comp->GetXaxis()->SetRangeUser(9,100);

	shape->Draw("same hist");
      }
      else
      {
        shape->GetXaxis()->SetTitle("E_{T,Rel}^{miss} [GeV]");
	shape->GetYaxis()->SetTitle("Estimated Events");
        shape->Draw("hist");
	shape->GetXaxis()->SetRangeUser(9,100);

        comp->Draw("same hist");
      }

      TLegend *leg = new TLegend(0.7,0.75,0.9,0.85);
      leg->SetTextSize(LEGTEXTSIZE);
      leg->SetFillColor(0);
      leg->SetBorderSize(0);
      leg->AddEntry(shape, "ABCD Shape","f");
      leg->AddEntry(comp, "Z/DY MC","f");
      leg->Draw();

      TLatex* header = new TLatex();
      header->SetTextSize(0.06);
      header->SetNDC();

      std::string text;
      if (nPart == 0)
      {
	text = "ee ";
      }
      else
      {
	text = "#mu#mu ";
      }
      if (nJet == 0)
      {
	text += "0 Jet";
      }
      else if (nJet == 1)
      {
	text += "1 Jet";
      }
      else if (nJet == 2)
      {
	text += "2 Jet";
      }
      header->DrawLatex(0.45,0.91,text.c_str());

      TLatex* lumtex = new TLatex();
      lumtex->SetNDC();
      lumtex->SetTextSize(0.03);
      lumtex->SetTextColor(1);
      lumtex->DrawLatex(0.69,0.69,("#int Ldt = " + makeString(LUMINOSITY) + " fb^{-1}").c_str());

      std::string name = "macros/HWWplots/zcontrol/" + label + "_premet_shape.eps";
      pad->Print(name.c_str());

      pad->Clear();
      shape->GetYaxis()->SetTitle("Unit Area");
      comp->GetYaxis()->SetTitle("Unit Area");

      if (comp->GetBinContent(comp->GetMaximumBin()) > shape->GetBinContent(shape->GetMaximumBin()))
      {
	comp->DrawNormalized("hist");
	comp->GetXaxis()->SetTitle("E_{T,Rel}^{miss} [GeV]");
	comp->GetYaxis()->SetTitle("Unit Area");
	comp->GetXaxis()->SetRangeUser(9,100);
	shape->DrawNormalized("hist same");
	comp->GetYaxis()->SetTitle("Unit Area");
      }
      else
      {
        shape->DrawNormalized("hist");
	shape->GetXaxis()->SetRangeUser(9,100);
        shape->GetXaxis()->SetTitle("E_{T,Rel}^{miss} [GeV]");
	shape->GetYaxis()->SetTitle("Unit Area");
        comp->DrawNormalized("same hist");
	shape->DrawNormalized("same hist axis");
	shape->GetYaxis()->SetTitle("Unit Area");
      }

      leg->Draw();
      header->DrawLatex(0.45,0.91,text.c_str());

      name = "macros/HWWplots/zcontrol/" + label + "_premet_norm.eps";
      pad->Print(name.c_str());
      lumtex->DrawLatex(0.69,0.69,("#int Ldt = " + makeString(LUMINOSITY) + " fb^{-1}").c_str());

      outfile << "\\hline" << std::endl;
      outfile << "\\end{tabular}" << std::endl;
      outfile.close();

      //And . . . make other plots
      //Because you aren't making enough plots unless you can fill ten slides with nothing else.
      //Seriously.

      pad->Clear();
      shape->Scale((Cf+Df)/(C+D));

      if (othercomp->GetBinContent(othercomp->GetMaximumBin()) > shape->GetBinContent(shape->GetMaximumBin()))
      {
	othercomp->Draw("hist");
	othercomp->GetXaxis()->SetTitle("E_{T,Rel}^{miss} [GeV]");
	othercomp->GetYaxis()->SetTitle("Estimated Events");
	othercomp->GetXaxis()->SetRangeUser(39,100);

	if (nJet == 0)
	{
	  othercomp->GetYaxis()->SetRangeUser(0.0000001,2);
	}

	shape->Draw("hist same");
      }
      else
      {
        shape->GetXaxis()->SetTitle("E_{T,Rel}^{miss} [GeV]");
	shape->GetYaxis()->SetTitle("Estimated Events");
        shape->Draw("hist");
	shape->GetXaxis()->SetRangeUser(39,100);

        othercomp->Draw("same hist");
	shape->Draw("hist same");
      }

      header->DrawLatex(0.45,0.91,text.c_str());
      lumtex->DrawLatex(0.69,0.69,("#int Ldt = " + makeString(LUMINOSITY) + " fb^{-1}").c_str());
      leg->Draw();

      name = "macros/HWWplots/zcontrol/" + label + "_final_shape.eps";
      pad->Print(name.c_str());


    }//closes loop over njets
  }//closes loop over particles

  results << "\\hline" << std::endl;
  results << "\\end{tabular}" << std::endl;
  results.close();
}//closes main function
