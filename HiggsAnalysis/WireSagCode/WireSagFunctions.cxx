#include "WireSagAnalysis.h"

int WireSagAnalysis::findTower(double eta, double phi)
{
  int tower = 0;

  if ((eta >= 1.0) && (eta < 1.4))
  {
    tower = 400;
  }
  else if ((eta >= 1.4) && (eta < 1.8))
  {
    tower = 300;
  }
  else if ((eta >= 1.8) && (eta < 2.2))
  {
    tower = 200;
  }
  else if ((eta >= 2.2) && (eta < 2.7))
  {
    tower = 100;
  }
  else
  {
    std::cout << "Found a muon out of range in eta!" << std::endl;
  }
  std::cout << tower << std::endl;

  double index = phi*16/3.14159;
  std::cout << index << std::endl;
  if (TMath::Abs(index) < 1)
  {
    tower += 1;
  }
  else if (TMath::Abs(index) < 9)
  {
    tower += 9;
  }
  else if (index > 0)
  {
    tower += (int)index + 1;
  }
  else if (index < 0)
  {
    tower += 17 - TMath::Abs((int)index);
  }

  return(tower);
}

double WireSagAnalysis::getX(double eta, double phi)
{
  double x = 0;
  double z = 14.294;

  x = (z*TMath::Cos(phi))/(TMath::Cos(2*TMath::ATan(TMath::Exp(-1*eta))));

  return(x);
}

double WireSagAnalysis::getY(double eta, double phi)
{
  double y = getX(eta,TMath::Abs(phi))*TMath::Tan(TMath::Abs(phi));

  if (phi < 0)
  {
    y = -1*y;
  }

  return(y);
}


