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

GPTargetGeometry::GPTargetGeometry()
{
  dSphereRadiu=40*cm;
  targetMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_W");
  spaceMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
}

GPTargetGeometry::~GPTargetGeometry()
{
}

G4VPhysicalVolume* GPTargetGeometry::Construct(G4LogicalVolume* motherLog,G4ThreeVector point,G4double dSphereR,G4double dLX,G4double dLY,G4double dLZ,int iIndex)
{
  vecPoint=point;
  dSphereRadiu=dSphereR;
  dLengthX = dLX;
  dLengthY = dLY;
  dLengthZ = dLZ;
  dWidthX = 4*dSphereRadiu;
  dWidthY = 2*sqrt(3.0)*dSphereRadiu;
  dWidthZ = 4*sqrt(6.0)*dSphereRadiu/3;

  iNumX = dLengthX/dWidthX-1;
  iNumY = dLengthY/dWidthY-1;
  iNumZ = dLengthZ/dWidthZ-1;
  /*
  G4cout<<"X "<<iNumX
   <<", Y "<<iNumY
   <<", Z "<<iNumZ
   <<G4endl;
   */

  return Granular(motherLog);
}

G4VPhysicalVolume* GPTargetGeometry::Granular(G4LogicalVolume* MotherLog)
{

  G4Box* ExpXYZBox= new G4Box("ExpXYZBox",dLengthX*m/2,dLengthY*m/2,dLengthZ*m/2);
  G4LogicalVolume* ExpXYZLog = new G4LogicalVolume(ExpXYZBox,spaceMaterial,"ExpXYZLog",0,0,0);
  G4VPhysicalVolume* ExpXYZPhy = new G4PVPlacement(0,
      vecPoint,
      ExpXYZLog,
      "ExpXYZPhy",
      MotherLog,
      false,
      0);

  G4Box* ExpXYBox= new G4Box("ExpXYBox",
    dLengthX*m/2,
    dLengthY*m/2,
    (sqrt(6.0)/3+1)*dSphereRadiu*m);
  G4LogicalVolume* ExpXYLog = new G4LogicalVolume(ExpXYBox,spaceMaterial,"ExpXYLog",0,0,0);
  G4VPhysicalVolume* ExpXYPhy = new G4PVReplica("ExpXYPhys",
      ExpXYLog,
      ExpXYZLog,
      kZAxis,
      iNumZ,
      dWidthZ*m);

  ///*
  G4Box* ExpXBox= new G4Box("ExpXBox",
    (dLengthX+1)*m/2,
    (2*sqrt(3.0)/3+1)*dSphereRadiu*m,
    (sqrt(6.0)/3+1)*dSphereRadiu*m);

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

G4VPhysicalVolume* GPTargetGeometry::GranularCell(G4LogicalVolume* MotherLog)
{

  /*
  G4Para* ExpCellSolid = new G4Para("ExpCellSolid",
                      dSphereRadiu*m,
		      sqrt(3.0)*dSphereRadiu/2*m,
		      sqrt(6.0)*dSphereRadiu/3*m,
		      60,
		      //acos(sqrt(3.0)/3));
		      acos(sqrt(6.0)/3),
		      150);
  */
  G4Box* ExpCellSolid = new G4Box("ExpCellSolid",
    3*dSphereRadiu*m,
    (2*sqrt(3.0)/3+1)*dSphereRadiu*m,
    (sqrt(6.0)/3+1)*dSphereRadiu*m);
  G4LogicalVolume* ExpCellLog= new G4LogicalVolume(ExpCellSolid,
      spaceMaterial,
      "ExpCellSolid",
      0,0,0);
 
  G4ThreeVector sphPoint= G4ThreeVector(-2*dSphereRadiu*m,-2*sqrt(3.0)*dSphereRadiu/3*m,-sqrt(6.0)*dSphereRadiu/3*m);
  G4Orb* sphOrbA = new G4Orb("sphOrbA",dSphereRadiu*m);
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
  sphPoint= G4ThreeVector(0,-2.0*sqrt(3.0)*dSphereRadiu/3*m,-sqrt(6.0)*dSphereRadiu/3*m); 
  G4Orb* sphOrbB = new G4Orb("sphOrbB",dSphereRadiu*m);
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

  sphPoint= G4ThreeVector(dSphereRadiu*m,-sqrt(3.0)*dSphereRadiu/3*m,sqrt(6.0)*dSphereRadiu/3*m); 
  G4Orb* sphOrbC = new G4Orb("sphOrbC",dSphereRadiu*m);
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

  sphPoint= G4ThreeVector(-dSphereRadiu*m,-sqrt(3.0)*dSphereRadiu/3*m,sqrt(6.0)*dSphereRadiu/3*m); 
  G4Orb* sphOrbD = new G4Orb("sphOrbD",dSphereRadiu*m);
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

  sphPoint= G4ThreeVector(-dSphereRadiu*m,sqrt(3.0)*dSphereRadiu/3*m,-sqrt(6.0)*dSphereRadiu/3*m); 
  G4Orb* sphOrbE = new G4Orb("sphOrbE",dSphereRadiu*m);
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

  sphPoint= G4ThreeVector(dSphereRadiu*m,sqrt(3.0)*dSphereRadiu*m/3,-sqrt(6.0)*dSphereRadiu*m/3); 
  G4Orb* sphOrbF = new G4Orb("sphOrbF",dSphereRadiu*m);
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

  sphPoint= G4ThreeVector(2*dSphereRadiu*m,2*sqrt(3.0)*dSphereRadiu*m/3,sqrt(6.0)*dSphereRadiu*m/3); 
  G4Orb* sphOrbG = new G4Orb("sphOrbG",dSphereRadiu*m);
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

  sphPoint= G4ThreeVector(0,2*sqrt(3.0)*dSphereRadiu*m/3,sqrt(6.0)*dSphereRadiu*m/3); 
  G4Orb* sphOrbH = new G4Orb("sphOrbH",dSphereRadiu*m);
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
      MotherLog,
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
