// $Id: GPCrystalGeometry.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//
#include "Crystal.h"
#include "RunParameters.h"

#include "GPCrystalGeometry.hh"
#include "GPCrystalManager.hh"
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
GPCrystalGeometry::GPCrystalGeometry()
{
  cryMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");
  spaceMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

    dCrystalTubeInnerRadius = 0;
    dCrystalTubeOuterRadius = 20e-3;
    dCrystalTubeLength = 1.0e-3;
    dCrystalTubeStartAngle = 0;
    dCrystalTubeSpanningAngle = 360;
    iCrystalLimitStepFlag = 0;
    dCrystalLimitStepMax = 0.002;
    iCrystalHitFlag = 0;

}

GPCrystalGeometry::~GPCrystalGeometry()
{
  GPCrystalManager::GetInstance()->Clear();
}

void GPCrystalGeometry::Init()
{
}
G4VPhysicalVolume* GPCrystalGeometry::Construct(G4LogicalVolume* motherLog,G4ThreeVector point)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPCrystalGeometry::Construct(G4LogicalVolume,G4ThreeVector)"<<G4endl;
#endif

  vecPosition = point;
  Init();

  //------------------------------ crystal tube

  crystalTube = new G4Tubs("crystalTube",
      m*dCrystalTubeInnerRadius,
      m*dCrystalTubeOuterRadius,
      m*dCrystalTubeLength/2.0,
      deg*dCrystalTubeStartAngle,
      deg*dCrystalTubeSpanningAngle);

  crystalLog = new G4LogicalVolume(crystalTube,cryMaterial,"crystalLog");
  crystalPhys = new G4PVPlacement(0,
             vecPosition*m,
             crystalLog,"crystal",motherLog,false,0);

  crystalLog->SetFieldManager(GPFieldSetup::GetGPFieldSetup()->GetLocalFieldManager("crystal"),true);

  if(iCrystalLimitStepFlag)
    crystalLog->SetUserLimits(new G4UserLimits(dCrystalLimitStepMax*m));

  G4VisAttributes* crystalLogVisAtt= new G4VisAttributes(G4Colour(1.0,0.6,0,0.3));
  crystalLogVisAtt->SetVisibility(true);
  crystalLogVisAtt->SetForceSolid(true);
  crystalLog->SetVisAttributes(crystalLogVisAtt);

  if(iCrystalHitFlag==1)
    SetHit(crystalLog);

  ConstructCrystal();
  return crystalPhys;

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPCrystalGeometry::Construct(G4LogicalVolume,G4ThreeVector)"<<G4endl;
#endif
}

void GPCrystalGeometry::ConstructCrystal()
{
  GPCrystalManager::GetInstance()->Clear();
  crysW = new Crystal("W",111);
  runPara = new RunParameters(*crysW);
  runPara->setZexit(dCrystalTubeLength*m/angstrom); //?? G4Fot unit of length is angstrom;
  GPCrystalManager::GetInstance()->SetCrystal(crysW);
  GPCrystalManager::GetInstance()->SetRunPara(runPara);
}

void GPCrystalGeometry::Print()
{
  G4cout
   <<"\n----------------------------Crystal construct---------------------------"
   <<G4endl;
  G4cout
    <<"Crystal type: Tubs construct."
    <<"\nRadius of Crystal: "<<dCrystalTubeOuterRadius*m/mm<<" mm"
    <<"\nLength of Crystal: "<<dCrystalTubeLength*m/mm<<" mm"
    <<"\nStep Limit Flag of Crystal: "<<iCrystalLimitStepFlag
    <<"\nStep Limit of Crystal: "<<dCrystalLimitStepMax*m/mm<<" mm"
    <<"\nHit Flag of Crystal: "<<iCrystalHitFlag
    <<G4endl;
  
   
  G4cout
   <<"\n------------------------------------------------------------------------"
   <<G4endl;

}

void GPCrystalGeometry::SetParameter(std::string str,std::string strGlobal)
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
    dCrystalTubeInnerRadius = dValueNew;
    else if(key=="or")
    dCrystalTubeOuterRadius = dValueNew;
    else if(key=="l")
    dCrystalTubeLength = dValueNew;
    else if(key=="sa")
    dCrystalTubeStartAngle = dValueNew;
    else if(key=="ea")
    dCrystalTubeSpanningAngle = dValueNew;
    else if(key=="hit.flag")
    iCrystalHitFlag = dValueNew;
    else if(key=="limit.step.max")
    dCrystalLimitStepMax = dValueNew;
    else if(key=="limit.step.flag")
    iCrystalLimitStepFlag = dValueNew;
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

G4double GPCrystalGeometry::GetParameter(std::string name) const
{
    if(name=="ir")
    return dCrystalTubeInnerRadius;
    else if(name=="or")
    return dCrystalTubeOuterRadius;
    else if(name=="l")
    return dCrystalTubeLength;
    else if(name=="sa")
    return dCrystalTubeStartAngle;
    else if(name=="ea")
    return dCrystalTubeSpanningAngle;
    else if(name=="hit.flag")
    return iCrystalHitFlag;
    else if(name=="limit.step.max")
    return dCrystalLimitStepMax;
    else if(name=="limit.step.flag")
    return iCrystalLimitStepFlag;
    else if(name=="coordG4FotToGeant4.z")
    return (vecPosition.z()-dCrystalTubeLength/2);
    else if(name=="border.z.positive")
    return (vecPosition.z()+dCrystalTubeLength/2);

    else
    {
      std::cout<<"key does not exist.\n"<<std::endl;
      return -1;
    }
}

//*/
G4VPhysicalVolume* GPCrystalGeometry::SetHit(G4LogicalVolume* motherLog)
{
  G4ThreeVector point(0,0,dCrystalTubeLength/2-0.0005);
  return SetHitAtom(motherLog,point);
}

G4VPhysicalVolume* GPCrystalGeometry::SetHitAtom(G4LogicalVolume* motherLog,G4ThreeVector point)
{
  //------------------------------ target tube
    
  G4Tubs* crystalHitTube = new G4Tubs("crystalHitTube",
      m*dCrystalTubeInnerRadius,
      m*dCrystalTubeOuterRadius,
      m*0.0005,
      deg*dCrystalTubeStartAngle,
      deg*dCrystalTubeSpanningAngle);

  G4LogicalVolume* crystalHitLog = new G4LogicalVolume(crystalHitTube,cryMaterial,"crystalHitLog");
  G4VPhysicalVolume* crystalHitPhys = new G4PVPlacement(0,
             point*m,
             crystalHitLog,"crystalHit",motherLog,false,0);

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4MultiFunctionalDetector* crystalMultiFunDet=(G4MultiFunctionalDetector*)SDman->FindSensitiveDetector("/PositronSource/Crystal/MultiFunDet");
  if(crystalMultiFunDet==NULL)
  {
    G4MultiFunctionalDetector* crystalMultiFunDet = new G4MultiFunctionalDetector("/PositronSource/Crystal/MultiFunDet");
    GPSurfaceParticleScorer* crystalParticleScorer = new GPSurfaceParticleScorer("CrystalParticleScorerZPlus",1,2);
    crystalMultiFunDet->RegisterPrimitive(crystalParticleScorer);
    SDman->AddNewDetector(crystalMultiFunDet);
  }
  crystalHitLog->SetSensitiveDetector(crystalMultiFunDet); 
  

  return crystalHitPhys;
}


void GPCrystalGeometry::SetMaterial (G4String strMa)
{
  cryMaterial = G4NistManager::Instance()->FindOrBuildMaterial(strMa);
}

void GPCrystalGeometry::Print(std::ofstream& fstOutput)
{
  fstOutput
   <<"\nCrystal structure:";

  fstOutput
    <<"\nCrystal type, Tubs construct."
    <<"\nRadius of Crystal: "<<dCrystalTubeOuterRadius*m/mm<<" mm"
    <<"\nLength of Crystal: "<<dCrystalTubeLength*m/mm<<" mm"
    <<"\nStep Limit Flag of Crystal: "<<iCrystalLimitStepFlag
    <<"\nStep Limit of Crystal: "<<dCrystalLimitStepMax*m/mm<<" mm"
    <<"\nHit Flag of Crystal: "<<iCrystalHitFlag
    <<G4endl;
  
}
