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
#include "G4VisAttributes.hh"

#include "G4UserLimits.hh"
#include "G4ThreeVector.hh"
#include "G4UIcommand.hh"
#include "G4FieldManager.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include <cmath>
#include <sstream>
#include <iomanip>

#define MacRightAlign  std::setiosflags(std::ios_base::right)
#define MacLeftAlign  std::setiosflags(std::ios_base::left)

GPDetectorConstruction::GPDetectorConstruction()
 :  targetBox(0),targetLog(0),targetPhys(0),
    captureTube(0),captureLog(0),capturePhys(0),
    tranTube(0),tranLog(0),tranPhys(0),
    acceleratorTube(0),acceleratorLog(0),acceleratorPhys(0),
    worldBox(0),worldLog(0),worldPhys(0),
    fieldSetup(0),targetSD(0),targetRO(0)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPDetectorConstruction::GPDetectorConstruction()"<<G4endl;
#endif
    dTargetBoxY = 25e-3;
    dTargetBoxX = 25e-3;
    dTargetBoxZ = 6.0e-3;

    dTargetCellX=0.5e-3;
    dTargetCellY=0.5e-3;
    dTargetCellZ=1e-3;
    vectEddDim.push_back(ceil(dTargetBoxX/dTargetCellX));
    vectEddDim.push_back(ceil(dTargetBoxY/dTargetCellY));
    vectEddDim.push_back(ceil(dTargetBoxZ/dTargetCellZ));

    dCaptureTubeInnerRadius = 0.0e-3;
    dCaptureTubeOuterRadius = 20e-3;
    dCaptureTubeStartAngle = 0.0;
    dCaptureTubeSpanningAngle = 360.0;
    dCaptureTubeLength = 500.0e-3;
    dCaptureStepMax = 1.0e-2;

    dTranTubeInnerRadius = 0.0e-3;
    dTranTubeOuterRadius = 20e-3;
    dTranTubeStartAngle = 0.0;
    dTranTubeSpanningAngle = 360.0;
    dTranTubeLength =4500.0e-3;

    dAcceleratorTubeInnerRadius = 0.0e-3;
    dAcceleratorTubeOuterRadius = 20e-3;
    dAcceleratorTubeStartAngle = 0.0;
    dAcceleratorTubeSpanningAngle = 360.0;
    dAcceleratorTubeLength = 1000.0e-3;
    dAcceleratorStepMax = 1.0e-2;

    dWorldX = 500.0e-3;
    dWorldY = 500.0e-3;
    dWorldZ = 10000.0e-3;
    /*
    dTargetBoxY = 25*mm;
    dTargetBoxX = 25*mm;
    dTargetBoxZ = 6.0*mm;

    dTargetCellX=0.5*mm;
    dTargetCellY=0.5*mm;
    dTargetCellZ=1*mm;
    vectEddDim.push_back(ceil(dTargetBoxX/dTargetCellX));
    vectEddDim.push_back(ceil(dTargetBoxY/dTargetCellY));
    vectEddDim.push_back(ceil(dTargetBoxZ/dTargetCellZ));

    dCaptureTubeInnerRadius = 0.*mm;
    dCaptureTubeOuterRadius = 20*mm;
    dCaptureTubeStartAngle = 0.*deg;
    dCaptureTubeSpanningAngle = 360.*deg;
    dCaptureTubeLength = 500.0*mm;

    dTranTubeInnerRadius = 0.*mm;
    dTranTubeOuterRadius = 20*mm;
    dTranTubeStartAngle = 0.*deg;
    dTranTubeSpanningAngle = 360.*deg;
    dTranTubeLength =4500.0*mm;

    dAcceleratorTubeInnerRadius = 0.*mm;
    dAcceleratorTubeOuterRadius = 20*mm;
    dAcceleratorTubeStartAngle = 0.*deg;
    dAcceleratorTubeSpanningAngle = 360.*deg;
    dAcceleratorTubeLength = 1000.0*mm;

    dWorldX = 500.0*mm;
    dWorldY = 500.0*mm;
    dWorldZ = 10000.0*mm;
    */    
    DefineMaterials();
    SetTargetMaterial ("G4_W");
    SetWorldMaterial ("G4_Galactic");
    SetCaptureMaterial ("G4_Galactic");
    SetAcceleratorMaterial ("G4_Galactic");
    SetTranTubeMaterial ("G4_Galactic");
    fieldSetup = new GPFieldSetup();
    detectorMessenger = new GPDetectorMessenger(this);
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPDetectorConstruction::GPDetectorConstruction()"<<G4endl;
#endif

}

GPDetectorConstruction::~GPDetectorConstruction()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPDetectorConstruction::~GPDetectorConstruction()"<<G4endl;
#endif
    if(detectorMessenger) 	delete detectorMessenger;
    if(fieldSetup) 			delete fieldSetup;
    if(targetRO) 			delete targetRO;
    if(Vacuum) 				delete Vacuum;
    if(W) 					delete W;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPDetectorConstruction::~GPDetectorConstruction()"<<G4endl;
#endif
}

G4VPhysicalVolume* GPDetectorConstruction::Construct()
{ 
    return ConstructPositronResource();
}

G4VPhysicalVolume* GPDetectorConstruction::ConstructPositronResource()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPDetectorConstruction::ConstructPositronResource()"<<G4endl;
#endif
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  //------------------------------ world
  worldBox = new G4Box("worldBox",m*dWorldX/2,m*dWorldY/2,m*dWorldZ/2);
  worldLog = new G4LogicalVolume(worldBox,worldMaterial,"worldLog");
  worldPhys = new G4PVPlacement(0,G4ThreeVector(),worldLog,"world",0,false,0);

  //------------------------------ target box

  targetBox = new G4Box("targetBox",m*dTargetBoxY/2,m*dTargetBoxX/2,m*dTargetBoxZ/2);
  targetLog = new G4LogicalVolume(targetBox,targetMaterial,"targetLog");
  G4double targetPos_x = 0.0;
  G4double targetPos_y = 0.0;
  G4double targetPos_z = 0.0;
  targetPhys = new G4PVPlacement(0,
             G4ThreeVector(m*targetPos_x,m*targetPos_y,m*targetPos_z),
             targetLog,"targetPhys",worldLog,false,0);
  
  //------------------------------ capture tube

  captureTube = new G4Tubs("captureTube",m*dCaptureTubeInnerRadius,
					m*dCaptureTubeOuterRadius,m*dCaptureTubeLength/2.0,
                                    deg*dCaptureTubeStartAngle,deg*dCaptureTubeSpanningAngle);
  captureLog = new G4LogicalVolume(captureTube,captureMaterial,"captureLog");
  G4double capturePos_x = 0.0;
  G4double capturePos_y = 0.0;
  G4double capturePos_z = (dTargetBoxZ+dCaptureTubeLength)/2;
  capturePhys = new G4PVPlacement(0,
             G4ThreeVector(m*capturePos_x,m*capturePos_y,m*capturePos_z),
             captureLog,"capture",worldLog,false,0);

  captureLog->SetFieldManager(fieldSetup->GetLocalFieldManager("capture"),true);
  captureLog->SetUserLimits(new G4UserLimits(dCaptureStepMax*m));

  //------------------------------ accelerator tube

  acceleratorTube = new G4Tubs("acceleratorTube",m*dAcceleratorTubeInnerRadius,
                                    m*dAcceleratorTubeOuterRadius,m*dAcceleratorTubeLength/2.0,
                                    deg*dAcceleratorTubeStartAngle,deg*dAcceleratorTubeSpanningAngle);
  acceleratorLog = new G4LogicalVolume(acceleratorTube,acceleratorMaterial,"acceleratorLog");
  G4double acceleratorPos_x = 0.0;
  G4double acceleratorPos_y = 0.0;
  G4double acceleratorPos_z = dCaptureTubeLength+(dTargetBoxZ+dAcceleratorTubeLength)/2;
  acceleratorPhys = new G4PVPlacement(0,
             G4ThreeVector(m*acceleratorPos_x,m*acceleratorPos_y,m*acceleratorPos_z),
             acceleratorLog,"accelerator",worldLog,false,0);

  acceleratorLog->SetFieldManager(fieldSetup->GetLocalFieldManager("accelerator"),true);
  acceleratorLog->SetUserLimits(new G4UserLimits(dAcceleratorStepMax*m));

//--------------------------------transportion tube
  tranTube = new G4Tubs("tranTube",m*dTranTubeInnerRadius,
                                    m*dTranTubeOuterRadius,m*dTranTubeLength/2.0,
                                    deg*dTranTubeStartAngle,deg*dTranTubeSpanningAngle);
  tranLog = new G4LogicalVolume(tranTube,tranMaterial,"tranLog");
  G4double tranPos_x = 0.0;
  G4double tranPos_y = 0.0;
  G4double tranPos_z = -(dTargetBoxZ+dTranTubeLength)/2;
  tranPhys = new G4PVPlacement(0,
             G4ThreeVector(m*tranPos_x,m*tranPos_y,m*tranPos_z),
             tranLog,"tran",worldLog,false,0);


//----------------------------------------------------------------------------
//Sensitive Detector
//----------------------------------------------------------------------------
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String targetSDName="/mydet/target";
  G4String targetROName="targetROGeometry";

  if(targetSD)
  {
  targetSD->SetEddDim(vectEddDim);  
  }
  else
  {
  targetSD=new GPTargetSD(targetSDName,vectEddDim);
  }

  if(targetRO)
  {
  delete targetRO;
  }
  targetRO=new GPTargetROGeometry(targetROName,dTargetBoxX,dTargetBoxY,dTargetBoxZ,vectEddDim);
  targetRO->BuildROGeometry();
  targetRO->SetName(targetROName);
  targetSD->SetROgeometry(targetRO);  
  SDman->AddNewDetector(targetSD);
  targetLog->SetSensitiveDetector(targetSD); 

  // Visualization attributes
  //
  //worldLog->SetVisAttributes (G4VisAttributes::Invisible);
  G4VisAttributes* tranLogVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.3));
  tranLogVisAtt->SetVisibility(true);
  tranLogVisAtt->SetForceSolid(true);
  tranLog->SetVisAttributes(tranLogVisAtt);

  G4VisAttributes* targetLogVisAtt= new G4VisAttributes(G4Colour(1.0,0,0,0.3));
  targetLogVisAtt->SetVisibility(true);
  targetLogVisAtt->SetForceSolid(true);
  targetLog->SetVisAttributes(targetLogVisAtt);

  G4VisAttributes* captureLogVisAtt= new G4VisAttributes(G4Colour(0,1.0,1.0,0.3));
  captureLogVisAtt->SetVisibility(true);
  captureLogVisAtt->SetForceSolid(true);
  captureLog->SetVisAttributes(captureLogVisAtt);

  G4VisAttributes* acceleratorLogVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0,0.3));
  acceleratorLogVisAtt->SetVisibility(true);
  acceleratorLogVisAtt->SetForceSolid(true);
  acceleratorLog->SetVisAttributes(acceleratorLogVisAtt);
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
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPDetectorConstruction::ConstructPositronResource()"<<G4endl;
#endif

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
  dTargetBoxZ = val;
  vectEddDim[2] = ceil(val/dTargetCellZ);
  G4cout<<"The target thickness is set to "<<val<<" m"<<G4endl;
}

void GPDetectorConstruction::SetTargetXY(G4double val)
{
  dTargetBoxX = val;
  dTargetBoxY = val; 
  vectEddDim[0] = ceil(val/dTargetCellX);
  vectEddDim[1] = ceil(val/dTargetCellY);
  G4cout<<"The target box X and Y length is set to "<<val<<" m"<<G4endl;
}

void GPDetectorConstruction::SetWorldSizeXYZ(G4double valx,G4double valy, G4double valz)
{
  dWorldX = valx;
  dWorldY = valy;
  dWorldZ = valz;
  G4cout<<"The world volume is set to "<<valx<<"*"<<valy<<"*"<<valz<<" m^3!"<<G4endl;
}

void GPDetectorConstruction::SetWorldSizeX(G4double val)
{
  dWorldX = val;
  G4cout<<"The world x thickness is set to "<<val<<" m"<<G4endl;
}

void GPDetectorConstruction::SetWorldSizeY(G4double val)
{
  dWorldY = val;
  G4cout<<"The world y is set to "<<val<<" m"<<G4endl;
}

void GPDetectorConstruction::SetWorldSizeZ(G4double val)
{
  dWorldZ = val;
  G4cout<<"The world z is set to "<<val<<" m"<<G4endl;
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

void GPDetectorConstruction::SetAcceleratorMaterial(G4String materialChoice)
{
  // search the material by its name
  //G4Material* pttoMaterial = G4Material::GetMaterial(materialChoice);
  G4Material* pttoMaterial= G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  if (pttoMaterial) acceleratorMaterial = pttoMaterial;
  G4cout<<"The Accelerator material is set to "<<materialChoice<<G4endl;
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
    vectEddDim[0]=ceil(dTargetBoxX/dTargetCellX);
    vectEddDim[1]=ceil(dTargetBoxY/dTargetCellY);
    vectEddDim[2]=ceil(dTargetBoxZ/dTargetCellZ);
    
  	G4RunManager::GetRunManager()->DefineWorldVolume(ConstructPositronResource());
  	G4cout<<"Updated the detector!"<<G4endl;
    //PrintDetectorParameters();

  	return ;
}



void GPDetectorConstruction::PrintDetectorParameters()
{
  G4cout 
	<<"\n--------------------Print detector status-------------------\n"
        <<MacRightAlign<<std::setw(24)<<"The world box: " << dWorldX <<"*"<<dWorldY <<"*" <<dWorldZ<<" m^3\n" 
        <<MacRightAlign<<std::setw(24)<<"Target box: "<<dTargetBoxX <<"*"<<dTargetBoxY <<"*"<<dTargetBoxZ<< " m^3\n" 
        <<MacRightAlign<<std::setw(24)<<"Target EDD Cell: "<<dTargetCellX<<"*" <<dTargetCellY<<"*"<<dTargetCellZ<<" m^3\n"
        <<MacRightAlign<<std::setw(24)<<"Target Cell number: "<<vectEddDim[0]<<"*"<<vectEddDim[1]<<"*"<<vectEddDim[2]<<"\n"
        <<MacRightAlign<<std::setw(24)<<"Target material: "<< targetMaterial->GetName()<<"\n" 
        <<MacRightAlign<<std::setw(24)<<"Capture tube: "<<"radius "<<dCaptureTubeOuterRadius <<" m, length "<<dCaptureTubeLength <<" m\n" 
        <<MacRightAlign<<std::setw(24)<<"capture.step.max: "<<dCaptureStepMax<<" m\n" 
        <<MacRightAlign<<std::setw(24)<<"accelerator.step.max: "<<dAcceleratorStepMax<<" m\n" 
        << "------------------------------------------------------------\n"
        << G4endl;

}

const G4VPhysicalVolume* GPDetectorConstruction::GetPhysicalVolume(std::string name) const
{   
   	if(name=="target")
    	{return targetPhys;}

   	else if(name=="capture")
    	{return capturePhys;}

   	else if(name=="accelerator")
    	{return acceleratorPhys;}

   	else if(name=="transport")
    	{return tranPhys;}

	else if(name=="world")
    	{return worldPhys;}

	else return NULL;
}
void GPDetectorConstruction::SetUserLimits(std::string str)
{
	std::stringstream ss(str);
	std::string key;
	double      value;
	std::string unit;
	ss>>key>>value>>unit;   
      	value=(value*G4UIcommand::ValueOf(unit.c_str()))/m;
	if(key=="capture.step.max")
	{
        	dCaptureStepMax = value;
		G4cout<<"Set "<<key<<" to: "<<value<<" m"<<G4endl;  
	}
	else if(key=="accelerator.step.max")
	{
        	dAcceleratorStepMax = value;
		G4cout<<"Set "<<key<<" to: "<<value<<" m"<<G4endl;
	}
	else
	{
		G4cout<<"The key does't exist: "<<key<<G4endl;
	}

}


G4double GPDetectorConstruction::GetDetectorSize(std::string name) const
{
    if(name=="target.x")
    return dTargetBoxY;
    else if(name=="target.y")
    return dTargetBoxX;
    else if(name=="target.z")
    return dTargetBoxZ;
    
    else if(name=="capture.ir")
    return dCaptureTubeInnerRadius;
    else if(name=="capture.or")
    return dCaptureTubeOuterRadius;
    else if(name=="capture.l")
    return dCaptureTubeLength;
    else if(name=="capture.sa")
    return dCaptureTubeStartAngle;
    else if(name=="capture.ea")
    return dCaptureTubeSpanningAngle;
    
    else if(name=="transport.ir")
    return dTranTubeInnerRadius;
    else if(name=="transport.or")
    return dTranTubeOuterRadius;
    else if(name=="transport.l")
    return dTranTubeLength;
    else if(name=="transport.sa")
    return dTranTubeStartAngle;
    else if(name=="transport.ea")
    return dTranTubeSpanningAngle;
    
    else if(name=="accelerator.ir")
    return dAcceleratorTubeInnerRadius;
    else if(name=="accelerator.or")
    return dAcceleratorTubeOuterRadius;
    else if(name=="accelerator.l")
    return dAcceleratorTubeLength;
    else if(name=="accelerator.sa")
    return dAcceleratorTubeStartAngle;
    else if(name=="accelerator.ea")
    return dAcceleratorTubeSpanningAngle;
    
    else if(name=="world.x")
    return dWorldX;
    else if(name=="world.y")
    return dWorldY;
    else if(name=="world.z")
    return dWorldZ;
	
	else return 0;
}

void GPDetectorConstruction::SetDetectorSize(std::string str)
{
	std::stringstream ss(str);
	std::string		  unit;
	std::string		  key;
	G4double   		  value;
	
	ss>>key>>value>>unit;
    value=(value*G4UIcommand::ValueOf(unit.c_str()))/m;

    if(key=="target.x")
    dTargetBoxY = value;
    else if(key=="target.y")
    dTargetBoxX = value;
    else if(key=="target.z")
    dTargetBoxZ = value;
    
    else if(key=="capture.ir")
    dCaptureTubeInnerRadius = value;
    else if(key=="capture.or")
    dCaptureTubeOuterRadius = value;
    else if(key=="capture.l")
    dCaptureTubeLength = value;
    else if(key=="capture.sa")
    dCaptureTubeStartAngle = value;
    else if(key=="capture.ea")
    dCaptureTubeSpanningAngle = value;
    
    else if(key=="transport.ir")
    dTranTubeInnerRadius = value;
    else if(key=="transport.or")
    dTranTubeOuterRadius = value;
    else if(key=="transport.l")
    dTranTubeLength = value;
    else if(key=="transport.sa")
    dTranTubeStartAngle = value;
    else if(key=="transport.ea")
    dTranTubeSpanningAngle = value;
    
    else if(key=="accelerator.ir")
    dAcceleratorTubeInnerRadius = value;
    else if(key=="accelerator.or")
    dAcceleratorTubeOuterRadius = value;
    else if(key=="accelerator.l")
    dAcceleratorTubeLength = value;
    else if(key=="accelerator.sa")
    dAcceleratorTubeStartAngle = value;
    else if(key=="accelerator.ea")
    dAcceleratorTubeSpanningAngle = value;
    
    else if(key=="world.x")
    dWorldX = value;
    else if(key=="world.y")
    dWorldY = value;
    else if(key=="world.z")
    dWorldZ = value;

	else 
	{
     	std::cout<<"the key is not exist."<<std::endl;
		return;
	}

	std::cout<<"Set "<<key<<" to "<< value<<" m"<<std::endl;

}
