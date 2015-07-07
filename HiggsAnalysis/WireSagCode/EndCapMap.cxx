#include "EndCapMap.h"

EndCapMap::EndCapMap(std::string Sample, std::string tag)
{
  //Set parameters for histograms
  int nBins;
  double lowBin;
  double highBin;

  if (tag.compare("pull") == 0)
  {
    nBins = 100;
    lowBin = -2;
    highBin = 2;
  }
  else if (tag.compare("errqoverp") == 0)
  {
    nBins = 100;
    lowBin = -0.0001;
    highBin = 0.0005;
  }
  else if (tag.compare("del1overp") == 0)
  {
    nBins = 100;
    lowBin = -0.0005;
    highBin = 0.0005;
  }
  else if (tag.compare("recop") == 0)
  {
    nBins = 100;
    lowBin = 0;
    highBin = 0.001;
  }
  else if (tag.compare("truthp") == 0)
  {
    nBins = 100;
    lowBin = 0;
    highBin = 0.001;
  }
  else if (tag.compare("pT") == 0)
  {
    nBins = 25;
    lowBin = 750;
    highBin = 1250;
  }
  else if (tag.compare("resos") == 0)
  {
    nBins = 50;
    lowBin = -5;
    highBin = 5;
  }
  else if (tag.compare("deltapT") == 0)
  {
    nBins = 50;
    lowBin = -1;
    highBin = 1;
  }
  else if (tag.compare("resoP") == 0)
  {
    nBins = 75;
    lowBin = -1.5;
    highBin = 1.5;
  }
  else if (tag.compare("reso1OP") == 0)
  {
    nBins = 75;
    lowBin = -1.5;
    highBin = 1.5;
  }
  else if (tag.compare("dzero") == 0)
  {
    nBins = 100;
    lowBin = 0;
    highBin = 0.01;
  }
  else
  {
    std::cout << "EndCapMap doesn't recognize the variable it's been asked to analyze." << std::endl;
    std::cout << "Unrecognized variable is: " << tag << std::endl;
    std::cout << "Please add variable to c-tor before attempting to use." << std::endl;
    exit(0);
  }

  std::vector<std::string> charge;
  charge.push_back("p");
  charge.push_back("n");

  //Use nested for-loops to fill map
  for (int i = 1; i < 6; ++i)
  {
    for (int j = 1; j < 49; ++j)
    {
      for (int k = 0; k != (int)charge.size(); ++k)
      {
        std::string label = tag + "_" + charge[k] + "_" + makeString(i) + "_" + makeString(j);

        TH1D* hist = new TH1D(label.c_str(),label.c_str(),nBins,lowBin,highBin);

        histos.insert(std::pair<std::string,TH1D*>(label,hist));
      }
    }
  }

  variable = tag;
  sample = Sample;
}

EndCapMap::~EndCapMap()
{
  ;
}

std::string EndCapMap::makeString(int number)
{
  std::stringstream ss;
  ss << number;
  return ss.str();
}

std::string EndCapMap::getLocString(int charge, double eta, double phi)
{
  int etaString = 0;
  int phiString = 0;

//   etaString = 1+int(10*(eta - 1));
  if (eta < 1.206)
  {
    etaString = 1;
  }
  else if ((eta >= 1.206) && (eta < 1.403))
  {
    etaString = 2;
  }
  else if ((eta >= 1.403) && (eta < 1.691))
  {
    etaString = 3;
  }
  else if ((eta >= 1.691) && (eta < 2.115))
  {
    etaString = 4;
  }
  else if ((eta >= 2.115) && (eta <= 2.762))
  {
    etaString = 5;
  }
  else
  {
    std::cout << "getLocString function doesn't know what to do with this track!" << std::endl;
    std::cout << "eta = " << eta << std::endl;
  }

  if (TMath::Abs(phi) < 3.14159/48)
  {
    phiString = 1;
  }
  else if (TMath::Abs(phi) > 47*3.14159/48)
  {
    phiString = 25;
  }
  else if (phi > 0)
  {
    phiString = 2 + int((phi - 3.14159/48)*(24/3.14159));
  }
  else if (phi < 0)
  {
    phiString = 48 + int((phi + 3.14159/48)*(24/3.14159));
  }
  else
  {
    std::cout << "getLocString function doesn't know what to do with this track!" << std::endl;
    std::cout << "phi = " << phi << std::endl;
  }

  if (phiString == 48)
  {
    phiString = 1;
  }
  else
  {
    phiString = phiString + 1;
  }

//   if (TMath::Abs(phi) < 3.14159/16)
//   {
//     phiString = 1;
//   }
//   else if (TMath::Abs(phi) > 15*3.14159/16)
//   {
//     phiString = 9;
//   }
//   else if (phi > 0)
//   {
//     for (int i = 2; i < 9; ++i)
//     {
//       if ((phi >= (i - 1)*3.14159/16) && (phi < i*3.14159/16))
//       {
// 	phiString = i;
//       }
//     }
//   }
//   else if (phi < 0)
//   {

//   }
//   else
//   {
//     std::cout << "Ya gots a weeeeiiiiird value here.  It's phi = " << phi << std::endl;
//   }

  std::string q;
  if (charge > 0)
  {
    q = "p";
  }
  else
  {
    q = "n";
  }

  std::string label = variable + "_" + q + "_" + makeString(etaString) + "_" + makeString(phiString);

  return(label);
}

void EndCapMap::addEvent(int charge, double eta, double phi, double var, double weight)
{
  std::string key = getLocString(charge,eta,phi);

  histos[key]->Fill(var,weight);

}

void EndCapMap::fitHisto(TH1D* histo, double &mean, double &error, double &variance, double &varerr)
{
  //Define parameters of fitting function, if needed.
  int status;
  double lowend;
  double highend;

  if (variable.compare("pull") == 0)
  {
    lowend = -1.0;
    highend = 1.0;
  }
  else if (variable.compare("pT") == 0)
  {
    lowend = 750;
    highend = 1250;
  }
  else if (variable.compare("resoP") == 0)
  {
    lowend = histo->GetMean() - 2*(histo->GetRMS());
    highend = histo->GetMean() + 2*(histo->GetRMS()); 
  }
  else if (variable.compare("reso1OP") == 0)
  {
    lowend = histo->GetMean() - 1.5*(histo->GetRMS());
    highend = histo->GetMean() + 1.5*(histo->GetRMS());
  }
  else
  {
    std::cout << "You remembered to add the new variable to the c-tor, but not fitHisto." << std::endl;
    std::cout << "Do so before continuing." << std::endl;
    exit(0);
  }
  if (histo->Integral() > 10)
  {
    status = histo->Fit("gaus","Q","",lowend,highend);
    TF1* fit = histo->GetFunction("gaus");
    fit->SetLineColor(kRed + 1);
    mean = fit->GetParameter(1);
//     double temp = fit->GetParameter(1);
//     if (TMath::Abs(mean - temp) > 0.005)
//     {
//       std::cout << "Apparent difference between fit means!" << std::endl;
//       std::cout << "Histo name: " << histo->GetName() << std::endl;
//       std::cout << "mean in file = " << mean << std::endl;
//       std::cout << "mean parameter = " << temp << std::endl << std::endl;
//     }
    error = fit->GetParError(1);
//     variance = fit->Variance(lowend,highend);
    variance = fit->GetParameter(2);
//     if (variance != temp)
//     {
//       std::cout << "Difference between parameter2 and variance!" << std::endl;
//       std::cout << "variance: " << variance << std::endl;
//       std::cout << "parameter2: " << temp << std::endl << std::endl;
//     }
    varerr = fit->GetParError(2);
  }
  else
  {
    mean = 20;
    error = 0;
    variance = 20;
    varerr = 0;
  }

}

void EndCapMap::graphFittedMeans(int etagroup, int phigroup)
{
  //Calculate number of groups to use in eta and phi.
//   double blocks1 = 17.0/etagroup;
//   int nEtas;
//   if (blocks1 - int(blocks1) > 0)
//   {
//     nEtas = int(blocks1) + 1;
//   }
//   else
//   {
//     nEtas = int(blocks1);
//   }
  int nEtas = 5;

  double blocks2 = 48.0/phigroup;
  int nPhis;
  if (blocks2 - int(blocks2) > 0)
  {
    nPhis = int(blocks2) + 1;
  }
  else
  {
    nPhis = int(blocks2);
  }

  //WAIT!!!!!! You need to loop over two types of charge.
  //'Cause otherwise you won't see anything.
  //How could you have forgotten?  You've made this mistake BEFORE.
  std::vector<std::string> charges;
  charges.push_back("p");
  charges.push_back("n");

  for (int n = 0;n != (int)charges.size(); ++n)
  {
    //Make 2D vectors for means and errors.  And fill them.
    //Don't forget--that's the tricky part.
    std::vector<std::vector<double> > means;
    std::vector<std::vector<double> > errors;
    std::vector<std::vector<double> > variances;
    std::vector<std::vector<double> > varerrs;

    for (int i = 1; i <= nEtas; ++i)
    {
      std::vector<double> temp1;
      std::vector<double> temp2;
      std::vector<double> temp3;
      std::vector<double> temp4;
      for (int j = 1; j <= nPhis; ++j)
      {
        temp1.push_back(0);
        temp2.push_back(0);
	temp3.push_back(0);
	temp4.push_back(0);
      }
      means.push_back(temp1);
      errors.push_back(temp2);
      variances.push_back(temp3);
      varerrs.push_back(temp4);
    }

    for (int i = 1; i <= nEtas; ++i)
    {
      for (int j = 1; j <= nPhis; ++j)
      {
        //Get all histograms that will fall in this sector.
        TH1D* combo;
        std::string name;
        for (int k = 1 + etagroup*(i - 1); k <= i*etagroup; ++k)
        {
	  for (int l = 1 + phigroup*(j - 1); l <= j*phigroup; ++l)
	  {
	    //Skip iteration if sector doesn't contain all necessary histograms.
	    if (k > 17 || l > 48) continue;

	    //Assemble name string.
	    name = variable + "_" + charges[n] + "_" + makeString(k) + "_" + makeString(l);

	    //Add histogram to combo.
	    if (k == 1 + etagroup*(i - 1) && l == 1 + phigroup*(j - 1))
	    {
	      combo = histos[name];
	    }
	    else
	    {
	      combo->Add(histos[name]);
	    }

	  }
        }

        //Perform fit
        TCanvas* canvas = new TCanvas((variable + charges[n] + "_" + makeString(i) + "_" + makeString(j)).c_str(),(variable + charges[n] + "_" + makeString(i) + "_" + makeString(j)).c_str(),14,33,600,600);
        canvas->SetFillColor(0);
        canvas->SetTitle((variable + charges[n] + "_" + makeString(i) + "_" + makeString(j)).c_str());

        double mean = 0;
        double error = 0;
	double variance = 0;
	double varerr = 0;
        fitHisto(combo, mean, error, variance, varerr);

        combo->Draw();
	combo->GetXaxis()->SetTitle("#Delta(1/p)p_{truth}");

        canvas->Print(("macros/WireSagplots/EndCapMaps/" + sample + "/" + variable + "/" + variable + "_" + charges[n] + "_" + makeString(i) + "_" + makeString(j) + ".png").c_str());

        //Add fit parameters to vectors of vectors
        (means[i-1])[j-1] = mean;
        (errors[i-1])[j-1] = error;
	(variances[i-1])[j-1] = variance;
	(varerrs[i-1])[j-1] = varerr;

      }
    }

    //Make eta slice graphs of means
  for (int i = 1; i <= nEtas; ++i)
  {
    //Get vectors
    double* vx = new double[nPhis];
    double* vex = new double[nPhis];
    double* vy = new double[nPhis];
    double* vey = new double[nPhis];
    double* vz = new double[nPhis];
    double* vez = new double[nPhis];

    for (int j = 1; j <= nPhis; ++j)
    {
      vx[j - 1] = j;
      vex[j - 1] = 0.0;
      vy[j - 1] = means[i - 1][j - 1];
      vey[j - 1] = errors[i - 1][j - 1];
      vz[j - 1] = variances[i - 1][j - 1];
      vez[j - 1] = varerrs[i - 1][j - 1];
    }

    //Do print the nice values to files so you can play with them later.
    std::string filename = "fits/" + sample + "_" + variable + "_mean_etaslice_" + makeString(i) + charges[n] + ".txt";
    ofstream outfile;
    outfile.open(filename.c_str());
    for (int j = 0; j < nPhis; ++j)
    {
      outfile << vx[j] << " " << vy[j] << " " << vey[j] << std::endl;
    } 
    outfile.close();

    std::string filename2 = "fits/" + sample + "_" + variable + "_sigma_etaslice_" + makeString(i) + charges[n] + ".txt";
    ofstream outfile2;
    outfile2.open(filename2.c_str());
    for (int j = 0; j < nPhis; ++j)
    {
      outfile2 << vx[j] << " " << vz[j] << " " << vez[j] << std::endl;
    } 
    outfile2.close();

//     //Put them in TGraphErrors thingamawhatever.  Pretty versions.
//     std::string label = variable + "_eta_" + charges[n];
//     for (int j = 1+etagroup*(i - 1); j <= i*etagroup; ++j)
//     {
//       label = label + "_" + makeString(j);
//     }

//     TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,800,600);
//     pad->SetFillColor(0);
//     pad->SetTitle(label.c_str());

//     TGraphErrors* graph = new TGraphErrors(nPhis,vx,vy,vex,vey);
//     graph->Draw("alp");

//     std::string name = "macros/WireSagplots/EndCapMaps/" + sample + "/" + variable + "/" + label + ".png";
//     pad->Print(name.c_str());
  }

  //Make eta slice graphs of variances
  for (int i = 1; i <= nEtas; ++i)
  {
    //Get vectors
    double* vx = new double[nPhis];
    double* vex = new double[nPhis];
    double* vy = new double[nPhis];
    double* vey = new double[nPhis];

    for (int j = 1; j <= nPhis; ++j)
    {
      vx[j - 1] = j;
      vex[j - 1] = 0.0;
      vy[j - 1] = variances[i - 1][j - 1];
      vey[j - 1] = varerrs[i - 1][j - 1];
    }

    //Put them in TGraphErrors thingamawhatever.  Pretty versions.
//     std::string label = "variance_eta_" + variable + "_" + charges[n];
//     for (int j = 1+etagroup*(i - 1); j <= i*etagroup; ++j)
//     {
//       label = label + "_" + makeString(j);
//     }

//     TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,800,600);
//     pad->SetFillColor(0);
//     pad->SetTitle(label.c_str());

//     TGraphErrors* graph = new TGraphErrors(nPhis,vx,vy,vex,vey);
//     graph->Draw("alp");

//     std::string name = "macros/WireSagplots/EndCapMaps/" + sample + "/" + variable + "/" + label + ".png";
//     pad->Print(name.c_str());
  }

  //Make phi slice graphs
//   for (int i = 1; i <= nPhis; ++i)
//   {
//     //Get vectors
//     double* vx = new double[nEtas];
//     double* vex = new double[nEtas];
//     double* vy = new double[nEtas];
//     double* vey = new double[nEtas];

//     for (int j = 1; j <= nEtas; ++j)
//     {
//       vx[j - 1] = j;
//       vex[j - 1] = 0.0;
//       vy[j - 1] = means[j - 1][i - 1];
//       vey[j - 1] = errors[j - 1][i - 1];
//     }
//     //Put them in TGraphErrors thingamawhatever.  Pretty versions.
//     std::string label = variable + "_phi_" + charges[n];
//     for (int j = 1+phigroup*(i - 1); j <= i*phigroup; ++j)
//     {
//       label = label + "_" + makeString(j);
//     }

//     TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,800,600);
//     pad->SetFillColor(0);
//     pad->SetTitle(label.c_str());

//     TGraphErrors* graph = new TGraphErrors(nEtas,vx,vy,vex,vey);
//     graph->SetMinimum(-0.5);
//     graph->SetMaximum(0.5);
//     graph->Draw("alp");

//     std::string name = "macros/WireSagplots/EndCapMaps/" + label + ".png";
//     pad->Print(name.c_str());

//   }
  }//Closes charge loop.
}

void EndCapMap::graphDistributions(int etagroup, int phigroup)
{
  //Calculate number of groups to use in eta and phi.
//   double blocks1 = 17.0/etagroup;
//   int nEtas;
//   if (blocks1 - int(blocks1) > 0)
//   {
//     nEtas = int(blocks1) + 1;
//   }
//   else
//   {
//     nEtas = int(blocks1);
//   }
  int nEtas = 5;

  double blocks2 = 48.0/phigroup;
  int nPhis;
  if (blocks2 - int(blocks2) > 0)
  {
    nPhis = int(blocks2) + 1;
  }
  else
  {
    nPhis = int(blocks2);
  }

  //Loop over positive and negative charges
  std::vector<std::string> charges;
  charges.push_back("p");
  charges.push_back("n");

  for (int n = 0;n != (int)charges.size(); ++n)
  {
    for (int i = 1; i <= nEtas; ++i)
    {
      for (int j = 1; j <= nPhis; ++j)
      {
        //Get all histograms that will fall in this sector.
        TH1D* combo;
        std::string name;
        for (int k = 1 + etagroup*(i - 1); k <= i*etagroup; ++k)
        {
	  for (int l = 1 + phigroup*(j - 1); l <= j*phigroup; ++l)
	  {
	    //Skip iteration if sector doesn't contain all necessary histograms.
	    if (k > 17 || l > 48) continue;

	    //Assemble name string.
	    name = variable + "_" + charges[n] + "_" + makeString(k) + "_" + makeString(l);

	    //Add histogram to combo.
	    if (k == 1 + etagroup*(i - 1) && l == 1 + phigroup*(j - 1))
	    {
	      combo = histos[name];
	    }
	    else
	    {
	      combo->Add(histos[name]);
	    }

	  }
        }

        //Draw Histo
        TCanvas* canvas = new TCanvas((variable + "_" + charges[n] + "_" + makeString(i) + "_" + makeString(j)).c_str(),(variable + charges[n] + "_" + makeString(i) + "_" + makeString(j)).c_str(),14,33,800,600);
        canvas->SetFillColor(0);
        canvas->SetTitle((variable + "_" + charges[n] + "_" + makeString(i) + makeString(j)).c_str());
	canvas->SetRightMargin(0.12);

        combo->Draw();

        canvas->Print(("macros/WireSagplots/EndCapMaps/" + sample + "/" + variable + "/" + variable + "_" + charges[n] + "_" + makeString(i) + "_" + makeString(j) + ".png").c_str());

      }
    }
  }
}


void EndCapMap::graphChamberCenters(int etagroup)
{
  //Calculate number of groups to use in eta.
  double blocks1 = 17.0/etagroup;
  int nEtas;
  if (blocks1 - int(blocks1) > 0)
  {
    nEtas = int(blocks1) + 1;
  }
  else
  {
    nEtas = int(blocks1);
  }

  //WAIT!!!!!! You need to loop over two types of charge.
  std::vector<std::string> charges;
  charges.push_back("p");
  charges.push_back("n");

  for (int n = 0;n != (int)charges.size(); ++n)
  {
    //Make 2D vectors for means and errors.  And fill them.
    std::vector<std::vector<double> > means;
    std::vector<std::vector<double> > errors;
    std::vector<std::vector<double> > variances;
    std::vector<std::vector<double> > varerrs;

    for (int i = 1; i <= nEtas; ++i)
    {
      std::vector<double> temp1;
      std::vector<double> temp2;
      std::vector<double> temp3;
      std::vector<double> temp4;
      for (int j = 1; j <= 16; ++j)
      {
        temp1.push_back(0);
        temp2.push_back(0);
	temp3.push_back(0);
	temp4.push_back(0);
      }
      means.push_back(temp1);
      errors.push_back(temp2);
      variances.push_back(temp3);
      varerrs.push_back(temp4);
    }

    for (int i = 1; i <= nEtas; ++i)
    {
      for (int j = 0; j < 16; ++j)
      {
        //Get all histograms that will fall in this sector.
        TH1D* combo;
        std::string name;
        for (int k = 1 + etagroup*(i - 1); k <= i*etagroup; ++k)
        {
	  //Skip iteration is sector doesn't contain full set of histograms
	  if (k > 17) continue;

	  //Assemble name string
	  name = variable + "_" + charges[n] + "_" + makeString(k) + "_" + makeString(3*j + 2);

	  //Add histogram to combo
	  if (k == 1 + etagroup*(i - 1))
	  {
	    combo = histos[name];
	  }
	  else
	  {
	    combo->Add(histos[name]);
	  }

        }

        //Perform fit
        TCanvas* canvas = new TCanvas((variable + "_" + charges[n] + "_center_" + makeString(i) + makeString(3*j + 2)).c_str(),(variable + "_" + charges[n] + "_center_" + makeString(i) + makeString(3*j + 2)).c_str(),14,33,800,600);
        canvas->SetFillColor(0);
        canvas->SetTitle((variable + "_" + charges[n] + "_center_" + makeString(i) + makeString(3*j + 2)).c_str());

        double mean = 0;
        double error = 0;
	double variance = 0;
	double varerr = 0;
        fitHisto(combo, mean, error, variance, varerr);

        combo->Draw();

        canvas->Print(("macros/WireSagplots/EndCapMaps/" + sample + "/" + variable + "_" + charges[n] + "_center_" + makeString(i) + makeString(3*j + 2) + ".png").c_str());

        //Add fit parameters to vectors of vectors
        (means[i-1])[j] = mean;
        (errors[i-1])[j] = error;
	(variances[i-1])[j] = variance;
	(varerrs[i-1])[j] = varerr;
      }
    }

    //Make eta slice graphs of means
  for (int i = 1; i <= nEtas; ++i)
  {
    //Get vectors
    double* vx = new double[16];
    double* vex = new double[16];
    double* vy = new double[16];
    double* vey = new double[16];

    for (int j = 1; j <= 16; ++j)
    {
      vx[j - 1] = j;
      vex[j - 1] = 0.0;
      vy[j - 1] = means[i - 1][j - 1];
      vey[j - 1] = errors[i - 1][j - 1];
    }

    //Put them in TGraphErrors thingamawhatever.  Pretty versions.
    std::string label = variable + "_eta_center_" + charges[n];
    for (int j = 1+etagroup*(i - 1); j <= i*etagroup; ++j)
    {
      label = label + "_" + makeString(j);
    }

    TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,800,600);
    pad->SetFillColor(0);
    pad->SetTitle(label.c_str());

    TGraphErrors* graph = new TGraphErrors(16,vx,vy,vex,vey);
    graph->Draw("alp");

    std::string name = "macros/WireSagplots/EndCapMaps/" + sample + "/" + label + ".png";
    pad->Print(name.c_str());
  }

//   //Make eta slice graphs of variances
//   for (int i = 1; i <= nEtas; ++i)
//   {
//     //Get vectors
//     double* vx = new double[16];
//     double* vex = new double[16];
//     double* vy = new double[16];
//     double* vey = new double[16];

//     for (int j = 1; j <= 16; ++j)
//     {
//       vx[j - 1] = j;
//       vex[j - 1] = 0.0;
//       vy[j - 1] = variances[i - 1][j - 1];
//       vey[j - 1] = varerrs[i - 1][j - 1];
//     }

//     //Put them in TGraphErrors thingamawhatever.  Pretty versions.
//     std::string label = "variance_eta_" + variable + "_center_" + charges[n];
//     for (int j = 1+etagroup*(i - 1); j <= i*etagroup; ++j)
//     {
//       label = label + "_" + makeString(j);
//     }

//     TCanvas* pad = new TCanvas(label.c_str(),label.c_str(),14,33,800,600);
//     pad->SetFillColor(0);
//     pad->SetTitle(label.c_str());

//     TGraphErrors* graph = new TGraphErrors(16,vx,vy,vex,vey);
//     graph->Draw("alp");

//     std::string name = "macros/WireSagplots/EndCapMaps/" + sample + "/" + label + ".png";
//     pad->Print(name.c_str());
//   }
  }//Closes charge loop
}
