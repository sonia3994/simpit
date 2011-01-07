#include "GPTargetROGeometryTubs.hh"
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

GPTargetROGeometryTubs::GPTargetROGeometryTubs()
  : G4VReadOutGeometry()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetROGeometryTubs::GPTargetROGeometryTubs()"<<G4endl;
#endif
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetROGeometryTubs::GPTargetROGeometryTubs()"<<G4endl;
#endif
}


GPTargetROGeometryTubs:: GPTargetROGeometryTubs(G4String name, G4double dRMin, G4double dRMax, G4double dDz,std::vector<G4int> n)
  : G4VReadOutGeometry(name), dummyMat(0),dTargetTubs_z(dDz),dTargetTubs_or(dRMax),dTargetTubs_ir(dRMin),iTargetTubs_RepZ(n[0]),iTargetTubs_RepR(n[1]),iTargetTubs_RepPhi(n[2])
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetROGeometryTubs::GPTargetROGeometryTubs(G4String, G4double, G4double, G4double,std::vector<G4int>)"<<G4endl;
#endif
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetROGeometryTubs::GPTargetROGeometryTubs(G4String, G4double, G4double, G4double,std::vector<G4int>)"<<G4endl;
#endif
}

GPTargetROGeometryTubs::~GPTargetROGeometryTubs()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetROGeometryTubs::~GPTargetROGeometryTubs()"<<G4endl;
#endif
  delete dummyMat;
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetROGeometryTubs::~GPTargetROGeometryTubs()"<<G4endl;
#endif
}

void GPTargetROGeometryTubs::Init()
{

  dExpHall_x = 120.e-3;
  dExpHall_y = 120.e-3;
  dExpHall_z = 120.e-3;

  //dTargetTubs_z=25e-3;
  //dTargetTubs_or=25e-3;
  //dTargetTubs_ir=6e-3;

  //dTargetTubs_dz=1e-3;
  //dTargetTubs_dr=1e-3;
  dTargetTubs_DPhi=360;

  dTargetTubs_dz=dTargetTubs_z/iTargetTubs_RepZ;
  dTargetTubs_dr=dTargetTubs_or/iTargetTubs_RepR;
  dTargetTubs_dphi=dTargetTubs_DPhi/iTargetTubs_RepPhi;

  dTargetTubsZCell_z=dTargetTubs_dz;
  dTargetTubsZCell_or=dTargetTubs_or;
  dTargetTubsZCell_ir=dTargetTubs_ir;

  dTargetTubsRCell_z=dTargetTubs_dz;
  dTargetTubsRCell_or=dTargetTubs_dr;
  dTargetTubsRCell_ir=dTargetTubs_ir;

  dTargetTubsPhiCell_z=dTargetTubs_dz;
  dTargetTubsPhiCell_or=dTargetTubs_dr;
  dTargetTubsPhiCell_ir=dTargetTubs_ir;
  dTargetTubsPhiCell_SPhi=0;
  dTargetTubsPhiCell_DPhi=dTargetTubs_dphi;

}

G4VPhysicalVolume* GPTargetROGeometryTubs::Build()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTargetROGeometryTubs::Build()"<<G4endl;
#endif
  Init();
  // A dummy material is used to fill the volumes of the readout geometry.
  // ( It will be allowed to set a NULL pointer in volumes of such virtual
  // division in future, since this material is irrelevant for tracking.)
  dummyMat  = new G4Material(name="dummyMatTubs", 1., 1.*g/mole, 1.*g/cm3);

  //Builds the ReadOut World:
  G4Box *ROWorldBox = new G4Box("ROWorldBoxTubs",
		 m*dExpHall_x/2,
		 m*dExpHall_y/2,
		 m*dExpHall_z/2);

  G4LogicalVolume *ROWorldLog = new G4LogicalVolume(ROWorldBox,
		  dummyMat,
		  "ROWorldLogicalTubs",
		  0, 0, 0);

  //ROWorldLog->SetVisAttributes(G4VisAttributes::Invisible);

  G4PVPlacement *ROWorldPhys = new G4PVPlacement(0,
		  G4ThreeVector(),
		  "ROWorldPhysicalTubs",
		  ROWorldLog,
		  0,false,0);
  // Target volume:

  G4Tubs *targetROTubs = new G4Tubs("targetROTubs",
		  m*dTargetTubs_ir,
		  m*dTargetTubs_or,
		  m*dTargetTubs_z/2,
		  0,
		  360);

  G4LogicalVolume *targetROLog = new G4LogicalVolume(targetROTubs,
		  dummyMat,
		  "targetROLogicalTubs",
		  0, 0, 0);

  G4PVPlacement *targetROPhys = new G4PVPlacement(0,
		  G4ThreeVector(),
		  "targetROPhysicalTubs",
		  targetROLog,
		  ROWorldPhys,
		  false,0);
  // -------------------------------
  // Target readout division:
  // -------------------------------
  // z division
  G4Tubs *targetROZDivTubs = new G4Tubs("targetROZDivTubs",
		  m*dTargetTubsZCell_ir,
		  m*dTargetTubsZCell_or,
		  m*dTargetTubsZCell_z/2,
		  0,
		  360);

  G4LogicalVolume *targetROZDivLog = new G4LogicalVolume(targetROZDivTubs,
		  dummyMat,
		  "targetROZDivisionLogicalTubs",
		  0, 0, 0);

  G4VPhysicalVolume * targetROZDivPhys = new G4PVReplica("targetROZDivisionPhysicalTubs",
		  targetROZDivLog,
		  targetROPhys,
		  kZAxis,
		  iTargetTubs_RepZ,
		  m*dTargetTubs_dz);

  // r division
  G4Tubs *targetRORDivTubs = new G4Tubs("targetRORDivTubs",
		  m*dTargetTubsRCell_ir,
		  m*dTargetTubsRCell_or,
		  m*dTargetTubsRCell_z/2,
		  0,
		  360);

  G4LogicalVolume *targetRORDivLog = new G4LogicalVolume(targetRORDivTubs,
		  dummyMat,
		  "targetRORDivisionLogicalTubs",
		  0, 0, 0);

  G4VPhysicalVolume * targetRORDivPhys = new G4PVReplica("targetRORDivisionPhysicalTubs",
		  targetRORDivLog,
		  targetROZDivPhys,
		  kRho,
		  iTargetTubs_RepR,
		  m*dTargetTubs_dr);

  // phi division
  G4Tubs *targetROPhiDivTubs = new G4Tubs("TargetROPhiTubs",
		  m*dTargetTubsPhiCell_ir,
		  m*dTargetTubsPhiCell_or,
		  m*dTargetTubsPhiCell_z/2,
		  dTargetTubsPhiCell_SPhi,
		  dTargetTubsPhiCell_DPhi);

  G4LogicalVolume *targetROPhiDivLog = new G4LogicalVolume(targetROPhiDivTubs, 
		  dummyMat,
		  "targetROZDivisionLogicalTubs",
		  0, 0, 0);

  G4VPhysicalVolume * targetROPhiDivPhys = new G4PVReplica("targetROPhiDivisionPhysicalTubs",
		  targetROPhiDivLog,
		  targetRORDivPhys,
		  kPhi,
		  iTargetTubs_RepPhi,
		  dTargetTubs_dphi);


  //Flags the cells as sensitive .The pointer here serves
  // as a flag only to check for sensitivity.
  // (Could we make it by a simple cast of a non-NULL value ?)
  GPDummySD * dummySensi = new GPDummySD();
  targetROPhiDivLog->SetSensitiveDetector(dummySensi);
  //targetRORDivLog->SetSensitiveDetector(dummySensi);

#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTargetROGeometryTubs::Build()"<<G4endl;
#endif
  return ROWorldPhys;
  
}
