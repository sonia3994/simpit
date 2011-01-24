// $Id: GPTargetGeometry.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//

#include "GPTargetGeometry.hh"

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

#include "globals.hh"

#include <sstream>
GPTargetGeometry::GPTargetGeometry()
{
  dSphereRadius=0.02;
  targetMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");
  spaceMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
}

GPTargetGeometry::~GPTargetGeometry()
{
}

G4VPhysicalVolume* GPTargetGeometry::Construct(G4LogicalVolume* motherLog,G4ThreeVector point,G4int iNumber,G4double dLX,G4double dLY,G4double dLZ,int iIndex)
{
  vecPoint=point;
  dLengthX = dLX;
  dLengthY = dLY;
  dLengthZ = dLZ;
  dSphereRadius = dLengthZ/(2+(iNumber-0.5)*4*sqrt(6)/3);
  dWidthX = 4*dSphereRadius;
  dWidthY = 2*sqrt(3.0)*dSphereRadius;
  dWidthZ = 4*sqrt(6.0)*dSphereRadius/3;

  iNumX = (dLengthX-2*dSphereRadius+0.5*dWidthX)/dWidthX;
  iNumY = (dLengthY-2*dSphereRadius+0.5*dWidthY)/dWidthY;
  iNumZ = iNumber;
  //return Granular(motherLog);
  return GranularHexagonal(motherLog,vecPoint);
}

void GPTargetGeometry::Print()
{
  G4cout<<"-----------Hexagonal construct------------\n"
   <<"\nRadius of sphere: "<<dSphereRadius
   <<"\nNumber of X cell: "<<iNumX
   <<"\nNumber of Y cell: "<<iNumY
   <<"\nNumber of Z cell: "<<iNumZ
   <<"\nZ axis Cell width: "<<dWidthZ/mm<< " mm"
   <<"\nActual thickness: "<<dLengthZ/mm<<" mm"
   <<G4endl;

}

G4VPhysicalVolume* GPTargetGeometry::Granular(G4LogicalVolume* motherLog)
{

  G4Box* ExpXYZBox= new G4Box("ExpXYZBox",dLengthX*m/2,dLengthY*m/2,dLengthZ*m/2);
  G4LogicalVolume* ExpXYZLog = new G4LogicalVolume(ExpXYZBox,spaceMaterial,"ExpXYZLog",0,0,0);
  G4VPhysicalVolume* ExpXYZPhy = new G4PVPlacement(0,
      vecPoint,
      ExpXYZLog,
      "ExpXYZPhy",
      motherLog,
      false,
      0);

  G4Box* ExpXYBox= new G4Box("ExpXYBox",
    dLengthX*m/2,
    dLengthY*m/2,
    (sqrt(6.0)/3+1)*dSphereRadius*m);
  G4LogicalVolume* ExpXYLog = new G4LogicalVolume(ExpXYBox,spaceMaterial,"ExpXYLog",0,0,0);
  G4VPhysicalVolume* ExpXYPhy = new G4PVReplica("ExpXYPhys",
      ExpXYLog,
      ExpXYZLog,
      kZAxis,
      iNumZ,
      dWidthZ*m);

  ///*
  G4Box* ExpXBox= new G4Box("ExpXBox",
    dLengthX*m/2,
    (2*sqrt(3.0)/3+1)*dSphereRadius*m,
    (sqrt(6.0)/3+1)*dSphereRadius*m);

  G4LogicalVolume* ExpXLog = new G4LogicalVolume(ExpXBox,spaceMaterial,"ExpXLog",0,0,0);
  G4VPhysicalVolume* ExpXPhy= new G4PVReplica("ExpXPhy",
      ExpXLog,
      ExpXYLog,
      kYAxis,
      iNumY,
      dWidthY*m);

  GranularCell(ExpXLog);
  //*/
  return ExpXYZPhy;

}

G4VPhysicalVolume* GPTargetGeometry::GranularCell(G4LogicalVolume* motherLog)
{

  /*
  G4Para* ExpCellSolid = new G4Para("ExpCellSolid",
                      dSphereRadius*m,
		      sqrt(3.0)*dSphereRadius/2*m,
		      sqrt(6.0)*dSphereRadius/3*m,
		      60,
		      //acos(sqrt(3.0)/3));
		      acos(sqrt(6.0)/3),
		      150);
  */
  G4Box* ExpCellSolid = new G4Box("ExpCellSolid",
    3*dSphereRadius*m,
    (2*sqrt(3.0)/3+1)*dSphereRadius*m,
    (sqrt(6.0)/3+1)*dSphereRadius*m);
  G4LogicalVolume* ExpCellLog= new G4LogicalVolume(ExpCellSolid,
      spaceMaterial,
      "ExpCellSolid",
      0,0,0);
 
  G4ThreeVector sphPoint= G4ThreeVector(-2*dSphereRadius*m,-2*sqrt(3.0)*dSphereRadius/3*m,-sqrt(6.0)*dSphereRadius/3*m);
  G4Orb* sphOrbA = new G4Orb("sphOrbA",dSphereRadius*m);
  G4LogicalVolume* sphLogA = new G4LogicalVolume(sphOrbA,
      targetMaterial,
      "sphLogA",
      0,0,0);
  G4PVPlacement* sphPhyA= new G4PVPlacement(0,
      sphPoint,
      sphLogA,
      "sphPhyA",
      ExpCellLog,
      false,
      0);
  sphPoint= G4ThreeVector(0,-2.0*sqrt(3.0)*dSphereRadius/3*m,-sqrt(6.0)*dSphereRadius/3*m); 
  G4Orb* sphOrbB = new G4Orb("sphOrbB",dSphereRadius*m);
  G4LogicalVolume* sphLogB = new G4LogicalVolume(sphOrbB,
      targetMaterial,
      "sphLogB",
      0,0,0);
  G4PVPlacement* sphPhyB= new G4PVPlacement(0,
      sphPoint,
      sphLogB,
      "sphPhyB",
      ExpCellLog,
      false,
      0);

  sphPoint= G4ThreeVector(dSphereRadius*m,-sqrt(3.0)*dSphereRadius/3*m,sqrt(6.0)*dSphereRadius/3*m); 
  G4Orb* sphOrbC = new G4Orb("sphOrbC",dSphereRadius*m);
  G4LogicalVolume* sphLogC = new G4LogicalVolume(sphOrbC,
      targetMaterial,
      "sphLogC",
      0,0,0);
  G4PVPlacement* sphPhyC= new G4PVPlacement(0,
      sphPoint,
      sphLogC,
      "sphPhyC",
      ExpCellLog,
      false,
      0);

  sphPoint= G4ThreeVector(-dSphereRadius*m,-sqrt(3.0)*dSphereRadius/3*m,sqrt(6.0)*dSphereRadius/3*m); 
  G4Orb* sphOrbD = new G4Orb("sphOrbD",dSphereRadius*m);
  G4LogicalVolume* sphLogD = new G4LogicalVolume(sphOrbD,
      targetMaterial,
      "sphLogD",
      0,0,0);
  G4PVPlacement* sphPhyD= new G4PVPlacement(0,
      sphPoint,
      sphLogD,
      "sphPhyD",
      ExpCellLog,
      false,
      0);

  sphPoint= G4ThreeVector(-dSphereRadius*m,sqrt(3.0)*dSphereRadius/3*m,-sqrt(6.0)*dSphereRadius/3*m); 
  G4Orb* sphOrbE = new G4Orb("sphOrbE",dSphereRadius*m);
  G4LogicalVolume* sphLogE = new G4LogicalVolume(sphOrbE,
      targetMaterial,
      "sphLogE",
      0,0,0);
  G4PVPlacement* sphPhyE= new G4PVPlacement(0,
      sphPoint,
      sphLogE,
      "sphPhyE",
      ExpCellLog,
      false,
      0);

  sphPoint= G4ThreeVector(dSphereRadius*m,sqrt(3.0)*dSphereRadius*m/3,-sqrt(6.0)*dSphereRadius*m/3); 
  G4Orb* sphOrbF = new G4Orb("sphOrbF",dSphereRadius*m);
  G4LogicalVolume* sphLogF = new G4LogicalVolume(sphOrbF,
      targetMaterial,
      "sphLogF",
      0,0,0);
  G4PVPlacement* sphPhyF= new G4PVPlacement(0,
      sphPoint,
      sphLogF,
      "sphPhyF",
      ExpCellLog,
      false,
      0);

  sphPoint= G4ThreeVector(2*dSphereRadius*m,2*sqrt(3.0)*dSphereRadius*m/3,sqrt(6.0)*dSphereRadius*m/3); 
  G4Orb* sphOrbG = new G4Orb("sphOrbG",dSphereRadius*m);
  G4LogicalVolume* sphLogG = new G4LogicalVolume(sphOrbG,
      targetMaterial,
      "sphLogG",
      0,0,0);
  G4PVPlacement* sphPhyG= new G4PVPlacement(0,
      sphPoint,
      sphLogG,
      "sphPhyG",
      ExpCellLog,
      false,
      0);

  sphPoint= G4ThreeVector(0,2*sqrt(3.0)*dSphereRadius*m/3,sqrt(6.0)*dSphereRadius*m/3); 
  G4Orb* sphOrbH = new G4Orb("sphOrbH",dSphereRadius*m);
  G4LogicalVolume* sphLogH = new G4LogicalVolume(sphOrbH,
      targetMaterial,
      "sphLogH",
      0,0,0);
  G4PVPlacement* sphPhyH= new G4PVPlacement(0,
      sphPoint,
      sphLogH,
      "sphPhyH",
      ExpCellLog,
      false,
      0);


  G4VPhysicalVolume* ExpCellPhy= new G4PVReplica("ExpCellPhy",
      ExpCellLog,
      motherLog,
      kXAxis,
      iNumX,
      dWidthX*m);
  
  G4VisAttributes* sphereLogVisAtt= new G4VisAttributes(G4Colour(1.0,0,1.0,0.3));
  sphereLogVisAtt->SetVisibility(true);
  sphereLogVisAtt->SetForceSolid(true);
  //ExpCellLog->SetVisAttributes(sphereLogVisAtt);
  sphLogA->SetVisAttributes(sphereLogVisAtt);
  sphLogB->SetVisAttributes(sphereLogVisAtt);
  sphLogC->SetVisAttributes(sphereLogVisAtt);
  sphLogD->SetVisAttributes(sphereLogVisAtt);
  sphLogE->SetVisAttributes(sphereLogVisAtt);
  sphLogF->SetVisAttributes(sphereLogVisAtt);
  sphLogG->SetVisAttributes(sphereLogVisAtt);
  sphLogH->SetVisAttributes(sphereLogVisAtt);

  return ExpCellPhy;                              

}

///*
G4VPhysicalVolume* GPTargetGeometry::GranularHexagonal(G4LogicalVolume* motherLog,G4ThreeVector point)
{
  G4Box* hexagonalSolid= new G4Box("hexagonalSolid",dLengthX*m/2,dLengthY*m/2,dLengthZ*m/2);
  G4LogicalVolume* hexagonalLog = new G4LogicalVolume(hexagonalSolid,spaceMaterial,"hexagonalLog",0,0,0);
  G4VPhysicalVolume* hexagonalPhy = new G4PVPlacement(0,
      point*m,
      hexagonalLog,
      "HexagonalPhy",
      motherLog,
      false,
      0);
  for(G4int k=0;k<iNumZ;k++)
  {
    for(G4int j=0;j<iNumY;j++)
    {
      for(G4int i=0;i<iNumX;i++)
      {
	G4ThreeVector vecCenter=G4ThreeVector(i*dWidthX+0.25*dWidthX+dSphereRadius-0.5*dLengthX,
	    j*dWidthY+0.25*dWidthY+dSphereRadius-0.5*dLengthY,
	    k*dWidthZ+0.25*dWidthZ+dSphereRadius-0.5*dLengthZ);
	GranularHexagonalCell(hexagonalLog,vecCenter,i+i*j+i*j*k);
      }
    }
  
  }
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
  sphPoint= G4ThreeVector(-2*dSphereRadius*m,-2*sqrt(3.0)*dSphereRadius/3*m,-sqrt(6.0)*dSphereRadius/3*m);
  sphPoint=sphPoint+vecCenter*m;
  G4Orb* sphOrbA = new G4Orb(strSolid,dSphereRadius*m);
  G4LogicalVolume* sphLogA = new G4LogicalVolume(sphOrbA,
      targetMaterial,
      strLog,
      0,0,0);
  G4PVPlacement* sphPhyA= new G4PVPlacement(0,
      sphPoint,
      sphLogA,
      strPhy,
      motherLog,
      false,
      0);

  strSolid="sphOrbB_"+strSuffix;
  strLog="sphLogB_"+strSuffix;
  strPhy="sphPhyB_"+strSuffix;
  sphPoint= G4ThreeVector(0,-2.0*sqrt(3.0)*dSphereRadius/3*m,-sqrt(6.0)*dSphereRadius/3*m); 
  sphPoint=sphPoint+vecCenter*m;
  G4Orb* sphOrbB = new G4Orb(strSolid,dSphereRadius*m);
  G4LogicalVolume* sphLogB = new G4LogicalVolume(sphOrbB,
      targetMaterial,
      strLog,
      0,0,0);
  G4PVPlacement* sphPhyB= new G4PVPlacement(0,
      sphPoint,
      sphLogB,
      strPhy,
      motherLog,
      false,
      0);

  strSolid="sphOrbC_"+strSuffix;
  strLog="sphLogC_"+strSuffix;
  strPhy="sphPhyC_"+strSuffix;
  sphPoint= G4ThreeVector(dSphereRadius*m,-sqrt(3.0)*dSphereRadius/3*m,sqrt(6.0)*dSphereRadius/3*m); 
  sphPoint=sphPoint+vecCenter*m;
  G4Orb* sphOrbC = new G4Orb(strSolid,dSphereRadius*m);
  G4LogicalVolume* sphLogC = new G4LogicalVolume(sphOrbC,
      targetMaterial,
      strLog,
      0,0,0);
  G4PVPlacement* sphPhyC= new G4PVPlacement(0,
      sphPoint,
      sphLogC,
      strPhy,
      motherLog,
      false,
      0);

  strSolid="sphOrbD_"+strSuffix;
  strLog="sphLogD_"+strSuffix;
  strPhy="sphPhyD_"+strSuffix;
  sphPoint= G4ThreeVector(-dSphereRadius*m,-sqrt(3.0)*dSphereRadius/3*m,sqrt(6.0)*dSphereRadius/3*m); 
  sphPoint=sphPoint+vecCenter*m;
  G4Orb* sphOrbD = new G4Orb(strSolid,dSphereRadius*m);
  G4LogicalVolume* sphLogD = new G4LogicalVolume(sphOrbD,
      targetMaterial,
      strLog,
      0,0,0);
  G4PVPlacement* sphPhyD= new G4PVPlacement(0,
      sphPoint,
      sphLogD,
      strPhy,
      motherLog,
      false,
      0);

  strSolid="sphOrbE_"+strSuffix;
  strLog="sphLogE_"+strSuffix;
  strPhy="sphPhyE_"+strSuffix;
  sphPoint= G4ThreeVector(-dSphereRadius*m,sqrt(3.0)*dSphereRadius/3*m,-sqrt(6.0)*dSphereRadius/3*m); 
  sphPoint=sphPoint+vecCenter*m;
  G4Orb* sphOrbE = new G4Orb(strSolid,dSphereRadius*m);
  G4LogicalVolume* sphLogE = new G4LogicalVolume(sphOrbE,
      targetMaterial,
      strLog,
      0,0,0);
  G4PVPlacement* sphPhyE= new G4PVPlacement(0,
      sphPoint,
      sphLogE,
      strPhy,
      motherLog,
      false,
      0);

  strSolid="sphOrbF_"+strSuffix;
  strLog="sphLogF_"+strSuffix;
  strPhy="sphPhyF_"+strSuffix;
  sphPoint= G4ThreeVector(dSphereRadius*m,sqrt(3.0)*dSphereRadius*m/3,-sqrt(6.0)*dSphereRadius*m/3); 
  sphPoint=sphPoint+vecCenter*m;
  G4Orb* sphOrbF = new G4Orb(strSolid,dSphereRadius*m);
  G4LogicalVolume* sphLogF = new G4LogicalVolume(sphOrbF,
      targetMaterial,
      strLog,
      0,0,0);
  G4PVPlacement* sphPhyF= new G4PVPlacement(0,
      sphPoint,
      sphLogF,
      strPhy,
      motherLog,
      false,
      0);

  strSolid="sphOrbG_"+strSuffix;
  strLog="sphLogG_"+strSuffix;
  strPhy="sphPhyG_"+strSuffix;
  sphPoint= G4ThreeVector(2*dSphereRadius*m,2*sqrt(3.0)*dSphereRadius*m/3,sqrt(6.0)*dSphereRadius*m/3); 
  sphPoint=sphPoint+vecCenter*m;
  G4Orb* sphOrbG = new G4Orb(strSolid,dSphereRadius*m);
  G4LogicalVolume* sphLogG = new G4LogicalVolume(sphOrbG,
      targetMaterial,
      strLog,
      0,0,0);
  G4PVPlacement* sphPhyG= new G4PVPlacement(0,
      sphPoint,
      sphLogG,
      strPhy,
      motherLog,
      false,
      0);

  strSolid="sphOrbH_"+strSuffix;
  strLog="sphLogH_"+strSuffix;
  strPhy="sphPhyH_"+strSuffix;
  sphPoint= G4ThreeVector(0,2*sqrt(3.0)*dSphereRadius*m/3,sqrt(6.0)*dSphereRadius*m/3); 
  sphPoint=sphPoint+vecCenter*m;
  G4Orb* sphOrbH = new G4Orb(strSolid,dSphereRadius*m);
  G4LogicalVolume* sphLogH = new G4LogicalVolume(sphOrbH,
      targetMaterial,
      strLog,
      0,0,0);
  G4PVPlacement* sphPhyH= new G4PVPlacement(0,
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
//*/
