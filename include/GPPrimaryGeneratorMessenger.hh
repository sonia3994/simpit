// $Id: GPPrimaryGeneratorMessenger.hh,v 1.8 2007/07/02 13:22:08 vnivanch Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef GPPrimaryGeneratorMessenger_h
#define GPPrimaryGeneratorMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class GPPrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcommand;
class G4UIcmdWithAString;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;
//class G4UIcmdWithADouble;
//class G4UIcmdWithoutParameter;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class GPPrimaryGeneratorMessenger: public G4UImessenger
{
public:
  GPPrimaryGeneratorMessenger			(GPPrimaryGeneratorAction*);
  virtual ~GPPrimaryGeneratorMessenger	();
    
  void SetNewValue					(G4UIcommand*, G4String);
  void ConvertToDoublePair				(const G4String& ,G4double&,G4double&); 
private:
  GPPrimaryGeneratorAction*			GPPrimary;
  G4UIdirectory*						generatorDir; 
  G4UIcmdWithAString*				ParStyleCmd;
  G4UIcmdWithAString*				InitFileCmd;
  G4UIcmdWithAString*				EnergyUnitCmd;
  G4UIcmdWithAString*				LengthUnitCmd;
  G4UIcmdWithAString*				MomentumUnitCmd;
  G4UIcmdWithAString*				MomentumDirectionCmd;
  G4UIcmdWithABool*				UseHEPEvtCmd;
  G4UIcmdWithAnInteger*				InitNumbCmd;
  G4UIcmdWithAnInteger*				VerboseCmd;
  G4UIcommand*					EneDistrCmd;
  G4UIcommand*					PosDistrCmd;
  G4UIcommand*					MomDistrCmd;
  G4UIcmdWithADoubleAndUnit*			ParPosZCmd;
  G4UIcmdWithADoubleAndUnit*			InputFileRMSFactorCmd;
  G4UIcmdWithADoubleAndUnit*			BunchLengthCmd;
  G4UIcmdWithoutParameter* 			PrintParaCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

