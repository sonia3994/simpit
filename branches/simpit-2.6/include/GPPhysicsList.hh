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
	G4double	dCutGamma;
	G4double 	dCutElectron;
	G4double	dCutPositron;
	G4int		iPolarizedFlag;
};

#endif







