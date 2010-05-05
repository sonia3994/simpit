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
