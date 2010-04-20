//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
#include "GPTargetROGeometry.hh"
#include "GPDummySD.hh"

#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4SDManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4ThreeVector.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"

GPTargetROGeometry::GPTargetROGeometry()
  : G4VReadOutGeometry()
{
}


GPTargetROGeometry:: GPTargetROGeometry(G4String name, G4double x, G4double y, G4double z,std::vector<G4int> n)
  : G4VReadOutGeometry(name), dummyMat(0),targetBox_x(x),targetBox_y(y),targetBox_z(z),targetBox_RepX(n[0]),targetBox_RepY(n[1]),targetBox_RepZ(n[2])
{
}

GPTargetROGeometry::~GPTargetROGeometry()
{
  delete dummyMat;
}

void GPTargetROGeometry::Init()
{

  expHall_x = 120.*mm;
  expHall_y = 120.*mm;
  expHall_z = 120.*mm;

  //targetBox_x=25*mm;
  //targetBox_y=25*mm;
  //targetBox_z=6*mm;

  //targetBox_dx=1*mm;
  //targetBox_dy=1*mm;
  //targetBox_dz=1*mm;

  targetBox_dx=targetBox_x/targetBox_RepX;
  targetBox_dy=targetBox_y/targetBox_RepY;
  targetBox_dz=targetBox_z/targetBox_RepZ;

  targetBoxXCell_x=targetBox_dx;
  targetBoxXCell_y=targetBox_y;
  targetBoxXCell_z=targetBox_z;

  targetBoxYCell_x=targetBox_dx;
  targetBoxYCell_y=targetBox_dy;
  targetBoxYCell_z=targetBox_z;

  targetBoxZCell_x=targetBox_dx;
  targetBoxZCell_y=targetBox_dy;
  targetBoxZCell_z=targetBox_dz;

}

G4VPhysicalVolume* GPTargetROGeometry::Build()
{
  Init();
  // A dummy material is used to fill the volumes of the readout geometry.
  // ( It will be allowed to set a NULL pointer in volumes of such virtual
  // division in future, since this material is irrelevant for tracking.)
  dummyMat  = new G4Material(name="dummyMat", 1., 1.*g/mole, 1.*g/cm3);

  //Builds the ReadOut World:
  G4Box *ROWorldBox = new G4Box("ROWorldBox",
		  expHall_x/2,
		  expHall_y/2,
		  expHall_z/2);

  G4LogicalVolume *ROWorldLog = new G4LogicalVolume(ROWorldBox,
		  dummyMat,
		  "ROWorldLogical",
		  0, 0, 0);

  ROWorldLog->SetVisAttributes(G4VisAttributes::Invisible);

  G4PVPlacement *ROWorldPhys = new G4PVPlacement(0,
		  G4ThreeVector(),
		  "ROWorldPhysical",
		  ROWorldLog,
		  0,false,0);
  // Target volume:

  G4Box *targetROBox = new G4Box("TargetROBox",
		  targetBox_x/2,
		  targetBox_y/2,
		  targetBox_z/2);

  G4LogicalVolume *targetROLog = new G4LogicalVolume(targetROBox,
		  dummyMat,
		  "targetROLogical",
		  0, 0, 0);

  G4PVPlacement *targetROPhys = new G4PVPlacement(0,
		  G4ThreeVector(),
		  "targetROPhysical",
		  targetROLog,
		  ROWorldPhys,
		  false,0);
  // -------------------------------
  // Target readout division:
  // -------------------------------
  // x division first: 48 sectors
  G4Box *targetROXDivBox = new G4Box("TargetROXDivBox",
		  targetBoxXCell_x/2,
		  targetBoxXCell_y/2,
		  targetBoxXCell_z/2);

  G4LogicalVolume *targetROXDivLog = new G4LogicalVolume(targetROXDivBox,
		  dummyMat,
		  "targetROXDivisionLogical",
		  0, 0, 0);

  G4VPhysicalVolume * targetROXDivPhys = new G4PVReplica("targetROXDivisionPhysical",
		  targetROXDivLog,
		  targetROPhys,
		  kXAxis,
		  targetBox_RepX,
		  targetBox_dx);

  // y division first: 48 sectors
  G4Box *targetROYDivBox = new G4Box("TargetROYDivBox",
		  targetBoxYCell_x/2,
		  targetBoxYCell_y/2,
		  targetBoxYCell_z/2);

  G4LogicalVolume *targetROYDivLog = new G4LogicalVolume(targetROYDivBox,
		  dummyMat,
		  "targetROYDivisionLogical",
		  0, 0, 0);

  G4VPhysicalVolume * targetROYDivPhys = new G4PVReplica("targetROYDivisionPhysical",
		  targetROYDivLog,
		  targetROXDivPhys,
		  kYAxis,
		  targetBox_RepY,
		  targetBox_dy);

  // z division first: 20 sectors
  G4Box *targetROZDivBox = new G4Box("TargetROZBox",
		  targetBoxZCell_x/2,
		  targetBoxZCell_y/2,
		  targetBoxZCell_z/2);

  G4LogicalVolume *targetROZDivLog = new G4LogicalVolume(targetROZDivBox, 
		  dummyMat,
		  "targetROZDivisionLogical",
		  0, 0, 0);

  G4VPhysicalVolume * targetROZDivPhys = new G4PVReplica("targetROZDivisionPhysical",
		  targetROZDivLog,
		  targetROYDivPhys,
		  kZAxis,
		  targetBox_RepZ,
		  targetBox_dz);


  //Flags the cells as sensitive .The pointer here serves
  // as a flag only to check for sensitivity.
  // (Could we make it by a simple cast of a non-NULL value ?)
  GPDummySD * dummySensi = new GPDummySD;
  targetROZDivLog->SetSensitiveDetector(dummySensi);

  return ROWorldPhys;
  
}
