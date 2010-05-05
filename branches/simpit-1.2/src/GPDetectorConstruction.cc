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
// $Id: GPDetectorConstruction.cc,v 1.9 2006/06/29 17:47:19 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#include "GPDetectorConstruction.hh"
#include "GPTargetROGeometry.hh"
#include "GPTargetSD.hh"
#include "GPDetectorMessenger.hh"
#include "GPFieldSetup.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4NistManager.hh"

#include "G4ThreeVector.hh"
#include "G4FieldManager.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include <cmath>
GPDetectorConstruction::GPDetectorConstruction()
 :  targetBox(0),targetLog(0),targetPhys(0),
    captureTube(0),captureLog(0),capturePhys(0),
    tranTube(0),tranLog(0),tranPhys(0),
    worldBox(0),worldLog(0),worldPhys(0),
    captureFieldSetup(0)
{
    targetBoxY = 25*mm;
    targetBoxX = 25*mm;
    targetBoxZ = 6.0*mm;
    TargetCellX=0.5*mm;
    //TargetCellX=0.185*mm;
    TargetCellY=0.5*mm;
    //TargetCellY=0.185*mm;
    TargetCellZ=1*mm;
    //TargetCellZ=0.875*mm;
    eddDim.push_back(ceil(targetBoxX/TargetCellX));
    eddDim.push_back(ceil(targetBoxY/TargetCellY));
    eddDim.push_back(ceil(targetBoxZ/TargetCellZ));

    captureTubeInnerRadius = 0.*mm;
    captureTubeOuterRadius = 50*mm;
    captureTubeStartAngle = 0.*deg;
    captureTubeSpanningAngle = 360.*deg;
    captureTubeLength = 500.0*mm;

    tranTubeInnerRadius = 0.*mm;
    tranTubeOuterRadius = 50*mm;
    tranTubeStartAngle = 0.*deg;
    tranTubeSpanningAngle = 360.*deg;
    tranTubeLength =4500.0*mm;

    worldX = 500.0*mm;
    worldY = 500.0*mm;
    worldZ = 10000.0*mm;
        
    DefineMaterials();
    //SetTargetMaterial ("Galactic");
    SetTargetMaterial ("G4_W");
    //SetWorldMaterial ("Galactic");
    SetWorldMaterial ("G4_Galactic");
    //SetCaptureMaterial ("Galactic");
    SetCaptureMaterial ("G4_Galactic");
    //SetTranTubeMaterial ("Galactic");
    SetTranTubeMaterial ("G4_Galactic");
    captureFieldSetup = new GPFieldSetup();
    detectorMessenger = new GPDetectorMessenger(this);

}

GPDetectorConstruction::~GPDetectorConstruction()
{
    if(detectorMessenger) delete detectorMessenger;
    if(captureFieldSetup) delete captureFieldSetup;
    if(targetRO) delete targetRO;
    if(Vacuum) delete Vacuum;
    if(W) delete W;
}

G4VPhysicalVolume* GPDetectorConstruction::Construct()
{ 
    return ConstructPositronResource();
}

G4VPhysicalVolume* GPDetectorConstruction::ConstructPositronResource()
{
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  worldBox = new G4Box("worldBox",worldX/2,worldY/2,worldZ/2);
  worldLog = new G4LogicalVolume(worldBox,worldMaterial,"worldLog");
  worldPhys = new G4PVPlacement(0,G4ThreeVector(),worldLog,"world",0,false,0);

  //------------------------------ target box

  targetBox = new G4Box("targetBox",targetBoxY/2,targetBoxX/2,targetBoxZ/2);
  targetLog = new G4LogicalVolume(targetBox,targetMaterial,"targetLog");
  G4double targetPos_x = 0.*mm;
  G4double targetPos_y = 0.*mm;
  G4double targetPos_z = 0.*mm;
  targetPhys = new G4PVPlacement(0,
             G4ThreeVector(targetPos_x,targetPos_y,targetPos_z),
             targetLog,"targetPhys",worldLog,false,0);
  
  //------------------------------ capture tube

  captureTube = new G4Tubs("captureTube",captureTubeInnerRadius,
                                    captureTubeOuterRadius,captureTubeLength/2.0,
                                    captureTubeStartAngle,captureTubeSpanningAngle);
  captureLog = new G4LogicalVolume(captureTube,captureMaterial,"captureLog");
  G4double capturePos_x = 0.*mm;
  G4double capturePos_y = 0.*mm;
  G4double capturePos_z = (targetBoxZ+captureTubeLength)/2;
  capturePhys = new G4PVPlacement(0,
             G4ThreeVector(capturePos_x,capturePos_y,capturePos_z),
             captureLog,"capture",worldLog,false,0);

  captureLog->SetFieldManager(captureFieldSetup->GetLocalFieldManager(),true);
//--------------------------------transportion tube
  tranTube = new G4Tubs("tranTube",tranTubeInnerRadius,
                                    tranTubeOuterRadius,tranTubeLength/2.0,
                                    tranTubeStartAngle,tranTubeSpanningAngle);
  tranLog = new G4LogicalVolume(tranTube,tranMaterial,"tranLog");
  G4double tranPos_x = 0.*mm;
  G4double tranPos_y = 0.*mm;
  G4double tranPos_z = -(targetBoxZ+tranTubeLength)/2;
  tranPhys = new G4PVPlacement(0,
             G4ThreeVector(tranPos_x,tranPos_y,tranPos_z),
             tranLog,"tran",worldLog,false,0);


//----------------------------------------------------------------------------
//Sensitive Detector
//----------------------------------------------------------------------------
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String targetSDName="/mydet/target";
  G4String targetROName="targetROGeometry";

  if(targetSD)
  {
  targetSD->SetEddDim(eddDim);  
  }
  else
  {
  targetSD=new GPTargetSD(targetSDName,eddDim);
  }

  if(targetRO)
  {
  delete targetRO;
  }
  targetRO=new GPTargetROGeometry(targetROName,targetBoxX,targetBoxY,targetBoxZ,eddDim);
  targetRO->BuildROGeometry();
  targetRO->SetName(targetROName);
  targetSD->SetROgeometry(targetRO);  
  SDman->AddNewDetector(targetSD);
  targetLog->SetSensitiveDetector(targetSD); 

  PrintDetectorParameters();
  // Visualization attributes
  //
/*  worldLog->SetVisAttributes (G4VisAttributes::Invisible);

  G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  targetLog->SetVisAttributes(simpleBoxVisAtt);
*/

 /*
  // Below are vis attributes that permits someone to test / play 
  // with the interactive expansion / contraction geometry system of the
  // vis/OpenInventor driver :
 {G4VisAttributes* simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0.0));
  simpleBoxVisAtt->SetVisibility(true);
  delete logicCalor->GetVisAttributes();
  logicCalor->SetVisAttributes(simpleBoxVisAtt);}

 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0));
  logicLayer->SetVisAttributes(atb);}
  
 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,1.0,0.0));
  atb->SetForceSolid(true);
  logicAbsorber->SetVisAttributes(atb);}
  
 {//Set opacity = 0.2 then transparency = 1 - 0.2 = 0.8
  G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.2));
  atb->SetForceSolid(true);
  logicGap->SetVisAttributes(atb);}
  */

  //
  //always return the physical World
  //

  return worldPhys;
}

void GPDetectorConstruction::DefineMaterials()
{
  //------------------------------------------------------ materials

  G4double a;  // atomic mass
  G4double z;  // atomic number
  G4double density;
  G4double pressure;
  G4double temperature;

//  G4Material* Ar = 
//  new G4Material("ArgonGas", z= 18., a= 39.95*g/mole, density= 1.782*mg/cm3);

//  G4Material* Al = 
//  new G4Material("Aluminum", z= 13., a= 26.98*g/mole, density= 2.7*g/cm3);

//  G4Material* Pb = 
//  new G4Material("Lead", z= 82., a= 207.19*g/mole, density= 11.35*g/cm3);
//  a = 183.84*g/mole;
//  G4Element* elW = new G4Element(name="Tungsten" ,symbol="W", z=74., a);
  density = universe_mean_density; //from PhysicalConstants.h
  pressure = 1.e-19*pascal;
  temperature = 0.1*kelvin;
  Vacuum = new G4Material("Galactic", z=1., a=1.01*g/mole, density,kStateGas,temperature,pressure);
  W = new G4Material("Tungsten",z=74.,a=183.84*g/mole,density=19.3*g/cm3);
 
}


void GPDetectorConstruction::SetTargetThickness(G4double val)
{
  targetBoxZ = val;
  eddDim[2] = ceil(val/TargetCellZ);
  G4cout<<"The target thickness is set to "<<val<<" mm!"<<G4endl;
}

void GPDetectorConstruction::SetTargetXY(G4double val)
{
  targetBoxX = val;
  targetBoxY = val; 
  eddDim[0] = ceil(val/TargetCellX);
  eddDim[1] = ceil(val/TargetCellY);
  G4cout<<"The target box X and Y length is set to "<<val<<" mm!"<<G4endl;
}

void GPDetectorConstruction::SetWorldSizeXYZ(G4double valx,G4double valy, G4double valz)
{
  worldX = valx;
  worldY = valy;
  worldZ = valz;
  G4cout<<"The world volume is set to "<<valx<<"*"<<valy<<"*"<<valz<<" mm^3!"<<G4endl;
}

void GPDetectorConstruction::SetWorldSizeX(G4double val)
{
  worldX = val;
  G4cout<<"The world x thickness is set to "<<val<<" mm!"<<G4endl;
}

void GPDetectorConstruction::SetWorldSizeY(G4double val)
{
  worldY = val;
  G4cout<<"The world y is set to "<<val<<" mm!"<<G4endl;
}

void GPDetectorConstruction::SetWorldSizeZ(G4double val)
{
  worldZ = val;
  G4cout<<"The world z is set to "<<val<<" mm!"<<G4endl;
}

void GPDetectorConstruction::SetTargetMaterial(G4String materialChoice)
{
  // search the material by its name   
  //G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);     
  G4Material* pttoMaterial= G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  if (pttoMaterial) targetMaterial = pttoMaterial;
  G4cout<<"The target material is set to "<<materialChoice<<G4endl;
}

void GPDetectorConstruction::SetWorldMaterial(G4String materialChoice)
{
  // search the material by its name
  //G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
  G4Material* pttoMaterial= G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  if (pttoMaterial) worldMaterial = pttoMaterial;
  G4cout<<"The world material is set to "<<materialChoice<<G4endl;
}

void GPDetectorConstruction::SetCaptureMaterial(G4String materialChoice)
{
  // search the material by its name
  //G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
  G4Material* pttoMaterial= G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  if (pttoMaterial) captureMaterial = pttoMaterial;
  G4cout<<"The Capture material is set to "<<materialChoice<<G4endl;
}

void GPDetectorConstruction::SetTranTubeMaterial(G4String materialChoice)
{
  // search the material by its name
  //G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
  G4Material* pttoMaterial= G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  if (pttoMaterial) tranMaterial = pttoMaterial;
  G4cout<<"The transportion tube  material is set to "<<materialChoice<<G4endl;
}

#include "G4RunManager.hh"

void GPDetectorConstruction::UpdateGeometry()
{
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructPositronResource());
  G4cout<<"Updated the construction!"<<G4endl;
  return ;
}



void GPDetectorConstruction::PrintDetectorParameters()
{
  G4cout << "\n------------------------------------------------------------"
         << "\nThe box is: " << worldX <<"*"<<worldY <<"*"
         <<worldZ<<"*mm^3\n" 
         << "Target thickness is:"<<targetBoxZ/mm << "mm\n" 
         << "Target material is :"<< targetMaterial->GetName()<<"\n" 
	 <<"Target EDD Cell delta is: "<<TargetCellX<<"*"
	 <<TargetCellY<<"*"<<TargetCellZ<<"mm^3\n"
	 <<"Cell number is: "<<eddDim[0]<<"*"<<eddDim[1]<<"*"<<eddDim[2]<<"\n"
         << "\n------------------------------------------------------------\n"
	 << G4endl;
}
