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
    G4UIcmdWithAString*				UserLimitsCmd;
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

