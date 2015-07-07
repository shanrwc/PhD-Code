#include "MTree/EventInfo.h"

namespace MTree {

  EventInfo::EventInfo() : _RunNumber(0), _EventNumber(0), _EventWeight(1.0), _BunchCrossID(-1), _LumiBlock(0), _LArError(0),
			   _GeneratorEventWeight(1.0), _SignalProcessID(-1), _MCEventNumber(0), _EventScale(0.0),
			   _AlphaQCD(0.0), _AlphaQED(0.0), _PdfID1(-1), _PdfID2(-1), _PdfX1(-1.0), _PdfX2(-1.0), 
			   _PdfScale(-1.0), _Pdf1(-1.0), _Pdf2(-1.0) {
}

EventInfo::~EventInfo() {

}

  void EventInfo::Initialize(int RunNum, int EvtNum, int BunchCrossID, int LumiBlock, double EventWeight, int LArError) {
  _RunNumber = RunNum;
  _EventNumber = EvtNum;
  _BunchCrossID = BunchCrossID;
  _LumiBlock = LumiBlock;
  _EventWeight = EventWeight;
  _LArError = LArError;
}
  
void EventInfo::SetGeneratorInfo(double GeneratorEventWeight, int SignalProcessID, int MCEventNumber, 
				 double EventScale, double AlphaQCD, double AlphaQED, int PdfID1,
				 int PdfID2, double PdfX1, double PdfX2, double PdfScale, double Pdf1,
				 double Pdf2) {
  _GeneratorEventWeight = GeneratorEventWeight;
  _SignalProcessID = SignalProcessID;
  _MCEventNumber = MCEventNumber;
  _EventScale = EventScale;
  _AlphaQCD = AlphaQCD;
  _AlphaQED = AlphaQED;
  _PdfID1 = PdfID1;
  _PdfID2 = PdfID2;
  _PdfX1 = PdfX1;
  _PdfX2 = PdfX2;
  _PdfScale = PdfScale;
  _Pdf1 = Pdf1;
  _Pdf2 = Pdf2;
}

void EventInfo::ClearVariables() {
  _RunNumber = _EventNumber = _BunchCrossID = _LumiBlock = _MCEventNumber = _LArError = 0;
  _EventWeight = _GeneratorEventWeight = 1.0;
  _SignalProcessID = _PdfID1 = _PdfID2 = -1;
  _EventScale = _AlphaQCD = _AlphaQED = 0.0; 
  _PdfX1 = _PdfX2 = _PdfScale = _Pdf1 = _Pdf2 = -1.0;
}

void EventInfo::PrintData(std::ostream& s) const {
  s << "*****\n* Event Info\n*****" << std::endl;
  s <<   "Run Number            : " << _RunNumber << "\nEvent Number  : " << _EventNumber 
    << "\nEvent Weight          : " << _EventWeight 
    << "\nGenerator Event Weight: " << _GeneratorEventWeight 
    << "\nBunch Cross ID        : " << _BunchCrossID    
    << "\nLumiBlock             : " << _LumiBlock
    << "\nLArError              : " << _LArError
    << "\nSignalProcessID       : " << _SignalProcessID 
    << "\nMCEventNumber         : " << _MCEventNumber
    << "\nEventScale            : " << _EventScale 
    << "\nAlphaQCD              : " << _AlphaQCD 
    << "\nAlphaQED              : " << _AlphaQED 
    << "\nPdfID1                : " << _PdfID1 
    << "\nPdfID2                : " << _PdfID2 
    << "\nPdfX1                 : " << _PdfX1 
    << "\nPdfX2                 : " << _PdfX2
    << "\nPdfScale              : " << _PdfScale 
    << "\nPdf1                  : " << _Pdf1 
    << "\nPdf2                  : " << _Pdf2 
    << std::endl;
  s << "*****\n" << std::endl;  
}

}

ClassImp(MTree::EventInfo)
