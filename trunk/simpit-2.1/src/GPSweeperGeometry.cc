// $Id: GPSweeperGeometry.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//

#include "GPSweeperGeometry.hh"
#include "GPSurfaceParticleScorer.hh"
#include "GPFieldSetup.hh"

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
GPSweeperGeometry::GPSweeperGeometry()
{
  swpMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
  spaceMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

    dSweeperTubeInnerRadius = 0;
    dSweeperTubeOuterRadius = 20e-3;
    dSweeperTubeLength = 2.0;
    dSweeperTubeStartAngle = 0;
    dSweeperTubeSpanningAngle = 360;
    iSweeperLimitStepFlag = 0;
    dSweeperLimitStepMax = 0.002;
    iSweeperHitFlag = 1;

}

GPSweeperGeometry::~GPSweeperGeometry()
{
}

void GPSweeperGeometry::Init()
{
}
G4VPhysicalVolume* GPSweeperGeometry::Construct(G4LogicalVolume* motherLog,G4ThreeVector point)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPSweeperGeometry::Construct(G4LogicalVolume,G4ThreeVector)"<<G4endl;
#endif

  vecPosition = point;
  Init();

  //------------------------------ sweeper tube

  sweeperTube = new G4Tubs("sweeperTube",
      m*dSweeperTubeInnerRadius,
      m*dSweeperTubeOuterRadius,
      m*dSweeperTubeLength/2.0,
      deg*dSweeperTubeStartAngle,
      deg*dSweeperTubeSpanningAngle);

  sweeperLog = new G4LogicalVolume(sweeperTube,swpMaterial,"sweeperLog");
  sweeperPhys = new G4PVPlacement(0,
             vecPosition*m,
             sweeperLog,"sweeper",motherLog,false,0);

  sweeperLog->SetFieldManager(GPFieldSetup::GetGPFieldSetup()->GetLocalFieldManager("sweeper"),true);

  if(iSweeperLimitStepFlag)
    sweeperLog->SetUserLimits(new G4UserLimits(dSweeperLimitStepMax*m));

  G4VisAttributes* sweeperLogVisAtt= new G4VisAttributes(G4Colour(1.0,0.3,0,0.3));
  sweeperLogVisAtt->SetVisibility(true);
  sweeperLogVisAtt->SetForceSolid(true);
  sweeperLog->SetVisAttributes(sweeperLogVisAtt);

  SetHit(sweeperLog);

  return sweeperPhys;

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPSweeperGeometry::Construct(G4LogicalVolume,G4ThreeVector)"<<G4endl;
#endif
}


void GPSweeperGeometry::Print()
{
  G4cout
   <<"\n----------------------------Sweeper construct---------------------------"
   <<G4endl;
  G4cout
    <<"Sweeper type: Tubs construct."
    <<"\nRadius of Sweeper: "<<dSweeperTubeOuterRadius*m/mm<<" mm"
    <<"\nLength of Sweeper: "<<dSweeperTubeLength*m/mm<<" mm"
    <<"\nStep Limit Flag of Sweeper: "<<iSweeperLimitStepFlag
    <<"\nStep Limit of Sweeper: "<<dSweeperLimitStepMax*m/mm<<" mm"
    <<"\nHit Flag of Sweeper: "<<iSweeperHitFlag
    <<G4endl;
  
   
  G4cout
   <<"\n------------------------------------------------------------------------"
   <<G4endl;

}

void GPSweeperGeometry::SetParameter(std::string str,std::string strGlobal)
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

    if(key=="ir")
    dSweeperTubeInnerRadius = dValueNew;
    else if(key=="or")
    dSweeperTubeOuterRadius = dValueNew;
    else if(key=="l")
    dSweeperTubeLength = dValueNew;
    else if(key=="sa")
    dSweeperTubeStartAngle = dValueNew;
    else if(key=="ea")
    dSweeperTubeSpanningAngle = dValueNew;
    else if(key=="hit.flag")
    iSweeperHitFlag = dValueNew;
    else if(key=="limit.step.max")
    dSweeperLimitStepMax = dValueNew;
    else if(key=="limit.step.flag")
    iSweeperLimitStepFlag = dValueNew;
    else 
    {
  	std::cout<<"the key is not exist."<<std::endl;
     	return;
    }

    Init();
    ss.clear();
    ss.str(strGlobal);
    ss>>key;
    std::cout<<"Set "<<key<<" to "<< dValueOrg<<" "<<unit<<std::endl;
}

G4double GPSweeperGeometry::GetParameter(std::string name) const
{
    if(name=="ir")
    return dSweeperTubeInnerRadius;
    else if(name=="or")
    return dSweeperTubeOuterRadius;
    else if(name=="l")
    return dSweeperTubeLength;
    else if(name=="sa")
    return dSweeperTubeStartAngle;
    else if(name=="ea")
    return dSweeperTubeSpanningAngle;
    else if(name=="hit.flag")
    return iSweeperHitFlag;
    else if(name=="limit.step.max")
    return dSweeperLimitStepMax;
    else if(name=="limit.step.flag")
    return iSweeperLimitStepFlag;

    else
    {
      std::cout<<"key does not exist.\n"<<std::endl;
      return -1;
    }
}

//*/
G4VPhysicalVolume* GPSweeperGeometry::SetHit(G4LogicalVolume* motherLog)
{
  G4ThreeVector point(0,0,dSweeperTubeLength/2-0.0005);
  return SetHitAtom(motherLog,point);
}

G4VPhysicalVolume* GPSweeperGeometry::SetHitAtom(G4LogicalVolume* motherLog,G4ThreeVector point)
{
  //------------------------------ target tube
    
  G4Tubs* sweeperHitTube = new G4Tubs("sweeperHitTube",
      m*dSweeperTubeInnerRadius,
      m*dSweeperTubeOuterRadius,
      m*0.0005,
      deg*dSweeperTubeStartAngle,
      deg*dSweeperTubeSpanningAngle);

  G4LogicalVolume* sweeperHitLog = new G4LogicalVolume(sweeperHitTube,swpMaterial,"sweeperHitLog");
  G4VPhysicalVolume* sweeperHitPhys = new G4PVPlacement(0,
             point*m,
             sweeperHitLog,"sweeperHit",motherLog,false,0);

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4MultiFunctionalDetector* sweeperMultiFunDet=(G4MultiFunctionalDetector*)SDman->FindSensitiveDetector("/PositronSource/Sweeper/MultiFunDet");
  if(sweeperMultiFunDet==NULL)
  {
    G4MultiFunctionalDetector* sweeperMultiFunDet = new G4MultiFunctionalDetector("/PositronSource/Sweeper/MultiFunDet");
    GPSurfaceParticleScorer* sweeperParticleScorer = new GPSurfaceParticleScorer("SweeperParticleScorerZPlus",1,2);
    sweeperMultiFunDet->RegisterPrimitive(sweeperParticleScorer);
    SDman->AddNewDetector(sweeperMultiFunDet);
  }
  sweeperHitLog->SetSensitiveDetector(sweeperMultiFunDet); 
  

  return sweeperHitPhys;
}


void GPSweeperGeometry::SetMaterial (G4String strMa)
{
  swpMaterial = G4NistManager::Instance()->FindOrBuildMaterial(strMa);
}

void GPSweeperGeometry::Print(std::ofstream& fstOutput)
{
  fstOutput
   <<"\nSweeper structure:";

  fstOutput
    <<"\nSweeper type, Tubs construct."
    <<"\nRadius of Sweeper: "<<dSweeperTubeOuterRadius*m/mm<<" mm"
    <<"\nLength of Sweeper: "<<dSweeperTubeLength*m/mm<<" mm"
    <<"\nStep Limit Flag of Sweeper: "<<iSweeperLimitStepFlag
    <<"\nStep Limit of Sweeper: "<<dSweeperLimitStepMax*m/mm<<" mm"
    <<"\nHit Flag of Sweeper: "<<iSweeperHitFlag
    <<G4endl;
  
}
