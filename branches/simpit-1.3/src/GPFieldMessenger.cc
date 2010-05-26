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

  FieldType = new G4UIcmdWithAnInteger("/GP/field/setFieldType",this);
  FieldType->SetGuidance("Unvarible now.");
  FieldType->SetGuidance("Select global magnetic field type:");
/*
  FieldType->SetGuidance("0 AMD");
  FieldType->SetGuidance("1 Feimi distribution QWT");
  FieldType->SetGuidance("2 Negative sqr QWT");
  FieldType->SetGuidance("3 Abrutp QWT");
*/
  FieldType->SetParameterName("FieldType",true);
  FieldType->SetDefaultValue(0);
  FieldType->AvailableForStates(G4State_PreInit,G4State_Idle);
 
  UpdateCmd = new G4UIcmdWithoutParameter("/GP/field/update",this);
  UpdateCmd->SetGuidance("Update field.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);
      
  MagFieldB0Cmd = new G4UIcmdWithADoubleAndUnit("/GP/field/setFieldB0",this);  
  MagFieldB0Cmd->SetGuidance("Define magnetic field B0.");
  MagFieldB0Cmd->SetGuidance("Magnetic field will be in Z direction.");
  MagFieldB0Cmd->SetParameterName("Bz",false,false);
  MagFieldB0Cmd->SetDefaultUnit("tesla");
  MagFieldB0Cmd->AvailableForStates(G4State_Idle); 
 
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


}

///////////////////////////////////////////////////////////////////////////////

GPFieldMessenger::~GPFieldMessenger()
{
  delete StepperCmd;
  delete FieldType;
  delete MagFieldB0Cmd;
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

  if( command == FieldType )
  { 
    //fieldPoint->SetCaptureType(FieldType->GetNewIntValue(newValue));
    //wait for writing
  }  

  if( command == UpdateCmd )
  { 
    fEMfieldSetup->UpdateField(); 
  }

  if( command == MagFieldB0Cmd )
  { 
    //fieldPoint->SetFieldValueB0(MagFieldB0Cmd->GetNewDoubleValue(newValue));
    //wait for writing
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
