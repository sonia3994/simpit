// $Id: GPCrystalPrimaryGA.hh,v 1.8 2006/06/29 18:08:36 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPCRYSTALPRIMARYGA_h
#define GPCRYSTALPRIMARYGA_h 1

#include "G4VPrimaryGenerator.hh"
#include "G4HEPEvtParticle.hh"
#include "Randomize.hh"
#include "G4UIcommand.hh"
#include "globals.hh"

#include <fstream>
#include <vector>
#include <sstream>
#include <string>
class G4Event;

class GPCrystalPrimaryGA:public G4VPrimaryGenerator
{
  public: 
    GPCrystalPrimaryGA();
    ~GPCrystalPrimaryGA();

  public:
    void GeneratePrimaryVertex(G4Event* evt);
    void SetParameter(std::string,std::string);
    G4double GetParameter(std::string, std::string) const;
    void Print(std::ofstream&);
    void Print();

  private:
    void Init();

  private:
    G4double	dUnitE;
    G4double	dUnitP;
    G4double	dUnitL;
    G4double	dUnitTime;
    G4double	dParticlePosZ;
    G4double	dBunchLength;
    G4double	dRadiusRMSFactor;
    G4bool	bRadiusRMSFactorFlag;

    G4int 	IDHEP;    // PDG code
    G4int 	iUseOriginalElectronFlag;    
    G4int 	iProducedPhoton;    
    G4double 	dMomentumX; // px in MeV
    G4double 	dMomentumY; // py in MeV
    G4double 	dMomentumZ; // pz in MeV
    G4double 	dMomentumTranAngleMean; //unit px and py mean in MeV
    G4double 	dMomentumTranAgngleRMS; //unit px and py rms in MeV 
    G4double 	dEnergy; // energy in MeV
    G4double 	dEnergyRMS; // energy rms in MeV
    G4double 	dEnergyMean; // energy mean in MeV
    G4double 	dMass; // mass in MeV
    G4double 	dPositionX; //production vertex x;
    G4double 	dPositionY; //production vertex y
    G4double 	dPositionZ; //production vertex z
    G4double 	dTime; //production time

    G4double 	polX;
    G4double 	polY;
    G4double 	polZ;

    CLHEP::RandGauss* 				randGauss;

};

#endif

