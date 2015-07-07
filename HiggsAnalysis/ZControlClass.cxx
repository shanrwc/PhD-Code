#include "ZControlClass.h"

ZControlClass::ZControlClass()
{
  numberA["ee_zeroj"] = 0;
  numberA["ee_onej"] = 0;
  numberA["ee_twoj"] = 0;
  numberA["mm_zeroj"] = 0;
  numberA["mm_onej"] = 0;
  numberA["mm_twoj"] = 0;

  numberB["ee_zeroj"] = 0;
  numberB["ee_onej"] = 0;
  numberB["ee_twoj"] = 0;
  numberB["mm_zeroj"] = 0;
  numberB["mm_onej"] = 0;
  numberB["mm_twoj"] = 0;

  numberC["ee_zeroj"] = 0;
  numberC["ee_onej"] = 0;
  numberC["ee_twoj"] = 0;
  numberC["mm_zeroj"] = 0;
  numberC["mm_onej"] = 0;
  numberC["mm_twoj"] = 0;
  numberD["ee_zeroj"] = 0;
  numberD["ee_onej"] = 0;
  numberD["ee_twoj"] = 0;
  numberD["mm_zeroj"] = 0;
  numberD["mm_onej"] = 0;
  numberD["mm_twoj"] = 0;

  TH2D* e0mllmet = new TH2D("e_0_MllMet","e_0_MllMet",200,0,200,150,0,150);
  e0mllmet->Sumw2();
  TH2D* e1mllmet = new TH2D("e_1_MllMet","e_1_MllMet",200,0,200,150,0,150);
  e1mllmet->Sumw2();
  TH2D* e2mllmet = new TH2D("e_2_MllMet","e_2_MllMet",200,0,200,150,0,150);
  e2mllmet->Sumw2();
  TH2D* m0mllmet = new TH2D("m_0_MllMet","m_0_MllMet",200,0,200,150,0,150);
  m0mllmet->Sumw2();
  TH2D* m1mllmet = new TH2D("m_1_MllMet","m_1_MllMet",200,0,200,150,0,150);
  m1mllmet->Sumw2();
  TH2D* m2mllmet = new TH2D("m_2_MllMet","m_2_MllMet",200,0,200,150,0,150);
  m2mllmet->Sumw2();

  mllmet["ee_zeroj"] = e0mllmet;
  mllmet["ee_onej"] = e1mllmet;
  mllmet["ee_twoj"] = e2mllmet;
  mllmet["mm_zeroj"] = m0mllmet;
  mllmet["mm_onej"] = m1mllmet;
  mllmet["mm_twoj"] = m2mllmet;

  TH1D* e0b = new TH1D("e_0_breg","e_0_breg",25,-0.5,24.5);
  e0b->Sumw2();
  TH1D* e1b = new TH1D("e_1_breg","e_1_breg",25,-0.5,24.5);
  e1b->Sumw2();
  TH1D* e2b = new TH1D("e_2_breg","e_2_breg",25,-0.5,24.5);
  e2b->Sumw2();
  TH1D* m0b = new TH1D("m_0_breg","m_0_breg",25,-0.5,24.5);
  m0b->Sumw2();
  TH1D* m1b = new TH1D("m_1_breg","m_1_breg",25,-0.5,24.5);
  m1b->Sumw2();
  TH1D* m2b = new TH1D("m_2_breg","m_2_breg",25,-0.5,24.5);
  m2b->Sumw2();

  breg["ee_zeroj"] = e0b;
  breg["ee_onej"] = e1b;
  breg["ee_twoj"] = e2b;
  breg["mm_zeroj"] = m0b;
  breg["mm_onej"] = m1b;
  breg["mm_twoj"] = m2b;

  TH1D* e0d = new TH1D("e_0_dreg","e_0_dreg",25,-0.5,24.5);
  e0d->Sumw2();
  TH1D* e1d = new TH1D("e_1_dreg","e_1_dreg",25,-0.5,24.5);
  e1d->Sumw2();
  TH1D* e2d = new TH1D("e_2_dreg","e_2_dreg",25,-0.5,24.5);
  e2d->Sumw2();
  TH1D* m0d = new TH1D("m_0_dreg","m_0_dreg",25,-0.5,24.5);
  m0d->Sumw2();
  TH1D* m1d = new TH1D("m_1_dreg","m_1_dreg",25,-0.5,24.5);
  m1d->Sumw2();
  TH1D* m2d = new TH1D("m_2_dreg","m_2_dreg",25,-0.5,24.5);
  m2d->Sumw2();

  dreg["ee_zeroj"] = e0d;
  dreg["ee_onej"] = e1d;
  dreg["ee_twoj"] = e2d;
  dreg["mm_zeroj"] = m0d;
  dreg["mm_onej"] = m1d;
  dreg["mm_twoj"] = m2d;

  code = new CutCodeClass();

  ttmet1 = new TH2D("ttmet_one","ttmet_one",100,0,100,100,0,100);
  ttmll1 = new TH2D("ttmll_one","ttmll_one",100,0,100,100,0,100);
  ttmet2 = new TH2D("ttmet_two","ttmet_two",100,0,100,100,0,100);
  ttmll2 = new TH2D("ttmll_two","ttmll_two",100,0,100,100,0,100);

  TH1D* met_a_ee_zero = new TH1D("met_a_ee_zero","met_a_ee_zero",150,0,150);
  TH1D* met_b_ee_zero = new TH1D("met_b_ee_zero","met_b_ee_zero",150,0,150);
  TH1D* met_c_ee_zero = new TH1D("met_c_ee_zero","met_c_ee_zero",150,0,150);
  TH1D* met_d_ee_zero = new TH1D("met_d_ee_zero","met_d_ee_zero",150,0,150);
  TH1D* mll_a_ee_zero = new TH1D("mll_a_ee_zero","mll_a_ee_zero",150,0,150);
  TH1D* mll_b_ee_zero = new TH1D("mll_b_ee_zero","mll_b_ee_zero",150,0,150);
  TH1D* mll_c_ee_zero = new TH1D("mll_c_ee_zero","mll_c_ee_zero",150,0,150);
  TH1D* mll_d_ee_zero = new TH1D("mll_d_ee_zero","mll_d_ee_zero",150,0,150);
  met_a_ee_zero->Sumw2();
  met_b_ee_zero->Sumw2();
  met_c_ee_zero->Sumw2();
  met_d_ee_zero->Sumw2();
  mll_a_ee_zero->Sumw2();
  mll_b_ee_zero->Sumw2();
  mll_c_ee_zero->Sumw2();
  mll_d_ee_zero->Sumw2();

  TH1D* met_a_ee_one = new TH1D("met_a_ee_one","met_a_ee_one",150,0,150);
  TH1D* met_b_ee_one = new TH1D("met_b_ee_one","met_b_ee_one",150,0,150);
  TH1D* met_c_ee_one = new TH1D("met_c_ee_one","met_c_ee_one",150,0,150);
  TH1D* met_d_ee_one = new TH1D("met_d_ee_one","met_d_ee_one",150,0,150);
  TH1D* mll_a_ee_one = new TH1D("mll_a_ee_one","mll_a_ee_one",150,0,150);
  TH1D* mll_b_ee_one = new TH1D("mll_b_ee_one","mll_b_ee_one",150,0,150);
  TH1D* mll_c_ee_one = new TH1D("mll_c_ee_one","mll_c_ee_one",150,0,150);
  TH1D* mll_d_ee_one = new TH1D("mll_d_ee_one","mll_d_ee_one",150,0,150);
  met_a_ee_one->Sumw2();
  met_b_ee_one->Sumw2();
  met_c_ee_one->Sumw2();
  met_d_ee_one->Sumw2();
  mll_a_ee_one->Sumw2();
  mll_b_ee_one->Sumw2();
  mll_c_ee_one->Sumw2();
  mll_d_ee_one->Sumw2();

  TH1D* met_a_ee_two = new TH1D("met_a_ee_two","met_a_ee_two",150,0,150);
  TH1D* met_b_ee_two = new TH1D("met_b_ee_two","met_b_ee_two",150,0,150);
  TH1D* met_c_ee_two = new TH1D("met_c_ee_two","met_c_ee_two",150,0,150);
  TH1D* met_d_ee_two = new TH1D("met_d_ee_two","met_d_ee_two",150,0,150);
  TH1D* mll_a_ee_two = new TH1D("mll_a_ee_two","mll_a_ee_two",150,0,150);
  TH1D* mll_b_ee_two = new TH1D("mll_b_ee_two","mll_b_ee_two",150,0,150);
  TH1D* mll_c_ee_two = new TH1D("mll_c_ee_two","mll_c_ee_two",150,0,150);
  TH1D* mll_d_ee_two = new TH1D("mll_d_ee_two","mll_d_ee_two",150,0,150);
  met_a_ee_two->Sumw2();
  met_b_ee_two->Sumw2();
  met_c_ee_two->Sumw2();
  met_d_ee_two->Sumw2();
  mll_a_ee_two->Sumw2();
  mll_b_ee_two->Sumw2();
  mll_c_ee_two->Sumw2();
  mll_d_ee_two->Sumw2();

  TH1D* met_a_mm_zero = new TH1D("met_a_mm_zero","met_a_mm_zero",150,0,150);
  TH1D* met_b_mm_zero = new TH1D("met_b_mm_zero","met_b_mm_zero",150,0,150);
  TH1D* met_c_mm_zero = new TH1D("met_c_mm_zero","met_c_mm_zero",150,0,150);
  TH1D* met_d_mm_zero = new TH1D("met_d_mm_zero","met_d_mm_zero",150,0,150);
  TH1D* mll_a_mm_zero = new TH1D("mll_a_mm_zero","mll_a_mm_zero",150,0,150);
  TH1D* mll_b_mm_zero = new TH1D("mll_b_mm_zero","mll_b_mm_zero",150,0,150);
  TH1D* mll_c_mm_zero = new TH1D("mll_c_mm_zero","mll_c_mm_zero",150,0,150);
  TH1D* mll_d_mm_zero = new TH1D("mll_d_mm_zero","mll_d_mm_zero",150,0,150);
  met_a_mm_zero->Sumw2();
  met_b_mm_zero->Sumw2();
  met_c_mm_zero->Sumw2();
  met_d_mm_zero->Sumw2();
  mll_a_mm_zero->Sumw2();
  mll_b_mm_zero->Sumw2();
  mll_c_mm_zero->Sumw2();
  mll_d_mm_zero->Sumw2();

  TH1D* met_a_mm_one = new TH1D("met_a_mm_one","met_a_mm_one",150,0,150);
  TH1D* met_b_mm_one = new TH1D("met_b_mm_one","met_b_mm_one",150,0,150);
  TH1D* met_c_mm_one = new TH1D("met_c_mm_one","met_c_mm_one",150,0,150);
  TH1D* met_d_mm_one = new TH1D("met_d_mm_one","met_d_mm_one",150,0,150);
  TH1D* mll_a_mm_one = new TH1D("mll_a_mm_one","mll_a_mm_one",150,0,150);
  TH1D* mll_b_mm_one = new TH1D("mll_b_mm_one","mll_b_mm_one",150,0,150);
  TH1D* mll_c_mm_one = new TH1D("mll_c_mm_one","mll_c_mm_one",150,0,150);
  TH1D* mll_d_mm_one = new TH1D("mll_d_mm_one","mll_d_mm_one",150,0,150);
  met_a_mm_one->Sumw2();
  met_b_mm_one->Sumw2();
  met_c_mm_one->Sumw2();
  met_d_mm_one->Sumw2();
  mll_a_mm_one->Sumw2();
  mll_b_mm_one->Sumw2();
  mll_c_mm_one->Sumw2();
  mll_d_mm_one->Sumw2();

  TH1D* met_a_mm_two = new TH1D("met_a_mm_two","met_a_mm_two",150,0,150);
  TH1D* met_b_mm_two = new TH1D("met_b_mm_two","met_b_mm_two",150,0,150);
  TH1D* met_c_mm_two = new TH1D("met_c_mm_two","met_c_mm_two",150,0,150);
  TH1D* met_d_mm_two = new TH1D("met_d_mm_two","met_d_mm_two",150,0,150);
  TH1D* mll_a_mm_two = new TH1D("mll_a_mm_two","mll_a_mm_two",150,0,150);
  TH1D* mll_b_mm_two = new TH1D("mll_b_mm_two","mll_b_mm_two",150,0,150);
  TH1D* mll_c_mm_two = new TH1D("mll_c_mm_two","mll_c_mm_two",150,0,150);
  TH1D* mll_d_mm_two = new TH1D("mll_d_mm_two","mll_d_mm_two",150,0,150);
  met_a_mm_two->Sumw2();
  met_b_mm_two->Sumw2();
  met_c_mm_two->Sumw2();
  met_d_mm_two->Sumw2();
  mll_a_mm_two->Sumw2();
  mll_b_mm_two->Sumw2();
  mll_c_mm_two->Sumw2();
  mll_d_mm_two->Sumw2();

  metAmap["ee_zeroj"] = met_a_ee_zero;
  metAmap["ee_onej"] = met_a_ee_one;
  metAmap["ee_twoj"] = met_a_ee_two;
  metAmap["mm_zeroj"] = met_a_mm_zero;
  metAmap["mm_onej"] = met_a_mm_one;
  metAmap["mm_twoj"] = met_a_mm_two;
  metBmap["ee_zeroj"] = met_b_ee_zero;
  metBmap["ee_onej"] = met_b_ee_one;
  metBmap["ee_twoj"] = met_b_ee_two;
  metBmap["mm_zeroj"] = met_b_mm_zero;
  metBmap["mm_onej"] = met_b_mm_one;
  metBmap["mm_twoj"] = met_b_mm_two;
  metCmap["ee_zeroj"] = met_c_ee_zero;
  metCmap["ee_onej"] = met_c_ee_one;
  metCmap["ee_twoj"] = met_c_ee_two;
  metCmap["mm_zeroj"] = met_c_mm_zero;
  metCmap["mm_onej"] = met_c_mm_one;
  metCmap["mm_twoj"] = met_c_mm_two;
  metDmap["ee_zeroj"] = met_d_ee_zero;
  metDmap["ee_onej"] = met_d_ee_one;
  metDmap["ee_twoj"] = met_d_ee_two;
  metDmap["mm_zeroj"] = met_d_mm_zero;
  metDmap["mm_onej"] = met_d_mm_one;
  metDmap["mm_twoj"] = met_d_mm_two;

  mllAmap["ee_zeroj"] = mll_a_ee_zero;
  mllAmap["ee_onej"] = mll_a_ee_one;
  mllAmap["ee_twoj"] = mll_a_ee_two;
  mllAmap["mm_zeroj"] = mll_a_mm_zero;
  mllAmap["mm_onej"] = mll_a_mm_one;
  mllAmap["mm_twoj"] = mll_a_mm_two;
  mllBmap["ee_zeroj"] = mll_b_ee_zero;
  mllBmap["ee_onej"] = mll_b_ee_one;
  mllBmap["ee_twoj"] = mll_b_ee_two;
  mllBmap["mm_zeroj"] = mll_b_mm_zero;
  mllBmap["mm_onej"] = mll_b_mm_one;
  mllBmap["mm_twoj"] = mll_b_mm_two;
  mllCmap["ee_zeroj"] = mll_c_ee_zero;
  mllCmap["ee_onej"] = mll_c_ee_one;
  mllCmap["ee_twoj"] = mll_c_ee_two;
  mllCmap["mm_zeroj"] = mll_c_mm_zero;
  mllCmap["mm_onej"] = mll_c_mm_one;
  mllCmap["mm_twoj"] = mll_c_mm_two;
  mllDmap["ee_zeroj"] = mll_d_ee_zero;
  mllDmap["ee_onej"] = mll_d_ee_one;
  mllDmap["ee_twoj"] = mll_d_ee_two;
  mllDmap["mm_zeroj"] = mll_d_mm_zero;
  mllDmap["mm_onej"] = mll_d_mm_one;
  mllDmap["mm_twoj"] = mll_d_mm_two;

  TH1D* be0scan = new TH1D("bscan_e_0","bscan_e_0",150,0,150);
  be0scan->Sumw2();
  TH1D* be1scan = new TH1D("bscan_e_1","bscan_e_1",150,0,150);
  be1scan->Sumw2();
  TH1D* be2scan = new TH1D("bscan_e_2","bscan_e_2",150,0,150);
  be2scan->Sumw2();
  TH1D* de0scan = new TH1D("dscan_e_0","dscan_e_0",150,0,150);
  de0scan->Sumw2();
  TH1D* de1scan = new TH1D("dscan_e_1","dscan_e_1",150,0,150);
  de1scan->Sumw2();
  TH1D* de2scan = new TH1D("dscan_e_2","dscan_e_2",150,0,150);
  de2scan->Sumw2();

  TH1D* bm0scan = new TH1D("bscan_m_0","bscan_m_0",150,0,150);
  bm0scan->Sumw2();
  TH1D* bm1scan = new TH1D("bscan_m_1","bscan_m_1",150,0,150);
  bm1scan->Sumw2();
  TH1D* bm2scan = new TH1D("bscan_m_2","bscan_m_2",150,0,150);
  bm2scan->Sumw2();
  TH1D* dm0scan = new TH1D("dscan_m_0","dscan_m_0",150,0,150);
  dm0scan->Sumw2();
  TH1D* dm1scan = new TH1D("dscan_m_1","dscan_m_1",150,0,150);
  dm1scan->Sumw2();
  TH1D* dm2scan = new TH1D("dscan_m_2","dscan_m_2",150,0,150);
  dm2scan->Sumw2();

  bscan["ee_zeroj"] = be0scan;
  bscan["ee_onej"] = be1scan;
  bscan["ee_twoj"] = be2scan;
  bscan["mm_zeroj"] = bm0scan;
  bscan["mm_onej"] = bm1scan;
  bscan["mm_twoj"] = bm2scan;

  dscan["ee_zeroj"] = de0scan;
  dscan["ee_onej"] = de1scan;
  dscan["ee_twoj"] = de2scan;
  dscan["mm_zeroj"] = dm0scan;
  dscan["mm_onej"] = dm1scan;
  dscan["mm_twoj"] = dm2scan;

  TH1D* be0width = new TH1D("b_e0_width","b_e0_width",20,-0.5,19.5);
  be0width->Sumw2();
  TH1D* be1width = new TH1D("b_e1_width","b_e1_width",20,-0.5,19.5);
  be1width->Sumw2();
  TH1D* be2width = new TH1D("b_e2_width","b_e2_width",20,-0.5,19.5);
  be2width->Sumw2();
  TH1D* bm0width = new TH1D("b_m0_width","b_m0_width",20,-0.5,19.5);
  bm0width->Sumw2();
  TH1D* bm1width = new TH1D("b_m1_width","b_m1_width",20,-0.5,19.5);
  bm1width->Sumw2();
  TH1D* bm2width = new TH1D("b_m2_width","b_m2_width",20,-0.5,19.5);
  bm2width->Sumw2();

  TH1D* de0width = new TH1D("d_e0_width","d_e0_width",20,-0.5,19.5);
  de0width->Sumw2();
  TH1D* de1width = new TH1D("d_e1_width","d_e1_width",20,-0.5,19.5);
  de1width->Sumw2();
  TH1D* de2width = new TH1D("d_e2_width","d_e2_width",20,-0.5,19.5);
  de2width->Sumw2();
  TH1D* dm0width = new TH1D("d_m0_width","d_m0_width",20,-0.5,19.5);
  dm0width->Sumw2();
  TH1D* dm1width = new TH1D("d_m1_width","d_m1_width",20,-0.5,19.5);
  dm1width->Sumw2();
  TH1D* dm2width = new TH1D("d_m2_width","d_m2_width",20,-0.5,19.5);
  dm2width->Sumw2();

  bwidth["ee_zeroj"] = be0width;
  bwidth["ee_onej"] = be1width;
  bwidth["ee_twoj"] = be2width;
  bwidth["mm_zeroj"] = bm0width;
  bwidth["mm_onej"] = bm1width;
  bwidth["mm_twoj"] = bm2width;

  dwidth["ee_zeroj"] = de0width;
  dwidth["ee_onej"] = de1width;
  dwidth["ee_twoj"] = de2width;
  dwidth["mm_zeroj"] = dm0width;
  dwidth["mm_onej"] = dm1width;
  dwidth["mm_twoj"] = dm2width;

  TH1D* be0met = new TH1D("b_e0_met","b_e0_met",40,19.5,59.5);
  be0met->Sumw2();
  TH1D* be1met = new TH1D("b_e1_met","b_e1_met",40,19.5,59.5);
  be1met->Sumw2();
  TH1D* be2met = new TH1D("b_e2_met","b_e2_met",40,19.5,59.5);
  be2met->Sumw2();
  TH1D* bm0met = new TH1D("b_m0_met","b_m0_met",40,19.5,59.5);
  bm0met->Sumw2();
  TH1D* bm1met = new TH1D("b_m1_met","b_m1_met",40,19.5,59.5);
  bm1met->Sumw2();
  TH1D* bm2met = new TH1D("b_m2_met","b_m2_met",40,19.5,59.5);
  bm2met->Sumw2();

  TH1D* de0met = new TH1D("d_e0_met","d_e0_met",40,19.5,59.5);
  de0met->Sumw2();
  TH1D* de1met = new TH1D("d_e1_met","d_e1_met",40,19.5,59.5);
  de1met->Sumw2();
  TH1D* de2met = new TH1D("d_e2_met","d_e2_met",40,19.5,59.5);
  de2met->Sumw2();
  TH1D* dm0met = new TH1D("d_m0_met","d_m0_met",40,19.5,59.5);
  dm0met->Sumw2();
  TH1D* dm1met = new TH1D("d_m1_met","d_m1_met",40,19.5,59.5);
  dm1met->Sumw2();
  TH1D* dm2met = new TH1D("d_m2_met","d_m2_met",40,19.5,59.5);
  dm2met->Sumw2();

  diffMetB["ee_zeroj"] = be0met;
  diffMetB["ee_onej"] = be1met;
  diffMetB["ee_twoj"] = be2met;
  diffMetB["mm_zeroj"] = bm0met;
  diffMetB["mm_onej"] = bm1met;
  diffMetB["mm_twoj"] = bm2met;

  diffMetD["ee_zeroj"] = de0met;
  diffMetD["ee_onej"] = de1met;
  diffMetD["ee_twoj"] = de2met;
  diffMetD["mm_zeroj"] = dm0met;
  diffMetD["mm_onej"] = dm1met;
  diffMetD["mm_twoj"] = dm2met;

  //Plots comparing MET to Npv and mu
  TH2D* e0npvmet = new TH2D("e_0_NpvMet","e_0_NpvMet",10,-0.5,9.5,100,0,100);
  e0npvmet->Sumw2();
  TH2D* e1npvmet = new TH2D("e_1_NpvMet","e_1_NpvMet",10,-0.5,9.5,100,0,100);
  e1npvmet->Sumw2();
  TH2D* e2npvmet = new TH2D("e_2_NpvMet","e_2_NpvMet",10,-0.5,9.5,100,0,100);
  e2npvmet->Sumw2();
  TH2D* m0npvmet = new TH2D("m_0_NpvMet","m_0_NpvMet",10,-0.5,9.5,100,0,100);
  m0npvmet->Sumw2();
  TH2D* m1npvmet = new TH2D("m_1_NpvMet","m_1_NpvMet",10,-0.5,9.5,100,0,100);
  m1npvmet->Sumw2();
  TH2D* m2npvmet = new TH2D("m_2_NpvMet","m_2_NpvMet",10,-0.5,9.5,100,0,100);
  m2npvmet->Sumw2();

  npvmet["ee_zeroj"] = e0npvmet;
  npvmet["ee_onej"] = e1npvmet;
  npvmet["ee_twoj"] = e2npvmet;
  npvmet["mm_zeroj"] = m0npvmet;
  npvmet["mm_onej"] = m1npvmet;
  npvmet["mm_twoj"] = m2npvmet;

  TH2D* e0mumet = new TH2D("e_0_MuMet","e_0_MuMet",20,-0.5,19.5,100,0,100);
  e0mumet->Sumw2();
  TH2D* e1mumet = new TH2D("e_1_MuMet","e_1_MuMet",20,-0.5,19.5,100,0,100);
  e1mumet->Sumw2();
  TH2D* e2mumet = new TH2D("e_2_MuMet","e_2_MuMet",20,-0.5,19.5,100,0,100);
  e2mumet->Sumw2();
  TH2D* m0mumet = new TH2D("m_0_MuMet","m_0_MuMet",20,-0.5,19.5,100,0,100);
  m0mumet->Sumw2();
  TH2D* m1mumet = new TH2D("m_1_MuMet","m_1_MuMet",20,-0.5,19.5,100,0,100);
  m1mumet->Sumw2();
  TH2D* m2mumet = new TH2D("m_2_MuMet","m_2_MuMet",20,-0.5,19.5,100,0,100);
  m2mumet->Sumw2();

  mumet["ee_zeroj"] = e0mumet;
  mumet["ee_onej"] = e1mumet;
  mumet["ee_twoj"] = e2mumet;
  mumet["mm_zeroj"] = m0mumet;
  mumet["mm_onej"] = m1mumet;
  mumet["mm_twoj"] = m2mumet;

  TH2D* e0npvnj = new TH2D("e_0_NpvNj","e_0_NpvNj",25,-0.5,24.5,10,-0.5,9.5);
  e0npvnj->Sumw2();
  TH2D* e1npvnj = new TH2D("e_1_NpvNj","e_1_NpvNj",25,-0.5,24.5,10,-0.5,9.5);
  e1npvnj->Sumw2();
  TH2D* e2npvnj = new TH2D("e_2_NpvNj","e_2_NpvNj",25,-0.5,24.5,10,-0.5,9.5);
  e2npvnj->Sumw2();
  TH2D* m0npvnj = new TH2D("m_0_NpvNj","m_0_NpvNj",25,-0.5,24.5,10,-0.5,9.5);
  m0npvnj->Sumw2();
  TH2D* m1npvnj = new TH2D("m_1_NpvNj","m_1_NpvNj",25,-0.5,24.5,10,-0.5,9.5);
  m1npvnj->Sumw2();
  TH2D* m2npvnj = new TH2D("m_2_NpvNj","m_2_NpvNj",25,-0.5,24.5,10,-0.5,9.5);
  m2npvnj->Sumw2();

  npvnjet["ee_zeroj"] = e0npvnj;
  npvnjet["ee_onej"] = e1npvnj;
  npvnjet["ee_twoj"] = e2npvnj;
  npvnjet["mm_zeroj"] = m0npvnj;
  npvnjet["mm_onej"] = m1npvnj;
  npvnjet["mm_twoj"] = m2npvnj;

  TH1D* e0npv = new TH1D("e_0_npv","e_0_npv",25,-0.5,24.5);
  TH1D* e1npv = new TH1D("e_1_npv","e_1_npv",25,-0.5,24.5);
  TH1D* e2npv = new TH1D("e_2_npv","e_2_npv",25,-0.5,24.5);
  TH1D* m0npv = new TH1D("m_0_npv","m_0_npv",25,-0.5,24.5);
  TH1D* m1npv = new TH1D("m_1_npv","m_1_npv",25,-0.5,24.5);
  TH1D* m2npv = new TH1D("m_2_npv","m_2_npv",25,-0.5,24.5);

  npvmap["ee_zeroj"] = e0npv;
  npvmap["ee_onej"] = e1npv;
  npvmap["ee_twoj"] = e2npv;
  npvmap["mm_zeroj"] = m0npv;
  npvmap["mm_onej"] = m1npv;
  npvmap["mm_twoj"] = m2npv;

  TH1D* e0lpv = new TH1D("e_0_lpv","e_0_lpv",150,0,150);
  TH1D* e1lpv = new TH1D("e_1_lpv","e_1_lpv",150,0,150);
  TH1D* e2lpv = new TH1D("e_2_lpv","e_2_lpv",150,0,150);
  TH1D* m0lpv = new TH1D("m_0_lpv","m_0_lpv",150,0,150);
  TH1D* m1lpv = new TH1D("m_1_lpv","m_1_lpv",150,0,150);
  TH1D* m2lpv = new TH1D("m_2_lpv","m_2_lpv",150,0,150);

  metlPV["ee_zeroj"] = e0lpv;
  metlPV["ee_onej"] = e1lpv;
  metlPV["ee_twoj"] = e2lpv;
  metlPV["mm_zeroj"] = m0lpv;
  metlPV["mm_onej"] = m1lpv;
  metlPV["mm_twoj"] = m2lpv;

  TH1D* e0hpv = new TH1D("e_0_hpv","e_0_hpv",150,0,150);
  TH1D* e1hpv = new TH1D("e_1_hpv","e_1_hpv",150,0,150);
  TH1D* e2hpv = new TH1D("e_2_hpv","e_2_hpv",150,0,150);
  TH1D* m0hpv = new TH1D("m_0_hpv","m_0_hpv",150,0,150);
  TH1D* m1hpv = new TH1D("m_1_hpv","m_1_hpv",150,0,150);
  TH1D* m2hpv = new TH1D("m_2_hpv","m_2_hpv",150,0,150);

  methPV["ee_zeroj"] = e0hpv;
  methPV["ee_onej"] = e1hpv;
  methPV["ee_twoj"] = e2hpv;
  methPV["mm_zeroj"] = m0hpv;
  methPV["mm_onej"] = m1hpv;
  methPV["mm_twoj"] = m2hpv;

  metlpv = new TH1D("lpv_met_ch","lpv_met_ch",150,0,150);
  methpv = new TH1D("hpv_met_ch","hpv_met_ch",150,0,150);

  TH1D* e0mTc = new TH1D("e_0_mTc","e_0_mTc",600,0,600);
  TH1D* e1mTc = new TH1D("e_1_mTc","e_1_mTc",600,0,600);
  TH1D* e2mTc = new TH1D("e_2_mTc","e_2_mTc",600,0,600);
  TH1D* m0mTc = new TH1D("m_0_mTc","m_0_mTc",600,0,600);
  TH1D* m1mTc = new TH1D("m_1_mTc","m_1_mTc",600,0,600);
  TH1D* m2mTc = new TH1D("m_2_mTc","m_2_mTc",600,0,600);
  cMetAft["ee_zeroj"] = e0mTc;
  cMetAft["ee_onej"] = e1mTc;
  cMetAft["ee_twoj"] = e2mTc;
  cMetAft["mm_zeroj"] = m0mTc;
  cMetAft["mm_onej"] = m1mTc;
  cMetAft["mm_twoj"] = m2mTc;  

  TH1D* e0mTd = new TH1D("e_0_mTd","e_0_mTd",600,0,600);
  TH1D* e1mTd = new TH1D("e_1_mTd","e_1_mTd",600,0,600);
  TH1D* e2mTd = new TH1D("e_2_mTd","e_2_mTd",600,0,600);
  TH1D* m0mTd = new TH1D("m_0_mTd","m_0_mTd",600,0,600);
  TH1D* m1mTd = new TH1D("m_1_mTd","m_1_mTd",600,0,600);
  TH1D* m2mTd = new TH1D("m_2_mTd","m_2_mTd",600,0,600);
  dMetAft["ee_zeroj"] = e0mTd;
  dMetAft["ee_onej"] = e1mTd;
  dMetAft["ee_twoj"] = e2mTd;
  dMetAft["mm_zeroj"] = m0mTd;
  dMetAft["mm_onej"] = m1mTd;
  dMetAft["mm_twoj"] = m2mTd; 

}

//#######################################################################################

ZControlClass::~ZControlClass()
{
  ;
}

//#######################################################################################

std::string ZControlClass::getChannel(std::vector<MTree::Lepton*> leptons, int nJets)
{
  std::string result;

  if (leptons[0]->LeptonPdgID() != leptons[1]->LeptonPdgID())
  {
    std::cout << "Found e-mu event in Z Control region!" << std::endl;
  }
  else
  {
    if (leptons[0]->LeptonPdgID() == 11)
    {
      result = "ee_";
    }
    else
    {
      result = "mm_";
    }
  }

  if (nJets == 0)
  {
    result = result + "zeroj";
  }
  else if (nJets == 1)
  {
    result = result + "onej";
  }
  else
  {
    result = result + "twoj";
  }

  return(result);
}

//#######################################################################################

void ZControlClass::addEvent(std::string channel, std::vector<MTree::Lepton*> leptons, std::vector<MTree::Jet*> jets, MTree::MissingET* met, double mll, double mT, double Npv, double mu, double weight)
{
  std::string chan = getChannel(leptons, (int)jets.size());

  mllmet[chan]->Fill(mll,met->MET(),weight);
//   npvmet[chan]->Fill(Npv,met->MET(),weight);
//   mumet[chan]->Fill(mu,met->MET(),weight);
  fill1DHistos(mll,met->MET(),chan,weight);
  scanZWindows(mll,met->MET(),chan,weight);

  bool passJet = false;

  if (((int)jets.size() == 0) && (code->getPtll(leptons) > PTLLZEROJET))
  {
    passJet = true;
  }
  if (((int)jets.size() == 1) && (code->getPtTotal(leptons,jets,met) < PTTOT12JET))
  {
    passJet = true;
  }
  if ((int)jets.size() > 1) passJet = true;

  if (fabs(mll - ZMASS) > ZWINDOW)
  {
    if (passJet && ((mll < 50) && (mll > 15)))
    {
      if (met->MET() > METCUTSF)
      {
	(numberA[chan]) += weight;
      }
      else
      {
	(numberB[chan]) += weight;
      }
    }
    else if (weight < 0 && ((mll < 50) && (mll > 15))) std::cout << "Rejected event with weight < 0 in A/B" << std::endl;

    return;
  }
  else
  {
    if (passJet)
    {
      if (met->MET() > METCUTSF)
      {
        (numberC[chan]) += weight;
      }
      else
      {
	(numberD[chan]) += weight;
      }
    }
    else if (weight < 0 &&  (met->MET() > METCUTSF) && ((int)jets.size() == 0)) std::cout << "Rejected event with weight " << weight << " in C" << std::endl;

    npvmap[chan]->Fill(Npv,weight);
    if (Npv < 6)
    {
      if (chan == "ee_zeroj")
      {
	metlpv->Fill(met->MET(),1.0);
      }
      metlPV[chan]->Fill(met->MET(),weight);
    }
    else
    {
      if (chan == "ee_zeroj")
      {
	methpv->Fill(met->MET(),1.0);
      }
      methPV[chan]->Fill(met->MET(),weight);
    }
    npvmet[chan]->Fill(Npv,met->MET(),weight);
    mumet[chan]->Fill(mu,met->MET(),weight);
    npvnjet[chan]->Fill(Npv,(int)jets.size(),weight);
    varyMETCut(met->MET(),chan,weight);

    int cutNr = 0;
    std::map<std::string, TH1D*>* flow;
    std::map<std::string, TH1D*>* metaft;
    if (met->MET() > METCUTSF)
    {
      flow = &breg;
      metaft = &cMetAft;
    }
    else
    {
      flow = &dreg;
      metaft = &dMetAft;
    }

    bool passAll = false;

    if ((int)jets.size() == 0)
    {
      (*flow)[chan]->Fill(cutNr,weight);
      ++cutNr;

      //pT(ll) > 30
      if (code->getPtll(leptons) > PTLLZEROJET)
      {
	(*flow)[chan]->Fill(cutNr,weight);
	passAll = true;
      }
      else
      {
	passAll = false;
      }
      ++cutNr;

    }//closes zero jet portion
    else if ((int)jets.size() == 1)
    {
      (*flow)[chan]->Fill(cutNr,weight);
      ++cutNr;

      bool failSVO = code->failJetPtSV0(jets);
      if (!failSVO)
      {
	(*flow)[chan]->Fill(cutNr,weight);
	passAll = true;
      }
      else
      {
	passAll = false;
      }
      ++cutNr;

      double ptTot = code->getPtTotal(leptons, jets, met);
      if (ptTot < PTTOT12JET)
      {
	(*flow)[chan]->Fill(cutNr,weight);
	passAll = true;
      }
      else
      {
	passAll = false;
      }
      ++cutNr;

      double tauM = code->getTauTauMass(*leptons[0],*leptons[1],*met);
      ttmet1->Fill(tauM,met->MET(),weight);
      ttmll1->Fill(tauM,mll,weight);
      if (fabs(tauM - ZMASS) > TAUMASS)
      {
	(*flow)[chan]->Fill(cutNr,weight);
	passAll = true;
      }
      else
      {
	passAll = false;
      }
      ++cutNr;

    }//closes one jet portion
    else
    {
      (*flow)[chan]->Fill(cutNr,weight);
      ++cutNr;

      if ((jets[0]->Eta() * jets[1]->Eta()) < 0)
      {
        (*flow)[chan]->Fill(cutNr,weight);
	passAll = true;
      }
      else
      {
	passAll = false;
      }
      ++cutNr;

      if (fabs(jets[0]->Eta() - jets[1]->Eta()) > JETDELTAETA)
      {
        (*flow)[chan]->Fill(cutNr,weight);
	passAll = true;
      }
      else
      {
	passAll = false;
      }
      ++cutNr;

      TLorentzVector fjet((TLorentzVector)*jets[0]);
      TLorentzVector sjet((TLorentzVector)*jets[1]);
      if ((fjet + sjet).M() > MJJFLOOR)
      {
        (*flow)[chan]->Fill(cutNr,weight);
	passAll = true;
      }
      else
      {
	passAll = false;
      }
      ++cutNr;

      bool fail3Jet = code->failThirdCentralJet(jets);
      if (!fail3Jet)
      {
        (*flow)[chan]->Fill(cutNr,weight);
	passAll = true;
      }
      else
      {
	passAll = false;
      }
      ++cutNr;

      bool failSV0 = code->failJetPtSV0(jets);
      if (!failSV0)
      {
        (*flow)[chan]->Fill(cutNr,weight);
	passAll = true;
      }
      else
      {
	passAll = false;
      }
      ++cutNr;

      double ptTot = code->getPtTotal(leptons, jets, met);
      if (ptTot < PTTOT12JET)
      {
	(*flow)[chan]->Fill(cutNr,weight);
	passAll = true;
      }
      else
      {
	passAll = false;
      }
      ++cutNr;

      double tauM = code->getTauTauMass(*leptons[0],*leptons[1],*met);
      ttmet2->Fill(tauM,met->MET(),weight);
      ttmll2->Fill(tauM,mll,weight);
      if (fabs(tauM - ZMASS) > TAUMASS)
      {
	(*flow)[chan]->Fill(cutNr,weight);
	passAll = true;
      }
      else
      {
	passAll = false;
      }
      ++cutNr;

    }//closes two jet portion

    if (TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])) < LOWDPHI)
    {
      (*flow)[chan]->Fill(cutNr,weight);
      passAll = true;
    }
    else
    {
      passAll = false;
    }
    ++cutNr;

    if (passAll)
    {
      (*metaft)[chan]->Fill(mT,weight);
    }

    if (TMath::Abs(leptons[0]->DeltaPhi(*leptons[1])) < HIGHDPHI)
    {
      (*flow)[chan]->Fill(cutNr,weight);
    }
    ++cutNr;
  }//Closes if-statement over mll
}

//#######################################################################################

void ZControlClass::fill1DHistos(double mll, double met, std::string chan,double weight)
{
  bool highMet;
  bool highMll;

  if (mll < (ZMASS - ZWINDOW))
  {
    highMll = false;
  }
  else if ((mll - ZMASS) < ZWINDOW)
  {
    highMll = true;
  }
  else
  {
    return;
  }
  if (met < METCUTSF)
  {
    highMet = false;
  }
  else
  {
    highMet = true;
  }

  if (highMet && !highMll)
  {
    metAmap[chan]->Fill(met,weight);
    mllAmap[chan]->Fill(mll,weight);
  }
  if (highMet && highMll)
  {
    metBmap[chan]->Fill(met,weight);
    mllBmap[chan]->Fill(mll,weight);
  }
  if (!highMet && !highMll)
  {
    metCmap[chan]->Fill(met,weight);
    mllCmap[chan]->Fill(mll,weight);
  }
  if (!highMet && highMll)
  {
    metDmap[chan]->Fill(met,weight);
    mllDmap[chan]->Fill(mll,weight);
  }
}

//#######################################################################################

void ZControlClass::scanZWindows(double mll, double met, std::string chan, double weight)
{
  std::map<std::string, TH1D*>* flow;
  std::map<std::string, TH1D*>* window;
  if (met > METCUTSF)
  {
    flow = &bscan;
    window = &bwidth;
  }
  else
  {
    flow = &dscan;
    window = &dwidth;
  }

  (*flow)[chan]->Fill(mll,weight);

  for (int i = -10; i != 10; ++i)
  {
    if (fabs(mll - ZMASS) < (ZWINDOW + i))
    {
      (*window)[chan]->Fill(i + 10,weight);
    }
  }
}

//#######################################################################################

void ZControlClass::varyMETCut(double met, std::string chan, double weight)
{
  for (int i = 20; i != 60; ++i)
  {
    if (met < double(i))
    {
      diffMetD[chan]->Fill(i,weight);
    }
    else
    {
      diffMetB[chan]->Fill(i,weight);
    }
  }
}

//#######################################################################################

void ZControlClass::printRegions(std::string sample)
{
  std::string filename = "event-summaries/" + sample + "_zContRegs.txt";
  std::ofstream outfile;
  outfile.open(filename.c_str());

  outfile << "Events in Region A" << std::endl;
  std::map<std::string, double>::iterator iter;
  for (iter = numberA.begin(); iter != numberA.end(); ++iter)
  {
    outfile << "A: " << (*iter).first << ": " << (*iter).second << std::endl;
  }
  outfile << std::endl;

  outfile << "Events in Region B" << std::endl;
  for (iter = numberB.begin(); iter != numberB.end(); ++iter)
  {
    outfile << "B: " << (*iter).first << ": " << (*iter).second << std::endl;
  }
  outfile << std::endl;

  outfile << "Events in Region C" << std::endl;
  for (iter = numberC.begin(); iter != numberC.end(); ++iter)
  {
    outfile << "C: " << (*iter).first << ": " << (*iter).second << std::endl;
  }
  outfile << std::endl;

  outfile << "Events in Region D" << std::endl;
  for (iter = numberD.begin(); iter != numberD.end(); ++iter)
  {
    outfile << "D: " << (*iter).first << ": " << (*iter).second << std::endl;
  }
  outfile << std::endl;

  outfile.close();
}

//#######################################################################################
