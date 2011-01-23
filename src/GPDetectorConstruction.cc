// $Id: GPDetectorConstruction.cc,v 1.9 2006/06/29 17:47:19 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#include "GPDetectorConstruction.hh"
#include "GPTargetROGeometry.hh"
#include "GPTargetROGeometryTubs.hh"
#include "GPTargetSD.hh"
#include "GPDetectorMessenger.hh"
#include "GPFieldSetup.hh"
#include "GPCaptureFieldManager.hh"
#include "GPSurfaceParticleScorer.hh"
#include "GPTargetGeometry.hh"


#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Material.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4NistManager.hh"
#include "G4VisAttributes.hh"
#include "G4MultiFunctionalDetector.hh"

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
 :  targetTubs(0),targetLog(0),targetPhys(0),
    captureTube(0),captureLog(0),capturePhys(0),
    tranTube(0),tranLog(0),tranPhys(0),
    acceleratorTube(0),acceleratorLog(0),acceleratorPhys(0),
    worldBox(0),worldLog(0),worldPhys(0),
    fieldSetup(0),targetSD(0),targetRO(0),targetROTubs(0)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPDetectorConstruction::GPDetectorConstruction()"<<G4endl;
#endif
    targetGeometry = new GPTargetGeometry();
    dTargetTubeInnerRadius = 0;
    dTargetTubeOuterRadius = 25e-3;
    dTargetTubeLength = 10.0e-3;
    dTargetTubeStartAngle = 0;
    dTargetTubeSpanningAngle = 360;
    dTargetGranularRadius = 0.00075;
    iTargetGranularFlag=1;

    dTargetCellZ=1e-3;
    dTargetCellR=1e-3;
    dTargetCellPhi=360;
    vectEddDim.push_back(ceil(dTargetTubeLength/dTargetCellZ));
    vectEddDim.push_back(ceil(dTargetTubeOuterRadius/dTargetCellR));
    vectEddDim.push_back(ceil(dTargetTubeSpanningAngle/dTargetCellPhi));

    dCaptureTubeInnerRadius = 0.0e-3;
    dCaptureTubeOuterRadius = 20e-3;
    dCaptureTubeStartAngle = 0.0;
    dCaptureTubeSpanningAngle = 360.0;
    dCaptureTubeLength = 500.0e-3;
    dCaptureStepMax = 1.0e-2;
    dLithiumTubeLength=1.0e-2;
    dLithiumTubeOuterRadius=1.0e-2;
    iCaptureType=0;

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
    if(targetGeometry)		delete targetGeometry;
    if(detectorMessenger) 	delete detectorMessenger;
    if(fieldSetup) 			delete fieldSetup;
    if(targetRO) 			delete targetRO;
    if(targetROTubs) 			delete targetROTubs;
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
  ConstructTarget();
  ConstructCapture();
  ConstructAccelerator();
  ConstructTranTubs();
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
void GPDetectorConstruction::ConstructTranTubs()
{
//--------------------------------transportion tube
  tranTube = new G4Tubs("tranTube",
      m*dTranTubeInnerRadius,
      m*dTranTubeOuterRadius,
      m*dTranTubeLength/2.0,
      deg*dTranTubeStartAngle,
      deg*dTranTubeSpanningAngle);

  tranLog = new G4LogicalVolume(tranTube,tranMaterial,"tranLog");
  G4double tranPos_x = 0.0;
  G4double tranPos_y = 0.0;
  G4double tranPos_z = -(dTargetTubeLength+dTranTubeLength)/2;
  tranPhys = new G4PVPlacement(0,
             G4ThreeVector(m*tranPos_x,m*tranPos_y,m*tranPos_z),
             tranLog,"tran",worldLog,false,0);

  // Visualization attributes
  //
  //worldLog->SetVisAttributes (G4VisAttributes::Invisible);
  G4VisAttributes* tranLogVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.3));
  tranLogVisAtt->SetVisibility(true);
  tranLogVisAtt->SetForceSolid(true);
  tranLog->SetVisAttributes(tranLogVisAtt);
}

void GPDetectorConstruction::ConstructTarget()
{
  //------------------------------ target tube

  targetTubs = new G4Tubs("targetTubs",
      m*dTargetTubeInnerRadius,
      m*dTargetTubeOuterRadius,
      m*dTargetTubeLength/2,
      deg*dTargetTubeStartAngle,
      deg*dTargetTubeSpanningAngle);

  targetLog = new G4LogicalVolume(targetTubs,Vacuum,"targetLog");
  G4double targetPos_x = 0.0;
  G4double targetPos_y = 0.0;
  G4double targetPos_z = 0.0;
  targetPhys = new G4PVPlacement(0,
             G4ThreeVector(m*targetPos_x,m*targetPos_y,m*targetPos_z),
             targetLog,"targetPhys",worldLog,false,0);
  
  G4double dTargetHitTubeInnerRadius=dTargetTubeInnerRadius;      
  G4double dTargetHitTubeOuterRadius=dTargetTubeOuterRadius;      
  G4double dTargetHitTubeLength=0.001;         
  G4double dTargetHitTubeStartAngle=dTargetTubeStartAngle;     
  G4double dTargetHitTubeSpanningAngle=dTargetTubeSpanningAngle; 
  ///*
  if(iTargetGranularFlag==1)
  {
    targetLog->SetMaterial(Vacuum);
    //targetGeometry->SetPoint(G4ThreeVector(0,0,0));
    targetGeometry->Construct(targetLog,
	G4ThreeVector(0,0,-dTargetHitTubeLength*0.5),
	dTargetGranularRadius,
        dTargetTubeOuterRadius*sqrt(2.0),
	dTargetTubeOuterRadius*sqrt(2.0),
	dTargetTubeLength-dTargetHitTubeLength,
	0);
  }
  //*/
  
  //sub detector of target: for hits

  G4ThreeVector targetHitPoint = G4ThreeVector( 0.0, 0.0, (dTargetTubeLength*0.5-dTargetHitTubeLength*0.5)*m);


  G4Tubs* targetHitTubs = new G4Tubs("targetHitTubs",
      m*dTargetHitTubeInnerRadius,
      m*dTargetHitTubeOuterRadius,
      m*dTargetHitTubeLength/2,
      deg*dTargetHitTubeStartAngle,
      deg*dTargetHitTubeSpanningAngle);

  G4LogicalVolume* targetHitLog = new G4LogicalVolume(targetHitTubs,Vacuum,"targetHitLog");
  
  G4VPhysicalVolume* targetHitPhys = new G4PVPlacement(0,
    targetHitPoint,
    targetHitLog,"targetHitPhys",targetLog,false,0);
    

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String targetSDName="/PositronSource/Target/EddSD";
  G4String targetROName="targetROGeometry";

  if(targetSD)
  {
    targetSD->SetEddDim(vectEddDim);  
  }
  else
  {
    targetSD=new GPTargetSD(targetSDName,vectEddDim);
  }
  /*
  if(targetRO)
  {
    delete targetRO;
  }
  targetRO=new GPTargetROGeometry(targetROName,dTargetTubeOuterRadius,dTargetTubeOuterRadius,dTargetTubeLength,vectEddDim);
  targetRO->BuildROGeometry();
  targetRO->SetName(targetROName);

  targetSD->SetROgeometry(targetRO);  

  */
  if(targetROTubs)
  {
    delete targetROTubs;
  }
  targetROTubs=new GPTargetROGeometryTubs(targetROName,dTargetTubeInnerRadius,dTargetTubeOuterRadius,dTargetTubeLength,vectEddDim);
  targetROTubs->BuildROGeometry();
  targetROTubs->SetName(targetROName);
  
  targetSD->SetROgeometry(targetROTubs);  

  SDman->AddNewDetector(targetSD);
  targetLog->SetSensitiveDetector(targetSD); 

  G4MultiFunctionalDetector* targetMultiFunDet=(G4MultiFunctionalDetector*)SDman->FindSensitiveDetector("/PositronSource/Target/MultiFunDet");
  GPSurfaceParticleScorer* targetParticleScorer=0;
  if(targetMultiFunDet==NULL)
  {
    targetMultiFunDet = new G4MultiFunctionalDetector("/PositronSource/Target/MultiFunDet");
    targetParticleScorer = new GPSurfaceParticleScorer("TargetParticleScorerZPlus",1,2);
    targetMultiFunDet->RegisterPrimitive(targetParticleScorer);
    SDman->AddNewDetector(targetMultiFunDet);
  }

  targetHitLog->SetSensitiveDetector(targetMultiFunDet); 
  /*
  G4VisAttributes* targetLogVisAtt= new G4VisAttributes(G4Colour(1.0,0,1.0,0.3));
  targetLogVisAtt->SetVisibility(true);
  targetLogVisAtt->SetForceSolid(true);
  targetLog->SetVisAttributes(targetLogVisAtt);
  */

}

void GPDetectorConstruction::ConstructCapture()
{
  //------------------------------ capture tube

  captureTube = new G4Tubs("captureTube",
      m*dCaptureTubeInnerRadius,
      m*dCaptureTubeOuterRadius,
      m*dCaptureTubeLength/2.0,
      deg*dCaptureTubeStartAngle,
      deg*dCaptureTubeSpanningAngle);

  captureLog = new G4LogicalVolume(captureTube,captureMaterial,"captureLog");
  G4double capturePos_x = 0.0;
  G4double capturePos_y = 0.0;
  G4double capturePos_z = (dTargetTubeLength+dCaptureTubeLength)/2;
  capturePhys = new G4PVPlacement(0,
             G4ThreeVector(m*capturePos_x,m*capturePos_y,m*capturePos_z),
             captureLog,"capture",worldLog,false,0);

  captureLog->SetFieldManager(fieldSetup->GetLocalFieldManager("capture"),true);
  //captureLog->SetUserLimits(new G4UserLimits(dCaptureStepMax*m));

  if(iCaptureType==4) 
  {
    SetLithiumLens(dLithiumTubeLength,dLithiumTubeOuterRadius);
  }

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4MultiFunctionalDetector* captureMultiFunDet=(G4MultiFunctionalDetector*)SDman->FindSensitiveDetector("/PositronSource/Capture/MultiFunDet");
  GPSurfaceParticleScorer* captureParticleScorer=0;
  if(captureMultiFunDet==NULL)
  {
    G4MultiFunctionalDetector* captureMultiFunDet = new G4MultiFunctionalDetector("/PositronSource/Capture/MultiFunDet");
    GPSurfaceParticleScorer* captureParticleScorer = new GPSurfaceParticleScorer("CaptureParticleScorerZPlus",1,2);
    captureMultiFunDet->RegisterPrimitive(captureParticleScorer);
    SDman->AddNewDetector(captureMultiFunDet);
  }
  captureLog->SetSensitiveDetector(captureMultiFunDet); 

  G4VisAttributes* captureLogVisAtt= new G4VisAttributes(G4Colour(0,1.0,1.0,0.3));
  captureLogVisAtt->SetVisibility(true);
  captureLogVisAtt->SetForceSolid(true);
  captureLog->SetVisAttributes(captureLogVisAtt);

}

void GPDetectorConstruction::ConstructAccelerator()
{
  //------------------------------ accelerator tube

  acceleratorTube = new G4Tubs("acceleratorTube",
      m*dAcceleratorTubeInnerRadius,
      m*dAcceleratorTubeOuterRadius,
      m*dAcceleratorTubeLength/2.0,
      deg*dAcceleratorTubeStartAngle,
      deg*dAcceleratorTubeSpanningAngle);

  acceleratorLog = new G4LogicalVolume(acceleratorTube,acceleratorMaterial,"acceleratorLog");
  G4double acceleratorPos_x = 0.0;
  G4double acceleratorPos_y = 0.0;
  G4double acceleratorPos_z = dCaptureTubeLength+(dTargetTubeLength+dAcceleratorTubeLength)/2;
  acceleratorPhys = new G4PVPlacement(0,
             G4ThreeVector(m*acceleratorPos_x,m*acceleratorPos_y,m*acceleratorPos_z),
             acceleratorLog,"accelerator",worldLog,false,0);

  acceleratorLog->SetFieldManager(fieldSetup->GetLocalFieldManager("accelerator"),true);
  //acceleratorLog->SetUserLimits(new G4UserLimits(dAcceleratorStepMax*m));

  G4VisAttributes* acceleratorLogVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0,0.3));
  acceleratorLogVisAtt->SetVisibility(true);
  acceleratorLogVisAtt->SetForceSolid(true);
  acceleratorLog->SetVisAttributes(acceleratorLogVisAtt);

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4MultiFunctionalDetector* acceleratorMultiFunDet=(G4MultiFunctionalDetector*)SDman->FindSensitiveDetector("/PositronSource/Accelerator/MultiFunDet");
  GPSurfaceParticleScorer* acceleratorParticleScorer=0;
  if(acceleratorMultiFunDet==NULL)
  {
    G4MultiFunctionalDetector* acceleratorMultiFunDet = new G4MultiFunctionalDetector("/PositronSource/Accelerator/MultiFunDet");
    GPSurfaceParticleScorer* acceleratorParticleScorer = new GPSurfaceParticleScorer("AcceleratorParticleScorerZPlus",1,2);
    acceleratorMultiFunDet->RegisterPrimitive(acceleratorParticleScorer);
    SDman->AddNewDetector(acceleratorMultiFunDet);
  }
  acceleratorLog->SetSensitiveDetector(acceleratorMultiFunDet); 
  
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
  G4Material* pttoMaterial= G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  if (pttoMaterial) worldMaterial = pttoMaterial;
  G4cout<<"The world material is set to "<<materialChoice<<G4endl;
}

void GPDetectorConstruction::SetCaptureMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial= G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  if (pttoMaterial) captureMaterial = pttoMaterial;
  G4cout<<"The Capture material is set to "<<materialChoice<<G4endl;
}

void GPDetectorConstruction::SetAcceleratorMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial= G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  if (pttoMaterial) acceleratorMaterial = pttoMaterial;
  G4cout<<"The Accelerator material is set to "<<materialChoice<<G4endl;
}

void GPDetectorConstruction::SetTranTubeMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial= G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  if (pttoMaterial) tranMaterial = pttoMaterial;
  G4cout<<"The transportion tube  material is set to "<<materialChoice<<G4endl;
}

#include "G4RunManager.hh"

void GPDetectorConstruction::UpdateGeometry()
{
    vectEddDim[0]=ceil(dTargetTubeLength/dTargetCellZ);
    vectEddDim[1]=ceil(dTargetTubeOuterRadius/dTargetCellR);
    vectEddDim[2]=ceil(dTargetTubeSpanningAngle/dTargetCellPhi);
    
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
        <<MacRightAlign<<std::setw(24)<<"Target tube: "<<"radius "<<dTargetTubeOuterRadius <<" m, length " <<dTargetTubeLength<< " m\n" 
        <<MacRightAlign<<std::setw(24)<<"Target EDD Cell: "<<dTargetCellZ<<"*" <<dTargetCellR<<"*"<<dTargetCellPhi<<" m^3\n"
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
    return dTargetTubeOuterRadius;
    else if(name=="target.y")
    return dTargetTubeOuterRadius;
    else if(name=="target.z")
    return dTargetTubeLength;
    
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
    else if(name=="capture.lithium.l")
    return dLithiumTubeLength;
    else if(name=="capture.lithium.or")
    return dLithiumTubeOuterRadius;
    
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
	G4double   		  dValueNew;
	G4double   		  dValueOrg;
	
	ss>>key>>dValueOrg>>unit;
    if(unit!="")
    dValueNew=(dValueOrg*G4UIcommand::ValueOf(unit.c_str()))/m;
    else dValueNew=dValueOrg;

    if(key=="target.x")
    dTargetTubeOuterRadius = dValueNew;
    else if(key=="target.y")
    dTargetTubeOuterRadius = dValueNew;
    else if(key=="target.z")
    dTargetTubeLength = dValueNew;
    else if(key=="target.granular.flag")
    iTargetGranularFlag = dValueNew;
    else if(key=="target.granular.radius")
    dTargetGranularRadius = dValueNew;
    
    else if(key=="capture.ir")
    dCaptureTubeInnerRadius = dValueNew;
    else if(key=="capture.or")
    dCaptureTubeOuterRadius = dValueNew;
    else if(key=="capture.l")
    dCaptureTubeLength = dValueNew;
    else if(key=="capture.sa")
    dCaptureTubeStartAngle = dValueNew;
    else if(key=="capture.ea")
    dCaptureTubeSpanningAngle = dValueNew;
    else if(key=="capture.lithium.l")
    dLithiumTubeLength = dValueNew;
    else if(key=="capture.lithium.or")
    dLithiumTubeOuterRadius = dValueNew;
    
    
    else if(key=="transport.ir")
    dTranTubeInnerRadius = dValueNew;
    else if(key=="transport.or")
    dTranTubeOuterRadius = dValueNew;
    else if(key=="transport.l")
    dTranTubeLength = dValueNew;
    else if(key=="transport.sa")
    dTranTubeStartAngle = dValueNew;
    else if(key=="transport.ea")
    dTranTubeSpanningAngle = dValueNew;
    
    else if(key=="accelerator.ir")
    dAcceleratorTubeInnerRadius = dValueNew;
    else if(key=="accelerator.or")
    dAcceleratorTubeOuterRadius = dValueNew;
    else if(key=="accelerator.l")
    dAcceleratorTubeLength = dValueNew;
    else if(key=="accelerator.sa")
    dAcceleratorTubeStartAngle = dValueNew;
    else if(key=="accelerator.ea")
    dAcceleratorTubeSpanningAngle = dValueNew;
    
    else if(key=="world.x")
    dWorldX = dValueNew;
    else if(key=="world.y")
    dWorldY = dValueNew;
    else if(key=="world.z")
    dWorldZ = dValueNew;

	else 
	{
     	std::cout<<"the key is not exist."<<std::endl;
		return;
	}

	std::cout<<"Set "<<key<<" to "<< dValueOrg<<" "<<unit<<std::endl;

}
void GPDetectorConstruction::SetLithiumLens(G4double dLength,G4double dOuterRadius,G4double dInnerRadius, G4double dStartAngle, G4double dSpanningAngle )
{

    //Lithium lens
    //
  G4double dLithiumTubeInnerRadius=dInnerRadius;
  G4double dLithiumTubeOuterRadius=dOuterRadius;
  G4double dLithiumTubeLength=dLength;
  G4double dLithiumTubeStartAngle=dStartAngle;
  G4double dLithiumTubeSpanningAngle=dSpanningAngle;
  G4double lithiumPos_x = 0.0;
  G4double lithiumPos_y = 0.0;
  G4double lithiumPos_z = (dCaptureTubeLength-dLithiumTubeLength)/2-0.1e-3;

  G4String sLithiumMaterial="G4_Li";

  G4Material* lithiumMaterial;
  G4Tubs* lithiumTube;
  G4LogicalVolume* lithiumLog;
  G4VPhysicalVolume* lithiumPhys;

  lithiumMaterial = G4NistManager::Instance()->FindOrBuildMaterial(sLithiumMaterial);
  if (lithiumMaterial) 
  G4cout<<"The lithium material is set to "<<sLithiumMaterial<<G4endl;
  else 
  G4cout<<"Set lithium material failed"<<G4endl;

  lithiumTube = new G4Tubs("lithiumTube",m*dLithiumTubeInnerRadius,
					m*dLithiumTubeOuterRadius,m*dLithiumTubeLength/2.0,
                                    deg*dLithiumTubeStartAngle,deg*dLithiumTubeSpanningAngle);
  lithiumLog = new G4LogicalVolume(lithiumTube,lithiumMaterial,"lithiumLog");
  lithiumPhys = new G4PVPlacement(0,
             G4ThreeVector(m*lithiumPos_x,m*lithiumPos_y,m*lithiumPos_z),
             lithiumLog,"lithium",captureLog,false,0);

  G4VisAttributes* lithiumLogVisAtt= new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.3));
  lithiumLogVisAtt->SetVisibility(true);
  lithiumLogVisAtt->SetForceSolid(true);
  lithiumLog->SetVisAttributes(lithiumLogVisAtt);

}
void GPDetectorConstruction::SetCaptureType(G4int i)
{
  iCaptureType=i;
  G4cout<<"Set capture type to:"<<i<<G4endl;
}
