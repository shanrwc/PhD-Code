#include "HHistosClass.h"

//########################################################################################

HHistosClass::HHistosClass()
{
  ee0jet = new std::map<int,std::vector<TH1D*> >;
  ee1jet = new std::map<int,std::vector<TH1D*> >;
  ee2jet = new std::map<int,std::vector<TH1D*> >;
  em0jet = new std::map<int,std::vector<TH1D*> >;
  em1jet = new std::map<int,std::vector<TH1D*> >;
  em2jet = new std::map<int,std::vector<TH1D*> >;
  mm0jet = new std::map<int,std::vector<TH1D*> >;
  mm1jet = new std::map<int,std::vector<TH1D*> >;
  mm2jet = new std::map<int,std::vector<TH1D*> >;  

  initialize("ee0",ee0jet);
  initialize("ee1",ee1jet);
  initialize("ee2",ee2jet);
  initialize("em0",em0jet);
  initialize("em1",em1jet);
  initialize("em2",em2jet);
  initialize("mm0",mm0jet);
  initialize("mm1",mm1jet);
  initialize("mm2",mm2jet);

}

//########################################################################################

HHistosClass::~HHistosClass()
{
  (*ee0jet).clear();
  (*ee1jet).clear();
  (*ee2jet).clear();
  (*em0jet).clear();
  (*em1jet).clear();
  (*em2jet).clear();
  (*mm0jet).clear();
  (*mm1jet).clear();
  (*mm2jet).clear();  
}

//#######################################################################################

void HHistosClass::initialize(std::string name, std::map<int,std::vector<TH1D*> >* pointer)
{
  for (int i = 0; i != 16; ++i)
  { 
    TH1D* leadpt = new TH1D(("leadpt_"+name+"_"+makeString(i)).c_str(),("leadpt_"+name+"_"+makeString(i)).c_str(),100,0,100);
    TH1D* subpt = new TH1D(("subpt_"+name+"_"+makeString(i)).c_str(),("subpt_"+name+"_"+makeString(i)).c_str(),100,0,100);
    TH1D* leadeta = new TH1D(("leadeta_"+name+"_"+makeString(i)).c_str(),("leadeta_"+name+"_"+makeString(i)).c_str(),60,-3,3);
    TH1D* subeta = new TH1D(("subeta_"+name+"_"+makeString(i)).c_str(),("subeta_"+name+"_"+makeString(i)).c_str(),60,-3,3);
    TH1D* leadphi = new TH1D(("leadphi_"+name+"_"+makeString(i)).c_str(),("leadphi_"+name+"_"+makeString(i)).c_str(),70,-3.5,3.5);
    TH1D* subphi = new TH1D(("subphi_"+name+"_"+makeString(i)).c_str(),("subphi_"+name+"_"+makeString(i)).c_str(),70,-3.5,3.5);
    TH1D* letiso = new TH1D(("letiso_"+name+"_"+makeString(i)).c_str(),("letiso_"+name+"_"+makeString(i)).c_str(),100,0,0.5);
    TH1D* setiso = new TH1D(("setiso_"+name+"_"+makeString(i)).c_str(),("setiso_"+name+"_"+makeString(i)).c_str(),100,0,0.5);
    TH1D* lptiso = new TH1D(("lptiso_"+name+"_"+makeString(i)).c_str(),("lptiso_"+name+"_"+makeString(i)).c_str(),100,0,0.5);
    TH1D* sptiso = new TH1D(("sptiso_"+name+"_"+makeString(i)).c_str(),("sptiso_"+name+"_"+makeString(i)).c_str(),100,0,0.5);
    TH1D* njets = new TH1D(("njets_"+name+"_"+makeString(i)).c_str(),("njets_"+name+"_"+makeString(i)).c_str(),10,-0.5,9.5);
    TH1D* met = new TH1D(("met_"+name+"_"+makeString(i)).c_str(),("met_"+name+"_"+makeString(i)).c_str(),200,0,200);
    TH1D* mT = new TH1D(("mT_"+name+"_"+makeString(i)).c_str(),("mT_"+name+"_"+makeString(i)).c_str(),200,0,200);
    TH1D* dphi = new TH1D(("dphi_"+name+"_"+makeString(i)).c_str(),("dphi_"+name+"_"+makeString(i)).c_str(),60,0,3);
    TH1D* mll = new TH1D(("mll_"+name+"_"+makeString(i)).c_str(),("mll_"+name+"_"+makeString(i)).c_str(),200,0,200);
    TH1D* jpt = new TH1D(("jpt_"+name+"_"+makeString(i)).c_str(),("jpt_"+name+"_"+makeString(i)).c_str(),200,0,200);
    TH1D* jeta = new TH1D(("jeta_"+name+"_"+makeString(i)).c_str(),("jeta_"+name+"_"+makeString(i)).c_str(),100,-5,5);
    TH1D* jphi = new TH1D(("jphi_"+name+"_"+makeString(i)).c_str(),("jphi_"+name+"_"+makeString(i)).c_str(),70,-3.5,3.5);
    TH1D* meteta = new TH1D(("meteta_"+name+"_"+makeString(i)).c_str(),("meteta_"+name+"_"+makeString(i)).c_str(),100,-5,5);
    TH1D* metphi = new TH1D(("metphi_"+name+"_"+makeString(i)).c_str(),("metphi_"+name+"_"+makeString(i)).c_str(),70,-3.5,3.5);
    TH1D* dpmetl = new TH1D(("dpmetl_"+name+"_"+makeString(i)).c_str(),("dpmetl_"+name+"_"+makeString(i)).c_str(),70,-3.5,3.5);
    TH1D* dpmetj = new TH1D(("dpmetj_"+name+"_"+makeString(i)).c_str(),("dpmetj_"+name+"_"+makeString(i)).c_str(),70,-3.5,3.5);

    std::vector<TH1D*> holder;
    holder.push_back(leadpt);
    holder.push_back(subpt);
    holder.push_back(leadeta);
    holder.push_back(subeta);
    holder.push_back(leadphi);
    holder.push_back(subphi);
    holder.push_back(letiso);
    holder.push_back(setiso);
    holder.push_back(lptiso);
    holder.push_back(sptiso);
    holder.push_back(njets);
    holder.push_back(met);
    holder.push_back(mT);
    holder.push_back(dphi);
    holder.push_back(mll);
    holder.push_back(jpt);
    holder.push_back(jeta);
    holder.push_back(jphi);
    holder.push_back(meteta);
    holder.push_back(metphi);
    holder.push_back(dpmetl);
    holder.push_back(dpmetj);

    (*pointer)[i] = holder;
  }
}

//#######################################################################################

std::vector<TH1D*> HHistosClass::getHistos(std::string channel, int cutNr)
{
  std::map<int,std::vector<TH1D*> >* temp;

  if (channel == "ee_zeroj")
  {
    temp = ee0jet;
  }
  else if (channel == "ee_onej")
  {
    temp = ee1jet;
  }
  else if (channel == "ee_twoj")
  {
    temp = ee2jet;
  }
  else if (channel == "em_zeroj")
  {
    temp = em0jet;
  }
  else if (channel == "em_onej")
  {
    temp = em1jet;
  }
  else if (channel == "em_twoj")
  {
    temp = em2jet;
  }
  else if (channel == "mm_zeroj")
  {
    temp = mm0jet;
  }
  else if (channel == "mm_onej")
  {
    temp = mm1jet;
  }
  else if (channel == "mm_twoj")
  {
    temp = mm2jet;
  }
  else
  {
    std::cout << "HHistosClass has been passed a channel it doesn't recognize!" << std::endl;
    temp = 0;
  }

  return((*temp)[cutNr]);

}

//#######################################################################################
int HHistosClass::addEvent(double xsec, int cutNr, std::vector<std::string>& channels,
			   std::vector<MTree::Lepton*>& leptons, std::vector<MTree::Jet*>& jets,
			   MTree::MissingET* metRel, double MT, double mll)
{
  for (int i = 0; i != (int)channels.size(); ++i)
  {
    if (((int)leptons.size() > 1) && (leptons[0]->LeptonPdgID() != leptons[1]->LeptonPdgID()) && (leptons[0]->LeptonPdgID() == 11))
    {
      std::swap(leptons[0],leptons[1]);
    }
    std::vector<TH1D*> holder = getHistos(channels[i],cutNr);
    if ((int)leptons.size() > 0)
    {
      holder[0]->Fill(leptons[0]->Pt(),xsec);
      holder[2]->Fill(leptons[0]->Eta(),xsec);
      holder[4]->Fill(leptons[0]->Phi(),xsec);
      if (leptons[0]->LeptonPdgID() == 11)
      {
        holder[6]->Fill(((MTree::Electron*)leptons[0])->ETCone30()/leptons[0]->Pt(),xsec);
        holder[8]->Fill(((MTree::Electron*)leptons[0])->PTCone30()/leptons[0]->Pt(),xsec);
      }
      else
      {
        holder[6]->Fill(((MTree::Muon*)leptons[0])->ETCone30()/leptons[0]->Pt(),xsec);
	holder[8]->Fill(((MTree::Muon*)leptons[0])->PTCone30()/leptons[0]->Pt(),xsec);
      }
    }
    if ((int)leptons.size() > 1)
    {
      holder[1]->Fill(leptons[1]->Pt(),xsec);
      holder[3]->Fill(leptons[1]->Eta(),xsec);
      holder[5]->Fill(leptons[1]->Phi(),xsec);
      if (leptons[1]->LeptonPdgID() == 11)
      {
        holder[7]->Fill(((MTree::Electron*)leptons[1])->ETCone30()/leptons[1]->Pt(),xsec);
        holder[9]->Fill(((MTree::Electron*)leptons[1])->PTCone30()/leptons[1]->Pt(),xsec);
      }
      else
      {
        holder[7]->Fill(((MTree::Muon*)leptons[1])->ETCone30()/leptons[1]->Pt(),xsec);
        holder[9]->Fill(((MTree::Muon*)leptons[1])->PTCone30()/leptons[1]->Pt(),xsec);
      }
      holder[13]->Fill(TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])),xsec);
      holder[14]->Fill(mll,xsec);
    }
    holder[10]->Fill((int)jets.size(),xsec);
    holder[11]->Fill(metRel->MET(),xsec);
    holder[12]->Fill(MT,xsec);
    holder[18]->Fill(metRel->Eta(),xsec);
    holder[19]->Fill(metRel->Phi(),xsec);

    if ((int)jets.size() > 0)
    {
      holder[15]->Fill(jets[0]->Pt(),xsec);
      holder[16]->Fill(jets[0]->Eta(),xsec);
      holder[17]->Fill(jets[0]->Phi(),xsec);
    }

    for (int i = 0; i != (int)leptons.size(); ++i)
    {
      double dPhi = metRel->DeltaPhi(*leptons[i]);
      holder[20]->Fill(dPhi,xsec/leptons.size());
    }
    for (int i = 0; i != (int)jets.size(); ++i)
    {
      double dPhi = metRel->DeltaPhi(*jets[i]);
      holder[21]->Fill(dPhi,xsec/jets.size());
    }
  }
  ++cutNr;
  return(cutNr);
}
//#######################################################################################
