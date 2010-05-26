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
  StepperCmd->SetGuidance("Select stepper type for magnetic field");
  StepperCmd->SetParameterName("StepperType",true);
  StepperCmd->SetDefaultValue(4);
  StepperCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  CaptureType = new G4UIcmdWithAnInteger("/GP/field/setCaptureType",this);
  CaptureType->SetGuidance("Select capture type for magnetic field:");
  CaptureType->SetGuidance("0 AMD");
  CaptureType->SetGuidance("1 Feimi distribution QWT");
  CaptureType->SetGuidance("2 Negative sqr QWT");
  CaptureType->SetGuidance("3 Abrutp QWT");
  CaptureType->SetParameterName("CaptureType",true);
  CaptureType->SetDefaultValue(0);
  CaptureType->AvailableForStates(G4State_PreInit,G4State_Idle);
 
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
       
  AMDAlphaCmd = new G4UIcmdWithADouble("/GP/field/setAMDAlpha",this);  
  AMDAlphaCmd->SetGuidance("Define AMD  magnetic field alpha, please transfer to the cm unit and don't input unit");
  AMDAlphaCmd->SetParameterName("AMDB0",false,false);
  AMDAlphaCmd->SetDefaultValue(0.22);
  AMDAlphaCmd->AvailableForStates(G4State_Idle); 
 
  CaptureFieldFlag = new G4UIcmdWithABool("/GP/field/setCaptureFieldFlag",this);
  CaptureFieldFlag->SetGuidance("Switch capture field.");
  CaptureFieldFlag->SetGuidance("This command MUST be applied before \"beamOn\" ");
  CaptureFieldFlag->SetParameterName("CaptureFieldFlag",true);
  CaptureFieldFlag->SetDefaultValue("1");
  CaptureFieldFlag->AvailableForStates(G4State_Idle);


}

///////////////////////////////////////////////////////////////////////////////

GPFieldMessenger::~GPFieldMessenger()
{
  delete StepperCmd;
  delete CaptureType;
  delete MagFieldB0Cmd;
  delete AMDAlphaCmd;
  delete MinStepCmd;
  delete GPdetDir;
  delete UpdateCmd;
  delete CaptureFieldFlag; 
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

  if( command == CaptureType )
  { 
    //fieldPoint->SetCaptureType(CaptureType->GetNewIntValue(newValue));
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

  if( command == AMDAlphaCmd )
  { 
    //fieldPoint->SetAMDFieldAlpha(AMDAlphaCmd->GetNewDoubleValue(newValue));
    //wait for writing
  }

  if( command == MinStepCmd )
  { 
    fEMfieldSetup->SetMinStep(MinStepCmd->GetNewDoubleValue(newValue));
  }

  if( command == CaptureFieldFlag )
  { 
    //fEMfieldSetup->SetCaptureFieldFlag(CaptureFieldFlag->GetNewBoolValue(newValue));
    fEMfieldSetup->SetGlobalFieldFlag(CaptureFieldFlag->GetNewBoolValue(newValue));
    //wait for writing
  }

}

//
//
/////////////////////////////////////////////////////////////////////////
