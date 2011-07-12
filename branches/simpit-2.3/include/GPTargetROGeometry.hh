#ifndef GPTargetROGeometry_h
#define GPTargetROGeometry_h 1

#include "G4VReadOutGeometry.hh"
#include <vector> 
#include <fstream> 

class G4Material;

class GPTargetROGeometry : public G4VReadOutGeometry
{
  public:
    GPTargetROGeometry();
    GPTargetROGeometry(G4String name, G4double x, G4double y, G4double z,std::vector<G4int> n);
    ~GPTargetROGeometry();

    void SetPosition(G4ThreeVector);
    void Print(std::ofstream&);

  private:
    G4VPhysicalVolume* Build();
    G4Material* dummyMat;
    void	Init();


    G4ThreeVector vPos;
    G4int iTargetBox_RepX;
    G4int iTargetBox_RepY;
    G4int iTargetBox_RepZ;

    G4double dExpHall_x;
    G4double dExpHall_y;
    G4double dExpHall_z;

    G4double dTargetBox_x;
    G4double dTargetBox_y;
    G4double dTargetBox_z;

    G4double dTargetBox_dx;
    G4double dTargetBox_dy;
    G4double dTargetBox_dz;

    G4double dTargetBoxXCell_x;
    G4double dTargetBoxXCell_y;
    G4double dTargetBoxXCell_z;

    G4double dTargetBoxYCell_x;
    G4double dTargetBoxYCell_y;
    G4double dTargetBoxYCell_z;

    G4double dTargetBoxZCell_x;
    G4double dTargetBoxZCell_y;
    G4double dTargetBoxZCell_z;
};

#endif
