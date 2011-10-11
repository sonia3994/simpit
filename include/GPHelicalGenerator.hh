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

#include <iostream>
//using namespace std;

extern CLHEP::RanecuEngine ranecuEngine;

class G4ParticleGun;
class G4Event;

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

    
    G4ParticleGun* 				particleGun;
    CLHEP::RandGauss* 			randGauss;
//    CLHEP::RandFlat*			randFlat;
};

#endif


