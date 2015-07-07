#include "ggFReweighting.h"

//using namespace std;
ggFReweighting::ggFReweighting(std::string generator, int higgsMass, std::string option, std::string inputFilePath)
{
  _inputFilePath = inputFilePath;
  initialize(generator, higgsMass, option);
}

ggFReweighting::~ggFReweighting()
{
  ;
}

void ggFReweighting::initialize(std::string generator, int higgsMass, std::string option)
{
  //// generator and higgsMass 
  _generator = generator;
  _higgsMass = higgsMass;
  _option = option;

  initializeGeneratorLib(_generatorLib);
  if(_generatorLib.find((_generator)) == _generatorLib.end()) 
    {
      std::cout << _generator  << " process is not available, please check !!!" << std::endl;
      exit(0);
    }
  initializeHiggsMassLib(_higgsMassLib, _generator);
  if(_higgsMassLib.find(_higgsMass) == _higgsMassLib.end()) 
    {
      std::cout << "Higgs Mass = " 
		<< _higgsMass << " is not available for "  << (_generator) 
		<< " please check !!!" << std::endl;
      exit(0);
    }

  ////// check the option /////////////
  if(_option != "Mean" && _option != "ScaleUp" && _option != "ScaleDown")
    {
      bool findPdfSet = false;
      char PdfSetName[200];
      for(int i = 0; i < 40; i++)
	{
	  sprintf(PdfSetName, "Pdfset%d", i+1);
	  if(PdfSetName == _option) 
	    {
	      findPdfSet = true;
	      break;
	    }
	}
      if(!findPdfSet)
	{
	  std::cout << "Option = " << _option << " is not available !" << std::endl;
	}
    }

  //// initial availabe generator and mass points 
  TFile* file = new TFile((_inputFilePath + "ggFHiggsPtWeight.root").c_str(), "read");
  char histName[200];
  
  if(_option == "Mean") sprintf(histName, "%s_H%d", _generator.c_str(), _higgsMass);
  else sprintf(histName, "%s_H%d_%s", _generator.c_str(), _higgsMass, _option.c_str());
  
  _weightHist = (TH1D*)file->Get(histName);
  _weightHist->SetDirectory(0);
  file->Close();
}

int ggFReweighting::higgsMass()
{
  return _higgsMass;
}

std::string ggFReweighting::option()
{
  return _option;
}
  
std::string ggFReweighting::generator()
{
  return _generator;
}

double ggFReweighting::getWeight(double higgsPt)
{
  return _weightHist->GetBinContent(_weightHist->FindBin(higgsPt));
}

double ggFReweighting::getStatError(double higgsPt)
{
  return _weightHist->GetBinError(_weightHist->FindBin(higgsPt));
}

std::pair<double, double> ggFReweighting::getWeightAndStatError(double higgsPt)
{
  std::pair<double, double> result(getWeight(higgsPt), getStatError(higgsPt));
  return result;
}

void ggFReweighting::printInfo()
{
  std::set<int>::iterator itMass;
  std::set<std::string>::iterator itGenerator;
  std::cout << " ggFReweiting : reweight normalized Higgs Pt to NNLL+NNLL by HqT " << std::endl;
  std::cout << " Statistical error: generator statistical error " << std::endl;
  std::cout << " Systematic error : "<< std::endl; 
  std::cout << "  (1) Pdf Uncertianty  MSTW2008 NNLO 90%C.L eigenvectors " << std::endl;
  std::cout << "  (2) Scale uncertainty: (mu_R*2, mu_F*2), (mu_R/2, mu_F/2) " << std::endl;
  std::cout << " Available generators  : " << std::endl;
  std::set<std::string> generatorLib;
  std::set<int> higgsMassLib;
  initializeGeneratorLib(generatorLib);
  for(itGenerator = generatorLib.begin(); itGenerator != generatorLib.end(); itGenerator++)
    {
      std::cout << std::setw(8) << *itGenerator << std::endl;
      initializeHiggsMassLib(higgsMassLib, *itGenerator);
      std::cout << " Available mass points : [";
      for(itMass = higgsMassLib.begin(); itMass != higgsMassLib.end(); itMass++)
	{
	  std::cout << std::setw(5) << *itMass;
	}
      std::cout << " ] GeV" << std::endl;
    
    }
  std::cout << std::endl;
}


void initializeGeneratorLib(std::set<std::string> &generatorLib)
{
  generatorLib.clear();
  generatorLib.insert("McAtNlo");
  generatorLib.insert("PowHeg");
  generatorLib.insert("PowHegMSSM");
}

void initializeHiggsMassLib(std::set<int> &higgsMassLib, std::string generator)
{
  int i;
  int mcatnlo_mass [] = {110, 120, 130, 140, 150, 160, 170, 180, 190, 200};
  int powheg_mass[] =  {100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155,
			160, 165, 170, 175, 180, 185, 190, 195, 200, 210,
			220, 240, 260, 280, 300, 320, 340, 360, 380, 400, 
			420, 440, 460, 480, 500, 520, 540, 560, 580, 600};
  int powhegMSSM_mass[] = { 90, 100, 110, 120, 130, 140, 150, 170,
			    200, 250, 300, 350, 400, 450, 500, 600};
  higgsMassLib.clear();
  if(generator == "McAtNlo")
    {
      for(i = 0; i < 10; i++) higgsMassLib.insert(mcatnlo_mass[i]);
    }

  if(generator == "PowHeg") 
    {
      for(i = 0; i < 42; i++) higgsMassLib.insert(powheg_mass[i]);
    }

  if(generator == "PowHegMSSM")
    {
      for(i = 0; i < 16; i++) higgsMassLib.insert(powhegMSSM_mass[i]);
    }
}
