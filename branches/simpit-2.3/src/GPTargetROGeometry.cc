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
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetROGeometry::GPTargetROGeometry()"<<G4endl;
#endif
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetROGeometry::GPTargetROGeometry()"<<G4endl;
#endif
}


GPTargetROGeometry:: GPTargetROGeometry(G4String name, G4double x, G4double y, G4double z,std::vector<G4int> n)
  : G4VReadOutGeometry(name), dummyMat(0),dTargetBox_x(x),dTargetBox_y(y),dTargetBox_z(z),iTargetBox_RepX(n[0]),iTargetBox_RepY(n[1]),iTargetBox_RepZ(n[2])
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetROGeometry::GPTargetROGeometry(G4String, G4double, G4double, G4double,std::vector<G4int>)"<<G4endl;
#endif
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetROGeometry::GPTargetROGeometry(G4String, G4double, G4double, G4double,std::vector<G4int>)"<<G4endl;
#endif
}

GPTargetROGeometry::~GPTargetROGeometry()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetROGeometry::~GPTargetROGeometry()"<<G4endl;
#endif
  delete dummyMat;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetROGeometry::~GPTargetROGeometry()"<<G4endl;
#endif
}

void GPTargetROGeometry::Init()
{

  dExpHall_x = 10.0;
  dExpHall_y = 10.0;
  dExpHall_z = 10.0;

  //dTargetBox_x=25e-3;
  //dTargetBox_y=25e-3;
  //dTargetBox_z=6e-3;

  //dTargetBox_dx=1e-3;
  //dTargetBox_dy=1e-3;
  //dTargetBox_dz=1e-3;

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
  G4Box *ROWorldBox = new G4Box(name+"_ROWorldBox",
		 m*dExpHall_x/2,
		 m*dExpHall_y/2,
		 m*dExpHall_z/2);

  G4LogicalVolume *ROWorldLog = new G4LogicalVolume(ROWorldBox,
		  dummyMat,
		  name+"_ROWorldLogical",
		  0, 0, 0);

  ROWorldLog->SetVisAttributes(G4VisAttributes::Invisible);

  G4PVPlacement *ROWorldPhys = new G4PVPlacement(0,
		  G4ThreeVector(),
		  name+"_ROWorldPhysical",
		  ROWorldLog,
		  0,false,0);
  // Target volume:

  G4Box *targetROBox = new G4Box(name+"_TargetROBox",
		  m*dTargetBox_x/2,
		  m*dTargetBox_y/2,
		  m*dTargetBox_z/2);

  G4LogicalVolume *targetROLog = new G4LogicalVolume(targetROBox,
		  dummyMat,
		  name+"_targetROLogical",
		  0, 0, 0);

  G4PVPlacement *targetROPhys = new G4PVPlacement(0,
		  vPos*m,
		  name+"_targetROPhysical",
		  targetROLog,
		  ROWorldPhys,
		  false,0);
  // -------------------------------
  // Target readout division:
  // -------------------------------
  // x division first: 48 sectors
  G4Box *targetROXDivBox = new G4Box(name+"_TargetROXDivBox",
		  m*dTargetBoxXCell_x/2,
		  m*dTargetBoxXCell_y/2,
		  m*dTargetBoxXCell_z/2);

  G4LogicalVolume *targetROXDivLog = new G4LogicalVolume(targetROXDivBox,
		  dummyMat,
		  name+"_targetROXDivisionLogical",
		  0, 0, 0);

  G4VPhysicalVolume * targetROXDivPhys = new G4PVReplica(name+"_targetROXDivisionPhysical",
		  targetROXDivLog,
		  targetROPhys,
		  kXAxis,
		  iTargetBox_RepX,
		  m*dTargetBox_dx);

  // y division first: 48 sectors
  G4Box *targetROYDivBox = new G4Box(name+"_TargetROYDivBox",
		  m*dTargetBoxYCell_x/2,
		  m*dTargetBoxYCell_y/2,
		  m*dTargetBoxYCell_z/2);

  G4LogicalVolume *targetROYDivLog = new G4LogicalVolume(targetROYDivBox,
		  dummyMat,
		  name+"_targetROYDivisionLogical",
		  0, 0, 0);

  G4VPhysicalVolume * targetROYDivPhys = new G4PVReplica(name+"_targetROYDivisionPhysical",
		  targetROYDivLog,
		  targetROXDivPhys,
		  kYAxis,
		  iTargetBox_RepY,
		  m*dTargetBox_dy);

  // z division first: 20 sectors
  G4Box *targetROZDivBox = new G4Box(name+"_TargetROZBox",
		  m*dTargetBoxZCell_x/2,
		  m*dTargetBoxZCell_y/2,
		  m*dTargetBoxZCell_z/2);

  G4LogicalVolume *targetROZDivLog = new G4LogicalVolume(targetROZDivBox, 
		  dummyMat,
		  name+"_targetROZDivisionLogical",
		  0, 0, 0);

  G4VPhysicalVolume * targetROZDivPhys = new G4PVReplica(name+"_targetROZDivisionPhysical",
		  targetROZDivLog,
		  targetROYDivPhys,
		  kZAxis,
		  iTargetBox_RepZ,
		  m*dTargetBox_dz);


  //Flags the cells as sensitive .The pointer here serves
  // as a flag only to check for sensitivity.
  // (Could we make it by a simple cast of a non-NULL value ?)
  GPDummySD* dummySensi = new GPDummySD();
  targetROZDivLog->SetSensitiveDetector(dummySensi);

  return ROWorldPhys;
  
}
void GPTargetROGeometry::SetPosition(G4ThreeVector pos)
{
  vPos = pos;
}
