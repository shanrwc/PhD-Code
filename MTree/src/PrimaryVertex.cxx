#include "MTree/PrimaryVertex.h"

namespace MTree {

  PrimaryVertex::PrimaryVertex() : _X(0.0), _Y(0.0), _Z(0.0), _Err_X(0.0), _Err_Y(0.0), _Err_Z(0.0),
				   _NTracks(0), _Chi2(0.0), _NDOF(0), _Type(0) {
}

PrimaryVertex::~PrimaryVertex() {
}

void PrimaryVertex::Initialize(double X, double Y, double Z, double Err_X, double Err_Y, double Err_Z, 
			       int NTracks, double Chi2, int NDOF, int Type) {
  _X = X;
  _Y = Y;
  _Z = Z;
  _Err_X = Err_X;
  _Err_Y = Err_Y;
  _Err_Z = Err_Z;
  _NTracks = NTracks;
  _Chi2 = Chi2;
  _NDOF = NDOF;
  _Type = Type;
}

void PrimaryVertex::ClearVariables() {
  _X = _Y = _Z = _Err_X = _Err_Y = _Err_Z = _Chi2 = 0.0;
  _NTracks = _NDOF = 0;
}

void PrimaryVertex::PrintData(std::ostream& s) const {
  s << "*****\n* Primary Vertex\n*****" << std::endl;
  s <<   "x Position: " << _X << " +- " << _Err_X << "\ny Position: " << _Y << " +- " << _Err_Y 
    << "\nz Position: " << _Z << " +- " << _Err_Z << "\nNr Tracks : " << _NTracks    
    << "\nChi 2     : " << _Chi2 
    << "\nNDOF      : " << _NDOF
    << "\nChi2/NDOF : " << (_NDOF != 0 ? _Chi2 / _NDOF : 0.0) 
    << "\nType      : " << _Type
    << "\nAddress   : " << this << std::endl;
  s << "*****\n" << std::endl;
}

}

ClassImp(MTree::PrimaryVertex)
