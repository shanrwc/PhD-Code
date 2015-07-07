#include "BaseAnalysis.h"
//#include "checkOQ.C"
#include "robustIsEMDefs.C"

BaseAnalysis::BaseAnalysis() : _debug(false) {

  _electrons = new TClonesArray("MTree::Electron");
//   _photons = new TClonesArray("MTree::Photon");
  _muons = new TClonesArray("MTree::Muon");
  _caloMuons = new TClonesArray("MTree::Muon");
  _jets = new TClonesArray("MTree::Jet");
  _tracks = new TClonesArray("MTree::Track");
//   _standAloneMuons = new TClonesArray("MTree::Track");
//   _muTagMuons = new TClonesArray("MTree::Track");
  _primaryVertices = new TClonesArray("MTree::PrimaryVertex");
  _truthParticles = new TClonesArray("MTree::TruthParticle");
//   _L1EmTauTriggerROIs = new TClonesArray("MTree::L1TriggerROI");
  _L1MuonTriggerROIs = new TClonesArray("MTree::L1TriggerROI");
//   _HLTTriggerROIs = new TClonesArray("MTree::HLTTriggerROI");
  _missingET = new MTree::MissingET();
  _missingETTerms = new TClonesArray("MTree::MissingET");
  _eventInfo = new MTree::EventInfo();
  _trigger = new MTree::Trigger();
  _triggerObjects = new TClonesArray("MTree::TriggerObject");

  AddAllFiles();
  FillCrossSections();
  FillSampleWeights();
  FillSetNumbers();
}

BaseAnalysis::~BaseAnalysis() {
}

double BaseAnalysis::GetCrossSection(std::string chainName)
{
  string name;
  if (chainName.find("JF17") != string::npos)
  {
    name = "JF17";
  }
  else if (chainName.find("-") != string::npos && ((chainName.find("Np0-") != string::npos) || (chainName.find("ttbar-") != string::npos) || chainName.find("PythiaZ") != string::npos))
  {
    chainName.erase(chainName.end() - 2, chainName.end());
    name = chainName;
  }
  else
  {
    name = chainName;
  }

  std::map<std::string, double>::iterator iMap = _crossSections.find(name);
  if (iMap == _crossSections.end()) {
    std::cout << "No cross section information found for chain " << name << std::endl;
    std::exit(1);
  }
  return _crossSections[name];
}

double BaseAnalysis::makeDouble(std::string input)
{
  std::istringstream stm;
  stm.str(input);
  double num;
  stm >> num;

  return(num);
}

double BaseAnalysis::GetSampleWeight(std::string chainName)
{

  if (chainName.find("Np0-") != string::npos)
  {
    chainName.erase(chainName.end() - 2, chainName.end());
  }
  if (chainName.find("ttbar-") != string::npos)
  {
    chainName.erase(chainName.end() - 2, chainName.end());
  }
  if (chainName.find("PythiaZee-") != string::npos)
  {
    chainName.erase(chainName.end() - 2, chainName.end());
  }
  if (chainName.find("PythiaZmm-") != string::npos)
  {
    chainName.erase(chainName.end() - 2, chainName.end());
  }

  std::string file = "HiggsAnalysis/samples/sampleWeights.txt";
  ifstream infile (file.c_str());
  std::string line;
  double weight = 0.0;
  if (infile.is_open())
  {
    while (infile.good())
    {
      getline(infile,line);
      if (line.find(" " + chainName) != string::npos)
      {
	std::vector<std::string> holder = splitString(line);
// 	std::string bit = holder[4];
	weight += makeDouble(holder[4]);
      }
    }
  }

  std::string file2 = "HiggsAnalysis/samples/sampleEfficiencies.txt";
  ifstream infile2 (file2.c_str());
  std::string line2;
  double effic = 0.0;
  if (infile2.is_open())
  {
    while (infile2.good())
    {
      getline(infile2,line2);
      if (line2.find(convertNameToNum(chainName)) != string::npos)
      {
	std::vector<std::string> holder = splitString(line2);
	effic += makeDouble(holder[4]);
      }
    }
  }

//   std::map<std::string, double>::iterator iMap = _sampleWeights.find(name);
//   if (iMap == _sampleWeights.end()) {
//     std::cout << "No weight information found for chain " << name << std::endl;
//     std::exit(1);
//   }
  if (weight == 0)
  {
    std::cout << "No weight information found for chain " << chainName << std::endl;
    std::exit(1);
  }
  if (effic == 0)
  {
    std::cout << "No efficiency found for chain " << chainName << std::endl;
    std::exit(1);
  }
  if (effic > 1)
  {
    std::cout << "Found bad efficiency; efficiency = " << effic << " for " << chainName << std::endl;
    std::exit(1);
  }
  return (weight/effic);
}

TChain* BaseAnalysis::GetChain(std::string chainName) {
  //
  // Return a pointer to a new TChain. Caller has to delete the pointer!
  //
  std::map<std::string,std::vector<std::string> >::iterator iMap = _fileNameMap.find(chainName); 
  if (iMap == _fileNameMap.end()) {
    std::cout << "ERROR: No Chain with name: \"" << chainName << "\"" << std::endl;
    std::cout << "The following Chain names are known:" << std::endl;
    for (iMap=_fileNameMap.begin(); iMap!=_fileNameMap.end(); ++iMap) {
      std::cout << "\"" << iMap->first << "\"" << std::endl;
    }
    std::exit(1);
  }
  int nrFiles = (int) _fileNameMap[chainName].size();
  TChain* chain = new TChain("MTree", "test");
  for (int i = 0; i != nrFiles; ++i) {
    TString fileName(_fileNameMap[chainName].at(i).c_str());
    chain->AddFile(fileName);
  }
  return chain;
}

TChain* BaseAnalysis::GetChain(std::vector<std::string> chainNames) {
  //
  // Return a pointer to a new TChain. Caller has to delete the pointer!
  //
  TChain* chain = new TChain("MTree");
  int NrChainNames = chainNames.size();
  for (int ii = 0; ii != NrChainNames; ++ii) {
    std::string chainName = chainNames[ii];
    std::map<std::string,std::vector<std::string> >::iterator iMap = _fileNameMap.find(chainName); 
    if (iMap == _fileNameMap.end()) {
      std::cout << "ERROR: No Chain with name: \"" << chainName << "\"" << std::endl;
      std::cout << "The following Chain names are known:" << std::endl;
      for (iMap=_fileNameMap.begin(); iMap!=_fileNameMap.end(); ++iMap) {
	std::cout << "\"" << iMap->first << "\"" << std::endl;
      }
      std::exit(1);
    }
    int nrFiles = (int) _fileNameMap[chainName].size();
    for (int i = 0; i != nrFiles; ++i) {
      TString fileName(_fileNameMap[chainName].at(i).c_str());
      chain->AddFile(fileName);
    }
  }
  return chain;
}


std::vector<std::string> BaseAnalysis::GetKnownChains() {
  std::vector<std::string> names;
  std::map<std::string, std::vector<std::string> >::iterator it = _fileNameMap.begin(), 
    itEnd = _fileNameMap.end(); 
  for (; it != itEnd; ++it) {
    names.push_back(it->first);
  }
  return names;
}

void BaseAnalysis::setBranchAddresses(TChain* tree) {  
  if (!tree) return;
//   tree->SetBranchStatus("*", 1);

  tree->SetBranchStatus("Electrons", 1);
//   tree->SetBranchStatus("Photons", 1);
  tree->SetBranchStatus("Muons", 1);
  tree->SetBranchStatus("CaloMuons", 1);
  tree->SetBranchStatus("Jets", 1);
  tree->SetBranchStatus("Tracks", 1);
//   tree->SetBranchStatus("StandAloneMuons", 1);
//   tree->SetBranchStatus("MuTagMuons", 1);
  tree->SetBranchStatus("PrimaryVertices", 1);
  if (tree->GetBranch("TruthParticles")) {
    tree->SetBranchStatus("TruthParticles", 1);
  }
//   tree->SetBranchStatus("L1EmTauTriggerROIs", 1);
  tree->SetBranchStatus("L1MuonTriggerROIs", 1);
//   tree->SetBranchStatus("HLTTriggerROIs", 1);
  tree->SetBranchStatus("MissingET", 1);
  tree->SetBranchStatus("MissingETTerms",1);
  tree->SetBranchStatus("EventInfo", 1);
  tree->SetBranchStatus("Trigger", 1);
  tree->SetBranchStatus("TriggerObjects",1);

  tree->SetBranchAddress("Electrons", &_electrons);
//   tree->SetBranchAddress("Photons", &_photons);
  tree->SetBranchAddress("Muons", &_muons);
  tree->SetBranchAddress("CaloMuons", &_caloMuons);
  tree->SetBranchAddress("Jets", &_jets);
  tree->SetBranchAddress("Tracks", &_tracks);
//   tree->SetBranchAddress("StandAloneMuons", &_standAloneMuons);
//   tree->SetBranchAddress("MuTagMuons", &_muTagMuons);
  tree->SetBranchAddress("PrimaryVertices", &_primaryVertices);
  if (tree->GetBranch("TruthParticles")) {
    tree->SetBranchAddress("TruthParticles", &_truthParticles);
  }
//   tree->SetBranchAddress("L1EmTauTriggerROIs", &_L1EmTauTriggerROIs);
  tree->SetBranchAddress("L1MuonTriggerROIs", &_L1MuonTriggerROIs);
//   tree->SetBranchAddress("HLTTriggerROIs", &_HLTTriggerROIs);
  tree->SetBranchAddress("MissingET", &_missingET);
  tree->SetBranchAddress("MissingETTerms",&_missingETTerms);
  tree->SetBranchAddress("EventInfo", &_eventInfo);
  tree->SetBranchAddress("Trigger", &_trigger);
  tree->SetBranchAddress("TriggerObjects",&_triggerObjects);

}

void BaseAnalysis::includeOverflow(TH1D* hist) {

  int nBins = hist->GetNbinsX();
  double last, over, last_err, over_err;
  last     = hist->GetBinContent(nBins);
  last_err = hist->GetBinError(nBins);
  over     = hist->GetBinContent(nBins+1);
  over_err = hist->GetBinError(nBins+1);
  if (over != 0.0) {
    hist->SetBinContent(nBins, last + over);
    hist->SetBinError(nBins, TMath::Sqrt(last_err*last_err + over_err*over_err));
    hist->SetBinContent(nBins+1, 0.0);
    hist->SetBinError(nBins+1, 0.0);
  }
}

void BaseAnalysis::includeUnderflow(TH1D* hist) {
 
  double first, under, first_err, under_err;
  first     = hist->GetBinContent(1);
  first_err = hist->GetBinError(1);
  under     = hist->GetBinContent(0);
  under_err = hist->GetBinError(0);
  if (under != 0.0) {
    hist->SetBinContent(1, first + under);
    hist->SetBinError(1, TMath::Sqrt(first_err*first_err + under_err*under_err));
    hist->SetBinContent(0, 0.0);
    hist->SetBinError(0, 0.0);
  }
}

void BaseAnalysis::removeNegativeBins(TH1D* hist) {
  int nBins = hist->GetNbinsX();
  for (int i = 1; i != nBins+1; ++i) {
    if (hist->GetBinContent(i) < 0.0) {
      hist->SetBinContent(i, 0.0);
      hist->SetBinError(i, 0.0);
    }
  }
}

double BaseAnalysis::efficiency_symm(double m, double n) {
  return ( 1./n*sqrt(m + m*m/n) );
}

double BaseAnalysis::eff_pos_err(double m, double n) {
  return ( 
	  ( n*(1+2*m) + sqrt(n*n*(1+2*m)*(1+2*m) - 4*n*(1+n)*m*m) ) / 
	  ( 2*n*(1+n) ) 
	  ) 
    - m/n;
}
double BaseAnalysis::eff_neg_err(double m, double n) {
  return m/n - ( 
		( n*(1+2*m) - sqrt(n*n*(1+2*m)*(1+2*m) - 4*n*(1+n)*m*m) ) / 
		( 2*n*(1+n) ) 
		);
}

double BaseAnalysis::poisson_up(double y) {
  double y1 = y + 1;
  double d = 1.0 - 1.0/(9.0*y1) + 1.0/(3*TMath::Sqrt(y1));
  return y1*d*d*d;
}

double BaseAnalysis::poisson_down(double y) {
  if (y == 0.0) return 0.0;
  double d = 1.0 - 1.0/(9.0*y) - 1.0/(3.0*TMath::Sqrt(y));
  return y*d*d*d;
}

TGraphAsymmErrors* BaseAnalysis::GetEfficiencyGraph(TH1D* num, TH1D* den) {
  int nrBins = num->GetNbinsX();
  double xmin = num->GetBinCenter(1);
  double width = num->GetBinWidth(1);
  double* x = new double[nrBins];
  double* y = new double[nrBins];
  double* exl = new double[nrBins];
  double* exh = new double[nrBins];
  double* eyl = new double[nrBins];
  double* eyh = new double[nrBins];
  for (int i = 0; i != nrBins; ++i) {
    double m = num->GetBinContent(i+1);
    double n = den->GetBinContent(i+1);
    x[i] = xmin + i*width;
    y[i] = n != 0.0 ? (m / n) : 0.0;
    exl[i] = 0.0;
    exh[i] = 0.0;
    eyl[i] = n != 0.0 ? eff_neg_err(m, n) : 0.0;
    eyh[i] = n != 0.0 ? eff_pos_err(m, n) : 0.0;
    if (y[i] == 0.0) {
      y[i] = -1.0;
      eyl[i] = 0.0;
      eyh[i] = 0.0;
    }
  }
  TGraphAsymmErrors* T_data = new TGraphAsymmErrors(nrBins, x, y, exl, exh, eyl, eyh);
  return T_data;
}

void BaseAnalysis::AddAllFiles() {

  std::vector<std::string> fileNames;
  fileNames.push_back("higgs");
  fileNames.push_back("data");
  fileNames.push_back("smww");
  fileNames.push_back("smw");
  fileNames.push_back("smz");
  fileNames.push_back("smdy");
  fileNames.push_back("smew");
  fileNames.push_back("top");
  fileNames.push_back("test");
//   fileNames.push_back("egamma");

  for (int nFiles = 0; nFiles != (int)fileNames.size(); ++nFiles)
  {
    std::string file = "HiggsAnalysis/samples/" + fileNames[nFiles] + ".txt";
    ifstream infile (file.c_str());
    std::string line;
    if (infile.is_open())
    {
      while (infile.good())
      {
	getline(infile,line);
	std::vector<std::string> holder = splitString(line);
	if ((int)holder.size() < 3) continue;
	AddFile(holder[0],holder[1],holder[2]);
      }
    }
  }
}

void BaseAnalysis::FillCrossSections() {
  //Note: these cross-sections include k-factors and event filter efficiencies.

  _crossSections["testskim"] = 1.0;

  _crossSections["ggfhww150"] = 770.471*0.699;
  _crossSections["vbfhww150"] = 70.568*0.699;

  _crossSections["WevNp0"] = 6870500*1.0*1.21;
  _crossSections["WevNp1"] = 1293000*1.0*1.21;
  _crossSections["WevNp2"] = 376600*1.0*1.21;
  _crossSections["WevNp3"] = 101290*1.0*1.21;
  _crossSections["WevNp4"] = 25246*1.0*1.21;
  _crossSections["WevNp5"] = 7123.9*1.0*1.21;
  _crossSections["WmvNp0"] = 6871100*1.0*1.21;
  _crossSections["WmvNp1"] = 1294700*1.0*1.21;
  _crossSections["WmvNp2"] = 376080*1.0*1.21;
  _crossSections["WmvNp3"] = 100720*1.0*1.21;
  _crossSections["WmvNp4"] = 25993*1.0*1.21;
  _crossSections["WmvNp5"] = 7130*1.0*1.21;
  _crossSections["WtvNp0"] = 6873300*1.0*1.21;
  _crossSections["WtvNp1"] = 1295200*1.0*1.21;
  _crossSections["WtvNp2"] = 375070*1.0*1.21;
  _crossSections["WtvNp3"] = 101770*1.0*1.21;
  _crossSections["WtvNp4"] = 25756*1.0*1.21;
  _crossSections["WtvNp5"] = 7001.6*1.0*1.21;

  _crossSections["PythiaWt"] = 8916330*1.17*1.0;

  _crossSections["WbbNp0"] = 3307.2*1.0*1.21;
  _crossSections["WbbNp1"] = 2675.6*1.0*1.21;
  _crossSections["WbbNp2"] = 1376.3*1.0*1.21;
  _crossSections["WbbNp3"] = 656.89*1.0*1.21;

  _crossSections["ZeeNp0"] = 664100*1.0*1.26;
  _crossSections["ZeeNp1"] = 132462*1.0*1.26;
  _crossSections["ZeeNp2"] = 40226*1.0*1.26;
  _crossSections["ZeeNp3"] = 11138*1.0*1.26;
  _crossSections["ZeeNp4"] = 3128.18*1.0*1.26;
  _crossSections["ZeeNp5"] = 753.425*1.0*1.26;
  _crossSections["ZmmNp0"] = 663790*1.0*1.26;
  _crossSections["ZmmNp1"] = 132950*1.0*1.26;
  _crossSections["ZmmNp2"] = 40375*1.0*1.26;
  _crossSections["ZmmNp3"] = 11161*1.0*1.26;
  _crossSections["ZmmNp4"] = 2898.7*1.0*1.26;
  _crossSections["ZmmNp5"] = 756.62*1.0*1.26;
  _crossSections["ZttNp0"] = 662500*1.0*1.26;
  _crossSections["ZttNp1"] = 133940*1.0*1.26;
  _crossSections["ZttNp2"] = 40295*1.0*1.26;
  _crossSections["ZttNp3"] = 11029*1.0*1.26;
  _crossSections["ZttNp4"] = 2804*1.0*1.26;
  _crossSections["ZttNp5"] = 780.54*1.0*1.26;

  _crossSections["PythiaZee"] = 855210*1.15*1.0;
  _crossSections["PythiaZmm"] = 855250*1.15*1.0;
  _crossSections["PythiaZtt"] = 856967*1.15*1.0;

  _crossSections["dyZeeNp0"] = 3051620*1.0*1.22;
  _crossSections["dyZeeNp1"] = 87870*1.0*1.22;
  _crossSections["dyZeeNp2"] = 41100*1.0*1.22;
  _crossSections["dyZeeNp3"] = 8460*1.0*1.22;
  _crossSections["dyZeeNp4"] = 1840*1.0*1.22;
  _crossSections["dyZeeNp5"] = 460*1.0*1.22;
  _crossSections["dyZmmNp0"] = 3051620*1.0*1.22;
  _crossSections["dyZmmNp1"] = 87870*1.0*1.22;
  _crossSections["dyZmmNp2"] = 40950*1.0*1.22;
  _crossSections["dyZmmNp3"] = 8410*1.0*1.22;
  _crossSections["dyZmmNp4"] = 1850*1.0*1.22;
  _crossSections["dyZmmNp5"] = 460*1.0*1.22;
  _crossSections["dyZttNp0"] = 3054680*1.0*1.22;
  _crossSections["dyZttNp1"] = 86480*1.0*1.22;
  _crossSections["dyZttNp2"] = 41530*1.0*1.22;
  _crossSections["dyZttNp3"] = 8370*1.0*1.22;
  _crossSections["dyZttNp4"] = 1840*1.0*1.22;
  _crossSections["dyZttNp5"] = 460*1.0*1.22;

  _crossSections["PythiaDYLee"] = 1795000*1.15*1.0;
  _crossSections["PythiaDYee"] = 1296200*1.15*1.0;
  _crossSections["PythiaDYLmm"] = 4428300*1.15*1.0;
  _crossSections["PythiaDYmm"] = 1296100.*1.15*1.0;
  _crossSections["PythiaDYtt"] = 3454*1.15*1.0;

  _crossSections["WWee"] = 524*1.0*1.0;
  _crossSections["WWem"] = 524*1.0*1.0;
  _crossSections["WWet"] = 524*1.0*1.0;  
  _crossSections["WWme"] = 524*1.0*1.0;
  _crossSections["WWmm"] = 524*1.0*1.0;
  _crossSections["WWmt"] = 524*1.0*1.0;
  _crossSections["WWte"] = 524*1.0*1.0;
  _crossSections["WWtm"] = 524*1.0*1.0;
  _crossSections["WWtt"] = 524*1.0*1.0;
  _crossSections["ggWWee"] = 15.3374*1.0*0.9885;
  _crossSections["ggWWem"] = 15.3434*1.0*0.9893;
  _crossSections["ggWWet"] = 14.309*1.0*0.9231;
  _crossSections["ggWWme"] = 15.2965*1.0*0.9897;
  _crossSections["ggWWmm"] = 15.3298*1.0*0.9888;
  _crossSections["ggWWmt"] = 14.3958*1.0*0.9297;
  _crossSections["ggWWte"] = 14.2896*1.0*0.3268;
  _crossSections["ggWWtm"] = 14.2896*1.0*0.9236;
  _crossSections["ggWWtt"] = 5.0666*1.0*0.9272; 

  _crossSections["WpZlvll"] = 159.24*1.0*1.0;
  _crossSections["WpZlvqq"] = 1688.9*1.0*1.0;
  _crossSections["WpZqqll"] = 498.36*1.0*1.0;
  _crossSections["WmZlvqq"] = 912.64*1.0*1.0;
  _crossSections["WmZlvll"] = 86.05*1.0*1.0;
  _crossSections["WmZqqll"] = 269.3*1.0*1.0;
  _crossSections["WpZtvll"] = 79.62*1.0*1.0;
  _crossSections["WpZlvtt"] = 79.62*1.0*1.0;
  _crossSections["WpZtvtt"] = 39.81*1.0*1.0;
  _crossSections["WmZtvll"] = 43.02*1.0*1.0;
  _crossSections["WmZlvtt"] = 43.02*1.0*1.0;
  _crossSections["WmZtvtt"] = 21.51*1.0*1.0;
  _crossSections["WpZqqtt"] = 249.18*1.0*1.0;
  _crossSections["WmZqqtt"] = 134.65*1.0*1.0;

  _crossSections["Wmevg"] = 18530*1.0*1.0;
  _crossSections["Wmmvg"] = 18620*1.0*1.0;
  _crossSections["Wmtvg"] = 1680*1.0*1.0;
  _crossSections["Wmhdgg"] = 118615;
  _crossSections["Wmlepgg"] = 118616;
  _crossSections["Wpevg"] = 27964*1.0*1.0;
  _crossSections["Wpmvg"] = 27939*1.0*1.0;
  _crossSections["Wptvg"] = 25417*1.0*1.0;
  _crossSections["Wphdgg"] = 118617;
  _crossSections["Wplepgg"] = 118618;

  _crossSections["ZZllll"] = 24.68*1.0*1.0;
  _crossSections["ZZllqq"] = 523.54*1.0*1.0;
  _crossSections["ZZllvv"] = 150.33*1.0*1.0;
  _crossSections["ZZttvv"] = 5689.5*1.0*1.0;
  _crossSections["ZZ2l2t"] = 24.68*1.0*1.0;
  _crossSections["ZZ4t"] = 6.17*1.0*1.0;

  _crossSections["ttbar"] = 164570*1.0*0.5562;
  _crossSections["ttbar2"] = 164570*1.0*0.5562;
  //Acer
  _crossSections["tb-e"] = 6930*1.0*1.0;
  _crossSections["tb-m"] = 6930*1.0*1.0;
  _crossSections["tb-t"] = 6930*1.0*1.0;
  _crossSections["tqb-e"] = 358.83*1.0*1.0;
  _crossSections["tqb-m"] = 358.83*1.0*1.0;
  _crossSections["tqb-t"] = 358.83*1.0*1.0;
  _crossSections["tW"] = 15600*1.0*1.0;

  //MC@Nlo/Jimmy
//   _crossSections["tb-e"] = 6340*1.0*1.0;
//   _crossSections["tb-m"] = 6340*1.0*1.0;
//   _crossSections["tb-t"] = 6340*1.0*1.0;
//   _crossSections["tqb-e"] = 430*1.0*1.0;
//   _crossSections["tqb-m"] = 430*1.0*1.0;
//   _crossSections["tqb-t"] = 430*1.0*1.0;
//   _crossSections["tW"] = 13100*1.0*1.0;

  _crossSections["fullZmmNp5"] = 1.0;
  _crossSections["fulltq-m"] = 1.0;
}

void BaseAnalysis::FillSampleWeights()
{
  _sampleWeights["testskim"] = 1.0;
  _sampleWeights["fullZmmNp5"] = 1.0;
  _sampleWeights["fulltq-m"] = 1.0;

}

void BaseAnalysis::AddFile(std::string chainName, std::string dataset, std::string fileName) {
  std::string fullFileName = dataset + "/" + fileName;

  if (_debug) std::cout << "Chain " << chainName << ", adding file " << dataset << "/" << fileName << std::endl; 
  
  std::map<std::string,std::vector<std::string> >::iterator iMap = _fileNameMap.find(chainName); 
  if (iMap == _fileNameMap.end()) {    
    _fileNameMap[chainName] = std::vector<std::string>();    
  }
  _fileNameMap[chainName].push_back(fullFileName);
}

std::vector<std::string> BaseAnalysis::GetAllSamples(std::string sample)
{
  std::vector<std::string> names;

  if (sample.compare("smww") == 0)
  {
    names.push_back("WWee");
    names.push_back("WWem");
    names.push_back("WWet");
    names.push_back("WWme");
    names.push_back("WWmm");
    names.push_back("WWmt");
//     names.push_back("WWte");
//     names.push_back("WWtm");
//     names.push_back("WWtt");
//     names.push_back("ggWWee");
//     names.push_back("ggWWem");
//     names.push_back("ggWWet");
//     names.push_back("ggWWme");
//     names.push_back("ggWWmm");
//     names.push_back("ggWWmt");
//     names.push_back("ggWWte");
//     names.push_back("ggWWtm");
//     names.push_back("ggWWtt");
  }
  else if (sample.compare("singleTop") == 0)
  {
//     names.push_back("tW");
    names.push_back("tb-e");
    names.push_back("tb-m");
    names.push_back("tb-t");
    names.push_back("tqb-e");
    names.push_back("tqb-m");
    names.push_back("tqb-t");
  }
  else if (sample.compare("singleW") == 0)
  {
    names.push_back("WevNp0");
    names.push_back("WevNp1");
    names.push_back("WevNp2");
    names.push_back("WevNp3");
    names.push_back("WevNp4");
    names.push_back("WevNp5");
    names.push_back("WmvNp0");
    names.push_back("WmvNp1");
    names.push_back("WmvNp2");
    names.push_back("WmvNp3");
    names.push_back("WmvNp4");
    names.push_back("WmvNp5");
  }
  else if (sample.compare("Wev") == 0)
  {
    names.push_back("WevNp0");
    names.push_back("WevNp1");
    names.push_back("WevNp2");
    names.push_back("WevNp3");
    names.push_back("WevNp4");
    names.push_back("WevNp5"); 
  }
  else if (sample.compare("Wmv") == 0)
  {
    names.push_back("WmvNp0");
    names.push_back("WmvNp1");
    names.push_back("WmvNp2");
    names.push_back("WmvNp3");
    names.push_back("WmvNp4");
    names.push_back("WmvNp5");
  }
  else if (sample.compare("singleZ") == 0)
  {
    names.push_back("ZeeNp0");
    names.push_back("ZeeNp1");
    names.push_back("ZeeNp2");
    names.push_back("ZeeNp3");
    names.push_back("ZeeNp4");
    names.push_back("ZeeNp5");
    names.push_back("ZmmNp0");
    names.push_back("ZmmNp1");
    names.push_back("ZmmNp2");
    names.push_back("ZmmNp3");
    names.push_back("ZmmNp4");
    names.push_back("ZmmNp5");
  }
  else if (sample.compare("Zee") == 0)
  {
    names.push_back("ZeeNp0");
    names.push_back("ZeeNp1");
    names.push_back("ZeeNp2");
    names.push_back("ZeeNp3");
    names.push_back("ZeeNp4");
    names.push_back("ZeeNp5");
  }
  else if (sample.compare("Zmm") == 0)
  {
    names.push_back("ZmmNp0");
    names.push_back("ZmmNp1");
    names.push_back("ZmmNp2");
    names.push_back("ZmmNp3");
    names.push_back("ZmmNp4");
    names.push_back("ZmmNp5");
  }
  else if (sample.compare("jets") == 0)
  {
    names.push_back("J0");
    names.push_back("J1");
    names.push_back("J2");
    names.push_back("J3");
    names.push_back("J4");
    names.push_back("J5");
    names.push_back("J6");
  }
  else if (sample.compare("singleZee") == 0)
  {
    names.push_back("shellZee");
    names.push_back("dyee");
  }
  else if (sample.compare("singleZmm") == 0)
  {
    names.push_back("shellZmm");
    names.push_back("dymm");
  }
  else
  {
    names.push_back(sample);
  }

  return(names);
}

//########################################################################################

double BaseAnalysis::CalculateSampleWeight(std::string sample)
{
  TChain* tree = GetChain(sample);
  //1. Set branch address so I can access EventInfo branch.
  MTree::EventInfo* info = new MTree::EventInfo();
  tree->SetBranchAddress("PrimaryVertices", &_primaryVertices);
  tree->SetBranchAddress("EventInfo",&info);

  //1.5 Get the TPileupReweighting class set up
  TPileupReweighting* pileup = new TPileupReweighting("pileup_reweighter");
  std::string name = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/mc10b/pv-hists/mc/" + convertNameToNum(sample) + ".root";
//   std::cout << name << std::endl;
  int isGood = pileup->initialize("ilumicalc_histograms_None_178044-187815.root",
                                  "avgintperbx",
                                  name.c_str(),
                                  "mu");
  if (isGood != 0)
  {
    std::cout << "TPileupReweighting object did not initialize properly!" << std::endl;
    std::cout << "Status Code: " << isGood << std::endl;
  }

  //2. Loop over events and add up event weights.
  int eventNr = 0;
//   int numZero = 0;
  double weightSum = 0.0;
  double rawWeightSum = 0.0;
  while (tree->GetEntry(eventNr++))
  {
    if (eventNr % 10000 == 0)
    {
      std::cout << "Analyzed " << eventNr <<" events." << std::endl;
    }

    double weight = info->GeneratorEventWeight();
    rawWeightSum += weight;

    float cor = 0;
    int status = pileup->getPileupWeight(info->LumiBlock(),cor);
    if (status < 0)
    {
      std::cout << "reweighting failed!  returned status " << status << std::endl;
    }
//     if (cor == 0)
//     {
//       std::cout << "reweighting doesn't seem to have changed anything . . ." << std::endl;
//     }
    weight *= cor;

    weightSum += weight;
  }

  delete info;
  std::cout<<"rawWeight: " <<rawWeightSum<<std::endl;
  std::cout<<"weightSum: "<<weightSum<<std::endl;

  return(weightSum);
}

//#######################################################################################

std::vector<std::string> BaseAnalysis::splitString(std::string phrase)
{
  std::stringstream ss(phrase);
  std::string buffer;
  std::vector<std::string> pieces;

  while(ss >> buffer)
  {
    pieces.push_back(buffer);
  }

  return(pieces);
}

//########################################################################################

void BaseAnalysis::FillSetNumbers()
{
  _setNumbers["testhww150"] = "109073";

  _setNumbers["ggfhww150"] = "116707";
  _setNumbers["vbfhww150"] = "125008";

  _setNumbers["WWee"] = "105921";
  _setNumbers["WWem"] = "105922";
  _setNumbers["WWet"] = "105923";
  _setNumbers["WWmm"] = "105924";
  _setNumbers["WWme"] = "105925";
  _setNumbers["WWmt"] = "105926";
  _setNumbers["WWtt"] = "105927";
  _setNumbers["WWte"] = "105928";
  _setNumbers["WWtm"] = "105929";
  _setNumbers["ggWWee"] = "106011";
  _setNumbers["ggWWem"] = "106012";
  _setNumbers["ggWWet"] = "106013";
  _setNumbers["ggWWmm"] = "106014";
  _setNumbers["ggWWme"] = "106015";
  _setNumbers["ggWWmt"] = "106016";
  _setNumbers["ggWWtt"] = "106017";
  _setNumbers["ggWWte"] = "106018";
  _setNumbers["ggWWtm"] = "106019";

  _setNumbers["WmZlvll"] = "105971";
  _setNumbers["WmZlvqq"] = "105970";
  _setNumbers["WmZlvtt"] = "106028";
  _setNumbers["WmZqqll"] = "105972";
  _setNumbers["WmZqqtt"] = "113190";
  _setNumbers["WmZtvll"] = "106027";
  _setNumbers["WmZtvtt"] = "106029";

  _setNumbers["WpZlvll"] = "105941";
  _setNumbers["WpZlvqq"] = "105940";
  _setNumbers["WpZlvtt"] = "106025";
  _setNumbers["WpZqqll"] = "105942";
  _setNumbers["WpZqqtt"] = "113191";
  _setNumbers["WpZtvll"] = "106024";
  _setNumbers["WpZtvtt"] = "106026";

  _setNumbers["ZZllll"] = "105931";
  _setNumbers["ZZllqq"] = "105930";
  _setNumbers["ZZllvv"] = "105932";
  _setNumbers["ZZttqq"] = "113193";
  _setNumbers["ZZttvv"] = "113192";
  _setNumbers["ZZ2l2t"] = "106036";
  _setNumbers["ZZ4t"] = "106037";
  _setNumbers["ZZ4l"] = "116600";

  _setNumbers["Wmevg"] = "108288";
  _setNumbers["Wmmvg"] = "108289";
  _setNumbers["Wmtvg"] = "108290";
  _setNumbers["Wmhdgg"] = "118615";
  _setNumbers["Wmlepgg"] = "118616";
  _setNumbers["Wpevg"] = "106001";
  _setNumbers["Wpmvg"] = "106002";
  _setNumbers["Wptvg"] = "106003";
  _setNumbers["Wphdgg"] = "118617";
  _setNumbers["Wplepgg"] = "118618";

  _setNumbers["WevNp0"] = "107680";
  _setNumbers["WevNp1"] = "107681";
  _setNumbers["WevNp2"] = "107682";
  _setNumbers["WevNp3"] = "107683";
  _setNumbers["WevNp4"] = "107684";
  _setNumbers["WevNp5"] = "107685";
  _setNumbers["WmvNp0"] = "107690";
  _setNumbers["WmvNp1"] = "107691";
  _setNumbers["WmvNp2"] = "107692";
  _setNumbers["WmvNp3"] = "107693";
  _setNumbers["WmvNp4"] = "107694";
  _setNumbers["WmvNp5"] = "107695";
  _setNumbers["WtvNp0"] = "107700";
  _setNumbers["WtvNp1"] = "107701";
  _setNumbers["WtvNp2"] = "107702";
  _setNumbers["WtvNp3"] = "107703";
  _setNumbers["WtvNp4"] = "107704";
  _setNumbers["WtvNp5"] = "107705";

  _setNumbers["PythiaWt"] = "106022";

  _setNumbers["WbbNp0"] = "106280";
  _setNumbers["WbbNp1"] = "106281";
  _setNumbers["WbbNp2"] = "106282";
  _setNumbers["WbbNp3"] = "106283";

  _setNumbers["ZeeNp0"] = "107650";
  _setNumbers["ZeeNp1"] = "107651";
  _setNumbers["ZeeNp2"] = "107652";
  _setNumbers["ZeeNp3"] = "107653";
  _setNumbers["ZeeNp4"] = "107654";
  _setNumbers["ZeeNp5"] = "107655";
  _setNumbers["ZmmNp0"] = "107660";
  _setNumbers["ZmmNp1"] = "107661";
  _setNumbers["ZmmNp2"] = "107662";
  _setNumbers["ZmmNp3"] = "107663";
  _setNumbers["ZmmNp4"] = "107664";
  _setNumbers["ZmmNp5"] = "107665";
  _setNumbers["ZttNp0"] = "107670";
  _setNumbers["ZttNp1"] = "107671";
  _setNumbers["ZttNp2"] = "107672";
  _setNumbers["ZttNp3"] = "107673";
  _setNumbers["ZttNp4"] = "107674";
  _setNumbers["ZttNp5"] = "107675";

  _setNumbers["PythiaZee"] = "106046";
  _setNumbers["PythiaZmm"] = "106047";
  _setNumbers["PythiaZtt"] = "106052";

  _setNumbers["dyZeeNp0"] = "116250";
  _setNumbers["dyZeeNp1"] = "116251";
  _setNumbers["dyZeeNp2"] = "116252";
  _setNumbers["dyZeeNp3"] = "116253";
  _setNumbers["dyZeeNp4"] = "116254";
  _setNumbers["dyZeeNp5"] = "116255";
  _setNumbers["dyZmmNp0"] = "116260";
  _setNumbers["dyZmmNp1"] = "116261";
  _setNumbers["dyZmmNp2"] = "116262";
  _setNumbers["dyZmmNp3"] = "116263";
  _setNumbers["dyZmmNp4"] = "116264";
  _setNumbers["dyZmmNp5"] = "116265";
  _setNumbers["dyZttNp0"] = "116270";
  _setNumbers["dyZttNp1"] = "116271";
  _setNumbers["dyZttNp2"] = "116272";
  _setNumbers["dyZttNp3"] = "116273";
  _setNumbers["dyZttNp4"] = "116274";
  _setNumbers["dyZttNp5"] = "116275";

  _setNumbers["PythiaDYLee"] = "105148";
  _setNumbers["PythiaDYee"] = "108320";
  _setNumbers["PythiaDYLmm"] = "113700";
  _setNumbers["PythiaDYmm"] = "113701";
  _setNumbers["PythiaDYtt"] = "107055";

  _setNumbers["ttbar"] = "105200";
  _setNumbers["ttbar2"] = "117200";
  _setNumbers["tW"] = "105500";
  _setNumbers["tb-e"] = "117360";
  _setNumbers["tb-m"] = "117361";
  _setNumbers["tb-t"] = "117362";
  _setNumbers["tqb-e"] = "108343";
  _setNumbers["tqb-m"] = "108344";
  _setNumbers["tqb-t"] = "108345";
}

//######################################################################################

std::string BaseAnalysis::convertNameToNum(std::string name)
{
  if (name.find("-") != string::npos && (name.find("Np0") != string::npos || name.find("ttbar") != string::npos || name.find("PythiaZ") != string::npos))
  {
    name.erase(name.end() - 2, name.end());
  }

  std::map<std::string, std::string>::iterator iMap = _setNumbers.find(name);
  if (iMap == _setNumbers.end()) 
  {
    std::cout << "No set number associated with " << name << std::endl;
    std::exit(1);
  }
  return _setNumbers[name];
}

//######################################################################################

void BaseAnalysis::GetAllNames()
{
  std::string filename = "all_sample_names.txt";
  std::ofstream outfile;
  outfile.open(filename.c_str());

  std::map<std::string,std::vector<std::string> >::iterator iMap;

  for (iMap = _fileNameMap.begin(); iMap != _fileNameMap.end(); ++iMap)
  {
    outfile << iMap->first << std::endl;
  } 

  outfile.close();

  std::cout << "file with file names generated" << std::endl;
}

//#######################################################################################

ClassImp(BaseAnalysis)
