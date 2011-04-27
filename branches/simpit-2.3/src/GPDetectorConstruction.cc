// $Id: GPDetectorConstruction.cc,v 1.9 2006/06/29 17:47:19 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#include "GPDetectorConstruction.hh"
#include "GPDetectorMessenger.hh"
#include "GPFieldSetup.hh"
#include "GPCaptureFieldManager.hh"
#include "GPSurfaceParticleScorer.hh"
#include "GPTargetGeometry.hh"
#include "GPCaptureGeometry.hh"
#include "GPAcceleratorGeometry.hh"
#include "GPSweeperGeometry.hh"
#include "GPCrystalGeometry.hh"

#include "GPModuleManager.hh"
#include "GPModuleStore.hh"
#include "GPGeometryStore.hh"
#include "GPGeometryManager.hh"


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
 :  targetPhys(0),
    capturePhys(0),
    acceleratorPhys(0),
    sweeperPhys(0),
    worldBox(0),worldLog(0),worldPhys(0)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPDetectorConstruction::GPDetectorConstruction()"<<G4endl;
#endif
    GPModuleManager::GetInstance()->ConstructModule();
    crystalGeometry = (GPCrystalGeometry*)GPGeometryStore::GetInstance()->FindGeometry("/crystal/geometry/");
    sweeperGeometry = (GPSweeperGeometry*)GPGeometryStore::GetInstance()->FindGeometry("/sweeper/geometry/");
    targetGeometry  = (GPTargetGeometry*) GPGeometryStore::GetInstance()->FindGeometry("/target/geometry/");
    captureGeometry = (GPCaptureGeometry*)GPGeometryStore::GetInstance()->FindGeometry("/capture/geometry/");
    acceleratorGeometry = (GPAcceleratorGeometry*)GPGeometryStore::GetInstance()->FindGeometry("/accelerator/geometry/");

    dWorldX = 500.0e-3;
    dWorldY = 500.0e-3;
    dWorldZ = 10000.0e-3;

    DefineMaterials();
    SetWorldMaterial ("G4_Galactic");

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
    GPFieldSetup::DestroyGPFieldSetup();
    if(Vacuum) 				delete Vacuum;
    if(W) 					delete W;
    GPModuleManager::GetInstance()->Delete();
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
  //worldBox = new G4Box("worldBox",m*dWorldX/2,m*dWorldY/2,m*dWorldZ/2);
  //worldLog = new G4LogicalVolume(worldBox,worldMaterial,"worldLog");
  //worldPhys = new G4PVPlacement(0,G4ThreeVector(),worldLog,"world",0,false,0);
  //GPGeometryManager::GetInstance()->ConstructGeometry(worldLog,G4ThreeVector(0,0,0));
  //GPModuleStore::GetInstance()->FindModule(GPModuleManager::GetInstance()->GetRootName())
    //->ConstructGeometry(worldLog,G4ThreeVector(0,0,0));
  worldPhys = GPModuleManager::GetInstance()->ConstructGeometry();
  //GPModuleManager::GetInstance()->Print();
  //ConstructTarget();
  /*
  G4double pz;
  G4ThreeVector vecTarPosition(0,0,0);
  pz = vecTarPosition.z()+targetGeometry->GetParameter("gz","")/2+captureGeometry->GetParameter("l","")/2;
  G4ThreeVector vecCapPosition(0,0,pz);
  pz = vecCapPosition.z()+captureGeometry->GetParameter("l","")/2+acceleratorGeometry->GetParameter("l","")/2;
  G4ThreeVector vecAccPosition(0,0,pz);

  pz = vecTarPosition.z()-targetGeometry->GetParameter("gz","")/2-sweeperGeometry->GetParameter("l","")/2;
  G4ThreeVector vecSwpPosition(0,0,pz);
  pz = vecSwpPosition.z()-sweeperGeometry->GetParameter("l","")/2-crystalGeometry->GetParameter("l","")/2;
  G4ThreeVector vecCryPosition(0,0,pz);

  targetPhys = targetGeometry->Construct(worldLog,vecTarPosition);
  capturePhys = captureGeometry->Construct(worldLog,vecCapPosition);
  acceleratorPhys = acceleratorGeometry->Construct(worldLog,vecAccPosition);
  sweeperPhys = sweeperGeometry->Construct(worldLog,vecSwpPosition);
  crystalGeometry->Construct(worldLog,vecCryPosition);
  */
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

void GPDetectorConstruction::SetWorldMaterial(G4String materialChoice)
{
  // search the material by its name
  G4Material* pttoMaterial= G4NistManager::Instance()->FindOrBuildMaterial(materialChoice);
  if (pttoMaterial) worldMaterial = pttoMaterial;
  G4cout<<"The world material is set to "<<materialChoice<<G4endl;
}

#include "G4RunManager.hh"

void GPDetectorConstruction::UpdateGeometry()
{
    
  	G4RunManager::GetRunManager()->DefineWorldVolume(ConstructPositronResource());
  	G4cout<<"Updated the detector!"<<G4endl;
    //PrintDetectorParameters();

  	return ;
}



void GPDetectorConstruction::PrintDetectorParameters()
{
  G4cout 
	<<"\n--------------------Print detector status-------------------"
        <<"\nThe world box: " << dWorldX <<"*"<<dWorldY <<"*" <<dWorldZ<<" m^3" 
        << G4endl;
  targetGeometry->Print();
  captureGeometry->Print();
  acceleratorGeometry->Print();
  sweeperGeometry->Print();
  crystalGeometry->Print();
  G4cout 
        << "\n------------------------------------------------------------"
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
    	{return sweeperPhys;}

	else if(name=="world")
    	{return worldPhys;}

	else return NULL;
}

G4double GPDetectorConstruction::GetParameter(std::string name) const
{
    std::string strInput=name;
    std::string strFirstLevel;
    std::string strLeft;
    size_t iFirstDot;
    iFirstDot = strInput.find(".");
    if(iFirstDot!=std::string::npos)
    {
    strFirstLevel=strInput.substr(0,iFirstDot);
    strLeft=strInput.substr(iFirstDot+1);
    }

    if(strFirstLevel=="target")
    {
      return targetGeometry->GetParameter(strLeft,name);
    }
    
    if(strFirstLevel=="capture")
    {
      return captureGeometry->GetParameter(strLeft,name);
    }

    if(strFirstLevel=="accelerator")
    {
      return acceleratorGeometry->GetParameter(strLeft,name);
    }

    if(strFirstLevel=="sweeper")
    {
      return sweeperGeometry->GetParameter(strLeft,name);
    }

    if(strFirstLevel=="crystal")
    {
      return crystalGeometry->GetParameter(strLeft,name);
    }
    
    else if(name=="world.x")
    return dWorldX;
    else if(name=="world.y")
    return dWorldY;
    else if(name=="world.z")
    return dWorldZ;
	
	else return 0;
}

void GPDetectorConstruction::SetParameter(std::string str)
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

    std::string strInput=str;
    std::string strFirstLevel;
    std::string strLeft;
    size_t iFirstDot;
    iFirstDot = strInput.find(".");
    if(iFirstDot!=std::string::npos)
    {
    strFirstLevel=strInput.substr(0,iFirstDot);
    strLeft=strInput.substr(iFirstDot+1);
    }

    if(strFirstLevel=="target")
    {
      targetGeometry->SetParameter(strLeft,str);
      return;
    }

    if(strFirstLevel=="capture")
    {
      captureGeometry->SetParameter(strLeft,str);
      return;
    }
    
    if(strFirstLevel=="accelerator")
    {
      acceleratorGeometry->SetParameter(strLeft,str);
      return;
    }
    
    if(strFirstLevel=="sweeper")
    {
      sweeperGeometry->SetParameter(strLeft,str);
      return;
    }

    if(strFirstLevel=="crystal")
    {
      crystalGeometry->SetParameter(strLeft,str);
      return;
    }

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

std::vector<G4int> GPDetectorConstruction::GetEddDim()
{
  return targetGeometry->GetEddDim();
}

void GPDetectorConstruction::Print(std::ofstream& fstOuput)
{
  fstOuput
	<<"\nDetector status:"
        <<"\nWorld box(m):" 
        <<"\nx, " << dWorldX 
        <<"\ny, " << dWorldY 
        <<"\nz, " << dWorldZ
        << G4endl;
  targetGeometry->Print(fstOuput);
  captureGeometry->Print(fstOuput);
  acceleratorGeometry->Print(fstOuput);
  sweeperGeometry->Print(fstOuput);
  crystalGeometry->Print(fstOuput);
  GPFieldSetup::GetGPFieldSetup()->Print(fstOuput);
}
