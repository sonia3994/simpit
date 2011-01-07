// Dummy sensitive used only to flag sensitivity
// in cells of RO geometry.
//


#include "GPDummySD.hh"
class G4Step;

GPDummySD::GPDummySD()
  : G4VSensitiveDetector("dummySD")
{}
