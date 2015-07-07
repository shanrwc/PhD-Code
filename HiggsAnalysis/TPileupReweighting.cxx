/******************************************************************************
Name:        TPileupReweighting

Author:      Karsten Koeneke (CERN)
Created:     April 2011

Description: Tool to get the calculated MC pilup weight.
******************************************************************************/

// Preprocessor magic for debugging
#define XXX std::cout << "I am here: " << __FILE__ << ":" << __LINE__ << std::endl;

// This class' header
#include "TPileupReweighting.h"

// STL includes
#include <iostream>

// include math
#include <math.h>

// ROOT includes
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH1F.h>
#include <TAxis.h>
#include <TString.h>






//=============================================================================
// Constructor
//=============================================================================
TPileupReweighting::TPileupReweighting(const char* name) :
  TNamed(name,"notitle"),
  m_dataRootFile(0),
  m_dataRootHist(0),
  m_mcRootFile(0),
  m_mcRootHist(0)
{
}



//=============================================================================
// Destructor
//=============================================================================
TPileupReweighting::~TPileupReweighting()
{
}



//=============================================================================
// Initialization
//=============================================================================
int TPileupReweighting::initialize( const TString& dataRootFileName,
                                          const TString& dataRootHistName,
                                          const TString& mcRootFileName,
                                          const TString& mcRootHistName )
{
  // use an int as a StatusCode
  int sc(0);

  // Cache the current directory in the root file
  TDirectory* origDir = gDirectory;

  //-------------------------------------------------------
  // Get all ROOT files and histograms and normalize them
  //-------------------------------------------------------

  // Load the data ROOT file
  m_dataRootFile = new TFile( dataRootFileName, "READ" );
  if ( !m_dataRootFile )
    {
      std::cerr << "ERROR: No data ROOT file!" << std::endl;
      sc = -1;
      return sc;
    }
  else // Get the data ROOT histogram
    {
      m_dataRootHist = (TH1F*)m_dataRootFile->Get( dataRootHistName );
      if ( !m_dataRootHist )
        {
          std::cerr << "ERROR: No data ROOT histogram!" << std::endl;
          sc = -2;
          return sc;
        }
      else
        {
          m_dataRootHist->SetDirectory(0);
        }
      m_dataRootFile->Close();
    }



  // Load the MC ROOT file
  m_mcRootFile = new TFile( mcRootFileName, "READ" );
  if ( !m_mcRootFile )
    {
      std::cerr << "ERROR: No MC ROOT file!" << std::endl;
      sc = -3;
      return sc;
    }
  else // Get the mc ROOT histogram
    {
      m_mcRootHist = (TH1F*)m_mcRootFile->Get( mcRootHistName );
      if ( !m_mcRootHist )
        {
          std::cerr << "ERROR: No MC ROOT histogram!" << std::endl;
          sc = -4;
          return sc;
        }
      else
        {
          m_mcRootHist->SetDirectory(0);
        }
      m_mcRootFile->Close();
    }


  // Get the normalized histogram
  sc = normalizeHistogram( m_dataRootHist );
  if ( sc != 0 )
    {
      sc += -5;
      return sc;
    }
  sc = normalizeHistogram( m_mcRootHist );
  if ( sc != 0 )
    {
      sc += -6;
      return sc;
    }


  // Test that the Monte Carlo histogram is binned correctly, in case of integer Monte Carlo mu values
  int mcBins = m_mcRootHist->GetNbinsX();
  for ( int i=1; i<mcBins; ++i )
    {
      float mcBinWidth  =  m_mcRootHist->GetBinWidth(i);
      float mcMuLowEdge = m_mcRootHist->GetBinLowEdge(i);
      if ( fabs(mcBinWidth - 1.0) < 0.01 && fabs( fabs(fmod(mcMuLowEdge,1)) - 0.5 ) > 0.01 )
        {
          // Found that the MC histogram is integer-binned with bin edges at integer numbers!
          sc = -10;
          break;
        }
    }

  // Return to the original ROOT directory
  gDirectory = origDir;

  return sc;
}



//=============================================================================
// Method to calculate the pileup weight for each event
//=============================================================================
int TPileupReweighting::getPileupWeight( float mu, float& eventWeight ) 
{
  // Declare the simple StatusCode
  int sc(0);

  // Check that we have valid pointers
  if ( !m_dataRootHist )
    {
      sc = -2;
      return sc;
    }
  if ( !m_mcRootHist )
    {
      sc = -3;
      return sc;
    }

  float binScaleFactor(1.0);

  // Deal with the sepcial case of mu=100 in MC10b which just means that mu=0:
  // Assume you have official MC histogram from web page
  if ( fabs(mu - 100.0) < 0.00001 && m_mcRootHist->GetXaxis()->GetXmax() < 99.0 ) 
    {
      mu = 0.0;
    }

  // Get the bin content of both histograms
  int mcBinNumber   = m_mcRootHist->GetXaxis()->FindFixBin(mu);
  int dataBinNumber = m_dataRootHist->GetXaxis()->FindFixBin(mu);

  float dataMuLowEdge  = m_mcRootHist->GetBinLowEdge(mcBinNumber);
  float dataMuHighEdge = m_mcRootHist->GetBinLowEdge(mcBinNumber) + m_mcRootHist->GetBinWidth(mcBinNumber);

  // Deal with the sepcial case of mu=100 in MC10b which just means that mu=0:
  // Seems like you created your own MC histogram without treating the mu=100->mu=0 case from MC10b
  if ( fabs(mu - 100.0) < 0.00001 && m_mcRootHist->GetXaxis()->GetXmax() > 99.0 )
    {
      dataBinNumber = m_dataRootHist->GetXaxis()->FindFixBin(0.0);
      dataMuLowEdge  = 0.0;
      dataMuHighEdge = 0.5 * m_mcRootHist->GetBinWidth(mcBinNumber);
      binScaleFactor = 2.0;
    }
  
  int dataBinNumberLowEdge  = m_dataRootHist->GetXaxis()->FindFixBin(dataMuLowEdge);
  int dataBinNumberHighEdge = m_dataRootHist->GetXaxis()->FindFixBin(dataMuHighEdge);
  
  float dataBinContent = m_dataRootHist->Integral( dataBinNumberLowEdge, dataBinNumberHighEdge-1 );
  float mcBinContent   = m_mcRootHist->GetBinContent(mcBinNumber);
  // Get the actual weight
  if ( mcBinContent > 0.0 )
    {
      eventWeight = binScaleFactor * dataBinContent / mcBinContent;
    }


  return sc;
}



//=============================================================================
// Method to calculate the ratio histogram
//=============================================================================
int TPileupReweighting::normalizeHistogram( TH1F* hist )
{
  // Declare the simple StatusCode
  int sc(0);

  // normalize the data histogram
  if ( hist )
    {
      if ( hist->GetSumOfWeights() > 0.0 )
        {
          hist->Scale( 1.0/hist->GetSumOfWeights() );
        }
      else
        {
          sc = -20;
        }
    }
  else
    {
      sc = -10;
    }

  return sc;
}



