#include <string>

#include "HiggsAnalysis/HWWAnalysis.h"

void countEvents(std::string sample)
{
  HWWAnalysis* analysis = new HWWAnalysis();

  std::vector<std::string> sets;
  sets = analysis->GetAllSamples(sample);

  for (int i = 0; i != (int)sets.size(); ++i)
  {
    TChain* tree = analysis->GetChain(sets[i]);
    std::cout << "Sample " << sets[i] << ": " << tree->GetEntries() << " events" << std::endl;
  }

}
