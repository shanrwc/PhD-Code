#ifndef __MTREE_PRIMARYVERTEX_H
#define __MTREE_PRIMARYVERTEX_H

#include "TROOT.h"
#include "TObject.h"
#include "TMath.h"

#include <iostream>

namespace MTree {

class PrimaryVertex : public TObject {
 public:
  PrimaryVertex();
  ~PrimaryVertex();
  
  void Initialize(double X, double Y, double Z, double Err_X, double Err_Y, double Err_Z, 
		  int NTracks, double Chi2, int NDOF, int Type);
  void ClearVariables();

  double X() const {return _X;}
  double Y() const {return _Y;}
  double Z() const {return _Z;}
  double ErrorX() const {return _Err_X;}
  double ErrorY() const {return _Err_Y;}
  double ErrorZ() const {return _Err_Z;}

  int NTracks() const {return _NTracks;}
  double Chi2() const {return _Chi2;}
  int NDOF() const {return _NDOF;}
  int Type() const {return _Type;}

  void PrintData(std::ostream& s) const;

  ClassDef(PrimaryVertex,1);
  
 private:
  
  double _X;
  double _Y;
  double _Z;
  double _Err_X;
  double _Err_Y;
  double _Err_Z;

  int _NTracks;
  double _Chi2;
  int _NDOF;
  int _Type;

};

}

#endif
