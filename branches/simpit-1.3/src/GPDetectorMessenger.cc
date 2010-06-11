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
// $Id: GPDetectorMessenger.cc,v 1.10 2006/06/29 17:48:58 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "GPDetectorMessenger.hh"

#include "GPDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

#include <sstream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPDetectorMessenger::GPDetectorMessenger(
                                           GPDetectorConstruction* GPDet)
:GPDetector(GPDet)
{ 
  G4bool omitable;
  G4UIparameter* parameter;
  GPDir = new G4UIdirectory("/GP/");
  GPDir->SetGuidance("UI commands of this example");
  
  detDir = new G4UIdirectory("/GP/detector/");
  detDir->SetGuidance("detector control");
       
  TarMaterCmd = new G4UIcmdWithAString("/GP/detector/targerMaterial",this);
  TarMaterCmd->SetGuidance("Set Material of the Target.");
  TarMaterCmd->SetParameterName("choice",false);
  TarMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  DetectorSizeCmd = new G4UIcmdWithAString("/GP/detector/setDetectorSize",this);
  DetectorSizeCmd->SetGuidance("Set detector size.");
  DetectorSizeCmd->SetGuidance("Format: {name} {value} {unit}");
  DetectorSizeCmd->SetGuidance("For example: setDetectorSize target_l 8 mm");
  DetectorSizeCmd->SetParameterName("choice",false);
  DetectorSizeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  TarThickCmd = new G4UIcmdWithADoubleAndUnit("/GP/detector/targetThickness",this);
  TarThickCmd->SetGuidance("Set Thickness of the Target");
  TarThickCmd->SetParameterName("Size",false);
  TarThickCmd->SetRange("Size>=0.");
  TarThickCmd->SetUnitCategory("Length");
  TarThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  TarBoxXYCmd = new G4UIcmdWithADoubleAndUnit("/GP/detector/targetBoxXY",this);
  TarBoxXYCmd->SetGuidance("Set X and Y length for the Target box");
  TarBoxXYCmd->SetParameterName("Size",false);
  TarBoxXYCmd->SetRange("Size>=0.");
  TarBoxXYCmd->SetUnitCategory("Length");
  TarBoxXYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  CapLengthCmd = new G4UIcmdWithADoubleAndUnit("/GP/detector/captureLength",this);
  CapLengthCmd->SetGuidance("Set length of the capture");
  CapLengthCmd->SetParameterName("Size",false);
  CapLengthCmd->SetRange("Size>=0.");
  CapLengthCmd->SetUnitCategory("Length");
  CapLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  CapRadiusCmd = new G4UIcmdWithADoubleAndUnit("/GP/detector/captureRadius",this);
  CapRadiusCmd->SetGuidance("Set radius of the capture");
  CapRadiusCmd->SetParameterName("Size",false);
  CapRadiusCmd->SetRange("Size>=0.");
  CapRadiusCmd->SetUnitCategory("Length");
  CapRadiusCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SizeWorldXYZCmd = new G4UIcommand("/GP/detector/worldXYZ", this);
  SizeWorldXYZCmd -> SetGuidance ("Set up world volume size.");
  parameter = new G4UIparameter("XSize", 'd', omitable = true);
  parameter -> SetDefaultValue (50.);
  parameter -> SetParameterRange("XSize>=0.");
  SizeWorldXYZCmd -> SetParameter (parameter);
  parameter = new G4UIparameter("YSize", 'd', omitable = true);
  parameter -> SetDefaultValue (50.);
  parameter -> SetParameterRange("YSize>=0.");
  SizeWorldXYZCmd -> SetParameter (parameter);
  parameter = new G4UIparameter("ZSize", 'd', omitable = true);
  parameter -> SetDefaultValue (3200.);
  parameter -> SetParameterRange("ZSize>=0.");
  SizeWorldXYZCmd -> SetParameter (parameter);
  parameter = new G4UIparameter ("unit", 's', omitable = true);
  parameter -> SetDefaultValue ("mm");
  SizeWorldXYZCmd -> SetParameter (parameter);
  SizeWorldXYZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  TargetCellXYZCmd = new G4UIcommand("/GP/detector/targetCellXYZ", this);
  TargetCellXYZCmd -> SetGuidance ("Set up target cell volume size.");
  parameter = new G4UIparameter("XSize", 'd', omitable = true);
  parameter -> SetDefaultValue (0.5);
  parameter -> SetParameterRange("XSize>=0.");
  TargetCellXYZCmd -> SetParameter (parameter);
  parameter = new G4UIparameter("YSize", 'd', omitable = true);
  parameter -> SetDefaultValue (0.5);
  parameter -> SetParameterRange("YSize>=0.");
  TargetCellXYZCmd -> SetParameter (parameter);
  parameter = new G4UIparameter("ZSize", 'd', omitable = true);
  parameter -> SetDefaultValue (0.5);
  parameter -> SetParameterRange("ZSize>=0.");
  TargetCellXYZCmd -> SetParameter (parameter);
  parameter = new G4UIparameter ("unit", 's', omitable = true);
  parameter -> SetDefaultValue ("mm");
  TargetCellXYZCmd -> SetParameter (parameter);
  TargetCellXYZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SizeWorldXCmd = new G4UIcmdWithADoubleAndUnit("/GP/detector/worldX",this);
  SizeWorldXCmd->SetGuidance("Set X size of the World");
  SizeWorldXCmd->SetParameterName("Size",false);
  SizeWorldXCmd->SetRange("Size>0.");
  SizeWorldXCmd->SetUnitCategory("Length");    
  SizeWorldXCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  SizeWorldYCmd = new G4UIcmdWithADoubleAndUnit("/GP/detector/worldY",this);
  SizeWorldYCmd->SetGuidance("Set Y size of the World");
  SizeWorldYCmd->SetParameterName("Size",false);
  SizeWorldYCmd->SetRange("Size>0.");
  SizeWorldYCmd->SetUnitCategory("Length");    
  SizeWorldYCmd->AvailableForStates(G4State_PreInit,G4State_Idle);  
  
  SizeWorldZCmd = new G4UIcmdWithADoubleAndUnit("/GP/detector/worldZ",this);
  SizeWorldZCmd->SetGuidance("Set Z size of the World");
  SizeWorldZCmd->SetParameterName("Size",false);
  SizeWorldZCmd->SetRange("Size>0.");
  SizeWorldZCmd->SetUnitCategory("Length");    
  SizeWorldZCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
/*  
  NbLayersCmd = new G4UIcmdWithAnInteger("/GP/detector/setNbOfLayers",this);
  NbLayersCmd->SetGuidance("Set number of layers.");
  NbLayersCmd->SetParameterName("NbLayers",false);
  NbLayersCmd->SetRange("NbLayers>0 && NbLayers<500");
  NbLayersCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
*/
  UpdateCmd = new G4UIcmdWithoutParameter("/GP/detector/update",this);
  UpdateCmd->SetGuidance("Update Positron Resource geometry.");
  UpdateCmd->SetGuidance("This command MUST be applied before \"/run/beamOn\" ");
  UpdateCmd->SetGuidance("if you changed geometrical value(s).");
  UpdateCmd->AvailableForStates(G4State_Idle);

  PrintParaCmd = new G4UIcmdWithoutParameter("/GP/detector/printParameters",this);
  PrintParaCmd->SetGuidance("Print geometry parameters.");
  PrintParaCmd->AvailableForStates(G4State_Idle);      

/*  MagFieldCmd = new G4UIcmdWithADoubleAndUnit("/GP/detector/setField",this);  
  MagFieldCmd->SetGuidance("Define magnetic field.");
  MagFieldCmd->SetGuidance("Magnetic field will be in Z direction.");
  MagFieldCmd->SetParameterName("Bz",false);
  MagFieldCmd->SetUnitCategory("Magnetic flux density");
  MagFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
*/  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GPDetectorMessenger::~GPDetectorMessenger()
{
//  delete NbLayersCmd;
  delete TarMaterCmd; 
  delete DetectorSizeCmd; 
  delete TarThickCmd;
  delete TarBoxXYCmd; 
  delete CapLengthCmd; 
  delete CapRadiusCmd;
  delete SizeWorldXCmd;   
  delete SizeWorldYCmd;
  delete SizeWorldZCmd;
  delete SizeWorldXYZCmd;   
  delete TargetCellXYZCmd;   
  delete UpdateCmd;
//  delete MagFieldCmd;
  delete PrintParaCmd;
  delete detDir;
  delete GPDir;  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GPDetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if( command == TarMaterCmd )
   { GPDetector->SetTargetMaterial(newValue);}
   
  if( command == DetectorSizeCmd )
   { GPDetector->SetDetectorSize(newValue);}
   
  if( command == TarThickCmd )
   { GPDetector->SetTargetThickness(TarThickCmd->GetNewDoubleValue(newValue));}

  if( command == TarBoxXYCmd )
   { GPDetector->SetTargetXY(TarBoxXYCmd->GetNewDoubleValue(newValue));}

  if( command == CapLengthCmd )
   { GPDetector->SetCaptureLength(CapLengthCmd->GetNewDoubleValue(newValue));}

  if( command == CapRadiusCmd )
   { GPDetector->SetCaptureRadius(CapRadiusCmd->GetNewDoubleValue(newValue));}
   
  if( command == SizeWorldXCmd )
   { GPDetector->SetWorldSizeX(SizeWorldXCmd->GetNewDoubleValue(newValue));}

  if( command == SizeWorldYCmd )
   { GPDetector->SetWorldSizeY(SizeWorldYCmd->GetNewDoubleValue(newValue));}

  if( command == SizeWorldZCmd )
   { GPDetector->SetWorldSizeZ(SizeWorldZCmd->GetNewDoubleValue(newValue));}

 if( command == SizeWorldXYZCmd )
   {
	std::stringstream is(newValue);
	G4double x,y,z;
	G4String unit;
	 is>>x>>y>>z>>unit;
	x=x*G4UIcommand::ValueOf(unit);
	y=y*G4UIcommand::ValueOf(unit);
	z=z*G4UIcommand::ValueOf(unit);
	GPDetector->SetWorldSizeXYZ(x,y,z);
   }

 if( command == TargetCellXYZCmd )
   {
	std::stringstream is(newValue);
	G4double x,y,z;
	G4String unit;
	 is>>x>>y>>z>>unit;
	x=x*G4UIcommand::ValueOf(unit);
	y=y*G4UIcommand::ValueOf(unit);
	z=z*G4UIcommand::ValueOf(unit);
	GPDetector->SetTargetCellXYZ(x,y,z);
   }


  if( command == PrintParaCmd )
   { GPDetector->PrintDetectorParameters();}


  if( command == UpdateCmd )
   { GPDetector->UpdateGeometry();  }
/*  if( command == MagFieldCmd )
   { GPDetector->SetMagField(MagFieldCmd->GetNewDoubleValue(newValue));}
*/
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
