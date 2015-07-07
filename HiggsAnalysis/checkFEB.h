#ifndef __CHECK_FEB_H
#define __CHECK_FEB_H

/*
Author: Aaron Armbruster
Data:   May 23, 2011
Description:

Tool to check if jet is inside dead FEB.
If function returns false, object is inside the FEB
and event should be rejected.


*/

bool passElectron(double eta, double phi, double dR = 0.1)
{
  return !((eta < 1.45+dR && eta > 0-dR) && (phi < -0.592+dR && phi > -0.788-dR));
}

bool passJet(double eta, double phi, double dR = 0.4)
{
  return !((eta < 1.45+dR && eta > 0-dR) && (phi < -0.592+dR && phi > -0.788-dR));
}

#endif
