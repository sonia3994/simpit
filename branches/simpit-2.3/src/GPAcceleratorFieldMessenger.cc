// $Id: GPAcceleratorFieldMessenger.cc,v 1.7 2006/06/29 17:19:32 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

#include "GPAcceleratorFieldMessenger.hh"

#include "GPAcceleratorFieldManagerPool.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//////////////////////////////////////////////////////////////////////////////

GPAcceleratorFieldMessenger::GPAcceleratorFieldMessenger(GPAcceleratorFieldManagerPool* pEMfield)
  : fEMfieldManager(pEMfield)
{ 
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPAcceleratorFieldMessenger::GPAcceleratorFieldMessenger(GPAcceleratorFieldManagerPool*)"<<G4endl;
#endif
  GPdetDir = new G4UIdirectory("/GP/field/accelerator/");
  GPdetDir->SetGuidance("Field tracking control.");

  StepperCmd = new G4UIcmdWithAnInteger("/GP/field/accelerator/stepperType",this);
  StepperCmd->SetGuidance("Select stepper type for magnetic field");
  StepperCmd->SetParameterName("StepperType",true);
  StepperCmd->SetDefaultValue(4);
  StepperCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  FieldType = new G4UIcmdWithAnInteger("/GP/field/accelerator/fieldType",this);
  FieldType->SetGuidance("Select capture type for magnetic field:");
  FieldType->SetGuidance("Unvariable now!");
/*
  FieldType->SetGuidance("0 AMD");
  FieldType->SetGuidance("1 Feimi distribution QWT");
  FieldType->SetGuidance("2 Negative sqr QWT");
  FieldType->SetGuidance("3 Abrutp QWT");
*/
  FieldType->SetParameterName("FieldType",true);
  FieldType->SetDefaultValue(0);
  FieldType->AvailableForStates(G4State_PreInit,G4State_Idle);
 
  UpdateCmd = new G4UIcmdWithoutParameter("/GP/field/accelerator/update",this);
  UpdateCmd->SetGuidance("Update field.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);
      
  MagFieldB0Cmd = new G4UIcmdWithADoubleAndUnit("/GP/field/accelerator/fieldB0",this);  
  MagFieldB0Cmd->SetGuidance("Define magnetic field B0.");
  MagFieldB0Cmd->SetGuidance("Magnetic field will be in Z direction.");
  MagFieldB0Cmd->SetParameterName("Bz",false,false);
  MagFieldB0Cmd->SetDefaultUnit("tesla");
  MagFieldB0Cmd->AvailableForStates(G4State_Idle); 
 
  EleFieldE0Cmd = new G4UIcmdWithADoubleAndUnit("/GP/field/accelerator/fieldE0",this);  
  EleFieldE0Cmd->SetGuidance("Define electric field E0.");
  EleFieldE0Cmd->SetGuidance("electric field will be in Z direction.");
  EleFieldE0Cmd->SetParameterName("Ez",false,false);
  EleFieldE0Cmd->SetDefaultUnit("volt/m");
  EleFieldE0Cmd->AvailableForStates(G4State_Idle); 
 
  MinStepCmd = new G4UIcmdWithADoubleAndUnit("/GP/field/accelerator/minStep",this);  
  MinStepCmd->SetGuidance("Define minimal step");
  MinStepCmd->SetParameterName("MinStep",false,false);
  MinStepCmd->SetDefaultUnit("m");
  MinStepCmd->AvailableForStates(G4State_Idle);  
       
  FieldFlag = new G4UIcmdWithABool("/GP/field/accelerator/fieldFlag",this);
  FieldFlag->SetGuidance("Switch accelerator field.");
  FieldFlag->SetGuidance("This command MUST be applied before \"beamOn\" ");
  FieldFlag->SetParameterName("FieldFlag",true);
  FieldFlag->SetDefaultValue("1");
  FieldFlag->AvailableForStates(G4State_Idle);


#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPAcceleratorFieldMessenger::GPAcceleratorFieldMessenger(GPAcceleratorFieldManagerPool*)"<<G4endl;
#endif
}

///////////////////////////////////////////////////////////////////////////////

GPAcceleratorFieldMessenger::~GPAcceleratorFieldMessenger()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPAcceleratorFieldMessenger::~GPAcceleratorFieldMessenger()"<<G4endl;
#endif
  delete StepperCmd;
  delete FieldType;
  delete MagFieldB0Cmd;
  delete EleFieldE0Cmd;
  delete MinStepCmd;
  delete GPdetDir;
  delete UpdateCmd;
  delete FieldFlag; 
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPAcceleratorFieldMessenger::~GPAcceleratorFieldMessenger()"<<G4endl;
#endif
}

////////////////////////////////////////////////////////////////////////////
//
//

void GPAcceleratorFieldMessenger::SetNewValue( G4UIcommand* command, G4String newValue)
{ 
  if( command == StepperCmd )
  { 
    fEMfieldManager->SetStepperType(StepperCmd->GetNewIntValue(newValue));
  }  

  if( command == FieldType )
  { 
    fieldPoint->SetFieldType(FieldType->GetNewIntValue(newValue));
  }  

  if( command == UpdateCmd )
  { 
    fEMfieldManager->UpdateField(); 
  }

  if( command == MagFieldB0Cmd )
  { 
    fieldPoint->SetFieldValueB0((MagFieldB0Cmd->GetNewDoubleValue(newValue))/tesla);
  }

  if( command == EleFieldE0Cmd )
  { 
    fieldPoint->SetFieldValueE0((EleFieldE0Cmd->GetNewDoubleValue(newValue))/(volt/m));
  }


  if( command == MinStepCmd )
  { 
    fEMfieldManager->SetMinStep((MinStepCmd->GetNewDoubleValue(newValue))/m);
  }

  if( command == FieldFlag )
  { 
    //fEMfieldManager->SetCaptureFieldFlag(FieldFlag->GetNewBoolValue(newValue));
    fEMfieldManager->SetFieldFlag(FieldFlag->GetNewBoolValue(newValue));
    //wait for writing
  }

}

//
//
/////////////////////////////////////////////////////////////////////////
