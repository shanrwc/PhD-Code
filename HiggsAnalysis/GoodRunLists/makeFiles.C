

void makeFiles()
{
  gSystem->Load("makeSampleFile_C.so");
  
  makeSampleFile("data11_7TeV.pro07.merged_LBSUMM_Eg_standard.xml", "lumi_egamma.txt","Egamma", "data11");
  makeSampleFile("data11_7TeV.pro07.merged_LBSUMM_Muon.xml","lumi_muon.txt","Muon", "data11");
  mergeXML("data11_7TeV.pro07.merged_LBSUMM_Eg_standard.xml", "data11_7TeV.pro07.merged_LBSUMM_Muon.xml", "Merged.xml");
}

void makeFiles(string dummy)
{
  gSystem->Load("makeSampleFile_C.so");
  
  makeSampleFile("Merged.xml","lumi_merged.txt","Merged", "data11");

}
