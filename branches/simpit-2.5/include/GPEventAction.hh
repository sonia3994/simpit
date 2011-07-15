// $Id: GPEventAction.hh,v 1.12 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPEventAction_h
#define GPEventAction_h 1
#include "GPTargetHit.hh"
#include "GPParticleHit.hh"

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4UserEventAction.hh"
#include <fstream>
using namespace std;

class GPEventActionMessenger;
//class GPParticleHitsCollection;
//class GPTargetHitsCollection;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GPEventAction : public G4UserEventAction// :public fstream
{
public:
  GPEventAction();
  virtual ~GPEventAction();

  void  BeginOfEventAction(const G4Event*);
  void  EndOfEventAction(const G4Event*);
  inline  G4int GetEventID() const {return iEventID;};
  inline void  SetPrintModulo(G4int    val)  {iPrintModel = val;};
  void  SetTrajectoryFlag(G4int i){iTrajectoryFlag=i;};                  
   
private:
   G4double  dEnergyTar;
   G4double  dTrackL;
   G4int     iPrintModel;
   G4int     iNPositronPerEvt;
   G4int     iEventID;
    //-1, record all trajectories, 0, doesn't record, other positive n, record n events' trajectory.
   G4int     iTrajectoryFlag;
                             
   GPEventActionMessenger*  eventMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
