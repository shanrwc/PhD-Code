#ifndef __MTREE_EVENTINFO_H
#define __MTREE_EVENTINFO_H

#include "TROOT.h"
#include "TObject.h"
#include "TMath.h"
#include <iostream>

namespace MTree {

class EventInfo : public TObject {
 public:
  EventInfo();
  ~EventInfo();
  void Initialize(int RunNum, int EvtNum, int BunchCrossID, int LumiBlock, double EventWeight, int LArError);
  void SetGeneratorInfo(double GeneratorEventWeight, int SignalProcessID, int MCEventNumber, 
			double EventScale, double AlphaQCD, double AlphaQED, int PdfID1,
			int PdfID2, double PdfX1, double PdfX2, double PdfScale, double Pdf1,
			double Pdf2);

  
  int RunNumber() const {return _RunNumber;}
  int EventNumber() const {return _EventNumber;}
  double EventWeight() const {return _EventWeight;}
  int BunchCrossID() const {return _BunchCrossID;}
  int LumiBlock() const {return _LumiBlock;}
  int LArError() const {return _LArError;}

  double GeneratorEventWeight() const {return _GeneratorEventWeight;}
  int SignalProcessID() const {return _SignalProcessID;}
  int MCEventNumber() const {return _MCEventNumber;}
  double EventScale() const {return _EventScale;}
  double AlphaQCD() const {return _AlphaQCD;}
  double AlphaQED() const {return _AlphaQED;}
  int PdfID1() const {return _PdfID1;}
  int PdfID2() const {return _PdfID2;}
  double PdfX1() const {return _PdfX1;}
  double PdfX2() const {return _PdfX2;}
  double PdfScale() const {return _PdfScale;}
  double Pdf1() const {return _Pdf1;}
  double Pdf2() const {return _Pdf2;}


  void ClearVariables();

  void PrintData(std::ostream& s) const;
  
  ClassDef(EventInfo,1);
  
 private:

  Int_t _RunNumber;
  Int_t _EventNumber;
  Double_t _EventWeight;
  Int_t _BunchCrossID;
  Int_t _LumiBlock;
  Int_t _LArError;

  Double_t _GeneratorEventWeight;
  Int_t _SignalProcessID;
  Int_t _MCEventNumber;
  Double_t _EventScale;
  Double_t _AlphaQCD;
  Double_t _AlphaQED;
  Int_t _PdfID1;
  Int_t _PdfID2;
  Double_t _PdfX1;
  Double_t _PdfX2;
  Double_t _PdfScale;
  Double_t _Pdf1;
  Double_t _Pdf2;  

};

}

#endif
