// Dummy sensitive used only to flag sensitivity
// in cells of RO geometry.
//

#ifndef GPDummySD_h
#define GPDummySD_h 1

#include "G4VSensitiveDetector.hh"
class G4Step;

class GPDummySD : public G4VSensitiveDetector
{
public:
  GPDummySD();
  ~GPDummySD() {}
  
  void Initialize(G4HCofThisEvent*) {}
  G4bool ProcessHits(G4Step*,G4TouchableHistory*) {return false;}
  void EndOfEvent(G4HCofThisEvent*) {}
  void clear() {}
  void DrawAll() {}
  void PrintAll() {}
};
/*
GPDummySD::GPDummySD()
  : G4VSensitiveDetector("dummySD")
{}
*/
#endif
