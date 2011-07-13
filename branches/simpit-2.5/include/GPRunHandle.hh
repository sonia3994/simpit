// $Id: GPRunHandle.hh,v 1.11 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPRUNHANDLE_H
#define GPRUNHANDLE_H 1

#include "GPObject.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
class GPRunHandle : public GPObject
{
public:
  GPRunHandle();
  virtual ~GPRunHandle();

  virtual void  BeginOfRunAction(const G4Run*)=0;
  virtual void  EndOfRunAction(const G4Run*)=0;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

