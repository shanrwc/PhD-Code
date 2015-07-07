#include "WWEventClass.h"

//########################################################################################

WWEventClass::WWEventClass()
{
  ;
}

//########################################################################################

WWEventClass::~WWEventClass()
{
  ;
}

//########################################################################################

void WWEventClass::printEvent(MTree::EventInfo* _eventInfo, std::vector<MTree::Lepton*> leptons, std::vector<MTree::Jet*> jets, MTree::MissingET* metWW, MTree::MissingET* metRel, double mT, double mll)
{
  //Open a text file
  std::string filename = makeString(_eventInfo->RunNumber()) + "-" + makeString(_eventInfo->EventNumber()) + ".txt";
  std::ofstream outfile;
  outfile.open(filename.c_str());

  //Print eventinfo to text file
  _eventInfo->PrintData(outfile);

  //Print lepton info to text file
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

  //Print jet info to text file
  for (int i = 0; i != (int)jets.size(); ++i)
  {
    jets[i]->PrintData(outfile);
  }

  //Print delta-phis between leptons and between leptons and jets
  for (int i = 0; i != (int)leptons.size(); ++i)
  {
    for (int j = i + 1; j != (int)leptons.size(); ++j)
    {
      double dPhi = leptons[i]->DeltaPhi(*leptons[j]);
      outfile << "Delta Phi between lepton " << i << " and lepton " << j << " = " << dPhi << std::endl;
    }
  }

  for (int i = 0; i != (int)leptons.size(); ++i)
  {
    for (int j = 0; j != (int)jets.size(); ++j)
    {
      double dPhi = leptons[i]->DeltaPhi(*jets[j]);
      outfile << "Delta Phi between lepton " << i << " and jet " << j << " = " << dPhi << std::endl;
    }
  }

  for (int i = 0; i != (int)jets.size(); ++i)
  {
    for (int j = i + 1; j != (int)jets.size(); ++j)
    {
      double dPhi = jets[i]->DeltaPhi(*jets[j]);
      outfile << "Delta Phi between jet " << i << " and jet " << j << " = " << dPhi << std::endl;
    }
  }

  //Print delta R's while I'm at it
  for (int i = 0; i != (int)leptons.size(); ++i)
  {
    for (int j = i + 1; j != (int)leptons.size(); ++j)
    {
      double dR = leptons[i]->DeltaR(*leptons[j]);
      outfile << "Delta R between lepton " << i << " and lepton " << j << " = " << dR << std::endl;
    }
  }

  for (int i = 0; i != (int)leptons.size(); ++i)
  {
    for (int j = 0; j != (int)jets.size(); ++j)
    {
      double dR = leptons[i]->DeltaR(*jets[j]);
      outfile << "Delta R between lepton " << i << " and jet " << j << " = " << dR << std::endl;
    }
  }

  for (int i = 0; i != (int)jets.size(); ++i)
  {
    for (int j = i + 1; j != (int)jets.size(); ++j)
    {
      double dR = jets[i]->DeltaR(*jets[j]);
      outfile << "Delta R between jet " << i << " and jet " << j << " = " << dR << std::endl;
    }
  }

  //Print MET magnitude and direction
  metWW->PrintData(outfile);
  outfile << "MetRel magnitude = " << metRel->MET() << std::endl << std::endl;

  //Print mll and mT
  outfile << "mll = " << mll << std::endl;
  outfile << "mT = " << mT << std::endl << std::endl;

  //Close text file
  outfile.close();
}

//########################################################################################
