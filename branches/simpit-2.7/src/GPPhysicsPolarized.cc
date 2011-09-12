// $Id: GPPhysicsPolarized.cc,v 1.6 2006/06/29 17:47:21 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

#include "GPPhysicsPolarized.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "globals.hh"


GPPhysicsPolarized::GPPhysicsPolarized(const G4String& name)
   :  G4VPhysicsConstructor(name)
{	
}

GPPhysicsPolarized::~GPPhysicsPolarized()
{
}
void GPPhysicsPolarized::ConstructProcess()
{
  // Define transportation process
  ConstructEM();

}

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4hMultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4ionIonisation.hh"

//polarized
#include "G4PolarizedPhotoElectricEffect.hh"
#include "G4PolarizedCompton.hh"
#include "G4PolarizedGammaConversion.hh"
#include "G4ePolarizedIonisation.hh"
#include "G4ePolarizedBremsstrahlung.hh"
#include "G4eplusPolarizedAnnihilation.hh"
void GPPhysicsPolarized::ConstructEM()
{

  G4ParticleDefinition* particle; 
  G4ProcessManager* pmanager; 
  G4String particleName;
  theParticleIterator->reset();

  while( (*theParticleIterator)() )
  {
    particle = theParticleIterator->value();
    pmanager = particle->GetProcessManager();
    particleName = particle->GetParticleName();

    // Polarized processes
    if (namePhysics=="polarized") 
    {
      if (particleName == "gamma") 
      {
	pmanager->AddDiscreteProcess(new G4PolarizedPhotoElectricEffect);
	pmanager->AddDiscreteProcess(new G4PolarizedCompton);
	pmanager->AddDiscreteProcess(new G4PolarizedGammaConversion);      
      }
      else if (particleName == "e-") 
      {
	//electron
	pmanager->AddProcess(new G4eMultipleScattering,   -1,1,1);
	pmanager->AddProcess(new G4ePolarizedIonisation, -1,2,2);
	pmanager->AddProcess(new G4ePolarizedBremsstrahlung,      -1,3,3);
      }       
      else if (particleName == "e+") 
      {
	//positron
	pmanager->AddProcess(new G4eMultipleScattering, -1, 1,1);
	pmanager->AddProcess(new G4ePolarizedIonisation, -1, 2,2);
	pmanager->AddProcess(new G4ePolarizedBremsstrahlung,    -1, 3,3);
	pmanager->AddProcess(new G4eplusPolarizedAnnihilation,   0,-1,4);
      }

    }
    // Unpolarized processes
    else
    {
      if (particleName == "gamma")
      {
	// gamma         
	pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
	pmanager->AddDiscreteProcess(new G4ComptonScattering);
	pmanager->AddDiscreteProcess(new G4GammaConversion);
      }

      else if (particleName == "e-")
      {
	//electron
	pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
	pmanager->AddProcess(new G4eIonisation,         -1, 2, 2);
	pmanager->AddProcess(new G4eBremsstrahlung,     -1, 3, 3);      
      }

      else if (particleName == "e+")
      {
	//positron
	pmanager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
	pmanager->AddProcess(new G4eIonisation,         -1, 2, 2);
	pmanager->AddProcess(new G4eBremsstrahlung,     -1, 3, 3);
	pmanager->AddProcess(new G4eplusAnnihilation,    0,-1, 4);
      }
    }

	/*
    ///*other particles
    if( particleName == "mu+" || 
	particleName == "mu-"    )
    {
      //muon  
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4MuIonisation,        -1, 2, 2);
      pmanager->AddProcess(new G4MuBremsstrahlung,    -1, 3, 3);
      pmanager->AddProcess(new G4MuPairProduction,    -1, 4, 4);       
    }

    else if( particleName == "proton" ||
	particleName == "pi-" ||
	particleName == "pi+"    )
    {
      //proton  
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
      pmanager->AddProcess(new G4hBremsstrahlung,     -1, 3, 3);
      pmanager->AddProcess(new G4hPairProduction,     -1, 4, 4);       
    }

    else if( particleName == "alpha" || 
	particleName == "He3" || 
	particleName == "GenericIon" ) 
    {
      //Ions 
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4ionIonisation,       -1, 2, 2);

    }

    else if ((!particle->IsShortLived()) &&
	(particle->GetPDGCharge() != 0.0) && 
	(particle->GetParticleName() != "chargedgeantino")) 
    {
      //all others charged particles except geantino
      pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
      pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
    }
	*/

  }

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

