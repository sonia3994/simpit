// $Id: GPPrimaryGeneratorAction.hh,v 1.5 2006/06/29 17:47:17 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPPrimaryGeneratorAction_h
#define GPPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/RandFlat.h"
#include "Randomize.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

#include <iostream>
//using namespace std;

extern CLHEP::RanecuEngine ranecuEngine;

class G4ParticleGun;
class G4Event;
class GPPrimaryGeneratorMessenger;
class GPHEPEvtInterface;
class GPCrystalPrimaryGA;
class GPHelicalGenerator;
class GPSimpitGenerator;

class GPPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
  GPPrimaryGeneratorAction();
  ~GPPrimaryGeneratorAction();

  public:
  void SetParameter(std::string);
  void SetParameter(std::string, std::string);
  G4double GetParameter(std::string);
  G4double GetParameter(std::string,std::string);
  void GeneratePrimaries(G4Event*);
  void Print();
  void Print(std::ofstream& );

  protected:
  void GeneratePrimariesFixedParticleGun(G4Event* );
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
  G4double 	dAngleThetaMean;
  G4double 	dAngleThetaRMS;
  G4double 	dParticlePosZ;
  G4double 	dBunchLength;
  G4ThreeVector vectMommentumDirection;
  G4double 	dPolX;
  G4double 	dPolY;
  G4double 	dPolZ;


  G4bool 				bFixedParticleGun;
  std::string				sGeneratorType;
  G4ParticleGun* 			particleGun;
  GPHEPEvtInterface* 			HEPEvt;
  GPCrystalPrimaryGA*			crystalGenerator;
  GPHelicalGenerator*			helicalGenerator;
  GPSimpitGenerator*			simpitGenerator;
  CLHEP::RandGauss* 			randGauss;
  CLHEP::RandFlat* 			randFlat;
  GPPrimaryGeneratorMessenger* 	primaryMessenger;
};

#endif


