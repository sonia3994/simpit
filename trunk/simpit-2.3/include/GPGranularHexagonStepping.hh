// $Id: GPGranularHexagonStepping.hh,v 1.10 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPGRANULARHEXAGONSTEPPING_H 
#define GPGRANULARHEXAGONSTEPPING_H 1

#include "GPSteppingHandle.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include <fstream>
#include <map>

class GPEventAction;
//class G4VPhysicalVolume;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GPGranularHexagonStepping : public GPSteppingHandle
{
public:
  GPGranularHexagonStepping(std::string, std::string);
  virtual ~GPGranularHexagonStepping();

  void UserSteppingAction(const G4Step*);
  void Prepare();
  void CleanUp();
  void Print(std::ofstream&);
  void Print();
    
private:
  G4int   verbose;
  std::ofstream fstHangle;
  std::map<G4String,G4double>	mapSphereEDD;
  std::map<G4String,std::vector<G4double> >	mapSphereEddVec;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
