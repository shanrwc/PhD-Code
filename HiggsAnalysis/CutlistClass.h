#ifndef __CUTLISTCLASS_H_
#define __CUTLISTCLASS_H_

#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

class CutlistClass
{
 public:
  CutlistClass();
  ~CutlistClass();

  //stores events in appropriate list
  void addEvent(int cutNr, int runNumber, int eventNumber);

  //prints out lists to text files
  void printLists(std::string sample);

 private:
  //The important stuff
  std::map<int, std::vector< std::string> > book;

  //This was necessary . . . why doesn't C++ have a better way?  I don't know.
  // . . .
  //I just don't know.
  template <typename T>
  std::string makeString(T const& value);

};

#endif
