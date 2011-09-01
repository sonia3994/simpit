
#ifndef GPParticleHit_h
#define GPParticleHit_h 1

#include "G4VHit.hh"
#include "G4Track.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
//#include "G4LogicalVolume.hh"
//#include "G4Transform3D.hh"
//#include "G4RotationMatrix.hh"

class G4AttDef;

class GPParticleHit : public G4VHit
{
  public:

      GPParticleHit();
      GPParticleHit(G4Step*, G4int);
      ~GPParticleHit();
      GPParticleHit(const GPParticleHit &right);
      const GPParticleHit& operator=(const GPParticleHit &right);
      G4int operator==(const GPParticleHit &right) const;

      inline void *operator new(size_t);
      inline void operator delete(void *aHit);

      inline G4ThreeVector GetPosition(){return vecPos;};
      inline G4ThreeVector GetMomentum(){return vecMom;};
      inline G4ThreeVector GetPolarization(){return v3Polarization;};
      inline G4double 	   GetTotalEnergy(){return dTotalEnergy;}; 
      inline G4double 	   GetGlobalTime(){return dGlobalTime;}; 
      inline void 	   SetParticleName(G4String str ){sParticleName=str;};
      inline G4String      GetParticleName(){return sParticleName;};
      inline void 	   SetTrackID(G4int i){iTrackID=i;};
      inline G4int 	   GetTrackID(){return iTrackID;};
      inline void 	   SetPDG(G4int i){iPDG=i;};
      inline G4int 	   GetPDG(){return iPDG;};

      void Draw();
      const std::map<G4String,G4AttDef>* GetAttDefs() const;
      std::vector<G4AttValue>* CreateAttValues() const;
      void Print();

  private:
      static std::map<G4String,G4AttDef> mapStrAttDef;
      G4ThreeVector     vecPos;
      G4ThreeVector     vecMom;
      G4ThreeVector     v3Polarization;
      G4String		sParticleName;
      G4double		dTotalEnergy;
      G4double		dGlobalTime;
      G4int		iTrackID;
      G4int		iPDG;


};

typedef G4THitsCollection<GPParticleHit> GPParticleHitsCollection;

extern G4Allocator<GPParticleHit> GPParticleHitAllocator;

inline void* GPParticleHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) GPParticleHitAllocator.MallocSingle();
  return aHit;
}

inline void GPParticleHit::operator delete(void *aHit)
{
  GPParticleHitAllocator.FreeSingle((GPParticleHit*) aHit);
}

#endif


