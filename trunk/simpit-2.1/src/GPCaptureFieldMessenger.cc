// $Id: GPCaptureFieldMessenger.cc,v 1.7 2006/06/29 17:19:32 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

#include "GPCaptureFieldMessenger.hh"

#include "GPCaptureFieldManager.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

//////////////////////////////////////////////////////////////////////////////

GPCaptureFieldMessenger::GPCaptureFieldMessenger(GPCaptureFieldManager* pEMfield)
  : fEMfieldManager(pEMfield)
{ 
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPCaptureFieldMessenger::GPCaptureFieldMessenger(GPCaptureFieldManager* )"<<G4endl;
#endif
  GPdetDir = new G4UIdirectory("/GP/field/capture/");
  GPdetDir->SetGuidance("Field tracking control.");

  StepperCmd = new G4UIcmdWithAnInteger("/GP/field/capture/stepperType",this);
  StepperCmd->SetGuidance("Select stepper type for magnetic field");
  StepperCmd->SetParameterName("StepperType",true);
  StepperCmd->SetDefaultValue(4);
  StepperCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  CaptureType = new G4UIcmdWithAnInteger("/GP/field/capture/fieldType",this);
  CaptureType->SetGuidance("Select capture type for magnetic field:");
  CaptureType->SetGuidance("0 AMD");
  CaptureType->SetGuidance("1 Feimi distribution QWT");
  CaptureType->SetGuidance("2 Negative sqr QWT");
  CaptureType->SetGuidance("3 Abrutp QWT");
  CaptureType->SetParameterName("CaptureType",true);
  CaptureType->SetDefaultValue(0);
  CaptureType->AvailableForStates(G4State_PreInit,G4State_Idle);
 
  MagFieldB0Cmd = new G4UIcmdWithADoubleAndUnit("/GP/field/capture/fieldB0",this);  
  MagFieldB0Cmd->SetGuidance("Define magnetic field B0.");
  MagFieldB0Cmd->SetGuidance("Magnetic field will be in Z direction.");
  MagFieldB0Cmd->SetParameterName("Bz",false,false);
  MagFieldB0Cmd->SetDefaultUnit("tesla");
  MagFieldB0Cmd->AvailableForStates(G4State_Idle); 
 
  MinStepCmd = new G4UIcmdWithADoubleAndUnit("/GP/field/capture/minStep",this);  
  MinStepCmd->SetGuidance("Define minimal step");
  MinStepCmd->SetGuidance("Magnetic field will be in Z direction.");
  MinStepCmd->SetParameterName("MinStep",false,false);
  MinStepCmd->SetDefaultUnit("mm");
  MinStepCmd->AvailableForStates(G4State_Idle);  
       
  QwtExtendCmd = new G4UIcmdWithADoubleAndUnit("/GP/field/capture/qwtExtend",this);  
  QwtExtendCmd->SetGuidance("Define minimal step");
  QwtExtendCmd->SetGuidance("Magnetic field will be in Z direction.");
  QwtExtendCmd->SetParameterName("MinStep",false,false);
  QwtExtendCmd->SetDefaultUnit("mm");
  QwtExtendCmd->AvailableForStates(G4State_Idle);  

  LithiumFocalLengthCmd = new G4UIcmdWithADoubleAndUnit("/GP/field/capture/lithiumFocalLength",this);  
  LithiumFocalLengthCmd->SetGuidance("Define lithium focal length step");
  LithiumFocalLengthCmd->SetParameterName("LithiumFocalLengthCmd",false,false);
  LithiumFocalLengthCmd->SetDefaultUnit("cm");
  LithiumFocalLengthCmd->AvailableForStates(G4State_Idle);  
       
  new G4UnitDefinition("tesla*m","tesla*m","MagneticRigidity",tesla*m);
  new G4UnitDefinition("gauss*cm","gauss*cm","MagneticRigidity",gauss*cm);
  MagneticRigidityCmd = new G4UIcmdWithADoubleAndUnit("/GP/field/capture/magneticRigidity",this);  
  MagneticRigidityCmd->SetGuidance("Set Magnetic Rigidity.");
  MagneticRigidityCmd->SetParameterName("MagneticRigidityCmd",false,false);
  MagneticRigidityCmd->SetDefaultValue(3.3e-2);
  MagneticRigidityCmd->SetDefaultUnit("tesla*m");
  MagneticRigidityCmd->AvailableForStates(G4State_Idle); 

  LithiumCurrentCmd = new G4UIcmdWithADoubleAndUnit("/GP/field/capture/lithiumCurrent",this);  
  LithiumCurrentCmd->SetGuidance("Set lithium current.");
  LithiumCurrentCmd->SetParameterName("LithiumCurrentCmd",false,false);
  LithiumCurrentCmd->SetDefaultValue(165000);
  LithiumCurrentCmd->SetDefaultUnit("ampere");
  LithiumCurrentCmd->AvailableForStates(G4State_Idle); 

  AMDAlphaCmd = new G4UIcmdWithADouble("/GP/field/capture/AMDAlpha",this);  
  AMDAlphaCmd->SetGuidance("Define AMD  magnetic field alpha, please transfer to the m unit and don't input unit");
  AMDAlphaCmd->SetParameterName("AMDB0",false,false);
  AMDAlphaCmd->SetDefaultValue(0.22);
  AMDAlphaCmd->AvailableForStates(G4State_Idle); 
 
  QWTFermiApproxAlphaCmd = new G4UIcmdWithADouble("/GP/field/capture/QWTFermiApproxAlpha",this);  
  QWTFermiApproxAlphaCmd->SetGuidance("Set QWT Fermi Approximate alpha");
  QWTFermiApproxAlphaCmd->SetParameterName("QWTFermiApproxAlphaCmd",false,false);
  QWTFermiApproxAlphaCmd->SetDefaultValue(300);
  QWTFermiApproxAlphaCmd->AvailableForStates(G4State_Idle); 

  FieldFlag = new G4UIcmdWithABool("/GP/field/capture/fieldFlag",this);
  FieldFlag->SetGuidance("Switch capture field.");
  FieldFlag->SetGuidance("This command MUST be applied before \"beamOn\" ");
  FieldFlag->SetParameterName("FieldFlag",true);
  FieldFlag->SetDefaultValue("1");
  FieldFlag->AvailableForStates(G4State_Idle);

  UpdateCmd = new G4UIcmdWithoutParameter("/GP/field/capture/update",this);
  UpdateCmd->SetGuidance("Update field.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);
      

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPCaptureFieldMessenger::GPCaptureFieldMessenger(GPCaptureFieldManager* )"<<G4endl;
#endif
}

///////////////////////////////////////////////////////////////////////////////

GPCaptureFieldMessenger::~GPCaptureFieldMessenger()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPCaptureFieldMessenger::~GPCaptureFieldMessenger()"<<G4endl;
#endif
  delete StepperCmd;
  delete CaptureType;
  delete MagFieldB0Cmd;
  delete AMDAlphaCmd;
  delete MinStepCmd;
  delete QwtExtendCmd;
  delete LithiumFocalLengthCmd;
  delete GPdetDir;
  delete UpdateCmd;
  delete FieldFlag; 
  delete QWTFermiApproxAlphaCmd;
  delete MagneticRigidityCmd;
  delete LithiumCurrentCmd;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPCaptureFieldMessenger::~GPCaptureFieldMessenger()"<<G4endl;
#endif
}

////////////////////////////////////////////////////////////////////////////
//
//

void GPCaptureFieldMessenger::SetNewValue( G4UIcommand* command, G4String newValue)
{ 
  if( command == StepperCmd )
  { 
    fEMfieldManager->SetStepperType(StepperCmd->GetNewIntValue(newValue));
  }  

  if( command == CaptureType )
  { 
    fieldPoint->SetCaptureType(CaptureType->GetNewIntValue(newValue));
  }  

  if( command == UpdateCmd )
  { 
    fEMfieldManager->UpdateField(); 
  }

  if( command == MagFieldB0Cmd )
  { 
    fieldPoint->SetFieldValueB0((MagFieldB0Cmd->GetNewDoubleValue(newValue))/tesla);
  }

  if( command == AMDAlphaCmd )
  { 
    fieldPoint->SetAMDFieldAlpha(AMDAlphaCmd->GetNewDoubleValue(newValue));
  }

  if( command == QWTFermiApproxAlphaCmd )
  { 
    fieldPoint->SetFermiApproximateAlpha(QWTFermiApproxAlphaCmd->GetNewDoubleValue(newValue));
  }

  if( command == MagneticRigidityCmd )
  { 
    fieldPoint->SetMagneticRigidity((MagneticRigidityCmd->GetNewDoubleValue(newValue))/(tesla*m));
  }

  if( command == LithiumCurrentCmd )
  { 
    fieldPoint->SetLithiumCurrent((LithiumCurrentCmd->GetNewDoubleValue(newValue))/ampere);
  }

  if( command == MinStepCmd )
  { 
    fEMfieldManager->SetMinStep((MinStepCmd->GetNewDoubleValue(newValue))/m);
  }

  if( command == QwtExtendCmd )
  { 
    fieldPoint->SetQwtExtend((QwtExtendCmd->GetNewDoubleValue(newValue))/m);
  }

  if( command == LithiumFocalLengthCmd )
  { 
    fieldPoint->SetLithiumFocalLength((LithiumFocalLengthCmd->GetNewDoubleValue(newValue))/m);
  }


  if( command == FieldFlag )
  { 
    fEMfieldManager->SetFieldFlag(FieldFlag->GetNewBoolValue(newValue));
  }

}

//
//
/////////////////////////////////////////////////////////////////////////
