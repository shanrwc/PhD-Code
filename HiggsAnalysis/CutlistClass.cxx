#include "CutlistClass.h"

CutlistClass::CutlistClass()
{
  ;
}

//########################################################################################

CutlistClass::~CutlistClass()
{
  book.clear();
}

//########################################################################################

template<typename T>
std::string CutlistClass::makeString(T const& value)
{
  std::stringstream sstr;
  sstr << value;
  return sstr.str();
}

//########################################################################################

void CutlistClass::addEvent(int cutNr, int runNumber, int eventNumber)
{
  std::string tag = makeString(runNumber) + "_" + makeString(eventNumber);
//   std::string tag = makeString(eventNumber);

  if (book.find(cutNr) == book.end())
  {

    std::vector<std::string> entry;
    entry.push_back(tag);

    book[cutNr] = entry;
  }
  else
  {

    book[cutNr].push_back(tag);
  }
}

//########################################################################################

void CutlistClass::printLists(std::string sample)
{
  std::map<int, std::vector<std::string> >::iterator iter;

  for (iter = book.begin(); iter != book.end(); ++iter)
  {
    //Open a file to put list in
    std::string filename = "event-summaries/" + sample + "_cut" + makeString(iter->first) + ".txt";
    std::ofstream outfile;
    outfile.open(filename.c_str());

    //Get vector, loop over entries, and write them out to the file
    std::vector<std::string> vec = iter->second;

    for (int i = 0; i != (int)vec.size(); ++i)
    {
      outfile << vec[i] << std::endl;
    }

    //Close the file
    outfile.close();
  }
}

//########################################################################################
