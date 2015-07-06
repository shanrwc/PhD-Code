#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2.h"
#include "THStack.h"
#include "TStyle.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TGraph.h"
#include "TGraphAsymmErrors.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>

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

void plotpTEffs()
{
  gStyle->SetOptStat(0);

  const Color_t COLOR05 = kAzure - 5;
  const Color_t COLOR1 = kRed + 1;
  const Color_t COLOR15 = kSpring - 6;
  const Color_t COLOR2 = kOrange - 4;

  const Width_t LINEWIDTH = 3;

  std::vector<std::string> files;
  files.push_back("Effs/166198JetTauEtmiss_effs.root");
  files.push_back("ZEffs/data_effsZ.root");

  std::vector<std::string> particles;
  particles.push_back("e");
  particles.push_back("m");

  std::vector<std::string> partNames;
  partNames.push_back("Electron ");
  partNames.push_back("Muon ");

  std::vector<std::string> maps;
  maps.push_back("20ptVpt");
  maps.push_back("30ptVpt");
  maps.push_back("40ptVpt");

  std::vector<std::string> propNames;
  propNames.push_back("ETCone20/p_{T}");
  propNames.push_back("ETCone30/p_{T}");
  propNames.push_back("ETCone40/p_{T}");

  for (int i = 0; i != (int)files.size(); ++i)
  {
    TFile *f = new TFile(("root-files/SUSY/" + files[i]).c_str());
    for (int j = 0; j != (int)particles.size(); ++j)
    {
      for (int k = 0; k != (int)maps.size(); ++k)
      {
	//Get 2D map with all the info
	std::string label = particles[j] + "_" + maps[k];

	TH2D* info = (TH2D*)f->Get(label.c_str());

	//Declare four arrays to store the numbers
	int nbinsx = 19;
	int nbinsy = info->GetNbinsY();
	double cut05[nbinsx];
	double errp05[nbinsx];
	double errn05[nbinsx];
	double cut1[nbinsx];
	double errp1[nbinsx];
	double errn1[nbinsx];
	double cut15[nbinsx];
	double errp15[nbinsx];
	double errn15[nbinsx];
	double cut2[nbinsx];
	double errp2[nbinsx];
	double errn2[nbinsx];

	double pts[nbinsx];
	double xerr1[nbinsx];
	double xerr2[nbinsx];

	//Loop over map, getting efficiency in each pt strip
	for (int m = 0; m != nbinsx; ++m)
	{
	  pts[m] = m*5 + 10;
	  xerr1[m] = 0;
	  xerr2[m] = 0;

	  double total;
	  double strip05;
	  double strip1;
	  double strip15;
	  double strip2;

	  if (m == 18)
	  {
	    total = info->Integral(m+1,info->GetNbinsX(),1,nbinsy);
	    strip05 = info->Integral(m+1,info->GetNbinsX(),1,1);
	    strip1 = info->Integral(m+1,info->GetNbinsX(),1,2);
	    strip15 = info->Integral(m+1,info->GetNbinsX(),1,3);
	    strip2 = info->Integral(m+1,info->GetNbinsX(),1,4);
	  }
	  else
	  {
	    total = info->Integral(m+1,m+1,1,nbinsy);
	    strip05 = info->Integral(m+1,m+1,1,1);
	    strip1 = info->Integral(m+1,m+1,1,2);
	    strip15 = info->Integral(m+1,m+1,1,3);
	    strip2 = info->Integral(m+1,m+1,1,4);
	  }

	  if (total == 0)
	  {
	    cut05[m] = 0;
	    errp05[m] = 0;
	    errn05[m] = 0;
	    cut1[m] = 0;
	    errp1[m] = 0;
	    errn1[m] = 0;
	    cut15[m] = 0;
	    errp15[m] = 0;
	    errn15[m] = 0;
	    cut2[m] = 0;
	    errp2[m] = 0;
	    errn2[m] = 0;
	  }
	  else
	  {
	    cut05[m] = strip05/total;
	    errp05[m] = eff_pos_err(strip05,total);
	    errn05[m] = eff_neg_err(strip05,total);
	    cut1[m] = strip1/total;
	    errp1[m] = eff_pos_err(strip1,total);
	    errn1[m] = eff_neg_err(strip1,total);
	    cut15[m] = strip15/total;
	    errp15[m] = eff_pos_err(strip15,total);
	    errn15[m] = eff_neg_err(strip15,total);
	    cut2[m] = strip2/total;
	    errp2[m] = eff_pos_err(strip2,total);
	    errn2[m] = eff_neg_err(strip2,total);
	  }
	}

	//Now make some pretty graphs or histos or something.  Probably graphs.
	if (i == 0)
	{
	  label = "fake_" + label;
	}
	else
	{
	  label = "true_" + label;
	}
	TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,600,600);
	pad->SetFillColor(0);
	pad->SetTitle(label.c_str());
	pad->SetTopMargin(0.1);

	TGraphAsymmErrors* first = new TGraphAsymmErrors(nbinsx,pts,cut05,xerr1,xerr2,errn05,errp05);
	first->SetLineColor(COLOR05);
	first->SetMarkerColor(COLOR05);
	first->SetLineWidth(LINEWIDTH);
	first->SetTitle("");
	first->GetXaxis()->SetTitle("p_{T} [GeV]");
	first->GetYaxis()->SetTitle("Efficiency");

	TGraphAsymmErrors* second = new TGraphAsymmErrors(nbinsx,pts,cut1,xerr1,xerr2,errn1,errp1);
	second->SetLineColor(COLOR1);
	second->SetMarkerColor(COLOR1);
	second->SetLineWidth(LINEWIDTH);
	second->SetTitle("");

	TGraphAsymmErrors* third = new TGraphAsymmErrors(nbinsx,pts,cut15,xerr1,xerr2,errn15,errp15);
	third->SetLineColor(COLOR15);
	third->SetMarkerColor(COLOR15);
	third->SetLineWidth(LINEWIDTH);
	third->SetTitle("");

	TGraphAsymmErrors* fourth = new TGraphAsymmErrors(nbinsx,pts,cut2,xerr1,xerr2,errn2,errp2); 
	fourth->SetLineColor(COLOR2);
	fourth->SetMarkerColor(COLOR2);
	fourth->SetLineWidth(LINEWIDTH);
	fourth->SetTitle("");

	first->Draw("ACP");
	first->GetYaxis()->SetRangeUser(0,1.6);
	second->Draw("CP same");
	third->Draw("CP same");
	fourth->Draw("CP same");

        TLegend* leg = new TLegend(0.65,0.7,0.87,0.87);
        leg->AddEntry(first,"Cut at 0.05","p");
        leg->AddEntry(second,"Cut at 0.1","p");
        leg->AddEntry(third,"Cut at 0.15","p");
        leg->AddEntry(fourth,"Cut at 0.2","p");
        leg->SetTextSize(0.035);
        leg->SetFillColor(0);
	leg->Draw();

        TLatex* text = new TLatex();
        text->SetNDC();
        text->SetTextSize(0.06);
        double xcor = 0.25;
	std::string header;
	if (i == 0)
	{
	  header = "Fake " + partNames[j] + propNames[k];
	}
	else
	{
	  header = "True " + partNames[j] + propNames[k];
	}
        text->DrawLatex(xcor,0.93,header.c_str());

	std::string name = "macros/SUSYplots/MMEffs/" + label + ".png";
	pad->Print(name.c_str());
      }
    }
  }

//   TFile* t = new TFile("root-files/SUSY/Effs/jetsdata_effs.root");
//   TFile* l = new TFile("root-files/SUSY/Effs/jetsdata_effsL.root");

//   std::vector<std::string> particles;
//   particles.push_back("elec");
//   particles.push_back("mu");

//   for (int i = 0; i != (int)particles.size(); ++i)
//   {
//     std::string label = particles[i] + "_eff";

//     TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,800,600);
//     pad->SetFillColor(0);
//     pad->SetTitle(label.c_str());
// //     pad->SetRightMargin(0.13);
// //     pad->SetTopMargin(0.08);

//     TH2D* part_t_b = (TH2D*)t->Get((label + "_l").c_str());
//     TH2D* part_t_a = (TH2D*)t->Get((label + "_t").c_str());
//     TH2D* part_eff = (TH2D*)part_t_a->Clone("part_eff");
//     part_eff->Divide(part_t_b);
//     TH1D* plot1 = part_eff->ProjectionX();
//     double nbinsx = part_eff->GetNbinsX();
//     double nbinsy = part_eff->GetNbinsY();
//     for (int j = 1; j <= nbinsx; ++j)
//     {
//       double total = 0.0;
//       for (int k = 1; k <= nbinsy; ++k)
//       {
// 	if (part_t_b->GetBinContent(j,k) > 0)
// 	{
// 	  total += part_eff->GetBinContent(j,k);
// 	}
//       }
//       plot1->SetBinContent(j, (total/nbinsy));
//     }
//     plot1->SetLineColor(kAzure - 6);
//     plot1->SetLineWidth(3);

//     TH2D* part_l_b = (TH2D*)l->Get((label + "_l").c_str());
//     TH2D* part_l_a = (TH2D*)l->Get((label + "_t").c_str());
//     TH2D* part2_eff = (TH2D*)part_l_a->Clone("part2_eff");
//     part2_eff->Divide(part_l_b);
//     TH1D* plot2 = part2_eff->ProjectionX();
//     nbinsx = part2_eff->GetNbinsX();
//     nbinsy = part2_eff->GetNbinsY();
//     for (int j = 1; j <= nbinsx; ++j)
//     {
//       double total = 0.0;
//       for (int k = 1; k <= nbinsy; ++k)
//       {
// 	if (part_l_b->GetBinContent(j,k) > 0)
// 	{
// 	  total += part2_eff->GetBinContent(j,k);
// 	}
//       }
//       plot2->SetBinContent(j, (total/nbinsy));
//     }
//     plot2->SetLineColor(kGreen + 3);
//     plot2->SetLineWidth(3); 

//     plot1->Draw("hist");
// //     plot2->Draw("hist same");

//     TLegend* leg = new TLegend(0.65,0.67,0.9,0.87);
//     leg->AddEntry(plot1,"ETCone40/pT < 0.1","f");
//     leg->AddEntry(plot2,"ETCone40/pT < 0.15","f");
//     leg->SetTextSize(0.03);
//     leg->SetFillColor(0); 
//     leg->Draw();

//     pad->Print(("macros/SUSYplots/" + label + ".png").c_str());
//   }

//   TFile* refs = new TFile("MMInput.root");

//   std::vector<std::string> fplots;
//   fplots.push_back("true_elec");
//   fplots.push_back("fake_elec");
//   fplots.push_back("true_muon");
//   fplots.push_back("fake_muon");

//   std::vector<std::string> ftitles;
//   ftitles.push_back("Efficiency for True Electrons");
//   ftitles.push_back("Efficiency for Fake Electrons");
//   ftitles.push_back("Efficiency for True Muons");
//   ftitles.push_back("Efficiency for Fake Muons");

//   for (int i = 0; i != (int)fplots.size(); ++i)
//   {
//     gStyle->SetPalette(1);
    

//     std::string label = fplots[i] + "_eff";

//     TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,800,600);
//     pad->SetFillColor(0);
//     pad->SetTitle(label.c_str());
//     pad->SetRightMargin(0.13);
//     pad->SetTopMargin(0.08);

//     TH2* plot = (TH2*)refs->Get(label.c_str());
//     plot->SetTitle("");
//     std::string particle;
//     if (i < 2)
//     {
//       particle = "Electron";
//     }
//     else
//     {
//       particle = "Muon";
//     }
//     plot->GetXaxis()->SetTitle((particle + " p_{T} [GeV]").c_str());
//     plot->GetXaxis()->SetRangeUser(10,100);
//     plot->GetYaxis()->SetTitle((particle + " #eta").c_str());

//     plot->Draw("colz");

//     TText* text = new TText();
//     text->SetNDC();
//     text->SetTextSize(0.06);
//     double xcor;
//     if (i < 2)
//     {
//       xcor = 0.25;
//     }
//     else
//     {
//       xcor = 0.28;
//     }
//     text->DrawText(xcor,0.93,ftitles[i].c_str());

//     std::string name = "macros/SUSYplots/MMEffs/" + label + ".eps";
// //     pad->Print(name.c_str());
//     name = "macros/SUSYplots/MMEffs/" + label + ".png";
//     pad->Print(name.c_str());
//   }

//   TCanvas* linen = new TCanvas("efficiencies","efficiencies",14,33,700,500);
//   linen->SetFillColor(0);
//   linen->SetTitle("efficiencies");
//   linen->SetRightMargin(0.13);

//   TH2* temp1 = (TH2*)refs->Get("true_elec_eff");
//   TH1* plot1 = temp1->ProjectionX();
//   int nbinsx = temp1->GetNbinsX();
//   int nbinsy = temp1->GetNbinsY();
//   for (int i = 1; i <= nbinsx; ++i)
//   {
//     double total = 0.0;
//     for (int j = 1; j <= nbinsy; ++j)
//     {
//       total = total +  temp1->GetBinContent(i,j);
//     }
//     plot1->SetBinContent(i,(total/nbinsy));
//   }
//   plot1->SetLineColor(kAzure - 5);
//   plot1->SetLineWidth(3);

//   TH2* temp2 = (TH2*)refs->Get("fake_elec_eff");
//   TH1* plot2 = temp2->ProjectionX();
//   nbinsx = temp2->GetNbinsX();
//   nbinsy = temp2->GetNbinsY();
//   for (int i = 1; i <= nbinsx; ++i)
//   {
//     double total = 0.0;
//     for (int j = 1; j <= nbinsy; ++j)
//     {
//       total += temp2->GetBinContent(i,j);
//     }
//     plot2->SetBinContent(i,(total/nbinsy));
//   }
//   plot2->SetLineColor(kRed + 1);
//   plot2->SetLineWidth(3);

//   TH2* temp3 = (TH2*)refs->Get("true_muon_eff");
//   TH1* plot3 = temp3->ProjectionX();
//   nbinsx = temp3->GetNbinsX();
//   nbinsy = temp3->GetNbinsY();
//   for (int i = 1; i <= nbinsx; ++i)
//   {
//     double total = 0.0;
//     for (int j = 1; j <= nbinsy; ++j)
//     {
//       total += temp3->GetBinContent(i,j);
//     }
//     plot3->SetBinContent(i,(total/nbinsy));
//   }
//   plot3->SetLineColor(kGreen + 3);
//   plot3->SetLineWidth(3);

//   TH2* temp4 = (TH2*)refs->Get("fake_muon_eff");
//   TH1* plot4 = temp4->ProjectionX();
//   nbinsx = temp4->GetNbinsX();
//   nbinsy = temp4->GetNbinsY();
//   for (int i = 1; i <= nbinsx; ++i)
//   {
//     double total = 0.0;
//     for (int j = 2; j < nbinsy; ++j)
//     {
//       total += temp4->GetBinContent(i,j);
//       std::cout << "(i,j) = " << i << ", " << j << std::endl;
//       std::cout << temp4->GetBinContent(i,j) << std::endl;
//     }
//     plot4->SetBinContent(i,(total/nbinsy));
//   }
//   plot4->SetLineColor(kOrange);
//   plot4->SetLineWidth(3);

//   plot3->GetYaxis()->SetRangeUser(0,3);
//   plot3->GetYaxis()->SetTitle("Efficiencies");
//   plot3->GetXaxis()->SetTitle("Lepton p_{T} [GeV]");

//   TLegend* leg = new TLegend(0.65,0.67,0.85,0.87);
//   leg->AddEntry(plot1,"True Electrons","f");
//   leg->AddEntry(plot2,"Fake Electrons","f");
//   leg->AddEntry(plot3,"True Muons","f");
//   leg->AddEntry(plot4,"Fake Muons","f");
//   leg->SetTextSize(0.03);
//   leg->SetFillColor(0);

//   plot3->Draw("hist");
//   plot1->Draw("hist same");
//   plot2->Draw("hist same");
//   plot4->Draw("hist same");
//   leg->Draw();

//   linen->Print("macros/SUSYplots/MMEffs/oneDeffs.png");
//   linen->Print("macros/SUSYplots/oneDeffs.eps");

//   for (int i = 0; i != (int)channels.size(); ++i)
//   {
//     std::cout << "Channel " << channels[i] << std::endl;
//     for (int j = 0; j != 3; ++j)
//     {
//       std::cout << "Signal estimate: " << (signals[channels[i] ])[j] << "     Background estimate: " << (backgrounds[channels[i] ])[j] << std::endl;
//     }
//     std::cout << std::endl;
//   }
}
