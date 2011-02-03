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

class GPPrimaryGeneratorMessenger;
//class G4VPrimaryGenerator;
class GPHEPEvtInterface;
class G4ParticleGun;
class G4Event;
class GPPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    GPPrimaryGeneratorAction();
    ~GPPrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event*);
    void SetVerboseLevel(G4int);
    void SetParticleStyle(G4String);
    void SetParticleInitNumber(G4int);
    void SetParticleEnergyDistr(G4double,G4double);
    void SetParticlePositionDistr(G4double,G4double);
    void SetParticlePositionZ(G4double);
    void SetParticleMomentumDirection(G4ThreeVector);
    void SetParticleMomentumDistr(G4double,G4double);
    void SetInputFile(G4String );
    void SetEnergyUnit(G4String);
    void SetMomentumUnit(G4String);
    void SetLengthUnit(G4String);
    void SetInputFileRMSFactor(G4double);    
    void SetBunchLength(G4double);    
    void PrintPrimaryMessage();
    inline void SetHEPEvtGenerator(G4bool f)
    { bHEPEvtFlag = f; G4cout<<"The HEPEvt flag is set to: "<<f<<G4endl;}

//    G4String GetParticleStyle()	{return iNParticles;};
    inline G4int GetParticleInitNumber()	{return iNParticles;}
    inline G4String GetInputFileName()		{return	sInputFile;}
    inline G4double GetParticleEnergyMean()	{return dEnergyMean;}
    inline G4double GetParticleEnergyRMS()	{return dEnergyRMS;}
    inline G4double GetParticlePositionMean()	{return dPositionMean;}
    inline G4double GetParticlePositionRMS()	{return dPositionRMS;}
    inline G4double GetParticleMomentumMean()	{return dMommentumMean;}
    inline G4double GetParticleMomentumRMS()	{return dMommentumRMS;}

    inline G4bool GetHEPEvtGenerator() { return bHEPEvtFlag; }

	protected:
	  void GeneratePrimariesFixedParticleGun(G4Event* );

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
    
    G4bool 						bHEPEvtFlag;
    G4bool 						bFixedParticleGun;
    G4ParticleGun* 				particleGun;
    GPHEPEvtInterface* 			HEPEvt;
    CLHEP::RandGauss* 			randGauss;
//    CLHEP::RandFlat*			randFlat;
    GPPrimaryGeneratorMessenger* 	primaryMessenger;
};

#endif


