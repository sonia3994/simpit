
#ifndef GPTargetSD_h
#define GPTargetSD_h 1

#include "G4VSensitiveDetector.hh"
#include "GPTargetHit.hh"
#include <vector>
class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class GPTargetSD : public G4VSensitiveDetector
{

  public:
      GPTargetSD(G4String name,std::vector<G4int>);
      ~GPTargetSD();

      void Initialize(G4HCofThisEvent* HCE);
      G4bool ProcessHits(G4Step* aStep,G4TouchableHistory* ROhist);
      inline void SetEddDim(std::vector<G4int> t){numberOfCellsInX=t[0];numberOfCellsInY=t[1];numberOfCellsInZ=t[2];}; 
      void EndOfEvent(G4HCofThisEvent* HCE);
      void clear();
      void DrawAll();
      void PrintAll();

  private:
      GPTargetHitsCollection *EddCollection;
      std::vector<G4int> CellID;
      //int*** CellID;
      int numberOfCellsInX;
      int numberOfCellsInY;
      int numberOfCellsInZ;
};




#endif

