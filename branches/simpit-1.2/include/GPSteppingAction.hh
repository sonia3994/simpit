//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: GPSteppingAction.hh,v 1.10 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPSteppingAction_h
#define GPSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include <fstream>

class GPSteppingMessenger;
class GPDetectorConstruction;
class GPEventAction;
class G4VPhysicalVolume;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GPSteppingAction : public G4UserSteppingAction
{
public:
  GPSteppingAction(GPDetectorConstruction*, GPEventAction*);
  virtual ~GPSteppingAction();

  void UserSteppingAction(const G4Step*);
  void SetSelectedParticle(G4String);
  void Init();
    
private:
  //void WriteToFileDT(G4ThreeVector,G4ThreeVector,G4double,G4double);
  //void WriteToFileDC(G4ThreeVector,G4ThreeVector,G4double,G4double);

  G4String 		particle;
  std::ofstream 	  writeFile;
  GPDetectorConstruction* detector;
  GPEventAction*          eventaction;
  GPSteppingMessenger*	steppingMessenger;
    const G4VPhysicalVolume* capturePhys;
    const G4VPhysicalVolume* targetPhys;
    const G4VPhysicalVolume* transferPhys;
    const G4VPhysicalVolume* vacuumPhys;
    const G4VPhysicalVolume* acceleratorPhys;

    G4double targetL;
    G4double targetX;
    G4double targetY;
    G4double captureL;
    G4double captureR;
    G4double acceleratorL;
    G4double acceleratorR;

	G4int   verbose;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
