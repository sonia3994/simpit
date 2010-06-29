// $Id: GPPhysicsList.cc,v 1.6 2006/06/29 17:47:21 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

#include "GPPhysicsList.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "globals.hh"


GPPhysicsList::GPPhysicsList()
{	
cutGamma=1*um;
cutElectron=1*um;
cutPositron=1*um;
}

GPPhysicsList::~GPPhysicsList()
{;}

void GPPhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 

  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBaryons();
  ConstructIons();
}

#include "G4BosonConstructor.hh"
void GPPhysicsList::ConstructBosons()
{
  G4BosonConstructor pConstructor;
  pConstructor.ConstructParticle();
/*  
   // pseudo-particles
  //G4Geantino::GeantinoDefinition();
  //G4ChargedGeantino::ChargedGeantinoDefinition();

  // gamma
  G4Gamma::GammaDefinition();

  // optical photon
  G4OpticalPhoton::OpticalPhotonDefinition();
*/
}

#include "G4LeptonConstructor.hh"
void GPPhysicsList::ConstructLeptons()
{

  // Construct all leptons
  G4LeptonConstructor pConstructor;
  pConstructor.ConstructParticle();
/*
  // leptons
  //  e+/-
  G4Electron::ElectronDefinition();
  G4Positron::PositronDefinition();
  // mu+/-
  G4MuonPlus::MuonPlusDefinition();
  G4MuonMinus::MuonMinusDefinition();
  // nu_e
  G4NeutrinoE::NeutrinoEDefinition();
  G4AntiNeutrinoE::AntiNeutrinoEDefinition();
  // nu_mu
  G4NeutrinoMu::NeutrinoMuDefinition();
  G4AntiNeutrinoMu::AntiNeutrinoMuDefinition();
*/
}

#include "G4MesonConstructor.hh"
void GPPhysicsList::ConstructMesons()
{

  //  Construct all mesons
  G4MesonConstructor pConstructor;
  pConstructor.ConstructParticle();
/*
  //  mesons
  //    light mesons
  G4PionPlus::PionPlusDefinition();
  G4PionMinus::PionMinusDefinition();
  G4PionZero::PionZeroDefinition();
  G4Eta::EtaDefinition();
  G4EtaPrime::EtaPrimeDefinition();
  G4KaonPlus::KaonPlusDefinition();
  G4KaonMinus::KaonMinusDefinition();
  G4KaonZero::KaonZeroDefinition();
  G4AntiKaonZero::AntiKaonZeroDefinition();
  G4KaonZeroLong::KaonZeroLongDefinition();
  G4KaonZeroShort::KaonZeroShortDefinition();
*/
}

#include "G4BaryonConstructor.hh"
void GPPhysicsList::ConstructBaryons()
{

  //  Construct all barions
  G4BaryonConstructor  pConstructor;
  pConstructor.ConstructParticle();  
/*
  //  barions
  G4Proton::ProtonDefinition();
  G4AntiProton::AntiProtonDefinition();

  G4Neutron::NeutronDefinition();
  G4AntiNeutron::AntiNeutronDefinition();
*/
}

#include "G4IonConstructor.hh"
void GPPhysicsList::ConstructIons()
{
  //  Construct light ions
  G4IonConstructor pConstructor;
  pConstructor.ConstructParticle();  
}

void GPPhysicsList::ConstructProcess()
{
  // Define transportation process
  AddTransportation();

  ConstructEM();
  ConstructDecay();

  AddStepMax();
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
void GPPhysicsList::ConstructEM()
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
    ///*
    else if( particleName == "mu+" || 
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

    //*/
  }

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Decay.hh"

void GPPhysicsList::ConstructDecay()
{
  // Add Decay Process
  G4ParticleDefinition* particle;
  G4ProcessManager* pmanager;
  G4Decay* theDecayProcess = new G4Decay();
  theParticleIterator->reset();

  while( (*theParticleIterator)() )
  {
    particle = theParticleIterator->value();
    pmanager = particle->GetProcessManager();
    if (theDecayProcess->IsApplicable(*particle)) 
    { 
      pmanager ->AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }
}
  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "G4StepLimiter.hh"
#include "G4UserSpecialCuts.hh"

void GPPhysicsList::AddStepMax()
{
///*
  // Step limitation seen as a process
  G4StepLimiter* stepLimiter = new G4StepLimiter("MyStepLimiter");
  //G4UserSpecialCuts* userCuts = new G4UserSpecialCuts("MySpecialCuts");
  
  theParticleIterator->reset();
  while ((*theParticleIterator)()){
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();

      if (particle->GetPDGCharge() != 0.0)
        {
	  pmanager ->AddDiscreteProcess(stepLimiter);
	  //pmanager ->AddDiscreteProcess(userCuts);
        }
  }
//*/ 
}
//
void GPPhysicsList::SetCuts()
{
  //SetDefaultCutValue(1);

  // uppress error messages even in case e/gamma/proton do not exist            
  //G4int temp = GetVerboseLevel();   
  //SetVerboseLevel(0);                                                           

  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets 
  //   the default cut value for all particle types 
  //This method may not work!!!???
  //SetCutsWithDefault();   
  
  SetCutValue(cutGamma,"gamma");
  SetCutValue(cutElectron,"e+");
  SetCutValue(cutPositron,"e-");

  if (verboseLevel >0)
  {
    G4cout 
		<< "GPhysicsList::SetCuts:\n"
    	<< "gamma CutLength : " << G4BestUnit(cutGamma,"Length") 
    	<< "electron CutLength : " << G4BestUnit(cutElectron,"Length") 
    	<< "positron CutLength : " << G4BestUnit(cutPositron,"Length") 
		<< G4endl;
  }

  // Retrieve verbose level
  //SetVerboseLevel(temp);  
}

