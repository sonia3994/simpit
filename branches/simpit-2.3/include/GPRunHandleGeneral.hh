// $Id: GPRunHandleGeneral.hh,v 1.11 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPRUNHANDLEGENERAL_H
#define GPRUNHANDLEGENERAL_H 1

#include "GPRunHandle.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
class GPRunHandleGeneral : public GPRunHandle
{
  public:
    GPRunHandleGeneral(std::string , std::string );
    virtual ~GPRunHandleGeneral();

    virtual void  BeginOfRunAction(const G4Run*);
    virtual void  EndOfRunAction(const G4Run*);
  protected:
    int iRunID;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

