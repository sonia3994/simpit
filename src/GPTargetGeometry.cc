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
  iTargetHitFlag = 1;
  iTargetEddFlag = 1;
  iReadOutCylinderFlag = 0;

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

void GPTargetGeometry::Init()
{
  dTargetSDSolidZ=dTargetSolidZ;
  dTargetSDSolidR=dTargetSolidX/2;
  dTargetSDSolidAngle=360;

  dTargetSDCellZ=0.001;
  dTargetSDCellR=0.001;
  dTargetSDCellPhi = 360;

  if(iTargetHitFlag==1)
  {
    dTargetGlobalSolidZ = dTargetSolidZ+dTargetHitL;
    dIndexPoint = dTargetHitL;
  }
  else 
  {
    dTargetGlobalSolidZ = dTargetSolidZ;
    dIndexPoint = 0;
  }

  //GranularHexagonalInit();
}
G4VPhysicalVolume* GPTargetGeometry::Construct(G4LogicalVolume* motherLog,G4ThreeVector point)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetGeometry::Construct(G4LogicalVolume,G4ThreeVector)"<<G4endl;
#endif

  Init();

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
    GranularHexagonal(targetLog,G4ThreeVector(0,0,-dIndexPoint/2));
  }
  else
  {
    TubularTarget(targetLog,G4ThreeVector(0,0,-dIndexPoint/2));
  }
  
  if(iTargetHitFlag==1)
  SetTargetHit(targetLog,G4ThreeVector(0,0,dTargetSolidZ/2));

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
  dTargetSolidX = iTargetGranularXNumber*dWidthX+2*dTargetHexagonalSphereRadius-0.5*dWidthX;
  iTargetGranularYNumber = iTargetGranularXNumber;
  dTargetSolidY = iTargetGranularYNumber*dWidthY+2*dTargetHexagonalSphereRadius-0.5*dWidthY;
  //iTargetGranularYNumber = (dTargetSolidY-2*dTargetHexagonalSphereRadius+0.5*dWidthY)/dWidthY;
}

void GPTargetGeometry::Print()
{
  G4cout
   <<"\n----------------------------Target construct---------------------------"
   <<G4endl;
  if(iTargetGranularFlag==1)
  {
  G4cout
   <<"Target type: Hexagonal granular construct."
   <<"\nX of target: "<<dTargetSolidX*m/mm<<" mm"
   <<"\nY of target: "<<dTargetSolidY*m/mm<<" mm"
   <<"\nZ of target: "<<dTargetSolidZ*m/mm<<" mm"
   <<"\nRadius of sphere: "<<dTargetHexagonalSphereRadius*m/mm<<" mm"
   <<"\nZ axis Cell width: "<<dWidthZ*m/mm<< " mm"
   <<"\nNumber of X cell: "<<iTargetGranularXNumber
   <<"\nNumber of Y cell: "<<iTargetGranularYNumber
   <<"\nNumber of Z cell: "<<iTargetGranularZNumber
   <<G4endl;
  }
  else if(iTargetGranularFlag==0)
  {
  G4cout
    <<"Target type: Tubs construct."
    <<"\nRadius of target: "<<dTargetSolidX*m/mm/2<<" mm"
    <<"\nLength of target: "<<dTargetSolidZ*m/mm<<" mm"
    <<G4endl;
  }
   
  if(iTargetEddFlag==1&&iReadOutCylinderFlag==1) PrintSD();
  if(iTargetEddFlag==1&&iReadOutCylinderFlag!=1) 
  {
  G4cout
   <<"\nTarget Sensitive detector:"
   <<"\nX Cell width: "<<dTargetSDCellR*m/mm<<" mm"
   <<"\nY Cell width: "<<dTargetSDCellR*m/mm<<" mm"
   <<"\nZ Cell width: "<<dTargetSDCellZ*m/mm<<" mm"
   <<"\nX Cell Number: "<<dTargetSolidX/dTargetSDCellR
   <<"\nY Cell Number: "<<dTargetSolidX/dTargetSDCellR
   <<"\nZ Cell Number: "<<dTargetSolidZ/dTargetSDCellZ
   <<G4endl;
  }
  G4cout
   <<"\n------------------------------------------------------------------------"
   <<G4endl;

}
void GPTargetGeometry::PrintSD()
{
  G4cout
   <<"\n------------------------------Target Sensitive --------------------------"
   <<"\nZ Cell width: "<<dTargetSDCellZ*m/mm<<" mm"
   <<"\nR Cell width: "<<dTargetSDCellR*m/mm<<" mm"
   <<"\nPhi Cell width: "<<dTargetSDCellPhi<<" degree"
   <<"\nZ Cell Number: "<<vecEddDim[0]
   <<"\nR Cell Number: "<<vecEddDim[1]
   <<"\nPhi Cell Number: "<<vecEddDim[2]
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
	GranularHexagonalCell(hexagonalLog,
	    vecCenter,
	    i+j*iTargetGranularXNumber+k*iTargetGranularXNumber*iTargetGranularYNumber);
      }
    }
  
  }

  if(iTargetEddFlag==1) SetTargetSD(hexagonalLog);

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


  G4VisAttributes* sphereLogVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.3));
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

void GPTargetGeometry::SetParameter(std::string str,std::string strGlobal)
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
    else if(key=="hit.flag")
    iTargetHitFlag = dValueNew;

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

G4double GPTargetGeometry::GetParameter(std::string name) const
{
    if(name=="x")
    return dTargetSolidX;
    else if(name=="gz")
    return dTargetGlobalSolidZ;
    else if(name=="y")
    return dTargetSolidY;
    else if(name=="z")
    return dTargetSolidZ;
    else if(name=="granular.flag")
    return iTargetGranularFlag;
    else if(name=="granular.z.number")
    return iTargetGranularZNumber;
    else if(name=="granular.y.number")
    return iTargetGranularYNumber;
    else if(name=="granular.x.number")
    return iTargetGranularXNumber;

    else
    {
      std::cout<<"key does not exist.\n"<<std::endl;
      return -1;
    }
}

//*/
void GPTargetGeometry::SetTargetSD(G4LogicalVolume* logicalVolume)
{
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4String targetSDName="/PositronSource/Target/EddSD";
  G4String targetROName="targetROGeometry";

  vecEddDim[0]=(ceil(dTargetSDSolidZ/dTargetSDCellZ));
  vecEddDim[1]=(ceil(dTargetSDSolidR/dTargetSDCellR));
  vecEddDim[2]=(ceil(dTargetSDSolidAngle/dTargetSDCellPhi));

  GPTargetSD* targetSD = (GPTargetSD*)SDman->FindSensitiveDetector(targetSDName);
  if(targetSD==NULL)
    targetSD =new GPTargetSD(targetSDName,vecEddDim);

  if(iReadOutCylinderFlag)
  {
    targetSD->SetEddDim(vecEddDim);

    GPTargetROGeometryTubs* targetROTubs = (GPTargetROGeometryTubs*)targetSD->GetROgeometry(); 
    if(targetROTubs)
    {
      delete targetROTubs;
    }
    targetROTubs=new GPTargetROGeometryTubs(targetROName,
        0,
        dTargetSDSolidR,
        dTargetSDSolidZ,
        vecEddDim);

    targetROTubs->BuildROGeometry();
    targetSD->SetROgeometry(targetROTubs);  
  }

  else
  {
    vecEddDim[0]=(ceil(dTargetSolidX/dTargetSDCellR));
    vecEddDim[1]=(ceil(dTargetSolidY/dTargetSDCellR));
    vecEddDim[2]=(ceil(dTargetSolidZ/dTargetSDCellZ));
    targetSD->SetEddDim(vecEddDim);

    GPTargetROGeometry* targetRO = (GPTargetROGeometry*)targetSD->GetROgeometry(); 
    if(targetRO)
    {
      delete targetRO;
    }
    targetRO=new GPTargetROGeometry(targetROName,
        dTargetSolidX,
        dTargetSolidY,
        dTargetSolidZ,
        vecEddDim);

    targetRO->BuildROGeometry();
    
    targetSD->SetROgeometry(targetRO);  
  }

  SDman->AddNewDetector(targetSD);
  logicalVolume->SetSensitiveDetector(targetSD); 

}

G4VPhysicalVolume* GPTargetGeometry::SetTargetHit(G4LogicalVolume* targetLog,G4ThreeVector point)
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

G4VPhysicalVolume* GPTargetGeometry::TubularTarget(G4LogicalVolume* motherLog, G4ThreeVector point)
{
  G4Tubs* targetNormalSolid= new G4Tubs("targetNormalSolid",
      0,
      m*dTargetSolidX/2,
      m*dTargetSolidZ/2,
      0,
      deg*360);
  G4LogicalVolume* targetNormalLog = new G4LogicalVolume(targetNormalSolid,targetMaterial,"targetNormalLog");
  G4VPhysicalVolume* targetNormalPhy= new G4PVPlacement(0,
      point*m,
      targetNormalLog,
      "targetNormalPhys",
      motherLog,false,0);
  if(iTargetEddFlag==1) SetTargetSD(targetNormalLog); 
  
  G4VisAttributes* targetNormalLogVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0,0.3));
  targetNormalLogVisAtt->SetVisibility(true);
  targetNormalLogVisAtt->SetForceSolid(true);
  targetNormalLog->SetVisAttributes(targetNormalLogVisAtt);

  return targetNormalPhy;
}

void GPTargetGeometry::Print(std::ofstream& fstOutput)
{
  fstOutput
   <<"\nTarget structure:";

  if(iTargetGranularFlag==1)
  {
  fstOutput
   <<"\nTarget type, Hexagonal granular construct."
   <<"\nX of target, "<<dTargetSolidX*m/mm<<" mm"
   <<"\nY of target, "<<dTargetSolidY*m/mm<<" mm"
   <<"\nZ of target, "<<dTargetSolidZ*m/mm<<" mm"
   <<"\nRadius of sphere, "<<dTargetHexagonalSphereRadius*m/mm<<" mm"
   <<"\nZ axis Cell width, "<<dWidthZ*m/mm<< " mm"
   <<"\nNumber of X cell, "<<iTargetGranularXNumber
   <<"\nNumber of Y cell, "<<iTargetGranularYNumber
   <<"\nNumber of Z cell, "<<iTargetGranularZNumber
   <<G4endl;
  }
  else if(iTargetGranularFlag==0)
  {
  fstOutput
    <<"\nTarget type, Tubs construct."
    <<"\nRadius of target, "<<dTargetSolidX*m/mm/2<<" mm"
    <<"\nLength of target, "<<dTargetSolidZ*m/mm<<" mm"
    <<G4endl;
  }
  if(iTargetEddFlag==1&&iReadOutCylinderFlag==1) 
  {
  fstOutput
   <<"\nTarget Sensitive detector:"
   <<"\nZ Cell width, "<<dTargetSDCellZ*m/mm<<" mm"
   <<"\nR Cell width, "<<dTargetSDCellR*m/mm<<" mm"
   <<"\nPhi Cell width, "<<dTargetSDCellPhi<<" degree"
   <<"\nZ Cell Number, "<<vecEddDim[0]
   <<"\nR Cell Number, "<<vecEddDim[1]
   <<"\nPhi Cell Number, "<<vecEddDim[2]
   <<G4endl;
  }
  if(iTargetEddFlag==1&&iReadOutCylinderFlag!=1) 
  {
  fstOutput
   <<"\nTarget Sensitive detector:"
   <<"\nX Cell width, "<<dTargetSDCellR*m/mm<<" mm"
   <<"\nY Cell width, "<<dTargetSDCellR*m/mm<<" mm"
   <<"\nZ Cell width, "<<dTargetSDCellZ*m/mm<<" mm"
   <<"\nX Cell Number, "<<dTargetSolidX/dTargetSDCellR
   <<"\nY Cell Number, "<<dTargetSolidX/dTargetSDCellR
   <<"\nZ Cell Number, "<<dTargetSolidZ/dTargetSDCellZ
   <<G4endl;
  }
}
