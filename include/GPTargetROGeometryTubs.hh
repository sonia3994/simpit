#ifndef GPTargetROGeometryTubs_h
#define GPTargetROGeometryTubs_h 1

#include "G4VReadOutGeometry.hh"
#include <vector> 

class G4Material;

class GPTargetROGeometryTubs : public G4VReadOutGeometry
{
public:
  GPTargetROGeometryTubs();
  GPTargetROGeometryTubs(G4String name, G4double dRMin, G4double dRMax, G4double dDz,std::vector<G4int> n);
  ~GPTargetROGeometryTubs();

private:
  G4VPhysicalVolume* Build();
  G4Material* dummyMat;
  void	Init();


     G4int iTargetTubs_RepZ;
     G4int iTargetTubs_RepR;
     G4int iTargetTubs_RepPhi;

     G4double dExpHall_x;
     G4double dExpHall_y;
     G4double dExpHall_z;

     G4double dTargetTubs_z;
     G4double dTargetTubs_or;
     G4double dTargetTubs_ir;
     G4double dTargetTubs_SPhi;
     G4double dTargetTubs_DPhi;

     G4double dTargetTubs_dz;
     G4double dTargetTubs_dr;
     G4double dTargetTubs_dphi;

     G4double dTargetTubsZCell_z;
     G4double dTargetTubsZCell_or;
     G4double dTargetTubsZCell_ir;

     G4double dTargetTubsRCell_z;
     G4double dTargetTubsRCell_or;
     G4double dTargetTubsRCell_ir;

     G4double dTargetTubsPhiCell_z;
     G4double dTargetTubsPhiCell_or;
     G4double dTargetTubsPhiCell_ir;
     G4double dTargetTubsPhiCell_SPhi;
     G4double dTargetTubsPhiCell_DPhi;
};

#endif
