////////////////////////////////////////////////////////////////////////////////
//
#ifndef GPTubeSurfaceParticleScorer_h
#define GPTubeSurfaceParticleScorer_h 1

#include "GPParticleHit.hh"
#include "G4VPrimitiveScorer.hh"
//#include "G4THitsMap.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PSDirectionFlag.hh"
////////////////////////////////////////////////////////////////////////////////
// (Description)
//   This is a primitive scorer class for scoring Surface particle hit.
//
// Surface is defined at the +Z surface.
// Direction                -Z    +Z  
//   0  IN || OUT            |   ->|<-     fCurrent_InOut
//   1  IN                   |     |<-     fCurrent_In
//   2  OUT                  |   ->|       fCurrent_Out
//
//
// Created: 2010-09-08  Chenghai XU.
//
///////////////////////////////////////////////////////////////////////////////

class GPSurfaceParticleScorer : public G4VPrimitiveScorer
{
 
  public: // with description
      GPSurfaceParticleScorer(G4String name ,G4int surface, G4int direction, G4int depth=0);
      virtual ~GPSurfaceParticleScorer();

  protected: // with description
      virtual G4bool ProcessHits(G4Step*,G4TouchableHistory*);
      void CheckSurfaceAndDirection(G4Step*,G4VSolid*);
      void CheckForBox(G4Step*,G4Box*);
      void CheckForTube(G4Step*,G4Tubs*);

  public: 
      virtual void Initialize(G4HCofThisEvent*);
      virtual void EndOfEvent(G4HCofThisEvent*);
      virtual void clear();
      virtual void DrawAll();
      virtual void PrintAll();

  private:
      G4int  HCID;
      G4int  iSelectedSurface;
      G4int  iSelectedDirection;
      G4int  iSurface;
      G4int  iDirection;

      GPParticleHitsCollection* particleHitCollection;

};

#endif

