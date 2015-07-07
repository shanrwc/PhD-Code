#include "WireSagBase.h"

WireSagBase::WireSagBase() : _debug(false) 
{
  reco_muon_pt = new std::vector<double>();
  reco_muon_eta = new std::vector<double>();
  reco_muon_phi = new std::vector<double>();
  reco_muon_mass = new std::vector<double>();
  reco_muon_author = new std::vector<int>();
  reco_muon_iscomb = new std::vector<bool>();
  reco_muon_charge = new std::vector<int>();
  reco_muon_fitchi2 = new std::vector<double>();
  reco_muon_fitchi2dof = new std::vector<double>();
  reco_muon_matchchi2 = new std::vector<double>();
  reco_muon_matchchi2dof = new std::vector<double>();

  truth_muon_pt = new std::vector<double>();
  truth_muon_eta = new std::vector<double>();
  truth_muon_phi = new std::vector<double>();
  truth_muon_mass = new std::vector<double>();
  truth_muon_charge = new std::vector<int>();

  track_stand_pt = new std::vector<double>();
  track_stand_eta = new std::vector<double>();
  track_stand_phi = new std::vector<double>();
  track_stand_charge = new std::vector<int>();
  track_stand_ndof = new std::vector<double>();
  track_stand_chi2 = new std::vector<double>();
  track_stand_d0 = new std::vector<double>();
  track_stand_d0err = new std::vector<double>();
  track_stand_z0 = new std::vector<double>();
  track_stand_z0err = new std::vector<double>();
  track_stand_vd0 = new std::vector<double>();
  track_stand_vd0err = new std::vector<double>();
  track_stand_vz0 = new std::vector<double>();
  track_stand_vz0err = new std::vector<double>();
  track_stand_qoverp = new std::vector<double>();
  track_stand_qperr = new std::vector<double>();

  track_extra_pt = new std::vector<double>();
  track_extra_eta = new std::vector<double>();
  track_extra_phi = new std::vector<double>();
  track_extra_charge = new std::vector<int>();
  track_extra_ndof = new std::vector<double>();
  track_extra_chi2 = new std::vector<double>();
  track_extra_d0 = new std::vector<double>();
  track_extra_d0err = new std::vector<double>();
  track_extra_z0 = new std::vector<double>();
  track_extra_z0err = new std::vector<double>();
  track_extra_vd0 = new std::vector<double>();
  track_extra_vd0err = new std::vector<double>();
  track_extra_vz0 = new std::vector<double>();
  track_extra_vz0err = new std::vector<double>();
  track_extra_qoverp = new std::vector<double>();
  track_extra_qperr = new std::vector<double>();

  track_comb_pt = new std::vector<double>();
  track_comb_eta = new std::vector<double>();
  track_comb_phi = new std::vector<double>();
  track_comb_charge = new std::vector<int>();
  track_comb_ndof = new std::vector<double>();
  track_comb_chi2 = new std::vector<double>();
  track_comb_d0 = new std::vector<double>();
  track_comb_d0err = new std::vector<double>();
  track_comb_z0 = new std::vector<double>();
  track_comb_z0err = new std::vector<double>();
  track_comb_vd0 = new std::vector<double>();
  track_comb_vd0err = new std::vector<double>();
  track_comb_vz0 = new std::vector<double>();
  track_comb_vz0err = new std::vector<double>();
  track_comb_qoverp = new std::vector<double>();
  track_comb_qperr = new std::vector<double>();
  track_comb_nbhits = new std::vector<int>();
  track_comb_npixhits = new std::vector<int>();
  track_comb_nscthits = new std::vector<int>();
  track_comb_ntrthits = new std::vector<int>();

  AddAllFiles();
}

WireSagBase::~WireSagBase() 
{
}

TChain* WireSagBase::GetChain(std::string chainName) 
{
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
  TChain* chain = new TChain("WireSagTree", "test");
  for (int i = 0; i != nrFiles; ++i) {
    TString fileName(_fileNameMap[chainName].at(i).c_str());
    chain->Add(fileName);
  }
  return chain;
}

TChain* WireSagBase::GetChain(std::vector<std::string> chainNames) 
{
  //
  // Return a pointer to a new TChain. Caller has to delete the pointer!
  //
  TChain* chain = new TChain("WireSagTree");
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
      chain->Add(fileName);
    }
  }
  return chain;
}


std::vector<std::string> WireSagBase::GetKnownChains() {
  std::vector<std::string> names;
  std::map<std::string, std::vector<std::string> >::iterator it = _fileNameMap.begin(), 
    itEnd = _fileNameMap.end(); 
  for (; it != itEnd; ++it) {
    names.push_back(it->first);
  }
  return names;
}

void WireSagBase::setBranchAddresses(TChain* tree) {  
  if (!tree) return;
  tree->SetBranchStatus("*", 1);

  tree->SetBranchAddress("RunN", &run_number);
  tree->SetBranchAddress("EventN",&event_number);

  tree->SetBranchAddress("MuonN", &reco_muon_n);
  tree->SetBranchAddress("MuonPt", &reco_muon_pt);
  tree->SetBranchAddress("MuonEta", &reco_muon_eta);
  tree->SetBranchAddress("MuonPhi", &reco_muon_phi);
  tree->SetBranchAddress("MuonMass", &reco_muon_mass);
  tree->SetBranchAddress("MuonAuthor", &reco_muon_author);
  tree->SetBranchAddress("MuonIsComb", &reco_muon_iscomb);
  tree->SetBranchAddress("MuonCharge", &reco_muon_charge);
  tree->SetBranchAddress("MuonFitChi2", &reco_muon_fitchi2);
  tree->SetBranchAddress("MuonFitChi2Dof", &reco_muon_fitchi2dof);
  tree->SetBranchAddress("MuonMatchChi2", &reco_muon_matchchi2);
  tree->SetBranchAddress("MuonMatchChi2Dof", &reco_muon_matchchi2dof);

  tree->SetBranchAddress("TruthN", &truth_muon_n);
  tree->SetBranchAddress("TruthPt", &truth_muon_pt);
  tree->SetBranchAddress("TruthEta", &truth_muon_eta);
  tree->SetBranchAddress("TruthPhi", &truth_muon_phi);
  tree->SetBranchAddress("TruthMass", &truth_muon_mass);
  tree->SetBranchAddress("TruthCharge", &truth_muon_charge);

  tree->SetBranchAddress("StandN", &track_stand_n);
  tree->SetBranchAddress("StandPt", &track_stand_pt);
  tree->SetBranchAddress("StandEta", &track_stand_eta);
  tree->SetBranchAddress("StandPhi", &track_stand_phi);
  tree->SetBranchAddress("StandCharge", &track_stand_charge);
  tree->SetBranchAddress("StandNdof",&track_stand_ndof);
  tree->SetBranchAddress("StandChi2", &track_stand_chi2);
  tree->SetBranchAddress("StandD0", &track_stand_d0);
  tree->SetBranchAddress("StandD0Err", &track_stand_d0err);
  tree->SetBranchAddress("StandZ0", &track_stand_z0);
  tree->SetBranchAddress("StandZ0Err", &track_stand_z0err);
  tree->SetBranchAddress("StandQOverP", &track_stand_qoverp);
  tree->SetBranchAddress("StandQPErr", &track_stand_qperr);

  tree->SetBranchAddress("ExtraN", &track_extra_n);
  tree->SetBranchAddress("ExtraPt", &track_extra_pt);
  tree->SetBranchAddress("ExtraEta", &track_extra_eta);
  tree->SetBranchAddress("ExtraPhi", &track_extra_phi);
  tree->SetBranchAddress("ExtraCharge", &track_extra_charge);
  tree->SetBranchAddress("ExtraNdof",&track_extra_ndof);
  tree->SetBranchAddress("ExtraChi2", &track_extra_chi2);
  tree->SetBranchAddress("ExtraD0", &track_extra_d0);
  tree->SetBranchAddress("ExtraD0Err", &track_extra_d0err);
  tree->SetBranchAddress("ExtraZ0", &track_extra_z0);
  tree->SetBranchAddress("ExtraZ0Err", &track_extra_z0err);
  tree->SetBranchAddress("ExtraVD0", &track_extra_vd0);
  tree->SetBranchAddress("ExtraVD0Err", &track_extra_vd0err);
  tree->SetBranchAddress("ExtraVZ0", &track_extra_vz0);
  tree->SetBranchAddress("ExtraVZ0Err", &track_extra_vz0err);
  tree->SetBranchAddress("ExtraQOverP", &track_extra_qoverp);
  tree->SetBranchAddress("ExtraQPErr", &track_extra_qperr);

  tree->SetBranchAddress("CombN", &track_comb_n);
  tree->SetBranchAddress("CombPt", &track_comb_pt);
  tree->SetBranchAddress("CombEta", &track_comb_eta);
  tree->SetBranchAddress("CombPhi", &track_comb_phi);
  tree->SetBranchAddress("CombCharge", &track_comb_charge);
  tree->SetBranchAddress("CombNdof",&track_comb_ndof);
  tree->SetBranchAddress("CombChi2", &track_comb_chi2);
  tree->SetBranchAddress("CombD0", &track_comb_d0);
  tree->SetBranchAddress("CombD0Err", &track_comb_d0err);
  tree->SetBranchAddress("CombZ0", &track_comb_z0);
  tree->SetBranchAddress("CombZ0Err", &track_comb_z0err);
  tree->SetBranchAddress("CombVD0", &track_comb_vd0);
  tree->SetBranchAddress("CombVD0Err", &track_comb_vd0err);
  tree->SetBranchAddress("CombVZ0", &track_comb_vz0);
  tree->SetBranchAddress("CombVZ0Err", &track_comb_vz0err);
  tree->SetBranchAddress("CombQOverP", &track_comb_qoverp);
  tree->SetBranchAddress("CombQPErr", &track_comb_qperr);
  tree->SetBranchAddress("CombNBHits",&track_comb_nbhits);
  tree->SetBranchAddress("CombNPixHits",&track_comb_npixhits);
  tree->SetBranchAddress("CombNSCTHits",&track_comb_nscthits);
  tree->SetBranchAddress("CombNTRTHits",&track_comb_ntrthits);
}

void WireSagBase::includeOverflow(TH1D* hist) {

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

void WireSagBase::includeUnderflow(TH1D* hist) {
 
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

void WireSagBase::removeNegativeBins(TH1D* hist) {
  int nBins = hist->GetNbinsX();
  for (int i = 1; i != nBins+1; ++i) {
    if (hist->GetBinContent(i) < 0.0) {
      hist->SetBinContent(i, 0.0);
      hist->SetBinError(i, 0.0);
    }
  }
}

double WireSagBase::efficiency_symm(double m, double n) {
  return ( 1./n*sqrt(m + m*m/n) );
}

double WireSagBase::eff_pos_err(double m, double n) {
  return ( 
	  ( n*(1+2*m) + sqrt(n*n*(1+2*m)*(1+2*m) - 4*n*(1+n)*m*m) ) / 
	  ( 2*n*(1+n) ) 
	  ) 
    - m/n;
}
double WireSagBase::eff_neg_err(double m, double n) {
  return m/n - ( 
		( n*(1+2*m) - sqrt(n*n*(1+2*m)*(1+2*m) - 4*n*(1+n)*m*m) ) / 
		( 2*n*(1+n) ) 
		);
}

double WireSagBase::poisson_up(double y) {
  double y1 = y + 1;
  double d = 1.0 - 1.0/(9.0*y1) + 1.0/(3*TMath::Sqrt(y1));
  return y1*d*d*d;
}

double WireSagBase::poisson_down(double y) {
  if (y == 0.0) return 0.0;
  double d = 1.0 - 1.0/(9.0*y) - 1.0/(3.0*TMath::Sqrt(y));
  return y*d*d*d;
}

TGraphAsymmErrors* WireSagBase::GetEfficiencyGraph(TH1D* num, TH1D* den) {
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

void WireSagBase::AddAllFiles() 
{

  AddFile("geowstest","/home/swalch/testarea/16.0.2/WireSagTree/run","GeoWSSagTree.root");
  AddFile("rtwstest","/home/swalch/testarea/16.0.2/WireSagTree/run","GeoWSSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00001.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00002.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00003.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00004.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00005.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00006.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00007.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00008.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00009.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00010.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00011.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00012.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00013.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00014.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00015.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00016.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00017.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00018.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00019.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00020.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00021.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00022.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00023.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00024.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00025.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00026.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00027.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00028.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00029.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00030.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00031.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00032.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00033.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00034.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00035.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00036.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00037.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00038.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00039.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00040.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00041.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00042.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00043.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00044.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00045.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00046.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00047.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00048.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00049.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00050.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00051.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00052.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00053.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00054.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00055.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00056.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00057.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00058.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00059.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00060.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00061.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00062.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00063.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00064.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00065.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00066.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00067.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00068.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00069.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00070.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00071.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00072.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00073.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00074.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00075.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00076.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00077.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00078.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00079.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00080.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00081.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00082.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00083.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00084.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00085.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00086.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00087.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00088.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00089.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00090.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00091.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00092.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00093.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00094.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00095.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00096.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00097.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00098.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00099.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00100.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00101.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00102.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00103.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00104.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00105.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00106.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00107.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00108.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00109.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00110.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00111.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00112.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00113.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00114.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00115.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00116.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00117.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00118.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00119.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00120.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00121.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00122.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00123.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00124.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00125.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00126.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00127.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00128.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00129.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00130.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00131.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00132.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00133.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00134.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00135.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00136.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00137.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00138.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00139.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00140.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00141.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00142.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00143.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00144.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00145.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00146.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00147.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00148.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00149.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00150.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00151.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00152.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00153.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00154.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00155.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00156.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00157.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00158.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00159.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00160.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00161.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00162.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00163.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00164.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00165.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00166.WireSagTree.root");
  AddFile("nowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_noSag_valid4.Ver1.110115131948", "user.swalch.000378.EXT0._00167.WireSagTree.root");

  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00001.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00002.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00003.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00004.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00005.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00006.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00007.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00008.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00009.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00010.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00011.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00012.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00013.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00014.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00015.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00016.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00017.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00018.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00019.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00020.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00021.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00022.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00023.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00024.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00025.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00026.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00027.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00028.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00029.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00030.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00031.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00032.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00033.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00034.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00035.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00036.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00037.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00038.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00039.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00040.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00041.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00042.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00043.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00044.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00045.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00046.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00047.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00048.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00049.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00050.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00051.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00052.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00053.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00054.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00055.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00056.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00057.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00058.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00059.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00060.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00061.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00062.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00063.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00064.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00065.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00066.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00067.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00068.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00069.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00070.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00071.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00072.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00073.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00074.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00075.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00076.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00077.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00078.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00079.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00080.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00081.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00082.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00083.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00084.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00085.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00086.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00087.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00088.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00089.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00090.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00091.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00092.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00093.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00094.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00095.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00096.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00097.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00098.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00099.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00100.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00101.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00102.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00103.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00104.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00105.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00106.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00107.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00108.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00109.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00110.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00111.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00112.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00113.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00114.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00115.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00116.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00117.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00118.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00119.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00120.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00121.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00122.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00123.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00124.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00125.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00126.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00127.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00128.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00129.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00130.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00131.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00132.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00133.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00134.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00135.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00136.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00137.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00138.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00139.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00140.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00141.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00142.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00143.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00144.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00145.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00146.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00147.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00148.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00149.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00150.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00151.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00152.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00153.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00154.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00155.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00156.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00157.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00158.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00159.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00160.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00161.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00162.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00163.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00164.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00165.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00166.WireSagTree.root");
  AddFile("geowiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoSag_valid2.Ver1.110115202230", "user.swalch.000380.EXT0._00167.WireSagTree.root");

  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00001.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00002.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00003.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00004.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00005.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00006.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00007.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00008.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00009.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00010.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00011.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00012.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00013.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00014.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00015.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00016.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00017.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00018.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00019.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00020.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00021.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00022.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00023.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00024.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00025.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00026.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00027.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00028.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00029.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00030.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00031.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00032.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00033.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00034.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00035.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00036.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00037.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00038.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00039.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00040.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00041.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00042.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00043.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00044.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00045.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00046.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00047.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00048.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00049.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00050.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00051.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00052.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00053.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00054.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00055.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00056.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00057.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00058.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00059.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00060.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00061.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00062.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00063.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00064.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00065.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00066.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00067.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00068.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00069.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00070.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00071.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00072.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00073.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00074.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00075.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00076.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00077.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00078.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00079.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00080.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00081.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00082.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00083.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00084.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00085.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00086.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00087.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00088.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00089.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00090.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00091.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00092.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00093.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00094.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00095.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00096.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00097.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00098.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00099.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00100.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00101.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00102.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00103.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00104.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00105.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00106.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00107.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00108.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00109.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00110.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00111.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00112.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00113.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00114.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00115.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00116.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00117.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00118.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00119.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00120.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00121.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00122.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00123.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00124.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00125.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00126.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00127.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00128.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00129.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00130.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00131.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00132.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00133.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00134.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00135.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00136.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00137.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00138.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00139.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00140.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00141.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00142.WireSagTree.root");
  AddFile("rtwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geortSag_valid8.Ver2.110314210137", "user.swalch.000470.EXT0._00143.WireSagTree.root");

  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00001.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00002.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00003.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00004.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00005.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00006.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00007.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00008.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00009.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00010.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00011.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00012.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00013.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00014.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00015.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00016.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00017.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00018.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00019.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00020.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00021.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00022.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00023.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00024.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00025.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00026.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00027.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00028.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00029.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00030.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00031.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00032.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00033.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00034.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00035.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00036.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00037.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00038.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00039.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00040.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00041.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00042.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00043.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00044.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00045.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00046.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00047.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00048.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00049.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00050.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00051.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00052.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00053.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00054.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00055.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00056.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00057.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00058.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00059.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00060.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00061.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00062.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00063.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00064.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00065.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00066.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00067.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00068.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00069.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00070.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00071.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00072.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00073.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00074.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00075.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00076.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00077.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00078.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00079.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00080.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00081.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00082.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00083.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00084.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00085.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00086.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00087.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00088.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00089.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00090.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00091.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00092.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00093.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00094.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00095.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00096.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00097.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00098.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00099.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00100.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00101.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00102.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00103.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00104.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00105.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00106.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00107.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00108.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00109.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00110.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00111.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00112.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00113.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00114.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00115.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00116.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00117.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00118.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00119.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00120.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00121.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00122.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00123.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00124.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00125.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00126.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00127.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00128.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00129.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00130.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00131.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00132.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00133.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00134.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00135.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00136.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00137.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00138.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00139.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00140.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00141.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00142.WireSagTree.root");
  AddFile("gcorwiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCor_valid8.Ver3.110312080553", "user.swalch.000468.EXT0._00143.WireSagTree.root");

  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00001.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00002.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00003.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00004.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00005.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00006.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00007.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00008.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00009.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00010.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00011.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00012.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00013.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00014.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00015.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00016.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00017.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00018.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00019.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00020.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00021.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00022.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00024.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00025.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00026.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00027.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00028.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00029.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00030.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00031.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00032.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00033.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00034.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00035.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00036.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00037.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00038.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00039.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00040.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00041.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00042.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00043.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00044.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00045.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00046.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00047.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00048.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00049.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00050.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00051.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00052.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00053.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00054.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00055.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00056.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00057.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00058.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00059.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00060.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00061.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00062.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00063.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00064.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00065.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00066.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00067.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00068.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00069.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00070.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00071.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00072.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00073.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00074.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00075.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00076.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00077.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00078.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00079.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00080.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00081.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00082.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00083.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00084.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00085.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00086.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00087.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00088.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00089.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00090.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00091.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00092.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00093.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00094.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00095.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00096.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00097.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00098.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00099.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00100.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00101.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00102.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00103.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00104.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00105.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00106.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00107.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00108.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00109.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00110.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00111.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00112.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00113.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00114.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00115.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00116.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00117.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00118.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00119.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00120.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00121.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00122.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00123.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00124.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00125.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00126.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00127.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00128.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00129.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00130.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00131.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00132.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00133.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00134.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00135.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00136.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00137.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00138.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00139.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00140.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00141.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00142.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150", "user.swalch.000476.EXT0._00143.WireSagTree.root");
  AddFile("gcorspecial", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geoCorSpec_valid1.Ver3.110324171150_r1", "user.swalch.000476.EXT0._00023.WireSagTree.root.1");

  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504_r1", "user.swalch.000530.EXT0._00156.WireSagTree.root.1");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00001.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00002.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00003.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00004.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00005.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00006.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00007.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00008.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00009.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00010.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00011.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00012.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00013.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00014.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00015.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00016.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00017.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00018.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00019.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00020.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00021.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00022.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00023.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00024.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00025.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00026.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00027.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00028.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00029.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00030.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00031.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00032.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00033.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00034.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00035.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00036.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00037.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00038.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00039.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00040.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00041.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00042.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00043.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00044.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00045.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00046.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00047.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00048.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00049.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00050.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00051.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00052.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00053.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00054.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00055.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00056.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00057.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00058.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00059.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00060.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00061.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00062.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00063.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00064.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00065.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00066.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00067.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00068.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00069.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00070.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00071.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00072.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00073.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00074.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00075.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00076.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00077.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00078.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00079.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00080.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00081.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00082.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00083.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00084.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00085.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00086.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00087.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00088.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00089.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00090.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00091.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00092.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00093.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00094.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00095.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00096.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00097.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00098.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00099.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00100.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00101.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00102.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00103.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00104.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00105.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00106.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00107.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00108.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00109.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00110.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00111.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00112.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00113.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00114.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00115.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00116.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00117.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00118.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00119.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00120.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00121.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00122.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00123.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00124.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00125.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00126.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00127.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00128.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00129.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00130.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00131.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00132.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00133.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00134.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00135.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00136.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00137.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00138.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00139.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00140.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00141.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00142.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00143.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00144.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00145.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00146.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00147.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00148.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00149.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00150.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00151.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00152.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00153.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00154.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00155.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00157.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00158.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00159.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00160.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00161.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00162.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00163.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00164.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00165.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00166.WireSagTree.root");
  AddFile("rt2wiresag", "/net/s3_datad/MTree/WireSagTree/user.swalch.user.swalch.mc10_7TeV.107236.singlepart_mu1000.ESD_1M_geort2Sag_valid1.Ver3.110413105504", "user.swalch.000530.EXT0._00167.WireSagTree.root");
}

void WireSagBase::AddFile(std::string chainName, std::string dataset, std::string fileName) {
  std::string fullFileName = dataset + "/" + fileName;

  if (_debug) std::cout << "Chain " << chainName << ", adding file " << dataset << "/" << fileName << std::endl; 
  
  std::map<std::string,std::vector<std::string> >::iterator iMap = _fileNameMap.find(chainName); 
  if (iMap == _fileNameMap.end()) {    
    _fileNameMap[chainName] = std::vector<std::string>();    
  }
  _fileNameMap[chainName].push_back(fullFileName);
}

std::vector<std::string> WireSagBase::GetAllSamples(std::string sample)
{
  std::vector<std::string> names;

  if (sample.compare("smww") == 0)
  {
    names.push_back("WWee");
    names.push_back("WWem");
//     names.push_back("WWet");
    names.push_back("WWme");
    names.push_back("WWmm");
//     names.push_back("WWmt");
//     names.push_back("WWte");
//     names.push_back("WWtm");
//     names.push_back("WWtt");
    names.push_back("ggWWee");
    names.push_back("ggWWem");
//     names.push_back("ggWWet");
    names.push_back("ggWWme");
    names.push_back("ggWWmm");
//     names.push_back("ggWWmt");
//     names.push_back("ggWWte");
//     names.push_back("ggWWtm");
//     names.push_back("ggWWtt");
  }
  else if (sample.compare("singleTop") == 0)
  {
    names.push_back("tW");
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
  else if (sample.compare("data1-15") == 0)
  {
//     names.push_back("152166");
//     names.push_back("152214");
//     names.push_back("152221");
//     names.push_back("152345");
//     names.push_back("152409");
//     names.push_back("152441");
//     names.push_back("152508");
    names.push_back("152777");
    names.push_back("152844");
    names.push_back("152845");
    names.push_back("152878");
    names.push_back("152933");
    names.push_back("152994");
    names.push_back("153030");
    names.push_back("153134");
  }
  else if (sample.compare("data16-30") == 0)
  {
    names.push_back("153136");
    names.push_back("153159");
    names.push_back("153200");
    names.push_back("153565");
    names.push_back("154810");
    names.push_back("154813");
    names.push_back("154815");
    names.push_back("154817");
    names.push_back("155073");
    names.push_back("155112");
    names.push_back("155116");
    names.push_back("155118");
    names.push_back("155160");
    names.push_back("155228");
    names.push_back("155280");
  }
  else if (sample.compare("data31-36") == 0)
  {
    names.push_back("155569");
    names.push_back("155634");
    names.push_back("155669");
    names.push_back("155678");
    names.push_back("155697");
    names.push_back("156682");

  }
  else if (sample.compare("dijet") == 0)
  {
    names.push_back("dijet0");
    names.push_back("dijet1");
    names.push_back("dijet2");
    names.push_back("dijet3");
    names.push_back("dijet4");
  }
  else
  {
    names.push_back(sample);
  }

  return(names);
}



ClassImp(WireSagBase)
