//
// $Id: GPTrajectory.hh,v 1.3 2006/06/29 17:43:31 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPTrajectory_h
#define GPTrajectory_h 1

#include "G4VTrajectory.hh"
#include "G4Allocator.hh"
#include <stdlib.h>
#include "G4ThreeVector.hh"
#include "G4ios.hh"     
#include "globals.hh" 
#include "G4ParticleDefinition.hh" 
#include "G4TrajectoryPoint.hh"   
#include "G4Track.hh"
#include "G4Step.hh"
#include <vector>

class G4Polyline;
class G4AttDef;
class G4AttValue;

typedef std::vector<G4VTrajectoryPoint*> GPTrajectoryPointContainer;

class GPTrajectory : public G4VTrajectory
{
 public:
   GPTrajectory();
   GPTrajectory(const G4Track* aTrack);
   GPTrajectory(GPTrajectory &);
   virtual ~GPTrajectory();

   inline void* operator new(size_t);
   inline void  operator delete(void*);
   inline int operator == (const GPTrajectory& right) const
   {return (this==&right);} 

   virtual G4int GetTrackID() const
   { return fTrackID; }
   virtual G4int GetParentID() const
   { return fParentID; }
   virtual G4String GetParticleName() const
   { return ParticleName; }
   virtual G4double GetCharge() const
   { return PDGCharge; }
   virtual G4int GetPDGEncoding() const
   { return PDGEncoding; }
   virtual G4ThreeVector GetInitialMomentum() const
   { return momentum; }
   virtual int GetPointEntries() const
   { return positionRecord->size(); }
   virtual G4VTrajectoryPoint* GetPoint(G4int i) const 
   { return (*positionRecord)[i]; }

   virtual void ShowTrajectory(std::ostream& os=G4cout) const;
   virtual void DrawTrajectory(G4int i_mode=0) const;
   virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
   virtual std::vector<G4AttValue>* CreateAttValues() const;
   virtual void AppendStep(const G4Step* aStep);
   virtual void MergeTrajectory(G4VTrajectory* secondTrajectory);

   G4ParticleDefinition* GetParticleDefinition();

   inline const G4int GetTrackStatus() const
   { return fTrackStatus; }
   inline const G4ThreeVector& GetVertexPosition() const
   { return vertexPosition; }
   inline G4double GetGlobalTime() const
   { return globalTime; }
 private:
   GPTrajectoryPointContainer* positionRecord;
   G4int                        fTrackID;
   G4int                        fParentID;
   G4int                        fTrackStatus;
   G4ParticleDefinition*        fpParticleDefinition;
   G4String                     ParticleName;
   G4double                     PDGCharge;
   G4int                        PDGEncoding;
   G4ThreeVector                momentum;
   G4ThreeVector                vertexPosition;
   G4double                     globalTime;

};

extern G4Allocator<GPTrajectory> GPTrajectoryAllocator;

inline void* GPTrajectory::operator new(size_t)
{
  void* aTrajectory;
  aTrajectory = (void*)GPTrajectoryAllocator.MallocSingle();
  return aTrajectory;
}

inline void GPTrajectory::operator delete(void* aTrajectory)
{
  GPTrajectoryAllocator.FreeSingle((GPTrajectory*)aTrajectory);
}

#endif

