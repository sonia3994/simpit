// $Id: GPEventHandleGeneral.hh,v 1.12 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPEVENTHANDLEGENERAL_H
#define GPEVENTHANDLEGENERAL_H 1
#include "GPEventHandle.hh"
#include "GPParticleHit.hh"
#include <string>

class G4Event;
class GPEventHandleGeneral : public GPEventHandle
{
  public:
    GPEventHandleGeneral(std::string,std::string);
    virtual ~GPEventHandleGeneral();
  
    void  BeginOfEventAction(const G4Event*);
    void  EndOfEventAction(const G4Event*);
    void  ProcessParticleHits(GPParticleHitsCollection*,std::string);
  protected:
    int iEvtID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#endif

    
