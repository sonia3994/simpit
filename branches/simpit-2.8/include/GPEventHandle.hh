// $Id: GPEventHandle.hh,v 1.12 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPEVENTHANDLE_H
#define GPEVENTHANDLE_H 1
#include "GPObject.hh"
#include <string>

class G4Event;
class GPEventHandle : public GPObject
{
  public:
    GPEventHandle();
    virtual ~GPEventHandle();
  
    virtual void  BeginOfEventAction(const G4Event*) = 0 ;
    virtual void  EndOfEventAction(const G4Event*) = 0 ;
  protected:
      
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
