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
// $Id: GPAcceleratorFieldMessenger.cc,v 1.7 2006/06/29 17:19:32 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

#include "GPAcceleratorFieldMessenger.hh"

#include "GPAcceleratorFieldManager.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//////////////////////////////////////////////////////////////////////////////

GPAcceleratorFieldMessenger::GPAcceleratorFieldMessenger(GPAcceleratorFieldManager* pEMfield)
  : fEMfieldManager(pEMfield)
{ 
  GPdetDir = new G4UIdirectory("/GP/field/accelerator/");
  GPdetDir->SetGuidance("Field tracking control.");

  StepperCmd = new G4UIcmdWithAnInteger("/GP/field/accelerator/setStepperType",this);
  StepperCmd->SetGuidance("Select stepper type for magnetic field");
  StepperCmd->SetParameterName("StepperType",true);
  StepperCmd->SetDefaultValue(4);
  StepperCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  FieldType = new G4UIcmdWithAnInteger("/GP/field/accelerator/setFieldType",this);
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
      
  MagFieldB0Cmd = new G4UIcmdWithADoubleAndUnit("/GP/field/accelerator/setFieldB0",this);  
  MagFieldB0Cmd->SetGuidance("Define magnetic field B0.");
  MagFieldB0Cmd->SetGuidance("Magnetic field will be in Z direction.");
  MagFieldB0Cmd->SetParameterName("Bz",false,false);
  MagFieldB0Cmd->SetDefaultUnit("tesla");
  MagFieldB0Cmd->AvailableForStates(G4State_Idle); 
 
  EleFieldE0Cmd = new G4UIcmdWithADoubleAndUnit("/GP/field/accelerator/setFieldE0",this);  
  EleFieldE0Cmd->SetGuidance("Define electric field E0.");
  EleFieldE0Cmd->SetGuidance("electric field will be in Z direction.");
  EleFieldE0Cmd->SetParameterName("Ez",false,false);
  EleFieldE0Cmd->SetDefaultUnit("volt/m");
  EleFieldE0Cmd->AvailableForStates(G4State_Idle); 
 
  MinStepCmd = new G4UIcmdWithADoubleAndUnit("/GP/field/accelerator/setMinStep",this);  
  MinStepCmd->SetGuidance("Define minimal step");
  MinStepCmd->SetParameterName("MinStep",false,false);
  MinStepCmd->SetDefaultUnit("mm");
  MinStepCmd->AvailableForStates(G4State_Idle);  
       
  FieldFlag = new G4UIcmdWithABool("/GP/field/accelerator/setFieldFlag",this);
  FieldFlag->SetGuidance("Switch accelerator field.");
  FieldFlag->SetGuidance("This command MUST be applied before \"beamOn\" ");
  FieldFlag->SetParameterName("FieldFlag",true);
  FieldFlag->SetDefaultValue("1");
  FieldFlag->AvailableForStates(G4State_Idle);


}

///////////////////////////////////////////////////////////////////////////////

GPAcceleratorFieldMessenger::~GPAcceleratorFieldMessenger()
{
  delete StepperCmd;
  delete FieldType;
  delete MagFieldB0Cmd;
  delete EleFieldE0Cmd;
  delete MinStepCmd;
  delete GPdetDir;
  delete UpdateCmd;
  delete FieldFlag; 
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
    fieldPoint->SetFieldValueB0(MagFieldB0Cmd->GetNewDoubleValue(newValue));
  }

  if( command == EleFieldE0Cmd )
  { 
    fieldPoint->SetFieldValueE0(EleFieldE0Cmd->GetNewDoubleValue(newValue));
  }


  if( command == MinStepCmd )
  { 
    fEMfieldManager->SetMinStep(MinStepCmd->GetNewDoubleValue(newValue));
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
