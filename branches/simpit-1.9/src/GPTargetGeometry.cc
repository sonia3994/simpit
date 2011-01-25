// $Id: GPTargetGeometry.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//

#include "GPTargetGeometry.hh"
#include "GPTargetROGeometry.hh"
#include "GPTargetROGeometryTubs.hh"
#include "GPTargetSD.hh"
#include "GPSurfaceParticleScorer.hh"

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

#include "globals.hh"

#include <sstream>
GPTargetGeometry::GPTargetGeometry()
{
  targetMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");
  spaceMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

    dTargetHexagonalSphereRadius=0.02;
    dTargetSolidX = 0.025;
    dTargetSolidY = 0.025;
    dTargetSolidZ = 0.01;
    dTargetHitL = 0.001;
    iTargetGranularZNumber = 3;
    iTargetGranularFlag = 1;
    
    dTargetGlobalSolidX = dTargetSolidX+0.01;
    dTargetGlobalSolidY = dTargetSolidY+0.01;
    dTargetGlobalSolidZ = dTargetSolidZ+dTargetHitL;
    
  dTargetSDSolidZ=dTargetSolidZ;
  dTargetSDSolidR=dTargetSolidX/2;
  dTargetSDSolidAngle=360;

  dTargetSDCellZ=0.001;
  dTargetSDCellR=0.001;
  dTargetSDCellPhi = 360;

  vecEddDim = std::vector<G4int>(3,1);
}

GPTargetGeometry::~GPTargetGeometry()
{
}

G4VPhysicalVolume* GPTargetGeometry::Construct(G4LogicalVolume* motherLog,G4ThreeVector point)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetGeometry::Construct(G4LogicalVolume,G4ThreeVector)"<<G4endl;
#endif
  G4Box* targetSolid= new G4Box("targetSolid",
      m*dTargetGlobalSolidX/2,
      m*dTargetGlobalSolidY/2,
      m*dTargetGlobalSolidZ/2);

  G4LogicalVolume* targetLog = new G4LogicalVolume(targetSolid,spaceMaterial,"targetLog");
  G4VPhysicalVolume* targetPhys = new G4PVPlacement(0,
      point,
      targetLog,
      "targetPhys",
      motherLog,false,0);

  if(iTargetGranularFlag==1)
  {
    GranularHexagonal(targetLog,G4ThreeVector(0,0,-dTargetHitL/2));
  }
  else
  {
  }
  
  SetupTargetHit(targetLog,G4ThreeVector(0,0,dTargetSolidZ/2));
  return targetPhys;

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetGeometry::Construct(G4LogicalVolume,G4ThreeVector)"<<G4endl;
#endif
}

void GPTargetGeometry::GranularHexagonalInit()
{
  dTargetHexagonalSphereRadius = dTargetSolidZ/(2+(iTargetGranularZNumber-0.5)*4*sqrt(6)/3);
  dWidthX = 4*dTargetHexagonalSphereRadius;
  dWidthY = 2*sqrt(3.0)*dTargetHexagonalSphereRadius;
  dWidthZ = 4*sqrt(6.0)*dTargetHexagonalSphereRadius/3;

  iTargetGranularXNumber = (dTargetSolidX-2*dTargetHexagonalSphereRadius+0.5*dWidthX)/dWidthX;
  iTargetGranularYNumber = (dTargetSolidY-2*dTargetHexagonalSphereRadius+0.5*dWidthY)/dWidthY;
}

void GPTargetGeometry::Print()
{
  G4cout<<"-----------Hexagonal construct------------\n"
   <<"\nRadius of sphere: "<<dTargetHexagonalSphereRadius
   <<"\nNumber of X cell: "<<iTargetGranularXNumber
   <<"\nNumber of Y cell: "<<iTargetGranularYNumber
   <<"\nNumber of Z cell: "<<iTargetGranularZNumber
   <<"\nZ axis Cell width: "<<dWidthZ/mm<< " mm"
   <<"\nActual thickness: "<<dTargetSolidZ/mm<<" mm"
   <<G4endl;

}

G4VPhysicalVolume* GPTargetGeometry::GranularHexagonal(G4LogicalVolume* motherLog,G4ThreeVector point)
{
  GranularHexagonalInit();

  G4Box* hexagonalSolid= new G4Box("hexagonalSolid",dTargetSolidX*m/2,dTargetSolidY*m/2,dTargetSolidZ*m/2);
  G4LogicalVolume* hexagonalLog = new G4LogicalVolume(hexagonalSolid,spaceMaterial,"hexagonalLog",0,0,0);
  G4VPhysicalVolume* hexagonalPhy = new G4PVPlacement(0,
      point*m,
      hexagonalLog,
      "HexagonalPhy",
      motherLog,
      false,
      0);
  for(G4int k=0;k<iTargetGranularZNumber;k++)
  {
    for(G4int j=0;j<iTargetGranularYNumber;j++)
    {
      for(G4int i=0;i<iTargetGranularXNumber;i++)
      {
	G4ThreeVector vecCenter=G4ThreeVector(i*dWidthX+0.25*dWidthX+dTargetHexagonalSphereRadius-0.5*dTargetSolidX,
	    j*dWidthY+0.25*dWidthY+dTargetHexagonalSphereRadius-0.5*dTargetSolidY,
	    k*dWidthZ+0.25*dWidthZ+dTargetHexagonalSphereRadius-0.5*dTargetSolidZ);
	GranularHexagonalCell(hexagonalLog,vecCenter,i+i*j+i*j*k);
      }
    }
  
  }

  SetupTargetSD(hexagonalLog);
  return hexagonalPhy;

}

void GPTargetGeometry::GranularHexagonalCell(G4LogicalVolume* motherLog,G4ThreeVector vecCenter,long iIndex)
{
  G4ThreeVector sphPoint;
  std::stringstream sstStr;
  std::string strSuffix;
  std::string strSolid;
  std::string strLog;
  std::string strPhy;

  sstStr<<iIndex;
  sstStr>>strSuffix;
  
  strSolid="sphOrbA_"+strSuffix;
  strLog="sphLogA_"+strSuffix;
  strPhy="sphPhyA_"+strSuffix;
  sphPoint= G4ThreeVector(-2*dTargetHexagonalSphereRadius*m,-2*sqrt(3.0)*dTargetHexagonalSphereRadius/3*m,-sqrt(6.0)*dTargetHexagonalSphereRadius/3*m);
  sphPoint=sphPoint+vecCenter*m;
  G4Orb* sphOrbA = new G4Orb(strSolid,dTargetHexagonalSphereRadius*m);
  G4LogicalVolume* sphLogA = new G4LogicalVolume(sphOrbA,
      targetMaterial,
      strLog,
      0,0,0);
  new G4PVPlacement(0,
      sphPoint,
      sphLogA,
      strPhy,
      motherLog,
      false,
      0);

  strSolid="sphOrbB_"+strSuffix;
  strLog="sphLogB_"+strSuffix;
  strPhy="sphPhyB_"+strSuffix;
  sphPoint= G4ThreeVector(0,-2.0*sqrt(3.0)*dTargetHexagonalSphereRadius/3*m,-sqrt(6.0)*dTargetHexagonalSphereRadius/3*m); 
  sphPoint=sphPoint+vecCenter*m;
  G4Orb* sphOrbB = new G4Orb(strSolid,dTargetHexagonalSphereRadius*m);
  G4LogicalVolume* sphLogB = new G4LogicalVolume(sphOrbB,
      targetMaterial,
      strLog,
      0,0,0);
  new G4PVPlacement(0,
      sphPoint,
      sphLogB,
      strPhy,
      motherLog,
      false,
      0);

  strSolid="sphOrbC_"+strSuffix;
  strLog="sphLogC_"+strSuffix;
  strPhy="sphPhyC_"+strSuffix;
  sphPoint= G4ThreeVector(dTargetHexagonalSphereRadius*m,-sqrt(3.0)*dTargetHexagonalSphereRadius/3*m,sqrt(6.0)*dTargetHexagonalSphereRadius/3*m); 
  sphPoint=sphPoint+vecCenter*m;
  G4Orb* sphOrbC = new G4Orb(strSolid,dTargetHexagonalSphereRadius*m);
  G4LogicalVolume* sphLogC = new G4LogicalVolume(sphOrbC,
      targetMaterial,
      strLog,
      0,0,0);
  new G4PVPlacement(0,
      sphPoint,
      sphLogC,
      strPhy,
      motherLog,
      false,
      0);

  strSolid="sphOrbD_"+strSuffix;
  strLog="sphLogD_"+strSuffix;
  strPhy="sphPhyD_"+strSuffix;
  sphPoint= G4ThreeVector(-dTargetHexagonalSphereRadius*m,-sqrt(3.0)*dTargetHexagonalSphereRadius/3*m,sqrt(6.0)*dTargetHexagonalSphereRadius/3*m); 
  sphPoint=sphPoint+vecCenter*m;
  G4Orb* sphOrbD = new G4Orb(strSolid,dTargetHexagonalSphereRadius*m);
  G4LogicalVolume* sphLogD = new G4LogicalVolume(sphOrbD,
      targetMaterial,
      strLog,
      0,0,0);
  new G4PVPlacement(0,
      sphPoint,
      sphLogD,
      strPhy,
      motherLog,
      false,
      0);

  strSolid="sphOrbE_"+strSuffix;
  strLog="sphLogE_"+strSuffix;
  strPhy="sphPhyE_"+strSuffix;
  sphPoint= G4ThreeVector(-dTargetHexagonalSphereRadius*m,sqrt(3.0)*dTargetHexagonalSphereRadius/3*m,-sqrt(6.0)*dTargetHexagonalSphereRadius/3*m); 
  sphPoint=sphPoint+vecCenter*m;
  G4Orb* sphOrbE = new G4Orb(strSolid,dTargetHexagonalSphereRadius*m);
  G4LogicalVolume* sphLogE = new G4LogicalVolume(sphOrbE,
      targetMaterial,
      strLog,
      0,0,0);
  new G4PVPlacement(0,
      sphPoint,
      sphLogE,
      strPhy,
      motherLog,
      false,
      0);

  strSolid="sphOrbF_"+strSuffix;
  strLog="sphLogF_"+strSuffix;
  strPhy="sphPhyF_"+strSuffix;
  sphPoint= G4ThreeVector(dTargetHexagonalSphereRadius*m,sqrt(3.0)*dTargetHexagonalSphereRadius*m/3,-sqrt(6.0)*dTargetHexagonalSphereRadius*m/3); 
  sphPoint=sphPoint+vecCenter*m;
  G4Orb* sphOrbF = new G4Orb(strSolid,dTargetHexagonalSphereRadius*m);
  G4LogicalVolume* sphLogF = new G4LogicalVolume(sphOrbF,
      targetMaterial,
      strLog,
      0,0,0);
  new G4PVPlacement(0,
      sphPoint,
      sphLogF,
      strPhy,
      motherLog,
      false,
      0);

  strSolid="sphOrbG_"+strSuffix;
  strLog="sphLogG_"+strSuffix;
  strPhy="sphPhyG_"+strSuffix;
  sphPoint= G4ThreeVector(2*dTargetHexagonalSphereRadius*m,2*sqrt(3.0)*dTargetHexagonalSphereRadius*m/3,sqrt(6.0)*dTargetHexagonalSphereRadius*m/3); 
  sphPoint=sphPoint+vecCenter*m;
  G4Orb* sphOrbG = new G4Orb(strSolid,dTargetHexagonalSphereRadius*m);
  G4LogicalVolume* sphLogG = new G4LogicalVolume(sphOrbG,
      targetMaterial,
      strLog,
      0,0,0);
  new G4PVPlacement(0,
      sphPoint,
      sphLogG,
      strPhy,
      motherLog,
      false,
      0);

  strSolid="sphOrbH_"+strSuffix;
  strLog="sphLogH_"+strSuffix;
  strPhy="sphPhyH_"+strSuffix;
  sphPoint= G4ThreeVector(0,2*sqrt(3.0)*dTargetHexagonalSphereRadius*m/3,sqrt(6.0)*dTargetHexagonalSphereRadius*m/3); 
  sphPoint=sphPoint+vecCenter*m;
  G4Orb* sphOrbH = new G4Orb(strSolid,dTargetHexagonalSphereRadius*m);
  G4LogicalVolume* sphLogH = new G4LogicalVolume(sphOrbH,
      targetMaterial,
      strLog,
      0,0,0);
  new G4PVPlacement(0,
      sphPoint,
      sphLogH,
      strPhy,
      motherLog,
      false,
      0);


  G4VisAttributes* sphereLogVisAtt= new G4VisAttributes(G4Colour(1.0,0,1.0,0.3));
  sphereLogVisAtt->SetVisibility(true);
  sphereLogVisAtt->SetForceSolid(true);
  sphLogA->SetVisAttributes(sphereLogVisAtt);
  sphLogB->SetVisAttributes(sphereLogVisAtt);
  sphLogC->SetVisAttributes(sphereLogVisAtt);
  sphLogD->SetVisAttributes(sphereLogVisAtt);
  sphLogE->SetVisAttributes(sphereLogVisAtt);
  sphLogF->SetVisAttributes(sphereLogVisAtt);
  sphLogG->SetVisAttributes(sphereLogVisAtt);
  sphLogH->SetVisAttributes(sphereLogVisAtt);


}

void GPTargetGeometry::SetDetectorSize(std::string str,std::string strGlobal)
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

    if(key=="x")
    dTargetSolidX = dValueNew;
    else if(key=="y")
    dTargetSolidY = dValueNew;
    else if(key=="z")
    {
      dTargetSolidZ = dValueNew;
      dTargetGlobalSolidZ = dTargetSolidZ+dTargetHitL;
    }
    else if(key=="granular.flag")
    iTargetGranularFlag = dValueNew;
    else if(key=="granular.radius")
    dTargetHexagonalSphereRadius = dValueNew;
    else if(key=="granular.z.number")
    iTargetGranularZNumber = dValueNew;

     else 
     {
  	std::cout<<"the key is not exist."<<std::endl;
     	return;
     }

     ss<<strGlobal;
     ss>>key;
     std::cout<<"Set "<<key<<" to "<< dValueOrg<<" "<<unit<<std::endl;
}

G4double GPTargetGeometry::GetDetectorSize(std::string name) const
{
    if(name=="x")
    return dTargetSolidX;
    else if(name=="gz")
    return dTargetGlobalSolidZ;
    else if(name=="y")
    return dTargetSolidY;
    else if(name=="z")
    return dTargetSolidZ;

    else
    {
      std::cout<<"key does not exist.\n"<<std::endl;
      return -1;
    }
}

//*/
void GPTargetGeometry::SetupTargetSD(G4LogicalVolume* logicalVolume)
{
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String targetSDName="/PositronSource/Target/EddSD";
  G4String targetROName="targetROGeometry";

  vecEddDim[0]=(ceil(dTargetSDSolidZ/dTargetSDCellZ));
  vecEddDim[1]=(ceil(dTargetSDSolidR/dTargetSDCellR));
  vecEddDim[2]=(ceil(dTargetSDSolidAngle/dTargetSDCellPhi));

  GPTargetSD* targetSD = (GPTargetSD*)SDman->FindSensitiveDetector(targetSDName);
  if(targetSD)
  {
    targetSD->SetEddDim(vecEddDim);  
  }                                                                  
  else
  {
    targetSD=new GPTargetSD(targetSDName,vecEddDim);
  }

  GPTargetROGeometryTubs* targetROTubs = (GPTargetROGeometryTubs*)targetSD->GetROgeometry(); 
  if(targetROTubs)
  {
    delete targetROTubs;
  }
  targetROTubs=new GPTargetROGeometryTubs(targetROName,
      0,
      dTargetSolidX/2,
      dTargetSolidZ,
      vecEddDim);

  targetROTubs->BuildROGeometry();
  //targetROTubs->SetName(targetROName);
  
  targetSD->SetROgeometry(targetROTubs);  

  SDman->AddNewDetector(targetSD);
  logicalVolume->SetSensitiveDetector(targetSD); 

}

G4VPhysicalVolume* GPTargetGeometry::SetupTargetHit(G4LogicalVolume* targetLog,G4ThreeVector point)
{
  //------------------------------ target tube
  G4Box* targetHitSolid = new G4Box("targetHitSolid",
      m*dTargetGlobalSolidX/2,
      m*dTargetGlobalSolidY/2,
      m*dTargetHitL/2);

  G4LogicalVolume* targetHitLog = new G4LogicalVolume(targetHitSolid,spaceMaterial,"targetHitLog");
  
  G4VPhysicalVolume* targetHitPhys = new G4PVPlacement(0,
    point*m,
    targetHitLog,
    "targetHitPhys",
    targetLog,false,0);
    


  G4SDManager* SDman = G4SDManager::GetSDMpointer();
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

  return targetHitPhys;
}

std::vector<G4int> GPTargetGeometry::GetEddDim()
{
  return vecEddDim;
}

void GPTargetGeometry::SetTargetMaterial (G4String strMa)
{
  targetMaterial = G4NistManager::Instance()->FindOrBuildMaterial(strMa);
}

