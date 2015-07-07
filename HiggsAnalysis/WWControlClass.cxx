#include "WWControlClass.h"

WWControlClass::WWControlClass()
{
  code = new CutCodeClass();

  ee0ww = new TH1D("ee0ww","ee0ww",5,-0.5,4.5);
  em0ww = new TH1D("em0ww","em0ww",5,-0.5,4.5);
  mm0ww = new TH1D("mm0ww","mm0ww",5,-0.5,4.5);
  ee1ww = new TH1D("ee1ww","ee1ww",5,-0.5,4.5);
  em1ww = new TH1D("em1ww","em1ww",5,-0.5,4.5);
  mm1ww = new TH1D("mm1ww","mm1ww",5,-0.5,4.5);
}

//####################################################################################

WWControlClass::~WWControlClass()
{
  ;
}

//####################################################################################

std::string WWControlClass::getChannel(std::vector<MTree::Lepton*> leptons)
{
  std::string chan;

  if (leptons[0]->LeptonPdgID() != leptons[1]->LeptonPdgID())
  {
    chan = "em";
  }
  else
  {
    if (leptons[0]->LeptonPdgID() == 11)
    {
      chan = "ee";
    }
    else
    {
      chan = "mm";
    }
  }

  return(chan);
}

//####################################################################################

void WWControlClass::add0JetEvent(std::vector<MTree::Lepton*> leptons, double mll, double weight)
{
  if (mll > 80)
  {
    std::string chan = getChannel(leptons);

    if (chan == "ee")
    {
      ee0ww->Fill(0.0,weight);
    }
    else if (chan == "em")
    {
      em0ww->Fill(0.0,weight);
    }
    else if (chan == "mm")
    {
      mm0ww->Fill(0.0,weight);
    }
  } 
}

//####################################################################################

void WWControlClass::add1JetEvent(std::vector<MTree::Lepton*> leptons, std::vector<MTree::Jet*> jets, MTree::MissingET* metForWW, double mll, duoble weight)
{
  if (mll < 80) return;
  double ptTot = code->getPtTotal(leptons, jets, metForWW);
  if (ptTot > 35) return;
  double ttmass = code->getTauTauMass(leptons[0],leptons[1],metForWW);
  if (fabs(tauM - ZMASS) < TAUMASS) return;

  std::string chan = getChannel(leptons);

  if (chan == "ee"
  {
    ee1ww->Fill(0.0,weight);
  }
  else if (chan == "em")
  {
    em1ww->Fill(0.0,weight);
  }
  else if (chan == "mm")
  {
    mm1ww->Fill(0.0,weight);
  }
}

//####################################################################################
