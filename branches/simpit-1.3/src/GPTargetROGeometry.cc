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
  : G4VReadOutGeometry(name), dummyMat(0),dTargetBox_x(x),dTargetBox_y(y),dTargetBox_z(z),iTargetBox_RepX(n[0]),iTargetBox_RepY(n[1]),iTargetBox_RepZ(n[2])
{
}

GPTargetROGeometry::~GPTargetROGeometry()
{
  delete dummyMat;
}

void GPTargetROGeometry::Init()
{

  dExpHall_x = 120.*mm;
  dExpHall_y = 120.*mm;
  dExpHall_z = 120.*mm;

  //dTargetBox_x=25*mm;
  //dTargetBox_y=25*mm;
  //dTargetBox_z=6*mm;

  //dTargetBox_dx=1*mm;
  //dTargetBox_dy=1*mm;
  //dTargetBox_dz=1*mm;

  dTargetBox_dx=dTargetBox_x/iTargetBox_RepX;
  dTargetBox_dy=dTargetBox_y/iTargetBox_RepY;
  dTargetBox_dz=dTargetBox_z/iTargetBox_RepZ;

  dTargetBoxXCell_x=dTargetBox_dx;
  dTargetBoxXCell_y=dTargetBox_y;
  dTargetBoxXCell_z=dTargetBox_z;

  dTargetBoxYCell_x=dTargetBox_dx;
  dTargetBoxYCell_y=dTargetBox_dy;
  dTargetBoxYCell_z=dTargetBox_z;

  dTargetBoxZCell_x=dTargetBox_dx;
  dTargetBoxZCell_y=dTargetBox_dy;
  dTargetBoxZCell_z=dTargetBox_dz;

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
		  dExpHall_x/2,
		  dExpHall_y/2,
		  dExpHall_z/2);

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
		  dTargetBox_x/2,
		  dTargetBox_y/2,
		  dTargetBox_z/2);

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
		  dTargetBoxXCell_x/2,
		  dTargetBoxXCell_y/2,
		  dTargetBoxXCell_z/2);

  G4LogicalVolume *targetROXDivLog = new G4LogicalVolume(targetROXDivBox,
		  dummyMat,
		  "targetROXDivisionLogical",
		  0, 0, 0);

  G4VPhysicalVolume * targetROXDivPhys = new G4PVReplica("targetROXDivisionPhysical",
		  targetROXDivLog,
		  targetROPhys,
		  kXAxis,
		  iTargetBox_RepX,
		  dTargetBox_dx);

  // y division first: 48 sectors
  G4Box *targetROYDivBox = new G4Box("TargetROYDivBox",
		  dTargetBoxYCell_x/2,
		  dTargetBoxYCell_y/2,
		  dTargetBoxYCell_z/2);

  G4LogicalVolume *targetROYDivLog = new G4LogicalVolume(targetROYDivBox,
		  dummyMat,
		  "targetROYDivisionLogical",
		  0, 0, 0);

  G4VPhysicalVolume * targetROYDivPhys = new G4PVReplica("targetROYDivisionPhysical",
		  targetROYDivLog,
		  targetROXDivPhys,
		  kYAxis,
		  iTargetBox_RepY,
		  dTargetBox_dy);

  // z division first: 20 sectors
  G4Box *targetROZDivBox = new G4Box("TargetROZBox",
		  dTargetBoxZCell_x/2,
		  dTargetBoxZCell_y/2,
		  dTargetBoxZCell_z/2);

  G4LogicalVolume *targetROZDivLog = new G4LogicalVolume(targetROZDivBox, 
		  dummyMat,
		  "targetROZDivisionLogical",
		  0, 0, 0);

  G4VPhysicalVolume * targetROZDivPhys = new G4PVReplica("targetROZDivisionPhysical",
		  targetROZDivLog,
		  targetROYDivPhys,
		  kZAxis,
		  iTargetBox_RepZ,
		  dTargetBox_dz);


  //Flags the cells as sensitive .The pointer here serves
  // as a flag only to check for sensitivity.
  // (Could we make it by a simple cast of a non-NULL value ?)
  GPDummySD * dummySensi = new GPDummySD;
  targetROZDivLog->SetSensitiveDetector(dummySensi);

  return ROWorldPhys;
  
}
