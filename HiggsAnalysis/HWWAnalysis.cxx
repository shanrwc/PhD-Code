#include "HWWAnalysis.h"

HWWAnalysis::HWWAnalysis()
{
  selector = new HObjectSelector();
  cutflow = new CutflowClass();
  cutlist = new CutlistClass();
  histos = new HHistosClass();
  zregion = new ZControlClass();
  code = new CutCodeClass();
  wwevent = new WWEventClass();

  pileup = new TPileupReweighting("pileup_reweighter");

//   reweight = new ggFReweighting("PowHegMSSM",500,"Mean","./");

  metForWW = new MTree::MissingET();
  metRel = new MTree::MissingET();

  febEs = 0;
  badEs = 0;
  febJs = 0;
  badJs = 0;

  Ptll = new TH1D("truth_ptll","truth_ptll",2500,0,250);
  Mll = new TH1D("truth_mll","truth_mll",2500,0,250);
}

//#######################################################################################

void HWWAnalysis::Initialize(std::string sample, TFile* file)
{
  int isGood;
  std::string name;
  std::string hist;
  if (sample.find("data") == string::npos && sample.find("test") == string::npos)
  {
    name = "/net/s3_dataa/MTree/MTree-16-01-15/AOD/mc10b/pv-hists/mc/" + convertNameToNum(sample) + ".root";
    hist = "mu";
  }
  else
  {
    name = "mu_mc10b.root";
    hist = "mu_mc10b";
  }
    isGood = pileup->initialize("ilumicalc_histograms_None_178044-187815.root",
				"avgintperbx",
				name.c_str(),
				hist.c_str());
  if (isGood != 0)
  {
    std::cout << "TPileupReweighting object did not initialize properly!" << std::endl;
    std::cout << "Status Code: " << isGood << std::endl;
  }

  file->cd();

  selector->initJESTool();

  file->cd();

  calib = new Analysis::CalibrationDataInterfaceROOT("SV0","BTagCalibration.env");
  ajet = new Analysis::CalibrationDataVariables();
  ajet->jetAuthor = "AntiKt4Topo";

  file->cd();

  if (sample.find("hww") != std::string::npos)
  {
    std::string hmass;

    for (int i = 0; i != (int)sample.length(); ++i)
    {
      if (sample[i] == '-') break;
      if (isdigit(sample[i]))
      {
	hmass += sample[i];
      }
    }
    reweight = new ggFReweighting("PowHeg",makeDouble(hmass),"Mean","./");
    file->cd();
  }

}

//#######################################################################################

HWWAnalysis::~HWWAnalysis()
{
  delete pileup;
}

//#######################################################################################

void HWWAnalysis::h2wwCutflow(std::string sample, TFile* file, bool lowMass, double hMass, bool comset)
{
  if (_debug)
  {
    std::cout << "Calling h2wwCutflow" << std::endl;
    if (comset)
    {
      std::cout << "Running with generator event weights only" << std::endl;
    }
  }

  //Set up necessary flags, cutflows, etc.
  bool isMC = false;
  if ((sample.find("EGamma") == string::npos) && (sample.find("Muons") == string::npos) && ((sample.find("data") == string::npos) || (sample == "prldata")))
  {
    isMC = true;
  }

  std::string filename = "event-summaries/" + sample + "_evsum.txt";
  std::ofstream outfile;
  outfile.open(filename.c_str());

//   std::string jelFileName = "pileup_info.txt";
//   std::ofstream jelFile;
//   jelFile.open(jelFileName.c_str());

  std::vector<std::string> channels;
  channels.push_back("ee_zeroj");
  channels.push_back("em_zeroj");
  channels.push_back("mm_zeroj");
  channels.push_back("ee_onej");
  channels.push_back("em_onej");
  channels.push_back("mm_onej");
  channels.push_back("ee_twoj");
  channels.push_back("em_twoj");
  channels.push_back("mm_twoj");
  cutflow->setChannels(channels);

  if (_debug)
  {
    std::cout << "Setting up Good Run List" << std::endl;
  }
  TString list = "/home/swalch/macro-cosmos/HiggsAnalysis/GoodRunLists/higgs_b2-k6_grl.xml";
  Root::TGoodRunsListReader foo(list);
  foo.Interpret();
  Root::TGoodRunsList grl1 =  foo.GetMergedGoodRunsList();

  TH1D* mu_PV = new TH1D("mu_PV","mu_PV",30,-0.5,29.5);
  TH1D* mu_PV2 = new TH1D("mu_PV2","mu_PV2",30,-0.5,29.5);

//   reweight->printInfo();

  if (_debug)
  {
    std::cout << "Assembling component samples" << std::endl;
  }
  std::vector<std::string> samples = GetAllSamples(sample);
  std::vector<std::string>::iterator iter;
  std::vector<std::string>::iterator end = samples.end();

  for (iter = samples.begin(); iter != end; ++iter)
  {
    std::string sampleName = *iter;
    TChain* chain = GetChain(sampleName);
    setBranchAddresses(chain);

    Initialize(sample,file);

    double sampleWeight = 0.0;
    double sampleXSec = 1.0;
    if (isMC && !comset)
    {
      sampleWeight = GetSampleWeight(sampleName);
      sampleXSec = GetCrossSection(sampleName);
//       std::cout << "sampleWeight = " << sampleWeight << std::endl;
//       std::cout << "sampleXSec = " << sampleXSec << std::endl;
    }

    //////////////////////////////////////////////////////////////////
    /////////////////////Starting Event Loop Here/////////////////////
    //////////////////////////////////////////////////////////////////
    if (_debug)
    {
      std::cout << "Entering event loop" << std::endl;
    }
    int eventNr = 0;
    double weight = 1.0;
    while(chain->GetEntry(eventNr++))
    {
      if (_debug)
      {
	std::cout << "Analyzing event " << std::endl;
      }
      if (eventNr % 10000 == 0)
      {
	std::cout << "h2wwCutflow has analyzed " << eventNr << " events." << std::endl;
      }
//       if (eventNr > 199)
//       {
// 	eventNr = 1000000000;
//       }


      int RunNumber = 0;
      if (_debug)
      {
	std::cout << "Calculating event weight" << std::endl;
      }
      if (isMC)
      {
	RunNumber = 0;
	if (comset)
	{
	  weight = _eventInfo->GeneratorEventWeight();
	}
	else
	{
 	  weight = sampleXSec/sampleWeight*(_eventInfo->EventWeight());
	}
      }
      else
      {
	RunNumber = _eventInfo->RunNumber();
      }
      if (_debug)
      {
	std::cout << "Calculating higgs pT reweight" << std::endl;
      }
      weight *= getHiggsPtWeight(sample);
      if (_debug)
      {
	std::cout << "setting up channels and records" << std::endl;
      }
      channels.clear();
      channels.push_back("ee_zeroj");
      channels.push_back("em_zeroj");
      channels.push_back("mm_zeroj");
      channels.push_back("ee_onej");
      channels.push_back("em_onej");
      channels.push_back("mm_onej");
      channels.push_back("ee_twoj");
      channels.push_back("em_twoj");
      channels.push_back("mm_twoj");      
      int cutNr = 0;
      cutNr = cutflow->addEvent(cutNr,1.0,channels);
      cutNr = cutflow->addEvent(cutNr,weight,channels);

      int plotNr = 0;

      bool isEvent = false;
      int evNum = _eventInfo->EventNumber();
//       if (evNum == 35652677)
//       {
// 	isEvent = true;
// 	outfile << "Found event " << evNum << "!" << std::endl;
//       }

      if (_debug)
      {
	std::cout << "counting primary vertices" << std::endl;
      }
      bool goodPV = false;
      int nPVs;
      selectGoodPVs(goodPV,nPVs,_eventInfo->EventNumber());
      if (isMC)
      {
        mu_PV->Fill(nPVs,_eventInfo->GeneratorEventWeight()/fabs(_eventInfo->GeneratorEventWeight()));
      }
      else
      {
	mu_PV->Fill(nPVs,1.0);
      }

      //Good Run List
      if (_debug)
      {
	std::cout << "Checking good run list" << std::endl;
      }
      bool passed = true;
      if (!isMC)
      {
	int runNr = _eventInfo->RunNumber();
	int lumiBlock = _eventInfo->LumiBlock();
	passed = grl1.HasRunLumiBlock(runNr,lumiBlock);
      }
      if (!passed) continue;
      if (usePVWeight(sample))
      {
	float cor = 0;
	int status = pileup->getPileupWeight(_eventInfo->LumiBlock(),cor);
	if (status < 0)
	{
	  std::cout << "reweighting failed! Returned status " << status << std::endl;
	}
	weight *= cor;
      }
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      //First
      cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());
      if (isEvent) outfile << "passed GRL" << std::endl;

      //AND here take a quick pause to select objects, 
      //for comparing-to-Aaron's-number purposes.
      if (_debug)
      {
	std::cout << "collecting trigger objects" << std::endl;
      }
      int nTrigObjs = _triggerObjects->GetEntriesFast();
      std::vector<MTree::TriggerObject*> trigObjs;
      for (int i = 0; i != nTrigObjs; ++i)
      {
	trigObjs.push_back((MTree::TriggerObject*)_triggerObjects->At(i));
      }

      int nL1TrigObjs = _L1MuonTriggerROIs->GetEntriesFast();
      std::vector<MTree::L1TriggerROI*> L1TrigObjs;
      for (int i = 0; i != (int)nL1TrigObjs; ++i)
      {
	L1TrigObjs.push_back((MTree::L1TriggerROI*)_L1MuonTriggerROIs->At(i));
      }

      if (_debug)
      {
	std::cout << "selecting muons" << std::endl;
      }
      int nM = _muons->GetEntriesFast();
      std::vector<MTree::Muon*> muons;
      for (int i = 0; i != nM; ++i)
      {
	muons.push_back((MTree::Muon*)_muons->At(i));
      }
      double m_scale = 1.0;
      selector->selectMuons(isMC,_eventInfo->RunNumber(),_eventInfo->EventNumber(),muons,m_scale,trigObjs,L1TrigObjs);

      if (m_scale < 0.5)
      {
	std::cout << "Found a low muon scaling factor of " << m_scale << "!" << std::endl;
      }

      if (_debug)
      {
        std::cout << "selecting electrons" << std::endl;
      }
      int nE = _electrons->GetEntriesFast();
      std::vector<MTree::Electron*> electrons;
      for (int i = 0; i != nE; ++i)
      {
	electrons.push_back((MTree::Electron*)_electrons->At(i));
      }
      double e_scale = 1.0;
      selector->selectElectrons(isMC,RunNumber,_eventInfo->EventNumber(),electrons,e_scale,trigObjs,muons);

      if (e_scale < 0.5)
      {
	std::cout << "Found a low electron scaling factor of " << e_scale << "!" << std::endl;
      }

      if (_debug)
      {
	std::cout << "selecting jets" << std::endl;
      }
      int nJets = _jets->GetEntries();
      std::vector<MTree::Jet*> jets;
      for (int i = 0; i != nJets; ++i)
      {
	jets.push_back((MTree::Jet*)_jets->At(i));
      }
      selector->selectJets(_eventInfo->RunNumber(),_eventInfo->EventNumber(),isMC,jets, muons);

      if (_debug)
      {
	std::cout << "removing overlaps" << std::endl;
      }
      removeOverlap(0.1,muons,electrons);
      removeOverlap(0.1,electrons);
      removeOverlap(0.3,electrons,jets);

      if (_debug)
      {
	std::cout << "create lepton vector" << std::endl;
      }
      std::vector<MTree::Lepton*> leptons;
      for (int i = 0; i != (int)electrons.size(); ++i)
      {
	leptons.push_back(electrons[i]);
      }
      for (int i = 0; i != (int)muons.size(); ++i)
      {
	leptons.push_back(muons[i]);
      }

      if (_debug)
      {
	std::cout << "ordering by pT" << std::endl;
      }
      orderByPt(electrons);
      orderByPt(muons);
      orderByPt(leptons);
      orderByPt(jets);

      if (_debug)
      {
	std::cout << "correcting MET" << std::endl;
      }
      correctMET(leptons,jets);
      double met = metRel->MET();
      double mT = code->getMT(leptons,metForWW);

      if (_debug)
      {
	std::cout << "calculating mll" << std::endl;
      }
      std::vector<TLorentzVector> vecs;
      for (int i = 0; i != (int)leptons.size(); ++i)
      {
	TLorentzVector temp;
	  temp.SetPtEtaPhiM(leptons[i]->Pt(),leptons[i]->Eta(),leptons[i]->Phi(),leptons[i]->M());
	vecs.push_back(temp);
      }
      double mll;
      if ((int)leptons.size() > 1)
      {
        TLorentzVector combo = vecs[0] + vecs[1];
        mll = combo.M();
      }
      else
      {
	mll = 0;
      }
      //AND now back to the regularly scheduled code.

      // . . . after I make some truth level plots.

      int nTruthParticles = _truthParticles->GetEntriesFast();
      std::vector<MTree::TruthParticle*> daughters;
      for (int i = 0; i != nTruthParticles; ++i)
      {
	MTree::TruthParticle* truth = (MTree::TruthParticle*)_truthParticles->At(i);
	if ((TMath::Abs(truth->ParentPdgID()) == 23) && ((TMath::Abs(truth->PdgID()) == 11) || (TMath::Abs(truth->PdgID()) == 13)))
	{
	  daughters.push_back(truth);
        }
      }
      if (daughters.size() == 2)
      {
	TLorentzVector temp = *daughters[0] + *daughters[1];
	Ptll->Fill(temp.Pt(),weight);
	Mll->Fill(temp.M(),weight);
      }

      //Primary Vertex Selection
      if (_debug)
      {
	std::cout << "PV selection" << std::endl;
      }
      if (!goodPV) continue;
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      //second
      cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());
      if (isEvent) outfile << "passed PV" << std::endl;

      //Check for LAr flag goodness
      if (!isMC && _eventInfo->LArError() != 0) continue;
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      //third
      cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());

      //Event Cleaning
      if (_debug)
      {
	std::cout << "checking cleanliness" << std::endl;
      }
      std::string result;
      if (!isMC && badJetsPresent(leptons, result))
      {
	continue;
      }
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      //fourth
      cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());
      if (isEvent) outfile << "passed jet cleaning" << std::endl;

      //Check for dead front-end board
      bool passFEB = passDeadFEB(isMC,weight,_eventInfo->RunNumber(),_eventInfo->EventNumber());
      if (!passFEB) continue;
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      //fifth
      cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());
      if (isEvent) outfile << "passed dead FEB" << std::endl;

      //PAUSE HERE to add a couple extraneous cuts, useful only for making sure
      //that my cutflow agrees with Aaron's.
//       if ((int)leptons.size() == 0) continue;
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      //sixth
      cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());

//       if ((int)leptons.size() == 1) continue;
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      //seventh
      cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());

//       if ((int)leptons.size() > 2) continue;
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      //eighth
      cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());

      //Trigger requirement
      if (_debug)
      {
	std::cout << "checking trigger" << std::endl;
      }
      std::vector<bool> triggers;
      triggers.push_back(_trigger->PassedTrigger("EF_e20_medium"));
      triggers.push_back(_trigger->PassedTrigger("EF_mu18_MG"));
      triggers.push_back(_trigger->PassedTrigger("EF_mu40_MSonly_barrel"));
      triggers.push_back(_trigger->PassedTrigger("EF_e22_medium"));
      triggers.push_back(_trigger->PassedTrigger("EF_mu18_MG_medium"));
      triggers.push_back(_trigger->PassedTrigger("EF_mu40_MSonly_barrel_medium"));

      bool numMatch = false;
      bool passTrig = selector->checkTriggers((int)electrons.size(),(int)muons.size(), triggers, _eventInfo->RunNumber(),_eventInfo->EventNumber(), isMC, numMatch);
      if (!passTrig) continue;
      channels.clear();
      channels = findChannels(triggers);

      cutNr = cutflow->addEvent(cutNr,weight,channels);
      //ninth
      cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());
      if (isEvent) outfile << "passed trigger" << std::endl;

      if (!numMatch) continue;
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      //tenth
      cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());
      if (isEvent) outfile << "passed second trigger" << std::endl;
      if (isEvent) outfile << "event has " << (int)leptons.size() << " leptons" << std::endl;

      //Exactly two leptons
      if (_debug)
      {
	std::cout << "selecting leptons" << std::endl;
      }
      if ((int)leptons.size() != 2) continue;
      channels.clear();
      channels = findChannels(leptons);
      if (usePVWeight(sample))
      {
	//Multiplying in electron scale factors
	weight *= e_scale;

	//Multiplying in muon scale factors
	weight*= m_scale;

      }
      if (_debug)
      {
	std::cout << "reweighted event" << std::endl;
      }
      channels.clear();
      channels = findChannels(leptons);
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);
      //eleventh
      cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());
      if (isEvent)
      { 
	outfile << "has 2 leptons" << std::endl;
	for (int i = 0; i != (int)leptons.size(); ++i)
	{
	  if (leptons[i]->LeptonPdgID() == 11)
	  {
	    ((MTree::Electron*)leptons[i])->PrintData(outfile);
	  }
	  else
	  {
	    ((MTree::Muon*)leptons[i])->PrintData(outfile);
	  }
	}
      }
      mu_PV2->Fill(nPVs,weight);

      //Trigger matching
      double eTrigWeight = 0;
      bool passE = selector->getETrigPass(isMC, _eventInfo->RunNumber(),eTrigWeight);
      double mTrigWeight = 0;
      bool passM = selector->getMTrigPass(isMC, _eventInfo->RunNumber(),mTrigWeight);
      if (!(passE || passM)) continue;
      if (isMC)
      {
// 	if (passE) weight *= eTrigWeight;
// 	if (passM) weight *= mTrigWeight;
        weight *= selector->getTrigSF(leptons);
      }
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());
      if (isEvent) outfile << "really really passed trigger" << std::endl;

      //Leading lepton with pT > 25
      if (_debug)
      {
	std::cout << "checking pT_leading" << std::endl;
      }
      orderByPt(leptons);
      double pT = 0;
      for (int i = 0; i != (int)leptons.size(); ++i)
      {
	double temp = leptons[i]->Pt();
       if (temp > pT) pT = temp;
      }
      if (pT <= 25.0) continue;
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);
      cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());
      if (isEvent) outfile << "passed pT_lead cut" << std::endl;

      //Leptons have opposite charge
      if (_debug)
      {
	std::cout << "checking lepton charge" << std::endl;
      }
      if (leptons[0]->Charge() == leptons[1]->Charge()) continue;
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());
      if (isEvent) outfile << "passed charge" << std::endl;

      //if leptons have same flavor, Mll > 15
      if (_debug)
      {
	std::cout << "at first dilepton mass cut" << std::endl;
      }
      if (leptons[0]->LeptonPdgID() == leptons[1]->LeptonPdgID())
      {
	channels.clear();
	channels = findChannels(leptons,(int)jets.size());
	if ((int)channels.size() > 1)
	{
	  std::cout << "channel not uniquely defined in z-mass window" << std::endl;
	}
	if (mll < MLLFLOORSF) continue;
	zregion->addEvent(channels[0],leptons,jets,metRel,mll,mT,nPVs, _eventInfo->LumiBlock(),weight);
      }
      else
      {
	if (mll < MLLFLOOROF) continue;
      }
      channels.clear();
      channels = findChannels(leptons);
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());
      if (isEvent) outfile << "passed mll > 15" << std::endl;

      //if leptons have same flavor, |mll - mZ| > 10 GeV
      if (_debug)
      {
	std::cout << "at second dilepton mass cut" << std::endl;
      }
      if (leptons[0]->LeptonPdgID() == leptons[1]->LeptonPdgID())
      {
	if (fabs(mll - ZMASS) < ZWINDOW) 
	{
          continue;
	}
      }
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);
      cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());
      if (isEvent) outfile << "passed z mass window" << std::endl;

      //MET > 40 GeV
      if (_debug)
      {
	std::cout << "checking MET" << std::endl;
      }
      if ((leptons[0]->LeptonPdgID() == leptons[1]->LeptonPdgID()) && (met < METCUTSF)) continue;
      else if ((leptons[0]->LeptonPdgID() != leptons[1]->LeptonPdgID()) && (met < METCUTOF)) continue;
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);
      cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());
      if (isEvent) outfile << "passed MET" << std::endl;
      if (isEvent)
      {

        _eventInfo->PrintData(outfile);
        for (int i = 0; i != (int)leptons.size(); ++i)
        {
          if (leptons[i]->LeptonPdgID() == 11)
          {
            ((MTree::Electron*)leptons[i])->PrintData(outfile);
          }
          else
          {
            ((MTree::Muon*)leptons[i])->PrintData(outfile);
          }
        }
        for (int i = 0; i != (int)jets.size(); ++i)
        {
          jets[i]->PrintData(outfile);
        }
      }

      //Jet selections
      channels.clear();
      channels = findChannels(leptons,(int)jets.size());
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);
      cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());

      if ((int)jets.size() == 0)
      {
	if (_debug)
	{
	  std::cout << "analyzing 0 jet event" << std::endl;
	}
	//pT(ll) > 30
	if (code->getPtll(leptons) < PTLLZEROJET) continue;
        plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);
        cutNr = cutflow->addEvent(cutNr,weight,channels);

	double mllCeil;
	if (lowMass)
	{
	  mllCeil = LOW01MLLCEIL;
	}
	else
	{
	  mllCeil = HIGH01MLLCEIL;
	}
        if (mll > mllCeil) continue;
        cutNr = cutflow->addEvent(cutNr,weight,channels);
        plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);
      }
      else if ((int)jets.size() == 1)
      {
	if (_debug)
	{
	  std::cout << "analyzing 1 jet event" << std::endl;
	}
	//Cut if jet pt> 25 and SV0 > 5.72
	bool failSV0 = code->failJetPtSV0(jets);
	if (failSV0) continue;
	if (isMC)
	{
	  double temp = getBJetWeight(jets);
	  if (temp <= 0)
	  {
	    std::cout << "Got funky jet weight! " << temp << std::endl;
	  }
	  weight *= temp;
	}
	cutNr = cutflow->addEvent(cutNr,weight,channels);
        plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);

	//Require pT(total) < 30 GeV
	double ptTot = code->getPtTotal(leptons, jets, metForWW);
	if (ptTot > PTTOT12JET) continue;
        cutNr = cutflow->addEvent(cutNr,weight,channels);
        plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);

	//Z->tautau veto
	double tauM = code->getTauTauMass(*leptons[0],*leptons[1],*metForWW);
	if (fabs(tauM - ZMASS) < TAUMASS) continue;
        cutNr = cutflow->addEvent(cutNr,weight,channels);
        plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);

	double mllCeil;
	if (lowMass)
	{
	  mllCeil = LOW01MLLCEIL;
	}
	else
	{
	  mllCeil = HIGH01MLLCEIL;
	}
        if (mll > mllCeil) continue;
        cutNr = cutflow->addEvent(cutNr,weight,channels);
        plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);
	if ((int)channels.size() > 1)
	{
	  for (int i = 0; i != (int)channels.size(); ++i)
	  {
	    std::cout << channels[i] << std::endl;
	  }
	}
      }
      else
      {
	orderByPt(jets);
	if (_debug)
	{
	  std::cout << "analyzing 2+ jet event" << std::endl;
	}
	//eta(j1)*eta(j2) < 0
	if ((jets[0]->Eta() * jets[1]->Eta()) > 0) continue;
	cutNr = cutflow->addEvent(cutNr,weight,channels);
        plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);
        cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());
        if (isEvent) outfile << "passed eta1*eta2" << std::endl;

	//delta eta(j1-j2) > 3.8
	orderByPt(jets);
	if (fabs(jets[0]->Eta() - jets[1]->Eta()) < JETDELTAETA) continue;
	cutNr = cutflow->addEvent(cutNr,weight,channels);
        plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);
        cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());

	//Mjj > 500 GeV
	TLorentzVector fjet((TLorentzVector)*jets[0]);
	TLorentzVector sjet((TLorentzVector)*jets[1]);
	if ((fjet + sjet).M() <= MJJFLOOR) continue;
	cutNr = cutflow->addEvent(cutNr,weight,channels);
        plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);
        cutlist->addEvent(cutNr,_eventInfo->RunNumber(),_eventInfo->EventNumber());

	//No additional jets with pT > 25 and fabs(eta) < 3.2
	bool fail3Jet = code->failThirdCentralJet(jets);
	if (fail3Jet) continue;
	cutNr = cutflow->addEvent(cutNr,weight,channels);
        plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);

	//No additional jets with pT > 25 and SV0 > 5.72
	bool failSV0 = code->failJetPtSV0(jets);
	if (failSV0) continue;
	if (isMC)
	{
	  double temp = getBJetWeight(jets);
	  if (temp <= 0)
	  {
	    std::cout << "Got funky jet weight! " << temp << std::endl;
	  }
	  weight *= temp;
	}
	cutNr = cutflow->addEvent(cutNr,weight,channels);
        plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);

	//Require pT(total) < 30 GeV
	double ptTot = code->getPtTotal(leptons, jets, metForWW);
	if (ptTot > PTTOT12JET) continue;
        cutNr = cutflow->addEvent(cutNr,weight,channels);
        plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);

	//Z->tautau veto
	double tauM = code->getTauTauMass(*leptons[0],*leptons[1],*metForWW);
	if (fabs(tauM - ZMASS) < TAUMASS) continue;
        cutNr = cutflow->addEvent(cutNr,weight,channels);
        plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);

        if (mll > HIGH2MLLCEIL) continue;
        cutNr = cutflow->addEvent(cutNr,weight,channels);
        plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);
      }

      if (!isMC && (int)jets.size() < 2)
      {
	wwevent->printEvent(_eventInfo,leptons,jets,metForWW,metRel,mT,mll);
      }

      //Topological selections
      //del(phi_ll) < 1.3(1.8)
      if (_debug)
      {
	std::cout << "applying delPhi cut" << std::endl;
      }
      double dphiCut;
      if (lowMass)
      {
	dphiCut = LOWDPHI;
      }
      else
      {
	dphiCut = HIGHDPHI;
      }
      if (TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])) >= dphiCut) continue;
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);

      //0.75*mHiggs < mT < mHiggs
      if (_debug)
      {
	std::cout << "applying Higgs mass window" << std::endl;
      }
      if ((mT < 0.75*hMass) || (mT > hMass)) 
      {
        continue;
      }
      cutNr = cutflow->addEvent(cutNr,weight,channels);
      plotNr = histos->addEvent(weight,plotNr,channels,leptons,jets,metRel,mT,mll);

      if (_debug)
      {
	std::cout << "Reaching end of event loop" << std::endl;
      }

    }//Closes loop over events 
  }//Closes loop over sub-samples
  cutlist->printLists(sample);
  TH1D* elecCuts = selector->getECutflow();
  TH1D* muonCuts = selector->getMCutflow();
  TH1D* jetCuts = selector->getJCutflow();
  std::map<std::string, TH1D*>* flowsN = cutflow->getNumCutflows();
  std::map<std::string, TH1D*>* flowsC = cutflow->getXSecCutflows();

  if (elecCuts == 0 || muonCuts == 0 || jetCuts == 0)
  {
    std::cout << "object cutflows did not print correctly!" << std::endl;
  }
  if (flowsN == 0 || flowsC == 0)
  {
    std::cout << "event cutflows did not print correctly!" << std::endl;
  }
  mu_PV->Write();
  mu_PV2->Write();
  outfile.close();
  zregion->printRegions(sample);
  cutflow->printXSecCheck();
}

//#######################################################################################
