// $Id: GPPrimaryGeneratorMessenger.cc,v 1.9 2006/06/29 17:49:09 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPPrimaryGeneratorMessenger.hh"
#include "GPPrimaryGeneratorAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UnitsTable.hh"


//#include <strstream>
#include <sstream>
//#include <cctype>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPPrimaryGeneratorMessenger::GPPrimaryGeneratorMessenger(
                                          GPPrimaryGeneratorAction* GPGun)
:GPPrimary(GPGun)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPPrimaryGeneratorMessenger::GPPrimaryGeneratorMessenger(GPPrimaryGeneratorAction*)"<<G4endl;
#endif
  G4bool omitable;
  G4UIparameter* parameter;
  generatorDir = new G4UIdirectory("/GP/primary/");
  generatorDir->SetGuidance("PrimaryGenerator control");
   
  ParStyleCmd = new G4UIcmdWithAString("/GP/primary/initialParticle",this);
  ParStyleCmd->SetGuidance("Select type of the Particle.");
  ParStyleCmd->SetParameterName("ParStyleCmd",false);
  ParStyleCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  InitFileCmd = new G4UIcmdWithAString("/GP/primary/HEPEvtInputFile",this);
  InitFileCmd->SetGuidance("Select input file which cantains events.");
  InitFileCmd->SetParameterName("InitFileCmd",false);
  InitFileCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  EnergyUnitCmd = new G4UIcmdWithAString("/GP/primary/energyUnitForInputFile",this);
  EnergyUnitCmd->SetGuidance("Set the energy unit for the input file's data.");
  EnergyUnitCmd->SetParameterName("EnergyUnitCmd",false);
  EnergyUnitCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
 
  MomentumUnitCmd = new G4UIcmdWithAString("/GP/primary/momentumUnitForInputFile",this);
  MomentumUnitCmd->SetGuidance("Set the momentum unit for the input file's data.");
  MomentumUnitCmd->SetParameterName("MomentumUnitCmd",false);
  MomentumUnitCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
 
  MomentumDirectionCmd = new G4UIcmdWithAString("/GP/primary/momentumDirection",this);
  MomentumDirectionCmd->SetGuidance("Set the momentum direction for the paticle gun, the direction form (0,0,0) to (x,y,z). ");
  MomentumDirectionCmd->SetParameterName("MomentumDirectionCmd",false);
  MomentumDirectionCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
 
  LengthUnitCmd = new G4UIcmdWithAString("/GP/primary/lengthUnitForInputFile",this);
  LengthUnitCmd->SetGuidance("Set the length unit for the input file's data.");
  LengthUnitCmd->SetParameterName("LengthUnitCmd",false);
  LengthUnitCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  UseHEPEvtCmd = new G4UIcmdWithABool("/GP/primary/HEPEvtFlag",this);
  UseHEPEvtCmd->SetGuidance("Switch HEPEvt state, \"true\" or flase\".");
  UseHEPEvtCmd->SetParameterName("UseHEPEvtCmd",false);
  UseHEPEvtCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  InitNumbCmd = new G4UIcmdWithAnInteger("/GP/primary/numberPerEvent",this);
  InitNumbCmd->SetGuidance("Set number of particles.");
  InitNumbCmd->SetParameterName("NumParticle",false);
  InitNumbCmd->SetRange("NumParticle>0 && NumParticle<100");
  InitNumbCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  VerboseCmd = new G4UIcmdWithAnInteger("/GP/primary/verbose",this);
  VerboseCmd->SetGuidance("Set verbose level.");
  VerboseCmd->SetParameterName("verboseLevel",false);
  VerboseCmd->SetRange("verboseLevel>0 && verboseLevel<10");
  VerboseCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  EneDistrCmd = new G4UIcommand("/GP/primary/energyDistribution", this);
  EneDistrCmd -> SetGuidance ("Set energy distribution.");
  parameter = new G4UIparameter("mean", 'd', omitable = true);
  parameter -> SetDefaultValue (30.);
  EneDistrCmd -> SetParameter (parameter);
  parameter = new G4UIparameter("rms", 'd', omitable = true);
  parameter -> SetDefaultValue (10.);
  EneDistrCmd -> SetParameter (parameter);
  parameter = new G4UIparameter ("units", 's', omitable = true);
  parameter -> SetDefaultValue ("MeV");
  EneDistrCmd -> SetParameter (parameter);

  PosDistrCmd = new G4UIcommand("/GP/primary/positionDistribution", this);
  PosDistrCmd -> SetGuidance ("Set position distribution.");
  parameter = new G4UIparameter("mean", 'd', omitable = true);
  parameter -> SetDefaultValue (0.);
  PosDistrCmd -> SetParameter (parameter);
  parameter = new G4UIparameter("rms", 'd', omitable = true);
  parameter -> SetDefaultValue (1.);
  PosDistrCmd -> SetParameter (parameter);
  parameter = new G4UIparameter ("units", 's', omitable = true);
  parameter -> SetDefaultValue ("mm");
  PosDistrCmd -> SetParameter (parameter);

  MomDistrCmd = new G4UIcommand("/GP/primary/momentumDistribution", this);
  MomDistrCmd -> SetGuidance ("Set momentum distribution.");
  parameter = new G4UIparameter("mean", 'd', omitable = true);
  parameter -> SetDefaultValue (0.);
  MomDistrCmd -> SetParameter (parameter);
  parameter = new G4UIparameter("rms", 'd', omitable = true);
  parameter -> SetDefaultValue (0.01);
  MomDistrCmd -> SetParameter (parameter);

  ParPosZCmd = new G4UIcmdWithADoubleAndUnit("/GP/primary/particlePositonZ",this);
  ParPosZCmd->SetGuidance("Set incident paritcle Z position");
  ParPosZCmd->SetParameterName("ParitclePosZ",false);
  ParPosZCmd->SetUnitCategory("Length");    
  ParPosZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  BunchLengthCmd = new G4UIcmdWithADoubleAndUnit("/GP/primary/bunchLength",this);
  BunchLengthCmd->SetGuidance("Set bunch length");
  BunchLengthCmd->SetParameterName("BunchLength",false);
  BunchLengthCmd->SetUnitCategory("Time");    
  BunchLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  InputFileRMSFactorCmd = new G4UIcmdWithADoubleAndUnit("/GP/primary/RMSFactorForInputFile",this);
  InputFileRMSFactorCmd->SetGuidance("Set RMS factor for point electron beam impinge the crystal.");
  InputFileRMSFactorCmd->SetParameterName("RMSFactorElectronBeam",false);
  InputFileRMSFactorCmd->SetUnitCategory("Length");    
  InputFileRMSFactorCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  PrintParaCmd = new G4UIcmdWithoutParameter("/GP/primary/printParameters",this);
  PrintParaCmd->SetGuidance("Print primary particle parameters.");
  PrintParaCmd->AvailableForStates(G4State_Idle);     
  
  SetParameter = new G4UIcmdWithAString("/GP/primary/SetParameter",this);
  SetParameter->SetGuidance("Set Primary parameter.");
  SetParameter->SetGuidance("Usage: {key} {value} {unit}");
  SetParameter->SetGuidance("For example: SetParameter crystal.time.rms 10 ps");
  SetParameter->SetParameterName("choice",false);
  SetParameter->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  GetParameter = new G4UIcmdWithAString("/GP/primary/GetParameter",this);
  GetParameter->SetGuidance("Get Primary parameter.");
  GetParameter->SetGuidance("Usage: {key}");
  GetParameter->SetGuidance("For example: GetParameter crystal.time.rms");
  GetParameter->SetParameterName("choice",false);
  GetParameter->AvailableForStates(G4State_PreInit,G4State_Idle);
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPPrimaryGeneratorMessenger::GPPrimaryGeneratorMessenger(GPPrimaryGeneratorAction*)"<<G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPPrimaryGeneratorMessenger::~GPPrimaryGeneratorMessenger()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPPrimaryGeneratorMessenger::~GPPrimaryGeneratorMessenger()"<<G4endl;
#endif
  delete						generatorDir;
  delete						ParStyleCmd;
  delete						InitFileCmd;
  delete						UseHEPEvtCmd;
  delete						InitNumbCmd;
  delete						VerboseCmd;
  delete						EneDistrCmd;
  delete						PosDistrCmd;
  delete						MomDistrCmd;
  delete						ParPosZCmd;
  delete						BunchLengthCmd;
  delete						PrintParaCmd;
  delete						EnergyUnitCmd;
  delete						MomentumUnitCmd;
  delete						MomentumDirectionCmd;
  delete						LengthUnitCmd;
  delete						InputFileRMSFactorCmd;
  delete SetParameter; 
  delete GetParameter; 
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPPrimaryGeneratorMessenger::~GPPrimaryGeneratorMessenger()"<<G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPPrimaryGeneratorMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{ 
  if( command == ParStyleCmd )
   { GPPrimary->SetParticleStyle(newValue);}

  if( command == InitFileCmd )
   { GPPrimary->SetInputFile(newValue);}

  if( command == EnergyUnitCmd )
   { GPPrimary->SetEnergyUnit(newValue);}
  
  if( command == MomentumUnitCmd )
   { GPPrimary->SetMomentumUnit(newValue);}
  
  if( command == MomentumDirectionCmd )
   { 
	 std::stringstream ss(newValue);
	 G4double x,y,z;
     ss>>x>>y>>z;
     GPPrimary->SetParticleMomentumDirection(G4ThreeVector(x,y,z));
   }
  
  if( command == LengthUnitCmd )
   { GPPrimary->SetLengthUnit(newValue);}
  
  if( command == UseHEPEvtCmd )
   { GPPrimary->SetHEPEvtGenerator(UseHEPEvtCmd->GetNewBoolValue(newValue));}

  if( command == InitNumbCmd )
   { GPPrimary->SetParticleInitNumber(InitNumbCmd->GetNewIntValue(newValue));}
  
  if( command == VerboseCmd )
   { GPPrimary->SetVerboseLevel(VerboseCmd->GetNewIntValue(newValue));}


  if (command == EneDistrCmd) 
   {
    G4double mean, rms;
    ConvertToDoublePair(newValue, mean, rms);
    GPPrimary->SetParticleEnergyDistr(mean/MeV, rms/MeV);
   }

  if (command == PosDistrCmd) 
   {
    G4double mean, rms;
    ConvertToDoublePair(newValue, mean, rms);
    GPPrimary->SetParticlePositionDistr(mean/m, rms/m);
   }

  if (command == MomDistrCmd) 
   {
    G4double mean, rms;
    std::stringstream ss(newValue);
    ss>> mean>>rms;
    ss.clear();
    GPPrimary->SetParticleMomentumDistr(mean, rms);
   }
  
   if(command == ParPosZCmd)
   {
   GPPrimary->SetParticlePositionZ(ParPosZCmd->GetNewDoubleValue(newValue)/m);
   }

   if(command == BunchLengthCmd)
   {
   GPPrimary->SetBunchLength(BunchLengthCmd->GetNewDoubleValue(newValue)/picosecond);
   }

   if(command == PrintParaCmd)
   {GPPrimary->Print();}
   
   if(command == InputFileRMSFactorCmd)
   {
   GPPrimary->SetInputFileRMSFactor(InputFileRMSFactorCmd->GetNewDoubleValue(newValue)/m); 
   }
   if( command == SetParameter )
   { GPPrimary->SetParameter(newValue);}
   if( command == GetParameter )
   { GPPrimary->GetParameter(newValue);}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GPPrimaryGeneratorMessenger::ConvertToDoublePair(const G4String& paramString,
                                         G4double& xval,
                                         G4double& yval)
 {
   G4double x, y;
   G4String units;  
   std::stringstream is(paramString);
   is >> x >> y >> units;
   xval = x*G4UIcommand::ValueOf(units);
   yval = y*G4UIcommand::ValueOf(units);

 return;
 }

