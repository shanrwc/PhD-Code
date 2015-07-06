#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>


///////////////////////////////////////////////////////////////////////////////////

double MakeDouble(std::string input)
{
  std::istringstream stm;
  stm.str(input);
  double num;
  stm >> num;

  return(num);
}

//////////////////////////////////////////////////////////////////////////////////

std::vector<std::string> SplitString(std::string phrase)
{
  std::stringstream ss(phrase);
  std::string buffer;
  std::vector<std::string> pieces;

  while(ss >> buffer)
  {
    pieces.push_back(buffer);
  }

  return(pieces);
}

//////////////////////////////////////////////////////////////////////////////////

template<typename T>
std::string MakeString(T const& value)
{
  std::stringstream sstr;
  sstr.setf(ios::floatfield, ios::fixed);
  sstr.precision(2);
  sstr << value;
  return sstr.str();
}

//////////////////////////////////////////////////////////////////////////////////

std::string EvenString(std::string input, int length)
{
  while ((int)input.length() < length)
  {
    input += " ";
  }

  return(input);
}

/////////////////////////////////////////////////////////////////////////////////

double GetXSection(std::string sample)
{
  std::map<std::string, double> _crossSections;

  _crossSections["ggfhww150"] = 10500*0.699*0.04545424;
  _crossSections["ggfhwwt150"] = 10500*0.699*0.04797;
  _crossSections["ggfhwwtt150"] = 10500*0.699*0.01265625; 
  _crossSections["vbfhww150"] = 961.6*0.699*0.10608049;

  _crossSections["ggfhww160"] = 9080*0.909*0.04545;
  _crossSections["ggfhwwt160"] = 9080*0.909*0.04797;
  _crossSections["ggfhwwtt160"] = 9080*0.909*0.01266;

  _crossSections["full-ggfhww160"] = 9080*0.909*0.04545;

  _crossSections["pwhww160"] = 56.03;

  _crossSections["WevNp0"] = 6870500*1.0*1.21;
  _crossSections["WevNp1"] = 1293000*1.0*1.21;
  _crossSections["WevNp2"] = 376600*1.0*1.21;
  _crossSections["WevNp3"] = 101290*1.0*1.21;
  _crossSections["WevNp4"] = 25246*1.0*1.21;
  _crossSections["WevNp5"] = 7123.9*1.0*1.21;
  _crossSections["WmvNp0"] = 6871100*1.0*1.21;
  _crossSections["WmvNp1"] = 1294700*1.0*1.21;
  _crossSections["WmvNp2"] = 376080*1.0*1.21;
  _crossSections["WmvNp3"] = 100720*1.0*1.21;
  _crossSections["WmvNp4"] = 25993*1.0*1.21;
  _crossSections["WmvNp5"] = 7130*1.0*1.21;
  _crossSections["WtvNp0"] = 6873300*1.0*1.21;
  _crossSections["WtvNp1"] = 1295200*1.0*1.21;
  _crossSections["WtvNp2"] = 375070*1.0*1.21;
  _crossSections["WtvNp3"] = 101770*1.0*1.21;
  _crossSections["WtvNp4"] = 25756*1.0*1.21;
  _crossSections["WtvNp5"] = 7001.6*1.0*1.21;

  _crossSections["PythiaWt"] = 8916330*1.17*1.0;

  _crossSections["WbbNp0"] = 3307.2*1.0*1.21;
  _crossSections["WbbNp1"] = 2675.6*1.0*1.21;
  _crossSections["WbbNp2"] = 1376.3*1.0*1.21;
  _crossSections["WbbNp3"] = 656.89*1.0*1.21;

  _crossSections["ZeeNp0"] = 664100*1.0*1.26;
  _crossSections["ZeeNp1"] = 132462*1.0*1.26;
  _crossSections["ZeeNp2"] = 40226*1.0*1.26;
  _crossSections["ZeeNp3"] = 11138*1.0*1.26;
  _crossSections["ZeeNp4"] = 3128.18*1.0*1.26;
  _crossSections["ZeeNp5"] = 753.425*1.0*1.26;
  _crossSections["ZmmNp0"] = 663790*1.0*1.26;
  _crossSections["ZmmNp1"] = 132950*1.0*1.26;
  _crossSections["ZmmNp2"] = 40375*1.0*1.26;
  _crossSections["ZmmNp3"] = 11161*1.0*1.26;
  _crossSections["ZmmNp4"] = 2898.7*1.0*1.26;
  _crossSections["ZmmNp5"] = 756.62*1.0*1.26;
  _crossSections["ZttNp0"] = 662500*1.0*1.26;
  _crossSections["ZttNp1"] = 133940*1.0*1.26;
  _crossSections["ZttNp2"] = 40295*1.0*1.26;
  _crossSections["ZttNp3"] = 11029*1.0*1.26;
  _crossSections["ZttNp4"] = 2804*1.0*1.26;
  _crossSections["ZttNp5"] = 780.54*1.0*1.26;

  _crossSections["PythiaZee"] = 855210*1.15*1.0;
  _crossSections["PythiaZmm"] = 855250*1.15*1.0;
  _crossSections["PythiaZtt"] = 856967*1.15*1.0;

  _crossSections["dyZeeNp0"] = 3051620*1.0*1.22;
  _crossSections["dyZeeNp1"] = 87870*1.0*1.22;
  _crossSections["dyZeeNp2"] = 41100*1.0*1.22;
  _crossSections["dyZeeNp3"] = 8460*1.0*1.22;
  _crossSections["dyZeeNp4"] = 1840*1.0*1.22;
  _crossSections["dyZeeNp5"] = 460*1.0*1.22;
  _crossSections["dyZmmNp0"] = 3051620*1.0*1.22;
  _crossSections["dyZmmNp1"] = 87870*1.0*1.22;
  _crossSections["dyZmmNp2"] = 40950*1.0*1.22;
  _crossSections["dyZmmNp3"] = 8410*1.0*1.22;
  _crossSections["dyZmmNp4"] = 1850*1.0*1.22;
  _crossSections["dyZmmNp5"] = 460*1.0*1.22;
  _crossSections["dyZttNp0"] = 3054680*1.0*1.22;
  _crossSections["dyZttNp1"] = 86480*1.0*1.22;
  _crossSections["dyZttNp2"] = 41530*1.0*1.22;
  _crossSections["dyZttNp3"] = 8370*1.0*1.22;
  _crossSections["dyZttNp4"] = 1840*1.0*1.22;
  _crossSections["dyZttNp5"] = 460*1.0*1.22;

  _crossSections["PythiaDYLee"] = 1795000*1.15*1.0;
  _crossSections["PythiaDYee"] = 1296200*1.15*1.0;
  _crossSections["PythiaDYLmm"] = 4428300*1.15*1.0;
  _crossSections["PythiaDYmm"] = 1296100.*1.15*1.0;
  _crossSections["PythiaDYtt"] = 3454*1.15*1.0;

  _crossSections["WWee"] = 503.77*1.0*1.0;
  _crossSections["WWem"] = 503.77*1.0*1.0;
  _crossSections["WWet"] = 503.77*1.0*1.0;  
  _crossSections["WWme"] = 503.77*1.0*1.0;
  _crossSections["WWmm"] = 503.77*1.0*1.0;
  _crossSections["WWmt"] = 503.77*1.0*1.0;
  _crossSections["WWte"] = 503.77*1.0*1.0;
  _crossSections["WWtm"] = 503.77*1.0*1.0;
  _crossSections["WWtt"] = 503.77*1.0*1.0;
  _crossSections["ggWWee"] = 15.24*1.0*0.9885;
  _crossSections["ggWWem"] = 15.24*1.0*0.9893;
  _crossSections["ggWWet"] = 15.24*1.0*0.9231;
  _crossSections["ggWWme"] = 15.24*1.0*0.9897;
  _crossSections["ggWWmm"] = 15.24*1.0*0.9888;
  _crossSections["ggWWmt"] = 15.24*1.0*0.9297;
  _crossSections["ggWWte"] = 15.24*1.0*0.3268;
  _crossSections["ggWWtm"] = 15.24*1.0*0.9236;
  _crossSections["ggWWtt"] = 15.24*1.0*0.9272; 

  _crossSections["WpZlvll"] = 159.24*1.0*1.0;
  _crossSections["WpZlvqq"] = 1688.9*1.0*1.0;
  _crossSections["WpZqqll"] = 498.36*1.0*1.0;
  _crossSections["WmZlvqq"] = 912.64*1.0*1.0;
  _crossSections["WmZlvll"] = 86.05*1.0*1.0;
  _crossSections["WmZqqll"] = 269.3*1.0*1.0;
  _crossSections["WpZtvll"] = 79.62*1.0*1.0;
  _crossSections["WpZlvtt"] = 79.62*1.0*1.0;
  _crossSections["WpZtvtt"] = 39.81*1.0*1.0;
  _crossSections["WmZtvll"] = 43.02*1.0*1.0;
  _crossSections["WmZlvtt"] = 43.02*1.0*1.0;
  _crossSections["WmZtvtt"] = 21.51*1.0*1.0;
  _crossSections["WpZqqtt"] = 249.18*1.0*1.0;
  _crossSections["WmZqqtt"] = 134.65*1.0*1.0;

  _crossSections["Wmevg"] = 18530*1.0*1.0;
  _crossSections["Wmmvg"] = 18620*1.0*1.0;
  _crossSections["Wmtvg"] = 1680*1.0*1.0;
  _crossSections["Wmhdgg"] = 118615;
  _crossSections["Wmlepgg"] = 118616;
  _crossSections["Wpevg"] = 27964*1.0*1.0;
  _crossSections["Wpmvg"] = 27939*1.0*1.0;
  _crossSections["Wptvg"] = 25417*1.0*1.0;
  _crossSections["Wphdgg"] = 118617;
  _crossSections["Wplepgg"] = 118618;

  _crossSections["ZZllll"] = 24.68*1.0*1.0;
  _crossSections["ZZllqq"] = 523.54*1.0*1.0;
  _crossSections["ZZllvv"] = 150.33*1.0*1.0;
  _crossSections["ZZttvv"] = 5689.5*1.0*1.0;
  _crossSections["ZZ2l2t"] = 24.68*1.0*1.0;
  _crossSections["ZZ4t"] = 6.17*1.0*1.0;

  _crossSections["ttbar"] = 164570*1.0*0.5562;
  _crossSections["ttbar2"] = 164570*1.0*0.5562;
  //Acer
  _crossSections["tb-e"] = 6930*1.0*1.0;
  _crossSections["tb-m"] = 6930*1.0*1.0;
  _crossSections["tb-t"] = 6930*1.0*1.0;
  _crossSections["tqb-e"] = 358.83*1.0*1.0;
  _crossSections["tqb-m"] = 358.83*1.0*1.0;
  _crossSections["tqb-t"] = 358.83*1.0*1.0;
  _crossSections["tW"] = 15600*1.0*1.0;

  //MC@Nlo/Jimmy
//   _crossSections["tb-e"] = 6340*1.0*1.0;
//   _crossSections["tb-m"] = 6340*1.0*1.0;
//   _crossSections["tb-t"] = 6340*1.0*1.0;
//   _crossSections["tqb-e"] = 430*1.0*1.0;
//   _crossSections["tqb-m"] = 430*1.0*1.0;
//   _crossSections["tqb-t"] = 430*1.0*1.0;
//   _crossSections["tW"] = 13100*1.0*1.0;

  _crossSections["fullZmmNp5"] = 1.0;
  _crossSections["fulltq-m"] = 1.0;

  std::map<std::string, double>::iterator iMap = _crossSections.find(sample);
  if (iMap == _crossSections.end()) {
    std::cout << "No cross section information found for chain " << sample << std::endl;
    std::exit(1);
  }
  return _crossSections[sample];
}

///////////////////////////////////////////////////////////////////////////////////

std::string GetSampleNumber(std::string sampleName)
{
  std::map<std::string, std::string> _setNumbers;

  _setNumbers["testhww150"] = "109073";

  _setNumbers["ggfhww150"] = "109073";
  _setNumbers["ggfhwwt150"] = "109970";
  _setNumbers["ggfhwwtt150"] = "109971";
  _setNumbers["vbfhww150"] = "109945";

  _setNumbers["WWee"] = "105921";
  _setNumbers["WWem"] = "105922";
  _setNumbers["WWet"] = "105923";
  _setNumbers["WWmm"] = "105924";
  _setNumbers["WWme"] = "105925";
  _setNumbers["WWmt"] = "105926";
  _setNumbers["WWtt"] = "105927";
  _setNumbers["WWte"] = "105928";
  _setNumbers["WWtm"] = "105929";
  _setNumbers["ggWWee"] = "106011";
  _setNumbers["ggWWem"] = "106012";
  _setNumbers["ggWWet"] = "106013";
  _setNumbers["ggWWmm"] = "106014";
  _setNumbers["ggWWme"] = "106015";
  _setNumbers["ggWWmt"] = "106016";
  _setNumbers["ggWWtt"] = "106017";
  _setNumbers["ggWWte"] = "106018";
  _setNumbers["ggWWtm"] = "106019";

  _setNumbers["WmZlvll"] = "105971";
  _setNumbers["WmZlvqq"] = "105970";
  _setNumbers["WmZlvtt"] = "106028";
  _setNumbers["WmZqqll"] = "105972";
  _setNumbers["WmZqqtt"] = "113190";
  _setNumbers["WmZtvll"] = "106027";
  _setNumbers["WmZtvtt"] = "106029";

  _setNumbers["WpZlvll"] = "105941";
  _setNumbers["WpZlvqq"] = "105940";
  _setNumbers["WpZlvtt"] = "106025";
  _setNumbers["WpZqqll"] = "105942";
  _setNumbers["WpZqqtt"] = "113191";
  _setNumbers["WpZtvll"] = "106024";
  _setNumbers["WpZtvtt"] = "106026";

  _setNumbers["ZZllll"] = "105931";
  _setNumbers["ZZllqq"] = "105930";
  _setNumbers["ZZllvv"] = "105932";
  _setNumbers["ZZttqq"] = "113193";
  _setNumbers["ZZttvv"] = "113192";
  _setNumbers["ZZ2l2t"] = "106036";
  _setNumbers["ZZ4t"] = "106037";
  _setNumbers["ZZ4l"] = "116600";

  _setNumbers["Wmevg"] = "108288";
  _setNumbers["Wmmvg"] = "108289";
  _setNumbers["Wmtvg"] = "108290";
  _setNumbers["Wmhdgg"] = "118615";
  _setNumbers["Wmlepgg"] = "118616";
  _setNumbers["Wpevg"] = "106001";
  _setNumbers["Wpmvg"] = "106002";
  _setNumbers["Wptvg"] = "106003";
  _setNumbers["Wphdgg"] = "118617";
  _setNumbers["Wplepgg"] = "118618";

  _setNumbers["WevNp0"] = "107680";
  _setNumbers["WevNp1"] = "107681";
  _setNumbers["WevNp2"] = "107682";
  _setNumbers["WevNp3"] = "107683";
  _setNumbers["WevNp4"] = "107684";
  _setNumbers["WevNp5"] = "107685";
  _setNumbers["WmvNp0"] = "107690";
  _setNumbers["WmvNp1"] = "107691";
  _setNumbers["WmvNp2"] = "107692";
  _setNumbers["WmvNp3"] = "107693";
  _setNumbers["WmvNp4"] = "107694";
  _setNumbers["WmvNp5"] = "107695";
  _setNumbers["WtvNp0"] = "107700";
  _setNumbers["WtvNp1"] = "107701";
  _setNumbers["WtvNp2"] = "107702";
  _setNumbers["WtvNp3"] = "107703";
  _setNumbers["WtvNp4"] = "107704";
  _setNumbers["WtvNp5"] = "107705";

  _setNumbers["PythiaWt"] = "106022";

  _setNumbers["WbbNp0"] = "106280";
  _setNumbers["WbbNp1"] = "106281";
  _setNumbers["WbbNp2"] = "106282";
  _setNumbers["WbbNp3"] = "106283";

  _setNumbers["ZeeNp0"] = "107650";
  _setNumbers["ZeeNp1"] = "107651";
  _setNumbers["ZeeNp2"] = "107652";
  _setNumbers["ZeeNp3"] = "107653";
  _setNumbers["ZeeNp4"] = "107654";
  _setNumbers["ZeeNp5"] = "107655";
  _setNumbers["ZmmNp0"] = "107660";
  _setNumbers["ZmmNp1"] = "107661";
  _setNumbers["ZmmNp2"] = "107662";
  _setNumbers["ZmmNp3"] = "107663";
  _setNumbers["ZmmNp4"] = "107664";
  _setNumbers["ZmmNp5"] = "107665";
  _setNumbers["ZttNp0"] = "107670";
  _setNumbers["ZttNp1"] = "107671";
  _setNumbers["ZttNp2"] = "107672";
  _setNumbers["ZttNp3"] = "107673";
  _setNumbers["ZttNp4"] = "107674";
  _setNumbers["ZttNp5"] = "107675";

  _setNumbers["PythiaZee"] = "106046";
  _setNumbers["PythiaZmm"] = "106047";
  _setNumbers["PythiaZtt"] = "106052";

  _setNumbers["dyZeeNp0"] = "116250";
  _setNumbers["dyZeeNp1"] = "116251";
  _setNumbers["dyZeeNp2"] = "116252";
  _setNumbers["dyZeeNp3"] = "116253";
  _setNumbers["dyZeeNp4"] = "116254";
  _setNumbers["dyZeeNp5"] = "116255";
  _setNumbers["dyZmmNp0"] = "116260";
  _setNumbers["dyZmmNp1"] = "116261";
  _setNumbers["dyZmmNp2"] = "116262";
  _setNumbers["dyZmmNp3"] = "116263";
  _setNumbers["dyZmmNp4"] = "116264";
  _setNumbers["dyZmmNp5"] = "116265";
  _setNumbers["dyZttNp0"] = "116270";
  _setNumbers["dyZttNp1"] = "116271";
  _setNumbers["dyZttNp2"] = "116272";
  _setNumbers["dyZttNp3"] = "116273";
  _setNumbers["dyZttNp4"] = "116274";
  _setNumbers["dyZttNp5"] = "116275";

  _setNumbers["PythiaDYLee"] = "105148";
  _setNumbers["PythiaDYee"] = "108320";
  _setNumbers["PythiaDYLmm"] = "113700";
  _setNumbers["PythiaDYmm"] = "113701";
  _setNumbers["PythiaDYtt"] = "107055";

  _setNumbers["ttbar"] = "105200";
  _setNumbers["ttbar2"] = "117200";
  _setNumbers["tW"] = "105500";
  _setNumbers["tb-e"] = "117360";
  _setNumbers["tb-m"] = "117361";
  _setNumbers["tb-t"] = "117362";
  _setNumbers["tqb-e"] = "108343";
  _setNumbers["tqb-m"] = "108344";
  _setNumbers["tqb-t"] = "108345";

  std::map<std::string, std::string>::iterator iMap = _setNumbers.find(sampleName);
  if (iMap == _setNumbers.end()) 
  {
    std::cout << "No set number associated with " << sampleName << std::endl;
    std::exit(1);
  }
  return _setNumbers[sampleName];
}

///////////////////////////////////////////////////////////////////////////////////

double GetSampleWeight(std::string sample)
{
  std::string file = "HiggsAnalysis/samples/sampleWeights.txt";
  ifstream infile (file.c_str());
  std::string line;
  double weight = 0.0;
  if (infile.is_open())
  {
    while (infile.good())
    {
      getline(infile,line);
      if (line.find(" " + sample) != string::npos)
      {
	std::vector<std::string> holder = SplitString(line);
// 	std::string bit = holder[4];
	weight += MakeDouble(holder[4]);
      }
    }
  }

  std::string file2 = "HiggsAnalysis/samples/sampleEfficiencies.txt";
  ifstream infile2 (file2.c_str());
  std::string line2;
  double effic = 0.0;
  if (infile2.is_open())
  {
    while (infile2.good())
    {
      getline(infile2,line2);
      if (line2.find(GetSampleNumber(sample)) != string::npos)
      {
	std::vector<std::string> holder = SplitString(line2);
	effic += MakeDouble(holder[4]);
      }
    }
  }

  if (weight == 0)
  {
    std::cout << "No weight information found for chain " << sample << std::endl;
    std::exit(1);
  }
  if (effic == 0)
  {
    std::cout << "No efficiency found for chain " << sample << std::endl;
    std::exit(1);
  }
  if (effic > 1)
  {
    std::cout << "Found bad efficiency; efficiency = " << effic << " for " << sample << std::endl;
    std::exit(1);
  }
  return (weight/effic);
}
