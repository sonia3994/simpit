// $Id: GPHelicalGenerator.hh,v 1.5 2006/06/29 17:47:17 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPHelicalGenerator_h
#define GPHelicalGenerator_h 1

#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/RandFlat.h"
#include "G4VPrimaryGenerator.hh"
#include "G4UIcommand.hh"
#include "Randomize.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
#include <gsl/gsl_interp.h>
#include <iostream>
//using namespace std;

extern CLHEP::RanecuEngine ranecuEngine;

class G4ParticleGun;
class G4Event;
//class gsl_interp_accel;
//class gsl_spline;

class GPHelicalGenerator : public G4VPrimaryGenerator
{
  public:
    GPHelicalGenerator();
    ~GPHelicalGenerator();

  public:
    void SetParameter(std::string);
    void SetParameter(std::string, std::string);
    G4double GetParameter(std::string);
    G4double GetParameter(std::string,std::string);
    void GeneratePrimaryVertex(G4Event* evt);
    void Print();
    void Print(std::ofstream& );

  protected:
    void SetParticleStyle(G4String);
    void InitInterPolator();
    void ClearInterPolator();
    double PhotonEnergy();
    double PhotonSz(double);

  private:
    G4int 		verbose;
    G4int 		iNParticles;
    G4String 	sParticleStyle;
    G4String 	sInputFile;
    G4double 	dPositionMean;
    G4double 	dPositionRMS;
    G4double 	dEnergyMean;
    G4double 	dEnergyRMS;
    G4double 	dMommentumMean;
    G4double 	dMommentumRMS;
    G4double 	dParticlePosZ;
    G4double 	dBunchLength;
    G4ThreeVector vectMommentumDirection;
    G4double 	dPolX;
    G4double 	dPolY;
    G4double 	dPolZ;

    gsl_interp_accel* strEnergyAcc;
    //gsl_spline* strEnInterp;
    gsl_interp* strEnInterp;

    gsl_interp_accel* strPolAcc;
    //gsl_spline* strPolInterp;
    gsl_interp* strPolInterp;


    G4ParticleGun* 				particleGun;
    CLHEP::RandGauss* 			randGauss;
//    CLHEP::RandFlat*			randFlat;
  double *daEnergy;
  double *daEnSpec; 
  double *daPol; 
};

#endif


