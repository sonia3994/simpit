//
// $Id: GPTrackInformation.hh,v 1.2 2006/06/29 17:43:19 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPTrackInformation_h
#define GPTrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"

class GPTrackInformation : public G4VUserTrackInformation 
{
  public:
    GPTrackInformation();
    GPTrackInformation(const G4Track* aTrack);
    GPTrackInformation(const GPTrackInformation* aTrackInfo);
    virtual ~GPTrackInformation();
   
    inline void *operator new(size_t);
    inline void operator delete(void *aTrackInfo);
    inline int operator ==(const GPTrackInformation& right) const
    {return (this==&right);}

    GPTrackInformation& operator =(const GPTrackInformation& right);

    void SetSourceTrackInformation(const G4Track* aTrack);
    void Print() const;

  private:
    // Information of the primary track at the primary vertex
    G4int                 originalTrackID;  // Track ID of primary particle
    G4ParticleDefinition* particleDefinition;
    G4ThreeVector         originalPosition;
    G4ThreeVector         originalMomentum;
    G4double              originalEnergy;
    G4double              originalTime;

    G4int                 trackingStatus;
    // trackingStatus = 1 : primary or secondary track which has not yet reached to calorimeter
    //                = 0 : track which or ancester of which has reached to calorimeter

    //                = 2 : track or its ancester had once reached to calorimeter and
    //                      then escaped from it
    // Information of the track which reached to the calorimeter boundary at the boundary surface
    // This information is valid only for trackingStatus = 0 or 2
    G4int                 sourceTrackID;
    G4ParticleDefinition* sourceDefinition;
    G4ThreeVector         sourcePosition;
    G4ThreeVector         sourceMomentum;
    G4double              sourceEnergy;
    G4double              sourceTime;

  public:
    inline G4int GetOriginalTrackID() const {return originalTrackID;}
    inline G4ParticleDefinition* GetOriginalParticle() const {return particleDefinition;}
    inline G4ThreeVector GetOriginalPosition() const {return originalPosition;}
    inline G4ThreeVector GetOriginalMomentum() const {return originalMomentum;}
    inline G4double GetOriginalEnergy() const {return originalEnergy;}
    inline G4double GetOriginalTime() const {return originalTime;}

    inline G4int GetTrackingStatus() const {return trackingStatus;}
    inline void SetTrackingStatus(G4int i) {trackingStatus = i;}

    inline G4int GetSourceTrackID() const {return sourceTrackID;}
    inline G4ParticleDefinition* GetSourceParticle() const {return sourceDefinition;}
    inline G4ThreeVector GetSourcePosition() const {return sourcePosition;}
    inline G4ThreeVector GetSourceMomentum() const {return sourceMomentum;}
    inline G4double GetSourceEnergy() const {return sourceEnergy;}
    inline G4double GetSourceTime() const {return sourceTime;}
};

extern G4Allocator<GPTrackInformation> GPTrackInformationAllocator;

inline void* GPTrackInformation::operator new(size_t)
{ void* aTrackInfo;
  aTrackInfo = (void*)GPTrackInformationAllocator.MallocSingle();
  return aTrackInfo;
}

inline void GPTrackInformation::operator delete(void *aTrackInfo)
{ GPTrackInformationAllocator.FreeSingle((GPTrackInformation*)aTrackInfo);}

#endif

