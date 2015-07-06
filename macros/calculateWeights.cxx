#include <string>

#include "HiggsAnalysis/HWWAnalysis.h"

void calculateWeights(std::string sample)
{

  HWWAnalysis* analysis = new HWWAnalysis();

  std::vector<std::string> sets;
//   if (sample.find("hww") != string::npos)
//   {
//     sets.push_back("ggfhww160");
//     sets.push_back("ggfhwwt160");
//     sets.push_back("ggfhwwtt160");
//     sets.push_back("pwhww160");
//   }
  if (sample.find("smww") != string::npos)
  {
    sets.push_back("WWee");
    sets.push_back("WWem");
    sets.push_back("WWet");
    sets.push_back("WWme");
    sets.push_back("WWmm");
    sets.push_back("WWmt");
    sets.push_back("WWte");
    sets.push_back("WWtm");
    sets.push_back("WWtt");
    sets.push_back("ggWWee");
    sets.push_back("ggWWem");
    sets.push_back("ggWWet");
    sets.push_back("ggWWme");
    sets.push_back("ggWWmm");
    sets.push_back("ggWWmt");
    sets.push_back("ggWWte");
    sets.push_back("ggWWtm");
    sets.push_back("ggWWtt");
  }
  else if (sample.find("smwe") != string::npos)
  {
    sets.push_back("WevNp0");
    sets.push_back("WevNp1");
    sets.push_back("WevNp2");
    sets.push_back("WevNp3");
    sets.push_back("WevNp4");
    sets.push_back("WevNp5");
  }
  else if (sample.find("smwm") != string::npos)
  {
    sets.push_back("WmvNp0");
    sets.push_back("WmvNp1");
    sets.push_back("WmvNp2");
    sets.push_back("WmvNp3");
    sets.push_back("WmvNp4");
    sets.push_back("WmvNp5");
  }
  else if (sample.find("smwt") != string::npos)
  {
    sets.push_back("WtvNp0");
    sets.push_back("WtvNp1");
    sets.push_back("WtvNp2");
    sets.push_back("WtvNp3");
    sets.push_back("WtvNp4");
    sets.push_back("WtvNp5");
  }
  else if (sample.find("smze") !=string::npos)
  {
    sets.push_back("ZeeNp0");
    sets.push_back("ZeeNp1");
    sets.push_back("ZeeNp2");
    sets.push_back("ZeeNp3");
    sets.push_back("ZeeNp4");
    sets.push_back("ZeeNp5");
  }
  else if (sample.find("smzm") != string::npos)
  {
    sets.push_back("ZmmNp0");
    sets.push_back("ZmmNp1");
    sets.push_back("ZmmNp2");
    sets.push_back("ZmmNp3");
    sets.push_back("ZmmNp4");
    sets.push_back("ZmmNp5");
  }
  else if (sample.find("smzt") != string::npos)
  {
    sets.push_back("ZttNp0");
    sets.push_back("ZttNp1");
    sets.push_back("ZttNp2");
    sets.push_back("ZttNp3");
    sets.push_back("ZttNp4");
    sets.push_back("ZttNp5");
  }
  else if (sample.find("smdye") !=string::npos)
  {
    sets.push_back("dyZeeNp0");
    sets.push_back("dyZeeNp1");
    sets.push_back("dyZeeNp2");
    sets.push_back("dyZeeNp3");
    sets.push_back("dyZeeNp4");
    sets.push_back("dyZeeNp5");
  }
  else if (sample.find("smdym") != string::npos)
  {
    sets.push_back("dyZmmNp0");
    sets.push_back("dyZmmNp1");
    sets.push_back("dyZmmNp2");
    sets.push_back("dyZmmNp3");
    sets.push_back("dyZmmNp4");
    sets.push_back("dyZmmNp5");
  }
  else if (sample.find("smdyt") != string::npos)
  {
    sets.push_back("dyZttNp0");
    sets.push_back("dyZttNp1");
    sets.push_back("dyZttNp2");
    sets.push_back("dyZttNp3");
    sets.push_back("dyZttNp4");
    sets.push_back("dyZttNp5");
  }
  else if (sample.find("top") != string::npos)
  {
    sets.push_back("tW");
    sets.push_back("tb-e");
    sets.push_back("tb-m");
    sets.push_back("tb-t");
    sets.push_back("tqb-e");
    sets.push_back("tqb-m");
    sets.push_back("tqb-t");
    sets.push_back("ttbar");
  }
  else
  {
    sets.push_back(sample);
  }

  std::vector<double> weights;

  for (int i = 0; i != (int)sets.size(); ++i)
  {
    weights.push_back(analysis->CalculateSampleWeight(sets[i]));
  }

  double total = 0;
  for (int i = 0; i != (int)weights.size(); ++i)
  {
    std::cout << "Sample " <<sets[i]<< " has weight " << std::fixed << std::showpoint << std::setprecision(2) << weights[i] << std::endl;
    total = total + weights[i];
  }
  std::cout << "Sum of all weights: " << total << std::endl;
}
