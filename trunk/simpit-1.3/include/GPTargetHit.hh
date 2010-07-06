
#ifndef GPTargetHit_h
#define GPTargetHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"

class G4AttDef;

class GPTargetHit : public G4VHit
{
  public:

      GPTargetHit();
      GPTargetHit(G4LogicalVolume* logVol,G4int x,G4int y,G4int z);
      ~GPTargetHit();
      GPTargetHit(const GPTargetHit &right);
      const GPTargetHit& operator=(const GPTargetHit &right);
      G4int operator==(const GPTargetHit &right) const;

      inline void *operator new(size_t);
      inline void operator delete(void *aHit);

      void Draw();
      const std::map<G4String,G4AttDef>* GetAttDefs() const;
      std::vector<G4AttValue>* CreateAttValues() const;
      void Print();

  private:
      G4int XCellID;
      G4int YCellID;
      G4int ZCellID;
      G4double edep;
      G4ThreeVector pos;
      G4RotationMatrix rot;
      const G4LogicalVolume* pLogV;
      static std::map<G4String,G4AttDef> fAttDefs;

  public:
      inline void SetCellID(G4int x,G4int y, G4int z)
      {
        XCellID = x;
        YCellID = y;
        ZCellID = z;
      }
      inline G4int GetZ() { return ZCellID; }
      inline G4int GetX() { return XCellID; }
      inline G4int GetY() { return YCellID; }
      inline void SetEdep(G4double de)
      { edep = de; }
      inline void AddEdep(G4double de)
      { edep += de; }
      inline G4double GetEdep()
      { return edep; }
      inline void SetPos(G4ThreeVector xyz)
      { pos = xyz; }
      inline G4ThreeVector GetPos()
      { return pos; }
      inline void SetRot(G4RotationMatrix rmat)
      { rot = rmat; }
      inline G4RotationMatrix GetRot()
      { return rot; }
      inline const G4LogicalVolume * GetLogV()
      { return pLogV; }

};

typedef G4THitsCollection<GPTargetHit> GPTargetHitsCollection;

extern G4Allocator<GPTargetHit> GPTargetHitAllocator;

inline void* GPTargetHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) GPTargetHitAllocator.MallocSingle();
  return aHit;
}

inline void GPTargetHit::operator delete(void *aHit)
{
  GPTargetHitAllocator.FreeSingle((GPTargetHit*) aHit);
}

#endif


