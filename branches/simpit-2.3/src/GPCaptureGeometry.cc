// $Id: GPCaptureGeometry.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//

#include "GPCaptureGeometry.hh"
#include "GPSurfaceParticleScorer.hh"
#include "GPFieldSetup.hh"

#include "GPGeometryStore.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Para.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVReplica.hh"
#include "G4VisAttributes.hh"
#include "G4Orb.hh"
#include "G4UIcommand.hh"
#include "G4SDManager.hh"
#include "G4UserLimits.hh"

#include "globals.hh"

#include <sstream>
GPCaptureGeometry::GPCaptureGeometry(std::string sName, std::string sFatherName)
{
  SetActive(1);
  SetName(sName);
  SetFatherName(sFatherName);
  GPGeometryStore::GetInstance()->AddGeometry(GetName(),this);

  capMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  spaceMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  dCaptureTubeInnerRadius = 0;
  dCaptureTubeOuterRadius = 20e-3;
  dCaptureTubeLength = 0.5;
  dCaptureTubeStartAngle = 0;
  dCaptureTubeSpanningAngle = 360;
  iCaptureLimitStepFlag = 0;
  dCaptureLimitStepMax = 0.002;
  iCaptureHitFlag = 1;
  iCaptureType = 0;
  dLithiumTubeLength = 1e-3;
  dLithiumTubeOuterRadius = 1e-3;

}
GPCaptureGeometry::GPCaptureGeometry()
{
  capMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  spaceMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

    dCaptureTubeInnerRadius = 0;
    dCaptureTubeOuterRadius = 20e-3;
    dCaptureTubeLength = 0.5;
    dCaptureTubeStartAngle = 0;
    dCaptureTubeSpanningAngle = 360;
    iCaptureLimitStepFlag = 0;
    dCaptureLimitStepMax = 0.002;
    iCaptureHitFlag = 1;
    iCaptureType = 0;
    dLithiumTubeLength = 1e-3;
    dLithiumTubeOuterRadius = 1e-3;

}

GPCaptureGeometry::~GPCaptureGeometry()
{
  GPGeometryStore::GetInstance()->EraseItem(GetName());
}

void GPCaptureGeometry::Init()
{
  dGlobalLength=dCaptureTubeLength+1e-3;
}
G4VPhysicalVolume* GPCaptureGeometry::Construct(G4LogicalVolume* motherLog)
{
  return Construct(motherLog,vPosition);
}
G4VPhysicalVolume* GPCaptureGeometry::Construct(G4LogicalVolume* motherLog,G4ThreeVector point)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPCaptureGeometry::Construct(G4LogicalVolume,G4ThreeVector)"<<G4endl;
#endif

  Init();

  //------------------------------ capture tube
  captureTube = new G4Tubs("captureTube",
      m*dCaptureTubeInnerRadius,
      m*dCaptureTubeOuterRadius,
      m*(dCaptureTubeLength+1e-3)/2.0,
      deg*dCaptureTubeStartAngle,
      deg*dCaptureTubeSpanningAngle);

  captureLog = new G4LogicalVolume(captureTube,capMaterial,"captureLog");
  capturePhys = new G4PVPlacement(0,
      	     point*m,
             captureLog,"capture",motherLog,false,0);

  captureLog->SetFieldManager(GPFieldSetup::GetGPFieldSetup()->GetLocalFieldManager(GetName()+"field/"),true);
  if(iCaptureType==4) 
  {
    G4ThreeVector vecLithium(0,0,(dCaptureTubeLength+1e-3)/2-dLithiumTubeLength/2-1e-3);
    SetLithiumLens(captureLog,vecLithium,dLithiumTubeLength,dLithiumTubeOuterRadius);
  }

  if(iCaptureLimitStepFlag)
    captureLog->SetUserLimits(new G4UserLimits(dCaptureLimitStepMax*m));



  G4VisAttributes* captureLogVisAtt= new G4VisAttributes(G4Colour(0,1.0,1.0,0.3));
  captureLogVisAtt->SetVisibility(true);
  captureLogVisAtt->SetForceSolid(true);
  captureLog->SetVisAttributes(captureLogVisAtt);

  if(iCaptureHitFlag==1)
    SetHit(captureLog);

  return capturePhys;

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPCaptureGeometry::Construct(G4LogicalVolume,G4ThreeVector)"<<G4endl;
#endif
}


void GPCaptureGeometry::Print()
{
  G4cout
   <<"\n----------------------------Capture construct---------------------------"
   <<G4endl;
  G4cout
    <<"Capture type: Tubs construct."
    <<"\nRadius of Capture: "<<dCaptureTubeOuterRadius*m/mm<<" mm"
    <<"\nLength of Capture: "<<dCaptureTubeLength*m/mm<<" mm"
    <<"\nStep Limit Flag of Capture: "<<iCaptureLimitStepFlag
    <<"\nStep Limit of Capture: "<<dCaptureLimitStepMax*m/mm<<" mm"
    <<"\nHit Flag of Capture: "<<iCaptureHitFlag
    <<G4endl;
  
   
  G4cout
   <<"\n------------------------------------------------------------------------"
   <<G4endl;

}

void GPCaptureGeometry::SetParameter(std::string str,std::string strGlobal)
{
	std::stringstream ss(str);
	std::string		  sUnit;
	std::string		  sKey;
	G4double   		  dValueNew;
	G4double   		  dValueOrg;
	
	ss>>sKey>>dValueOrg>>sUnit;
    if(sUnit!="")
    dValueNew=(dValueOrg*G4UIcommand::ValueOf(sUnit.c_str()))/m;
    else dValueNew=dValueOrg;

    if(sKey=="ir")
      dCaptureTubeInnerRadius = dValueNew;
    else if(sKey=="or")
      dCaptureTubeOuterRadius = dValueNew;
    else if(sKey=="l")
      dCaptureTubeLength = dValueNew;
    else if(sKey=="sa")
      dCaptureTubeStartAngle = dValueNew;
    else if(sKey=="ea")
      dCaptureTubeSpanningAngle = dValueNew;
    else if(sKey=="type")
      iCaptureType = dValueNew;
    else if(sKey=="lithium.l")
      dLithiumTubeLength = dValueNew;
    else if(sKey=="lithium.or")
      dLithiumTubeOuterRadius = dValueNew;
    else if(sKey=="hit.flag")
      iCaptureHitFlag = dValueNew;
    else if(sKey=="limit.step.max")
      dCaptureLimitStepMax = dValueNew;
    else if(sKey=="limit.step.flag")
      iCaptureLimitStepFlag = dValueNew;
    else 
    {
  	std::cout<<"the Key is not exist."<<std::endl;
     	return;
    }

    Init();
    std::cout<<GetName()<<": Set "<<sKey<<": "<< dValueOrg<<" "<<sUnit<<std::endl;
}

G4double GPCaptureGeometry::GetParameter(std::string sKey, std::string sGlobal) const
{
    if(sKey=="ir")
    return dCaptureTubeInnerRadius;
    else if(sKey=="or")
    return dCaptureTubeOuterRadius;
    else if(sKey=="l")
    return (dCaptureTubeLength+1e-3);
    else if(sKey=="length")
    return dGlobalLength;
    else if(sKey=="sa")
    return dCaptureTubeStartAngle;
    else if(sKey=="ea")
    return dCaptureTubeSpanningAngle;
    else if(sKey=="type")
      return iCaptureType;
    else if(sKey=="lithium.l")
      return dLithiumTubeLength;
    else if(sKey=="lithium.or")
      return dLithiumTubeOuterRadius;
    else if(sKey=="hit.flag")
    return iCaptureHitFlag;
    else if(sKey=="limit.step.max")
    return dCaptureLimitStepMax;
    else if(sKey=="limit.step.flag")
    return iCaptureLimitStepFlag;

    else
    {
      std::cout<<"Key does not exist.\n"<<std::endl;
      return -1;
    }
}

//*/
G4VPhysicalVolume* GPCaptureGeometry::SetHit(G4LogicalVolume* motherLog)
{
  G4ThreeVector point(0,0,(dCaptureTubeLength+1e-3)/2-0.5e-3);
  return SetHitAtom(motherLog,point);
}

G4VPhysicalVolume* GPCaptureGeometry::SetHitAtom(G4LogicalVolume* motherLog,G4ThreeVector point)
{
  //------------------------------ target tube
    
  G4Tubs* captureHitTube = new G4Tubs("captureHitTube",
      m*dCaptureTubeInnerRadius,
      m*dCaptureTubeOuterRadius,
      m*0.0005,
      deg*dCaptureTubeStartAngle,
      deg*dCaptureTubeSpanningAngle);

  G4LogicalVolume* captureHitLog = new G4LogicalVolume(captureHitTube,capMaterial,"captureHitLog");
  G4VPhysicalVolume* captureHitPhys = new G4PVPlacement(0,
             point*m,
             captureHitLog,"captureHit",motherLog,false,0);

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4MultiFunctionalDetector* captureMultiFunDet=(G4MultiFunctionalDetector*)SDman->FindSensitiveDetector(GetName()+"sd");
  //G4MultiFunctionalDetector* captureMultiFunDet=(G4MultiFunctionalDetector*)SDman->FindSensitiveDetector("/PositronSource/Capture/MultiFunDet");
  //GPSurfaceParticleScorer* captureParticleScorer=0;
  if(captureMultiFunDet==NULL)
  {
    //G4MultiFunctionalDetector* captureMultiFunDet = new G4MultiFunctionalDetector("/PositronSource/Capture/MultiFunDet");
    G4MultiFunctionalDetector* captureMultiFunDet = new G4MultiFunctionalDetector(GetName()+"sd");
    GPSurfaceParticleScorer* captureParticleScorer = new GPSurfaceParticleScorer("CaptureParticleScorerZPlus",1,2);
    captureMultiFunDet->RegisterPrimitive(captureParticleScorer);
    SDman->AddNewDetector(captureMultiFunDet);
  }
  
  captureHitLog->SetSensitiveDetector(captureMultiFunDet); 
  

  return captureHitPhys;
}


void GPCaptureGeometry::Print(std::ofstream& fstOutput)
{
  fstOutput
   <<"\nCapture structure:";

  fstOutput
    <<"\nCapture type, Tubs construct."
    <<"\nRadius of Capture: "<<dCaptureTubeOuterRadius*m/mm<<" mm"
    <<"\nLength of Capture: "<<dCaptureTubeLength*m/mm<<" mm"
    <<"\nStep Limit Flag of Capture: "<<iCaptureLimitStepFlag
    <<"\nStep Limit of Capture: "<<dCaptureLimitStepMax*m/mm<<" mm"
    <<"\nHit Flag of Capture: "<<iCaptureHitFlag
    <<G4endl;
  
}
void GPCaptureGeometry::SetLithiumLens(G4LogicalVolume* mother, G4ThreeVector vPosition, G4double dLength,G4double dOuterRadius,G4double dInnerRadius, G4double dStartAngle, G4double dSpanningAngle )
{

    //Lithium lens
    //
  G4double dLithiumTubeInnerRadius=dInnerRadius;
  G4double dLithiumTubeOuterRadius=dOuterRadius;
  G4double dLithiumTubeLength=dLength;
  G4double dLithiumTubeStartAngle=dStartAngle;
  G4double dLithiumTubeSpanningAngle=dSpanningAngle;

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
      	     vPosition*m,
             lithiumLog,"lithium",mother,false,0);

  G4VisAttributes* lithiumLogVisAtt= new G4VisAttributes(G4Colour(0.5,0.5,0.5,0.3));
  lithiumLogVisAtt->SetVisibility(true);
  lithiumLogVisAtt->SetForceSolid(true);
  lithiumLog->SetVisAttributes(lithiumLogVisAtt);

}
