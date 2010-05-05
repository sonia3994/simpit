//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
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

#include <iostream>
//using namespace std;

extern CLHEP::RanecuEngine ranecuEngine;

class GPPrimaryGeneratorMessenger;
class GPDetectorConstruction;
//class G4VPrimaryGenerator;
class GPHEPEvtInterface;
class G4ParticleGun;
class G4Event;

class GPPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    GPPrimaryGeneratorAction(GPDetectorConstruction*);
    ~GPPrimaryGeneratorAction();

  public:
    void GeneratePrimaries(G4Event*);
    void SetVerboseLevel(G4int);
    void SetParticleStyle(G4String);
    void SetParticleInitNumber(G4int);
    void SetParticleEnergyDistr(G4double,G4double);
    void SetParticlePositionDistr(G4double,G4double);
    void SetParticlePositionZ(G4double);
    void SetParticleMomentumDistr(G4double,G4double);
//    G4String GetParticleStyle()	{return n_particle;};
    inline G4int GetParticleInitNumber()	{return n_particle;}
    inline G4String GetInputFileName()		{return	inputFile;}
    inline G4double GetParticleEnergyMean()	{return eEnergyMean;}
    inline G4double GetParticleEnergyRMS()	{return eEnergyRMS;}
    inline G4double GetParticlePositionMean()	{return ePositionMean;}
    inline G4double GetParticlePositionRMS()	{return ePositionRMS;}
    inline G4double GetParticleMomentumMean()	{return eMomentumMean;}
    inline G4double GetParticleMomentumRMS()	{return eMomentumRMS;}

    inline void SetHEPEvtGenerator(G4bool f)
    { HEPEvtFlag = f; G4cout<<"The HEPEvt flag is set to: "<<f<<G4endl;}
    inline G4bool GetHEPEvtGenerator() { return HEPEvtFlag; }
    void SetInputFile(G4String );
    void SetEnergyUnit(G4String);
    void SetMomentumUnit(G4String);
    void SetLengthUnit(G4String);
    void SetInputFileRMSFactor(G4double);    
    void SetBunchLength(G4double);    
    void PrintPrimaryMessage();

  private:
    G4int 		verbose;
    G4int 		n_particle;
    G4String 	particleStyle;
    G4String 	inputFile;
    G4double 	ePositionMean;
    G4double 	ePositionRMS;
    G4double 	eEnergyMean;
    G4double 	eEnergyRMS;
    G4double 	eMomentumMean;
    G4double 	eMomentumRMS;
    G4double 	particlePosZ;
    G4double 	bunchLength;
    
    G4bool 						HEPEvtFlag;
    G4ParticleGun* 				particleGun;
    GPHEPEvtInterface* 			HEPEvt;
    GPDetectorConstruction* 	myDetector;
    CLHEP::RandGauss* 			randGauss;
//    CLHEP::RandFlat*			randFlat;
    GPPrimaryGeneratorMessenger* 	primaryMessenger;
};

#endif


