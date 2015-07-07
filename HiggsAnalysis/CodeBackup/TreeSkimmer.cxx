#include "TreeSkimmer.h"

TreeSkimmer::TreeSkimmer()
{
  ;
}

//###################################################################################

TreeSkimmer::~TreeSkimmer()
{
  ;
}

//###################################################################################

void TreeSkimmer::dileptonSkim(std::string chainName)
{
  std::vector<std::string> samples = GetAllSamples(chainName);
  std::vector<std::string>::iterator iter;
  std::vector<std::string>::iterator end = samples.end();

  for (iter = samples.begin(); iter != end; ++iter)
  {
    std::string name = *iter;

    TChain* tree = GetChain(name);
    tree->SetBranchStatus("*",1);

    tree->SetBranchAddress("Electrons",&_electrons);
    tree->SetBranchAddress("Muons",&_muons);
    tree->SetBranchAddress("EventInfo",&_eventInfo);
    tree->SetBranchAddress("Tracks",&_tracks);

    std::string filename = name + ".root";
    TFile* outFile = new TFile(filename.c_str(), "RECREATE");
    TTree* newtree = tree->CloneTree(0);

    int entry = 0;
    int eCuts[6] = {0,0,0,0,0,0};
    int mCuts[5] = {0,0,0,0,0};
    int init = tree->GetEntriesFast();
    int final = 0;

    while (tree->GetEntry(entry++))
    {
      if (entry % 10000 == 0) std::cout << "Skimmed " << entry << " events" << std::endl;

      int nLeptons = 0;

      int nElectrons = _electrons->GetEntries();
      for (int i = 0; i != nElectrons; ++i)
      {
        MTree::Electron* elec = (MTree::Electron*)_electrons->At(i);
        MTree::Track* track = elec->GetTrack();

	eCuts[0] += 1;
        double clE = elec->ClusterE();
        double clEta = elec->ClusterEta();
        double absClEta = fabs(clEta);

        if (!track) continue;
	eCuts[1] += 1;
        int nSil = track->NrOfPixelHits() + track->NrOfSCTHits();
        double eta4v = nSil>=4?track->Eta():clEta;

        double trTheta = 2*atan(exp(-fabs(eta4v)));
        double Et = clE*sin(trTheta);

        if (Et < 10) continue;
	eCuts[2] += 1;
        if (absClEta > 2.47 || (absClEta > 1.37 && absClEta < 1.52)) continue;
	eCuts[3] += 1;
        if (elec->Author() != 1 && elec->Author() != 3) continue;
	eCuts[4] += 1;
        if (!elec->IsLoose()) continue;
	eCuts[5] += 1;

        ++nLeptons;
      }

      if (nLeptons < 2)
      {
        int nMuons = _muons->GetEntries();
        for (int i = 0; i != nMuons; ++i)
        {
  	  MTree::Muon* muon = (MTree::Muon*)_muons->At(i);
	  mCuts[0] += 1;

          int author = muon->Author();
          if (author != 5 && author != 6 && author != 7) continue;
	  mCuts[1] += 1; 
	  MTree::Track* saTrack = muon->GetStandAloneTrack();
	  MTree::Track* idTrack = muon->GetTrack();

// 	  if (!saTrack) continue;
// 	  if (!idTrack) continue;
	  mCuts[2] += 1;
          double pt = muon->Pt();
          double eta = muon->Eta();
          double absEta = fabs(eta);

          if (pt < 10) continue;
	  mCuts[3] += 1;
	  if (absEta > 2.7) continue;
	  mCuts[4] += 1;

	  ++nLeptons;
        }
      }

      if (nLeptons < 2) continue;

      newtree->Fill();
      ++final;
      outFile = newtree->GetCurrentFile();
    }
    outFile->Write();
    outFile->Close();
    std::cout << "Results written to file" << std::endl << std::endl;

    std::cout << "Total electrons: " << eCuts[0] << std::endl;
    std::cout << "Has track: " << eCuts[1] << std::endl;
    std::cout << "Et > 10: " << eCuts[2] << std::endl;
    std::cout << "within eta: " << eCuts[3] << std::endl;
    std::cout << "author 1 or 3: " << eCuts[4] << std::endl;
    std::cout << "loose: " << eCuts[5] << std::endl << std::endl;

    std::cout << "Total muons: " << mCuts[0] << std::endl;
    std::cout << "Muonboy author: " << mCuts[1] << std::endl;
    std::cout << "Has tracks: " << mCuts[2] << std::endl;
    std::cout << "pT > 10: " << mCuts[3] << std::endl;
    std::cout << "eta < 2.7: " << mCuts[4] << std::endl << std::endl;

    std::cout << "Inital number of events: " << init << std::endl;
    std::cout << "Final number of events: " << final << std::endl;
  }
}

//##################################################################################

ClassImp(TreeSkimmer)
