// $Id: GPEventHandleCrystal.hh,v 1.12 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPEVENTHANDLECRYSTALOUT_H
#define GPEVENTHANDLECRYSTALOUT_H 1
#include "GPEventHandle.hh"
#include "GPParticleHit.hh"
#include <string>

class G4Event;
class GPEventHandleCrystal : public GPEventHandle
{
  public:
    GPEventHandleCrystal(std::string,std::string);
    virtual ~GPEventHandleCrystal();
  
    void  BeginOfEventAction(const G4Event*);
    void  EndOfEventAction(const G4Event*);
    void  ProcessParticleHits(GPParticleHitsCollection*,std::string);
  protected:
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#endif

    
