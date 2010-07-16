#include "GPSteppingMessenger.hh"
#include "GPSteppingAction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithAString.hh"



GPSteppingMessenger::GPSteppingMessenger(
                                          GPSteppingAction* GPGun)
:GPAction(GPGun)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPSteppingMessenger::GPSteppingMessenger(GPSteppingAction*)"<<G4endl;
#endif
  generatorDir = new G4UIdirectory("/GP/stepping/");
  generatorDir->SetGuidance("Stepping control");
   
  selectParCmd = new G4UIcmdWithAString("/GP/stepping/recordedParticle",this);
  selectParCmd->SetGuidance("Select Style of the Particle.");
  selectParCmd->SetParameterName("choice",false);
  selectParCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

/*  InitNumbCmd = new G4UIcmdWithAnInteger("/GP/primary/setParticleNum",this);
  InitNumbCmd->SetGuidance("Set number of particles.");
  InitNumbCmd->SetParameterName("NumParticle",false);
  InitNumbCmd->SetRange("NumParticle>0 && NumParticle<100");
  InitNumbCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  EneDistrCmd = new G4UIcommand("/GP/primary/setEnergyDistr", this);
  EneDistrCmd -> SetGuidance ("Set up energy distribution.");
  parameter = new G4UIparameter("mean", 'd', omitable = true);
  parameter -> SetDefaultValue (30.);
  EneDistrCmd -> SetParameter (parameter);
  parameter = new G4UIparameter("rms", 'd', omitable = true);
  parameter -> SetDefaultValue (10.);
  EneDistrCmd -> SetParameter (parameter);
  parameter = new G4UIparameter ("units", 's', omitable = true);
  parameter -> SetDefaultValue ("MeV");
  EneDistrCmd -> SetParameter (parameter);

  PosDistrCmd = new G4UIcommand("/GP/primary/setPositionDistr", this);
  PosDistrCmd -> SetGuidance ("Set up position distribution.");
  parameter = new G4UIparameter("mean", 'd', omitable = true);
  parameter -> SetDefaultValue (0.);
  PosDistrCmd -> SetParameter (parameter);
  parameter = new G4UIparameter("rms", 'd', omitable = true);
  parameter -> SetDefaultValue (1.);
  PosDistrCmd -> SetParameter (parameter);
  parameter = new G4UIparameter ("units", 's', omitable = true);
  parameter -> SetDefaultValue ("mm");
  PosDistrCmd -> SetParameter (parameter);

  MomDistrCmd = new G4UIcommand("/GP/primary/setMomentumDistr", this);
  MomDistrCmd -> SetGuidance ("Set up momentum distribution.");
  parameter = new G4UIparameter("mean", 'd', omitable = true);
  parameter -> SetDefaultValue (0.);
  MomDistrCmd -> SetParameter (parameter);
  parameter = new G4UIparameter("rms", 'd', omitable = true);
  parameter -> SetDefaultValue (0.01);
  MomDistrCmd -> SetParameter (parameter);
*/  
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPSteppingMessenger::GPSteppingMessenger(GPSteppingAction*)"<<G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPSteppingMessenger::~GPSteppingMessenger()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPSteppingMessenger::~GPSteppingMessenger()"<<G4endl;
#endif
  delete generatorDir;
  delete selectParCmd;
//  delete InitNumbCmd;
//  delete EneDistrCmd;
// delete PosDistrCmd;
//  delete MomDistrCmd;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPSteppingMessenger::~GPSteppingMessenger()"<<G4endl;
#endif
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPSteppingMessenger::SetNewValue(
                                        G4UIcommand* command, G4String newValue)
{ 
  if( command == selectParCmd )
   { GPAction->SetSelectedParticle(newValue);}

/*  if( command == InitNumbCmd )
   { GPAction->SetParticleInitNumber(InitNumbCmd->GetNewIntValue(newValue));}

  if (command == EneDistrCmd) 
   {
    G4double mean, rms;
    ConvertToDoublePair(newValue, mean, rms);
    GPAction->SetParticleEnergyDistr(mean, rms);
   }

  if (command == PosDistrCmd) 
   {
    G4double mean, rms;
    ConvertToDoublePair(newValue, mean, rms);
    GPAction->SetParticlePositionDistr(mean, rms);
   }

  if (command == MomDistrCmd) 
   {
    G4double mean, rms;
    ConvertToDoublePair(newValue, mean, rms);
    GPAction->SetParticleMomentumDistr(mean, rms);
   }
*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*void GPSteppingMessenger::ConvertToDoublePair(const G4String& paramString,
                                         G4double& xval,
                                         G4double& yval)
 {
   G4double x, y;
   char unts[30];
  
   const char* t = paramString;
   std::istrstream is((char*)t);
   is >> x >> y >> unts;
   G4String unt = unts;

   xval = x*G4UIcommand::ValueOf(unt);
   yval = y*G4UIcommand::ValueOf(unt);

 return;
 }
*/
