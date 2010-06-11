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
// $Id: GPDetectorMessenger.hh,v 1.7 2006/06/29 17:48:34 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPDetectorMessenger_h
#define GPDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class GPDetectorConstruction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GPDetectorMessenger: public G4UImessenger
{
  public:
    GPDetectorMessenger				(GPDetectorConstruction* );
   ~GPDetectorMessenger				();
    
    void SetNewValue					(G4UIcommand*, G4String);
    
  private:
    GPDetectorConstruction*				GPDetector;
    
    G4UIdirectory*					GPDir;
    G4UIdirectory*					detDir;
    G4UIcmdWithAString*				TarMaterCmd;
    G4UIcmdWithAString*				DetectorSizeCmd;
    G4UIcmdWithADoubleAndUnit*			TarThickCmd;
    G4UIcmdWithADoubleAndUnit*			TarBoxXYCmd;
    G4UIcmdWithADoubleAndUnit*			CapLengthCmd;
    G4UIcmdWithADoubleAndUnit*			CapRadiusCmd;
    G4UIcommand* 					SizeWorldXYZCmd;
    G4UIcommand* 					TargetCellXYZCmd;
    G4UIcmdWithADoubleAndUnit*			SizeWorldXCmd;
    G4UIcmdWithADoubleAndUnit*			SizeWorldYCmd;
    G4UIcmdWithADoubleAndUnit*			SizeWorldZCmd;
//    G4UIcmdWithAnInteger*			NbLayersCmd;    
//    G4UIcmdWithADoubleAndUnit*		MagFieldCmd;
    G4UIcmdWithoutParameter*			UpdateCmd;
    G4UIcmdWithoutParameter*			PrintParaCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

