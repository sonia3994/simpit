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
//
// $Id: GPPhysicsList.hh,v 1.6 2006/06/29 17:47:15 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 
//
// GPPhysicsList
//  Construct/define particles and physics processes
//
//  Particle defined in ExampleN01
//    geantino
//
//  Process defined in ExampleN01
//    transportation
//

#ifndef GPPhysicsList_h
#define GPPhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class GPPhysicsList: public G4VUserPhysicsList
{
  public:
    GPPhysicsList();
    ~GPPhysicsList();

  protected:
    // Construct particle and physics process
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();
  protected:
    // these methods Construct particles 
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();
    void ConstructIons();

  protected:
  // these methods Construct physics processes and register them
    void ConstructDecay();
    void ConstructEM();
    void AddStepMax();
    void AddParameterisation();

  protected:
	G4double	cutGamma;
	G4double 	cutElectron;
	G4double	cutPositron;
};

#endif







