// $Id: GPFieldMessenger.cc,v 1.7 2006/06/29 17:19:32 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

#include "GPFieldMessenger.hh"

#include "GPFieldSetup.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//////////////////////////////////////////////////////////////////////////////

GPFieldMessenger::GPFieldMessenger(GPFieldSetup* pEMfield)
  : fEMfieldSetup(pEMfield)
{ 
  GPdetDir = new G4UIdirectory("/GP/field/");
  GPdetDir->SetGuidance("Field tracking control.");

  StepperCmd = new G4UIcmdWithAnInteger("/GP/field/setStepperType",this);
  StepperCmd->SetGuidance("Select stepper type for global  magnetic field");
  StepperCmd->SetParameterName("StepperType",true);
  StepperCmd->SetDefaultValue(4);
  StepperCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  MinStepCmd = new G4UIcmdWithADoubleAndUnit("/GP/field/setMinStep",this);  
  MinStepCmd->SetGuidance("Define minimal step");
  MinStepCmd->SetGuidance("Magnetic field will be in Z direction.");
  MinStepCmd->SetParameterName("MinStep",false,false);
  MinStepCmd->SetDefaultUnit("mm");
  MinStepCmd->AvailableForStates(G4State_Idle);  
       
  FieldFlag = new G4UIcmdWithABool("/GP/field/setFieldFlag",this);
  FieldFlag->SetGuidance("Switch global field.");
  FieldFlag->SetGuidance("This command MUST be applied before \"beamOn\" ");
  FieldFlag->SetParameterName("FieldFlag",true);
  FieldFlag->SetDefaultValue("1");
  FieldFlag->AvailableForStates(G4State_Idle);

  UpdateCmd = new G4UIcmdWithoutParameter("/GP/field/update",this);
  UpdateCmd->SetGuidance("Update field.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);

}

///////////////////////////////////////////////////////////////////////////////

GPFieldMessenger::~GPFieldMessenger()
{
  delete StepperCmd;
  delete MinStepCmd;
  delete GPdetDir;
  delete UpdateCmd;
  delete FieldFlag; 
}

////////////////////////////////////////////////////////////////////////////
//
//

void GPFieldMessenger::SetNewValue( G4UIcommand* command, G4String newValue)
{ 
  if( command == StepperCmd )
  { 
    fEMfieldSetup->SetStepperType(StepperCmd->GetNewIntValue(newValue));
  }  


  if( command == UpdateCmd )
  { 
    fEMfieldSetup->UpdateField(); 
  }


  if( command == MinStepCmd )
  { 
    fEMfieldSetup->SetMinStep(MinStepCmd->GetNewDoubleValue(newValue));
  }

  if( command == FieldFlag )
  { 
    fEMfieldSetup->SetFieldFlag(FieldFlag->GetNewBoolValue(newValue));
  }

}

//
//
/////////////////////////////////////////////////////////////////////////
