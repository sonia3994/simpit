#ifndef GPTargetROGeometry_h
#define GPTargetROGeometry_h 1

#include "G4VReadOutGeometry.hh"
#include <vector> 

class G4Material;

class GPTargetROGeometry : public G4VReadOutGeometry
{
public:
  GPTargetROGeometry();
  GPTargetROGeometry(G4String name, G4double x, G4double y, G4double z,std::vector<G4int> n);
  ~GPTargetROGeometry();

private:
  G4VPhysicalVolume* Build();
  G4Material* dummyMat;
  void	Init();



     G4double expHall_x;
     G4double expHall_y;
     G4double expHall_z;

     G4double targetBox_x;
     G4double targetBox_y;
     G4double targetBox_z;

     G4double targetBox_dx;
     G4double targetBox_dy;
     G4double targetBox_dz;

     G4int targetBox_RepX;
     G4int targetBox_RepY;
     G4int targetBox_RepZ;

     G4double targetBoxXCell_x;
     G4double targetBoxXCell_y;
     G4double targetBoxXCell_z;

     G4double targetBoxYCell_x;
     G4double targetBoxYCell_y;
     G4double targetBoxYCell_z;

     G4double targetBoxZCell_x;
     G4double targetBoxZCell_y;
     G4double targetBoxZCell_z;
};

#endif
