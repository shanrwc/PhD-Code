#include "WireSagAnalysis.h"



WireSagAnalysis::WireSagAnalysis()
{
  ;
}

WireSagAnalysis::~WireSagAnalysis()
{
  ;
}

///////////////////////////////////////////////////////////////////////////////////////////

void WireSagAnalysis::WireSagEst(std::string sample, std::vector<std::vector<std::vector<TH1D*> > > histos, TH2D** histos2)
{
  if (_debug)
  {
    std::cout << "Calling WireSagEst()" << std::endl;
  }

  //Set up any necessary record-keeping.
  RecordClass* record = new RecordClass("wiresag");
  RecordClass* history = new RecordClass("muons");

  EndCapMap* reso1OP = new EndCapMap(sample,"reso1OP");
  EndCapMap* dzero = new EndCapMap(sample,"dzero");

  //Aaaaaaaaaand some constants, to make plotting easier.
  const int POS = 0;
  const int NEG = 1;

  //Set up sample to analyze.
  std::vector<std::string> samples = GetAllSamples(sample);
  std::vector<std::string>::iterator iter;
  std::vector<std::string>::iterator end = samples.end();

  for (iter = samples.begin(); iter != end; ++iter)
  {
    std::string sampleName = *iter;
    TChain* chain = GetChain(sampleName);
    setBranchAddresses(chain);
    std::cout << "Analyzing sample " << sampleName << std::endl;

    if (_debug)
    {
      std::cout << "Entering event loop" << std::endl;
    }

    int eventNr = 0;
    double weight = 1.0;
    while(chain->GetEntry(eventNr++))
    {
      if (eventNr % 10000 == 0)
      {
        std::cout << "WireSagEst has analyzed " << eventNr << " events." << std::endl;
      }
      int cutNr = 0;
      cutNr = record->addEvent(cutNr,weight,"Initial number of events");

      if (_debug)
      {
	std::cout << "Looping over muons" << std::endl;
      }
      for (int i = 0; i != (int)(reco_muon_pt->size()); ++i)
      {
	int muNr = 0;
	muNr = history->addEvent(muNr,weight,"Initial number of muons");

        //Declare bunches of TLorentzVectors to use to compute stuff.
	if (_debug)
	{
	  std::cout << "Declaring bunches of TLorentzVectors" << std::endl;
	}
        TLorentzVector reco;
        reco.SetPtEtaPhiM(reco_muon_pt->at(i),reco_muon_eta->at(i),reco_muon_phi->at(i),reco_muon_mass->at(i));
        TLorentzVector stand;
        stand.SetPtEtaPhiM(track_stand_pt->at(i),track_stand_eta->at(i),track_stand_phi->at(i),0);
	TLorentzVector extra;
	extra.SetPtEtaPhiM(track_extra_pt->at(i),track_extra_eta->at(i),track_extra_phi->at(i),0);
	TLorentzVector indet;
	indet.SetPtEtaPhiM(track_comb_pt->at(i),track_comb_eta->at(i),track_comb_phi->at(i),0);

        //Find matching truth muon.
	if (_debug)
	{
	  std::cout << "Finding the matching truth particle" << std::endl;
	}
	TLorentzVector truth;
	double dR = 100.0;
	for (int j = 0; j != (int)(truth_muon_pt->size()); ++j)
	{
	  TLorentzVector temp;
	  temp.SetPtEtaPhiM(truth_muon_pt->at(j),truth_muon_eta->at(j),truth_muon_phi->at(j),truth_muon_mass->at(j));

	  if (TMath::Abs(reco.DeltaR(temp)) < dR)
	  {
	    truth = temp;
	    dR = TMath::Abs(reco.DeltaR(temp));
	  }
	}
	double pinv = (1/reco.P() - 1/truth.P())/(1/truth.P());

        //Fill a batch of plots.
	if (_debug)
	{
	  std::cout << "Filling first batch of plots" << std::endl;
	}
	if (reco_muon_charge->at(i) > 0)
	{
	  histos[0][0][POS]->Fill(reco.Pt(),weight);
	  histos[1][0][POS]->Fill(reco.P(),weight);
	  histos[2][0][POS]->Fill(reco.Eta(),weight);
	  histos[3][0][POS]->Fill(reco.Phi(),weight);
	  histos[4][0][POS]->Fill(reco_muon_charge->at(i),weight);
	  histos[5][0][POS]->Fill(extra.Pt(),weight);
	  histos[6][0][POS]->Fill(extra.P(),weight);
	  histos[7][0][POS]->Fill(reco_muon_matchchi2->at(i),weight);
	  histos[8][0][POS]->Fill(TMath::Abs(indet.DeltaR(extra)),weight);
	  histos[9][0][POS]->Fill(pinv,weight);
	}
	else 
        {
	  histos[0][0][NEG]->Fill(reco.Pt(),weight);
	  histos[1][0][NEG]->Fill(reco.P(),weight);
	  histos[2][0][NEG]->Fill(reco.Eta(),weight);
	  histos[3][0][NEG]->Fill(reco.Phi(),weight);
	  histos[4][0][NEG]->Fill(reco_muon_charge->at(i),weight);
	  histos[5][0][NEG]->Fill(extra.Pt(),weight);
	  histos[6][0][NEG]->Fill(extra.P(),weight);
	  histos[7][0][NEG]->Fill(reco_muon_matchchi2->at(i),weight);
	  histos[8][0][NEG]->Fill(TMath::Abs(indet.DeltaR(extra)),weight);
	  histos[9][0][NEG]->Fill(pinv,weight);
	}

        //Make quality cuts.
	if (_debug)
	{
	  std::cout << "Imposing quality requirements" << std::endl;
	}
	if ((reco_muon_author->at(i) != 12)) continue;
	muNr = history->addEvent(muNr,weight,"MuID muons");
	if (_debug)
	{
	  std::cout << "Passed MuID" << std::endl;
	}
// 	if ((reco_muon_author->at(i) != 6)) continue;
// 	muNr = history->addEvent(muNr,weight,"STACO muons");
// 	if (_debug)
// 	{
// 	  std::cout << "Passed STACO" << std::endl;
// 	}

	if (!(reco_muon_iscomb->at(i))) continue;
	muNr = history->addEvent(muNr,weight,"Combined muons");
	if (_debug)
	{
	  std::cout << "Passed Combined" << std::endl;
	}

	if (track_comb_npixhits->at(i) < 1) continue;
	if (track_comb_nscthits->at(i) < 4) continue;
	muNr = history->addEvent(muNr,weight,"ID hits");
	if (_debug)
	{
	  std::cout << "Passed ID hits reqs" << std::endl;
	}

	if (track_extra_pt->at(i) < 50) continue;
	muNr = history->addEvent(muNr,weight,"pT_extra > 50 GeV");
	if (_debug)
	{
	  std::cout << "passed pT lower bound" << std::endl;
	}

	if (track_extra_pt->at(i) > 3000) continue;
	muNr = history->addEvent(muNr,weight,"pT_extra < 3000 GeV");
	if (_debug)
	{
	  std::cout << "passed pT upper bound" << std::endl;
	}

        //Fill another batch of plots.
	if (_debug)
	{
	  std::cout << "Filling second batch of plots" << std::endl;
	}
	if (reco_muon_charge->at(i) > 0)
	{
	  histos[0][1][POS]->Fill(reco.Pt(),weight);
	  histos[1][1][POS]->Fill(reco.P(),weight);
	  histos[2][1][POS]->Fill(reco.Eta(),weight);
	  histos[3][1][POS]->Fill(reco.Phi(),weight);
	  histos[4][1][POS]->Fill(reco_muon_charge->at(i),weight);
	  histos[5][1][POS]->Fill(extra.Pt(),weight);
	  histos[6][1][POS]->Fill(extra.P(),weight);
	  histos[7][1][POS]->Fill(reco_muon_matchchi2->at(i),weight);
	  histos[8][1][POS]->Fill(TMath::Abs(indet.DeltaR(extra)),weight);
	  histos[9][1][POS]->Fill(pinv,weight);
	}
	else 
        {
	  histos[0][1][NEG]->Fill(reco.Pt(),weight);
	  histos[1][1][NEG]->Fill(reco.P(),weight);
	  histos[2][1][NEG]->Fill(reco.Eta(),weight);
	  histos[3][1][NEG]->Fill(reco.Phi(),weight);
	  histos[4][1][NEG]->Fill(reco_muon_charge->at(i),weight);
	  histos[5][1][NEG]->Fill(extra.Pt(),weight);
	  histos[6][1][NEG]->Fill(extra.P(),weight);
	  histos[7][1][NEG]->Fill(reco_muon_matchchi2->at(i),weight);
	  histos[8][1][NEG]->Fill(TMath::Abs(indet.DeltaR(extra)),weight);
	  histos[9][1][NEG]->Fill(pinv,weight);
	}

        //Fill EndCapMaps.
	if (_debug)
	{
	  std::cout << "Filling EndCapMaps" << std::endl;
	}
	reso1OP->addEvent(reco_muon_charge->at(i),reco.Eta(),reco.Phi(),pinv,weight);
	dzero->addEvent(reco_muon_charge->at(i),reco.Eta(),reco.Phi(),track_comb_vd0->at(i),weight);

        //Fill plots for EML5-1/5/9/13 chambers.
	if (_debug)
	{
	  std::cout << "Filling EML5 plots" << std::endl;
	}
	//0.08726 radians = 5 degrees
	double high1 = 0.08726;
	double high2 = 3.14159 - 0.08726;
	double mid1 = 3.14159/2 - 0.08726;
	double mid2 = 3.14159/2 + 0.08726;
	double phi = reco.Phi();
	if ((reco.Eta() > 1.053) && (reco.Eta() < 1.206))
	{
	  if (TMath::Abs(phi) < high1)
	  {
	    if (reco_muon_charge->at(i) > 0)
	    {
	      histos[0][2][POS]->Fill(reco.Pt(),weight);
	      histos[1][2][POS]->Fill(reco.P(),weight);
	      histos[2][2][POS]->Fill(reco.Eta(),weight);
	      histos[3][2][POS]->Fill(reco.Phi(),weight);
	      histos[4][2][POS]->Fill(reco_muon_charge->at(i),weight);
	      histos[5][2][POS]->Fill(extra.Pt(),weight);
	      histos[6][2][POS]->Fill(extra.P(),weight);
	      histos[7][2][POS]->Fill(reco_muon_matchchi2->at(i),weight);
	      histos[8][2][POS]->Fill(TMath::Abs(indet.DeltaR(extra)),weight);
	      histos[9][2][POS]->Fill(pinv,weight);
	    }
	    else
	    {
	      histos[0][2][NEG]->Fill(reco.Pt(),weight);
	      histos[1][2][NEG]->Fill(reco.P(),weight);
	      histos[2][2][NEG]->Fill(reco.Eta(),weight);
	      histos[3][2][NEG]->Fill(reco.Phi(),weight);
	      histos[4][2][NEG]->Fill(reco_muon_charge->at(i),weight);
	      histos[5][2][NEG]->Fill(extra.Pt(),weight);
	      histos[6][2][NEG]->Fill(extra.P(),weight);
	      histos[7][2][NEG]->Fill(reco_muon_matchchi2->at(i),weight);
	      histos[8][2][NEG]->Fill(TMath::Abs(indet.DeltaR(extra)),weight);
	      histos[9][2][NEG]->Fill(pinv,weight);	      
	    }
	  }
	  else if ((TMath::Abs(phi) > mid1) && (TMath::Abs(phi) < mid2) && (phi > 0))
	  {
	    if (reco_muon_charge->at(i) > 0)
	    {
	      histos[0][3][POS]->Fill(reco.Pt(),weight);
	      histos[1][3][POS]->Fill(reco.P(),weight);
	      histos[2][3][POS]->Fill(reco.Eta(),weight);
	      histos[3][3][POS]->Fill(reco.Phi(),weight);
	      histos[4][3][POS]->Fill(reco_muon_charge->at(i),weight);
	      histos[5][3][POS]->Fill(extra.Pt(),weight);
	      histos[6][3][POS]->Fill(extra.P(),weight);
	      histos[7][3][POS]->Fill(reco_muon_matchchi2->at(i),weight);
	      histos[8][3][POS]->Fill(TMath::Abs(indet.DeltaR(extra)),weight);
	      histos[9][3][POS]->Fill(pinv,weight);

	      histos2[0]->Fill(reco.P(),reco.Eta(),weight);
	      histos2[4]->Fill(reco.P(),reco.Phi(),weight);
	      histos2[8]->Fill(reco.Eta(),reco.Phi(),weight);	      
	    }
	    else
	    {
	      histos[0][3][NEG]->Fill(reco.Pt(),weight);
	      histos[1][3][NEG]->Fill(reco.P(),weight);
	      histos[2][3][NEG]->Fill(reco.Eta(),weight);
	      histos[3][3][NEG]->Fill(reco.Phi(),weight);
	      histos[4][3][NEG]->Fill(reco_muon_charge->at(i),weight);
	      histos[5][3][NEG]->Fill(extra.Pt(),weight);
	      histos[6][3][NEG]->Fill(extra.P(),weight);
	      histos[7][3][NEG]->Fill(reco_muon_matchchi2->at(i),weight);
	      histos[8][3][NEG]->Fill(TMath::Abs(indet.DeltaR(extra)),weight); 
	      histos[9][3][NEG]->Fill(pinv,weight);

	      histos2[1]->Fill(reco.P(),reco.Eta(),weight);
	      histos2[5]->Fill(reco.P(),reco.Phi(),weight);
	      histos2[9]->Fill(reco.Eta(),reco.Phi(),weight);
	    }
	  }
	  else if (TMath::Abs(phi) > high2)
	  {
	    if (reco_muon_charge->at(i) > 0)
	    {
	      histos[0][4][POS]->Fill(reco.Pt(),weight);
	      histos[1][4][POS]->Fill(reco.P(),weight);
	      histos[2][4][POS]->Fill(reco.Eta(),weight);
	      histos[3][4][POS]->Fill(reco.Phi(),weight);
	      histos[4][4][POS]->Fill(reco_muon_charge->at(i),weight);
	      histos[5][4][POS]->Fill(extra.Pt(),weight);
	      histos[6][4][POS]->Fill(extra.P(),weight);
	      histos[7][4][POS]->Fill(reco_muon_matchchi2->at(i),weight);
	      histos[8][4][POS]->Fill(TMath::Abs(indet.DeltaR(extra)),weight);
	      histos[9][4][POS]->Fill(pinv,weight);	   
	    }
	    else
	    {
	      histos[0][4][NEG]->Fill(reco.Pt(),weight);
	      histos[1][4][NEG]->Fill(reco.P(),weight);
	      histos[2][4][NEG]->Fill(reco.Eta(),weight);
	      histos[3][4][NEG]->Fill(reco.Phi(),weight);
	      histos[4][4][NEG]->Fill(reco_muon_charge->at(i),weight);
	      histos[5][4][NEG]->Fill(extra.Pt(),weight);
	      histos[6][4][NEG]->Fill(extra.P(),weight);
	      histos[7][4][NEG]->Fill(reco_muon_matchchi2->at(i),weight);
	      histos[8][4][NEG]->Fill(TMath::Abs(indet.DeltaR(extra)),weight);
	      histos[9][4][NEG]->Fill(pinv,weight); 	      
	    }
	  }
	  else if ((TMath::Abs(phi) > mid1) && (TMath::Abs(phi) < mid2) && (phi < 0))
	  {
	    if (reco_muon_charge->at(i) > 0)
	    {
	      histos[0][5][POS]->Fill(reco.Pt(),weight);
	      histos[1][5][POS]->Fill(reco.P(),weight);
	      histos[2][5][POS]->Fill(reco.Eta(),weight);
	      histos[3][5][POS]->Fill(reco.Phi(),weight);
	      histos[4][5][POS]->Fill(reco_muon_charge->at(i),weight);
	      histos[5][5][POS]->Fill(extra.Pt(),weight);
	      histos[6][5][POS]->Fill(extra.P(),weight);
	      histos[7][5][POS]->Fill(reco_muon_matchchi2->at(i),weight);
	      histos[8][5][POS]->Fill(TMath::Abs(indet.DeltaR(extra)),weight);
	      histos[9][5][POS]->Fill(pinv,weight);

	      histos2[2]->Fill(reco.P(),reco.Eta(),weight);
	      histos2[6]->Fill(reco.P(),reco.Phi(),weight);
	      histos2[10]->Fill(reco.Eta(),reco.Phi(),weight);	      
	    }
	    else
	    {
	      histos[0][5][NEG]->Fill(reco.Pt(),weight);
	      histos[1][5][NEG]->Fill(reco.P(),weight);
	      histos[2][5][NEG]->Fill(reco.Eta(),weight);
	      histos[3][5][NEG]->Fill(reco.Phi(),weight);
	      histos[4][5][NEG]->Fill(reco_muon_charge->at(i),weight);
	      histos[5][5][NEG]->Fill(extra.Pt(),weight);
	      histos[6][5][NEG]->Fill(extra.P(),weight);
	      histos[7][5][NEG]->Fill(reco_muon_matchchi2->at(i),weight);
	      histos[8][5][NEG]->Fill(TMath::Abs(indet.DeltaR(extra)),weight); 
	      histos[9][5][NEG]->Fill(pinv,weight);

	      histos2[3]->Fill(reco.P(),reco.Eta(),weight);
	      histos2[7]->Fill(reco.P(),reco.Phi(),weight);
	      histos2[11]->Fill(reco.Eta(),reco.Phi(),weight);		      
	    }
	  }
	}


      }


      //FOR TESTING PURPOSES ONLY-REMOVE FOR REAL WORK
//       if (eventNr > 50000)
//       {
// 	eventNr = 10000000;
//       }

      if (_debug)
      {
	std::cout << "Reached end of event loop" << std::endl;
      }

    }//Closes event loop
  }//Closes sample loop

  reso1OP->graphFittedMeans(1,1);
  dzero->graphDistributions(1,1);

  record->print(sample + "_events");
  history->print(sample + "_muons");
}//Closes WireSagEst fuction

///////////////////////////////////////////////////////////////////////////////////////////

void WireSagAnalysis::WireSagComparison(std::string sample1, std::string sample2, TH1D** histos)
{
  if (_debug)
  {
    std::cout << "Calling WireSagComparison" << std::endl;
  }

  //Set up any necessary record-keeping
  EndCapMap* map = new EndCapMap(sample1 + "_" + sample2, "deltapT");

  //Set up samples (in the plural!) to run over
  std::cout << "Comparing " << sample1 << " to " << sample2 << std::endl;

  TChain* chain1 = GetChain(sample1);
  setBranchAddresses(chain1);

  TChain* chain2 = GetChain(sample2);
  setBranchAddresses(chain2);

  if (chain1->GetEntries() != chain2->GetEntries())
  {
    std::cout << "Samples contain different numbers of events!" << std::endl;
  }

  int eventNr = 0;
  while (chain1->GetEntry(eventNr++))
  {
    if (eventNr % 10000 == 0)
    {
      std::cout << "WireSagComparison has analyzed " << eventNr << " events." << std::endl;
    }

    //Get muons to analyze from first chain and store them locally.
    std::vector<double> chain1_pts;
    std::vector<double> chain1_etas;
    std::vector<double> chain1_phis;
    std::vector<int> chain1_qs;
    int event1 = -1;

      for (int i = 0; i != (int)(reco_muon_pt->size()); ++i)
      {

	if (_debug)
	{
	  std::cout << "Making muon selections" << std::endl;
	}
	if (reco_muon_author->at(i) != 12) continue;
	if (_debug)
	{
	  std::cout << "Passed MuID" << std::endl;
	}
	if (!(reco_muon_iscomb->at(i))) continue;
	if (_debug)
	{
	  std::cout << "Passed Combined" << std::endl;
	}
	if (reco_muon_pt->at(i) < 10) continue;
	if (_debug)
	{
	  std::cout << "Passed pT > 10 GeV" << std::endl;
	}
	if (track_comb_npixhits->at(i) < 1) continue;
	if (track_comb_nscthits->at(i) < 3) continue;
	if (_debug)
	{
	  std::cout << "Passed ID cuts" << std::endl;
	}

	event1 = event_number;
	chain1_pts.push_back(reco_muon_pt->at(i));
	chain1_etas.push_back(reco_muon_eta->at(i));
	chain1_phis.push_back(reco_muon_phi->at(i));
	chain1_qs.push_back(reco_muon_charge->at(i));
      }

      chain2->GetEntry(eventNr - 1);

      std::vector<double> chain2_pts;
      std::vector<double> chain2_etas;
      std::vector<double> chain2_phis;
      std::vector<int> chain2_qs;
      int event2 = -1;

      for (int i = 0; i != (int)(reco_muon_pt->size()); ++i)
      {

	if (_debug)
	{
	  std::cout << "Making muon selections" << std::endl;
	}
	if (reco_muon_author->at(i) != 12) continue;
	if (_debug)
	{
	  std::cout << "Passed MuID" << std::endl;
	}
	if (!(reco_muon_iscomb->at(i))) continue;
	if (_debug)
	{
	  std::cout << "Passed Combined" << std::endl;
	}
	if (reco_muon_pt->at(i) < 10) continue;
	if (_debug)
	{
	  std::cout << "Passed pT > 10 GeV" << std::endl;
	}
	if (track_comb_npixhits->at(i) < 1) continue;
	if (track_comb_nscthits->at(i) < 3) continue;
	if (_debug)
	{
	  std::cout << "Passed ID cuts" << std::endl;
	}

	event2 = event_number;
	chain2_pts.push_back(reco_muon_pt->at(i));
	chain2_etas.push_back(reco_muon_eta->at(i));
	chain2_phis.push_back(reco_muon_phi->at(i));
	chain2_qs.push_back(reco_muon_charge->at(i));
      }

      if (event1 == event2)
      {
	if (chain1_pts.size() != chain2_pts.size())
	{
	  std::cout << "Chains contain different muons for same event!" << std::endl;
	}
	for (int j = 0; j != (int)chain1_pts.size(); ++j)
	{
	  if (chain1_qs[j] != chain2_qs[j])
	  {
	    std::cout << "Muons are not in the same order in the event!" << std::endl;
	    continue;
	  }
	  double deltapT = chain1_pts[j] - chain2_pts[j];
	  map->addEvent(chain1_qs[j],chain1_etas[j],chain1_phis[j],deltapT,1.0);
	}


	for (int j = 0; j != (int)chain2_pts.size(); ++j)
	{

	}
      }

//       if (eventNr > 10000)
//       {
// 	eventNr = 10000000;
//       }
      if (_debug)
      {
	std::cout << "reached end of event loop" << std::endl;
      }

  }//Closes while loop over events

  map->graphDistributions(4,3);
}//Closes wiresag comparison function
