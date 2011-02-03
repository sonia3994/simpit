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
    void SaveAs(std::ofstream*);

    void ProcessTrajectory(G4int code=-1);

    virtual G4int GetTrackID() const
    { return iTrackID; }
    virtual G4int GetParentID() const
    { return iParentID; }
    virtual G4String GetParticleName() const
    { return sParticleName; }
    virtual G4double GetCharge() const
    { return dPDGCharge; }
    virtual G4int GetPDGEncoding() const
    { return iPDGEncoding; }
    virtual G4ThreeVector GetInitialMomentum() const
    { return vecMomentum; }
    virtual int GetPointEntries() const
    { return positionRecord->size(); }
    virtual G4VTrajectoryPoint* GetPoint(G4int i) const 
    { return (*positionRecord)[i]; }
    virtual G4double GetTrackLength() const
    {return dTrackLength;}
    virtual G4double GetZLength() const
    {return dZLength;}
    virtual G4double GetRAtZ(G4double) const;

    virtual void ShowTrajectory(std::ostream& os=G4cout) const;
    virtual void DrawTrajectory(G4int i_mode=0) const;
    virtual const std::map<G4String,G4AttDef>* GetAttDefs() const;
    virtual std::vector<G4AttValue>* CreateAttValues() const;
    virtual void AppendStep(const G4Step* aStep);
    virtual void MergeTrajectory(G4VTrajectory* secondTrajectory);

    G4ParticleDefinition* GetParticleDefinition();

    inline G4int GetTrackStatus() const
    { return iTrackStatus; }
    inline const G4ThreeVector& GetVertexPosition() const
    { return vecVertexPosition; }
    inline G4double GetGlobalTime() const
    { return dGlobalTime; }
  private:
    GPTrajectoryPointContainer* positionRecord;
    G4int                        iTrackID;
    G4int                        iParentID;
    G4int                        iTrackStatus;
    G4int                        iPDGEncoding;
    G4ParticleDefinition*        fpParticleDefinition;
    G4ThreeVector                vecMomentum;
    G4ThreeVector                vecVertexPosition;
    G4String                     sParticleName;
    G4double                     dPDGCharge;
    G4double                     dGlobalTime;
    G4double                     dTrackLength;
    G4double                     dZLength;
    std::vector<std::vector<G4double>* >*	vecFullInfor;

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

