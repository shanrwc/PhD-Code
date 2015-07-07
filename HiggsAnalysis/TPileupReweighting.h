// Dear emacs, this is -*-c++-*-

#ifndef __TPILEUPREWEIGHTING__
#define __TPILEUPREWEIGHTING__

/**
   @class TPileupReweighting
   @brief Tool to get the calculated MC pilup weight.

   @author Karsten Koeneke

*/

/******************************************************************************
Name:        TPileupReweighting

Author:      Karsten Koeneke (CERN)
Created:     April 2011

Description: Tool to get the calculated MC pilup weight.
******************************************************************************/

#include "TNamed.h"
#include <TFile.h>
#include <TH1F.h>
#include <TString.h>



  class TPileupReweighting : public TNamed {

  public: 
    /** Standard constructor */
    TPileupReweighting(const char* name);
    
    /** Standard destructor */
    ~TPileupReweighting();
  
  public:
    /** Initialize this class once before the event loop starts */
    int initialize( const TString& dataRootFileName,
                    const TString& dataRootHistName,
                    const TString& mcRootFileName,
                    const TString& mcRootHistName );
    
    /** The method to calculate the event weight */
    inline float getPileupWeight( float mu ) 
    {
      float eventWeight(-1.0);
      int sc(0);
      sc = getPileupWeight( mu, eventWeight );
      if ( sc == 0 )
        {
          return eventWeight;
        }
      else
        {
          return (float)sc;
        }
    }
    
    /** The method to calculate the event weight, with return status code */
    int getPileupWeight( float mu, float& eventWeight );
    

    // Private methods
  private:
    /** The method to calculate the ratio histogram */
    int normalizeHistogram( TH1F* histo );
    
        
    // Private members
  private:
    
    /** The data ROOT file */
    TFile* m_dataRootFile;
    
    /** The data histogram */
    TH1F* m_dataRootHist;
    
    
    /** The MC ROOT file */
    TFile* m_mcRootFile;
    
    /** The MC histogram */
    TH1F* m_mcRootHist;
    

    ClassDef(TPileupReweighting,1)

  }; // End: class definition

#endif
