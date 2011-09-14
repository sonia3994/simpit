// $Id: GPPhysicsPolarized.hh,v 1.6 2006/06/29 17:47:15 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 
//
// GPPhysicsPolarized
//  Construct/define particles and physics processes
//
//  Particle defined in ExampleN01
//    geantino
//
//  Process defined in ExampleN01
//    transportation
//

#ifndef GPPhysicsPolarized_h
#define GPPhysicsPolarized_h 1

#include "G4VUserPhysicsList.hh"
#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

class GPPhysicsPolarized: public G4VPhysicsConstructor
//class GPPhysicsPolarized: public G4VPhysicsConstructor
{
  public:
    GPPhysicsPolarized(const G4String& name = "polarized");
    ~GPPhysicsPolarized();

  public:
    // This method is dummy for physics
    void ConstructParticle() {};
 
    // This method will be invoked in the Construct() method.
    // each physics process will be instantiated and
    // registered to the process manager of each particle type 
    void ConstructProcess();
    // Construct particle and physics process
  protected:
    void ConstructEM();
};

#endif







