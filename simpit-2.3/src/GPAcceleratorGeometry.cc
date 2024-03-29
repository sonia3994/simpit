// $Id: GPAcceleratorGeometry.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//

#include "GPAcceleratorGeometry.hh"
#include "GPFieldManagerPool.hh"
#include "GPSurfaceParticleScorer.hh"
#include "GPFieldSetup.hh"

#include "GPGeometryStore.hh"
#include "GPObject.hh"

#include "G4FieldManager.hh"
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
GPAcceleratorGeometry::GPAcceleratorGeometry(std::string sName, std::string sFatherName)
{
  SetActive(1);
  SetName(sName);
  SetFatherName(sFatherName);
  GPGeometryStore::GetInstance()->AddGeometry(GetName(),this);

  accMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  spaceMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  dAcceleratorTubeInnerRadius = 0;
  dAcceleratorTubeOuterRadius = 20e-3;
  dAcceleratorTubeLength = 1.0;
  dAcceleratorTubeStartAngle = 0;
  dAcceleratorTubeSpanningAngle = 360;
  iAcceleratorLimitStepFlag = 0;
  dAcceleratorLimitStepMax = 0.002;
  iAcceleratorHitFlag = 1;

}
GPAcceleratorGeometry::GPAcceleratorGeometry()
{
  accMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  spaceMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

    dAcceleratorTubeInnerRadius = 0;
    dAcceleratorTubeOuterRadius = 20e-3;
    dAcceleratorTubeLength = 1.0;
    dAcceleratorTubeStartAngle = 0;
    dAcceleratorTubeSpanningAngle = 360;
    iAcceleratorLimitStepFlag = 0;
    dAcceleratorLimitStepMax = 0.002;
    iAcceleratorHitFlag = 1;

}

GPAcceleratorGeometry::~GPAcceleratorGeometry()
{
 //GPObject* object = (GPObject*)GPFieldSetup::GetGPFieldSetup()->FindFieldManagerPool(GetName()+"field/");
 //if(object)
    //((GPObject*)fieldManagerPool)->SetActive(0);
    fieldManagerPool->SetActive(0);

  GPGeometryStore::GetInstance()->EraseItem(GetName());
}

void GPAcceleratorGeometry::Init()
{
  dGlobalLength=dAcceleratorTubeLength;
}
G4VPhysicalVolume* GPAcceleratorGeometry::Construct(G4LogicalVolume* motherLog)
{
  return Construct(motherLog,vPosition);
}
G4VPhysicalVolume* GPAcceleratorGeometry::Construct(G4LogicalVolume* motherLog,G4ThreeVector point)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPAcceleratorGeometry::Construct(G4LogicalVolume,G4ThreeVector)"<<G4endl;
#endif

  vPosition = point;
  Init();

  //------------------------------ accelerator tube

  acceleratorTube = new G4Tubs("acceleratorTube",
      m*dAcceleratorTubeInnerRadius,
      m*dAcceleratorTubeOuterRadius,
      m*dAcceleratorTubeLength/2.0,
      deg*dAcceleratorTubeStartAngle,
      deg*dAcceleratorTubeSpanningAngle);

  acceleratorLog = new G4LogicalVolume(acceleratorTube,accMaterial,"acceleratorLog");
  acceleratorPhys = new G4PVPlacement(0,
             point*m,
             acceleratorLog,"accelerator",motherLog,false,0);

  fieldManagerPool = GPFieldSetup::GetGPFieldSetup()->FindFieldManagerPool(GetName()+"field/");
  acceleratorLog->SetFieldManager(fieldManagerPool->GetFieldManager(),true);

  if(iAcceleratorLimitStepFlag)
    acceleratorLog->SetUserLimits(new G4UserLimits(dAcceleratorLimitStepMax*m));

  G4VisAttributes* acceleratorLogVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,0,0.3));
  acceleratorLogVisAtt->SetVisibility(true);
  acceleratorLogVisAtt->SetForceSolid(true);
  acceleratorLog->SetVisAttributes(acceleratorLogVisAtt);

  if(iAcceleratorHitFlag==1)
    SetHit(acceleratorLog);

  return acceleratorPhys;

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPAcceleratorGeometry::Construct(G4LogicalVolume,G4ThreeVector)"<<G4endl;
#endif
}


void GPAcceleratorGeometry::Print()
{
  G4cout
   <<"\n----------------------------Accelerator construct---------------------------"
   <<G4endl;
  G4cout
    <<"Accelerator type: Tubs construct."
    <<"\nRadius of Accelerator: "<<dAcceleratorTubeOuterRadius*m/mm<<" mm"
    <<"\nLength of Accelerator: "<<dAcceleratorTubeLength*m/mm<<" mm"
    <<"\nStep Limit Flag of Accelerator: "<<iAcceleratorLimitStepFlag
    <<"\nStep Limit of Accelerator: "<<dAcceleratorLimitStepMax*m/mm<<" mm"
    <<"\nHit Flag of Accelerator: "<<iAcceleratorHitFlag
    <<G4endl;
  
   
  G4cout
   <<"\n------------------------------------------------------------------------"
   <<G4endl;

}

void GPAcceleratorGeometry::SetParameter(std::string str,std::string strGlobal)
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
    dAcceleratorTubeInnerRadius = dValueNew;
    else if(sKey=="or")
    dAcceleratorTubeOuterRadius = dValueNew;
    else if(sKey=="l")
    dAcceleratorTubeLength = dValueNew;
    else if(sKey=="sa")
    dAcceleratorTubeStartAngle = dValueNew;
    else if(sKey=="ea")
    dAcceleratorTubeSpanningAngle = dValueNew;
    else if(sKey=="hit.flag")
    iAcceleratorHitFlag = dValueNew;
    else if(sKey=="limit.step.max")
    dAcceleratorLimitStepMax = dValueNew;
    else if(sKey=="limit.step.flag")
    iAcceleratorLimitStepFlag = dValueNew;
    else 
    {
  	std::cout<<"the Key is not exist."<<std::endl;
     	return;
    }

    Init();
    std::cout<<GetName()<<": Set "<<sKey<<": "<< dValueOrg<<" "<<sUnit<<std::endl;
}

G4double GPAcceleratorGeometry::GetParameter(std::string sKey, std::string sGlobal) const
{
    if(sKey=="ir")
    return dAcceleratorTubeInnerRadius;
    else if(sKey=="or")
    return dAcceleratorTubeOuterRadius;
    else if(sKey=="l")
    return dAcceleratorTubeLength;
    else if(sKey=="length")
    return dGlobalLength;
    else if(sKey=="sa")
    return dAcceleratorTubeStartAngle;
    else if(sKey=="ea")
    return dAcceleratorTubeSpanningAngle;
    else if(sKey=="hit.flag")
    return iAcceleratorHitFlag;
    else if(sKey=="limit.step.max")
    return dAcceleratorLimitStepMax;
    else if(sKey=="limit.step.flag")
    return iAcceleratorLimitStepFlag;

    else
    {
      std::cout<<((GPObject*) this)->GetName()<<": "+sKey+": Key does not exist."<<std::endl;
      return -1;
    }
}

//*/
G4VPhysicalVolume* GPAcceleratorGeometry::SetHit(G4LogicalVolume* motherLog)
{
  G4ThreeVector point(0,0,dAcceleratorTubeLength/2-0.0005);
  return SetHitAtom(motherLog,point);
}

G4VPhysicalVolume* GPAcceleratorGeometry::SetHitAtom(G4LogicalVolume* motherLog,G4ThreeVector point)
{
  //------------------------------ target tube
    
  G4Tubs* acceleratorHitTube = new G4Tubs("acceleratorHitTube",
      m*dAcceleratorTubeInnerRadius,
      m*dAcceleratorTubeOuterRadius,
      m*0.0005,
      deg*dAcceleratorTubeStartAngle,
      deg*dAcceleratorTubeSpanningAngle);

  G4LogicalVolume* acceleratorHitLog = new G4LogicalVolume(acceleratorHitTube,accMaterial,"acceleratorHitLog");
  G4VPhysicalVolume* acceleratorHitPhys = new G4PVPlacement(0,
             point*m,
             acceleratorHitLog,"acceleratorHit",motherLog,false,0);

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4MultiFunctionalDetector* acceleratorMultiFunDet=(G4MultiFunctionalDetector*)SDman->FindSensitiveDetector(GetName()+"sd");
  //G4MultiFunctionalDetector* acceleratorMultiFunDet=(G4MultiFunctionalDetector*)SDman->FindSensitiveDetector("/PositronSource/Accelerator/MultiFunDet");
  if(acceleratorMultiFunDet==NULL)
  {
    G4MultiFunctionalDetector* acceleratorMultiFunDet = new G4MultiFunctionalDetector(GetName()+"sd");
    //G4MultiFunctionalDetector* acceleratorMultiFunDet = new G4MultiFunctionalDetector("/PositronSource/Accelerator/MultiFunDet");
    GPSurfaceParticleScorer* acceleratorParticleScorer = new GPSurfaceParticleScorer("AcceleratorParticleScorerZPlus",1,2);
    acceleratorMultiFunDet->RegisterPrimitive(acceleratorParticleScorer);
    SDman->AddNewDetector(acceleratorMultiFunDet);
  }
  acceleratorHitLog->SetSensitiveDetector(acceleratorMultiFunDet); 
  

  return acceleratorHitPhys;
}


void GPAcceleratorGeometry::Print(std::ofstream& fstOutput)
{
  fstOutput
   <<"\nAccelerator structure:";

  fstOutput
    <<"\nAccelerator type, Tubs construct."
    <<"\nRadius of Accelerator: "<<dAcceleratorTubeOuterRadius*m/mm<<" mm"
    <<"\nLength of Accelerator: "<<dAcceleratorTubeLength*m/mm<<" mm"
    <<"\nStep Limit Flag of Accelerator: "<<iAcceleratorLimitStepFlag
    <<"\nStep Limit of Accelerator: "<<dAcceleratorLimitStepMax*m/mm<<" mm"
    <<"\nHit Flag of Accelerator: "<<iAcceleratorHitFlag
    <<G4endl;
  
}
