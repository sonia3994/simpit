// $Id: GPEventAction.hh,v 1.12 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPEventAction_h
#define GPEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include <fstream>
using namespace std;
//ofstream positronfilename("/home/G4WorkShop/GammaPair/positron.data");

extern ofstream positronfilename;
class GPRunAction;
//class GPEventActionMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GPEventAction : public G4UserEventAction// :public fstream
{
public:
  GPEventAction(GPRunAction*);
  virtual ~GPEventAction();

  void  BeginOfEventAction(const G4Event*);
  void  EndOfEventAction(const G4Event*);
  inline  G4int GetEventID() const {return eventID;};
  inline void  SetPrintModulo(G4int    val)  {printModulo = val;};
  inline void AddTargetED(G4double de) {EnergyTar+=de;};
  inline void AddTargetStep(G4double TargetStep) {TrackL+=TargetStep;}; 
  void  AddPositron(G4ThreeVector,G4ThreeVector,G4double);                  
    
private:
   GPRunAction*  runAct;

   G4double  EnergyTar;
   G4double  TrackL;
//   G4double  de;                  
   G4int     printModulo;
   G4int     positronPerEvt;
   G4int     eventID;
//   fstream  positronfilename;
                             
//   GPEventActionMessenger*  eventMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
