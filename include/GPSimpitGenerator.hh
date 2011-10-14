// $Id: GPSimpitGenerator.hh,v 1.5 2006/06/29 17:47:17 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPSimpitGenerator_h
#define GPSimpitGenerator_h 1

#include "CLHEP/Random/RandGauss.h"
#include "CLHEP/Random/RandFlat.h"
#include "G4VPrimaryGenerator.hh"
#include "G4UIcommand.hh"
#include "Randomize.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

#include <iostream>
#include <fstream>
//using namespace std;


class G4ParticleGun;
class G4Event;

class GPSimpitGenerator : public G4VPrimaryGenerator
{
  public:
    GPSimpitGenerator();
    ~GPSimpitGenerator();

  public:
    void SetParameter(std::string);
    void SetParameter(std::string, std::string);
    G4double GetParameter(std::string);
    G4double GetParameter(std::string,std::string);
    void GeneratePrimaryVertex(G4Event* evt);
    void Print();
    void Print(std::ofstream& );

  protected:
    void SetInputFile(G4String);

  private:
    G4int 	verbose;
    G4String 	sInputFile;
    G4double 	dParticlePosZ;
    std::fstream 	fsInputFile;


    G4ParticleGun* 	particleGun;
};

#endif


