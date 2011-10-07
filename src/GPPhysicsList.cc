// $Id: GPPhysicsList.cc,v 1.6 2006/06/29 17:47:21 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//
// 

#include "GPPhysicsList.hh"
#include "GPPhysicsPolarized.hh"
#include "GPPhysicsMessenger.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"
#include "G4UnitsTable.hh"
#include "G4UIcommand.hh"
#include "globals.hh"


GPPhysicsList::GPPhysicsList()
{	
dCutGamma=1*mm;
dCutElectron=1*mm;
dCutPositron=1*mm;
iPolarizedFlag=1;
pPhysicsConstructor= new GPPhysicsPolarized();
pPhysicsMessenger = new GPPhysicsMessenger();
}

GPPhysicsList::~GPPhysicsList()
{
	delete pPhysicsConstructor;
        delete pPhysicsMessenger;
}


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
}

#include "G4LeptonConstructor.hh"
void GPPhysicsList::ConstructLeptons()
{

  // Construct all leptons
  G4LeptonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

#include "G4MesonConstructor.hh"
void GPPhysicsList::ConstructMesons()
{

  //  Construct all mesons
  G4MesonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

#include "G4BaryonConstructor.hh"
void GPPhysicsList::ConstructBaryons()
{

  //  Construct all barions
  G4BaryonConstructor  pConstructor;
  pConstructor.ConstructParticle();  
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

  //ConstructEM();
  pPhysicsConstructor->ConstructProcess();
  ConstructDecay();

  AddStepMax();
}

/*
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

    // Polarized processes
    if(iPolarizedFlag)
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

  }

}

*/

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
  
  SetCutValue(dCutGamma*m,"gamma");
  SetCutValue(dCutElectron*m,"e+");
  SetCutValue(dCutPositron*m,"e-");

  if (verboseLevel >0)
  {
    G4cout 
		<< "GPhysicsList::SetCuts:\n"
    	<< "gamma CutLength : " << G4BestUnit(dCutGamma,"Length") 
    	<< "electron CutLength : " << G4BestUnit(dCutElectron,"Length") 
    	<< "positron CutLength : " << G4BestUnit(dCutPositron,"Length") 
		<< G4endl;
  }

  // Retrieve verbose level
  //SetVerboseLevel(temp);  
}

void GPPhysicsList::SetParameter(std::string,std::string)
{
}
void GPPhysicsList::SetParameter(std::string sGlobal)
{
  std::stringstream ss(sGlobal);
  std::string		  sUnit;
  std::string		  sKey;
  std::string		  sValueOrg;
  G4double   		  dValueNew;
  G4double   		  dValueOrg;
      
  ss>>sKey>>sValueOrg>>sUnit;
  ss.clear();
  ss.str(sValueOrg);
  ss>>dValueOrg;

  if(sUnit!="")
  dValueNew=(dValueOrg*G4UIcommand::ValueOf(sUnit.c_str()))/m;
  else dValueNew=dValueOrg;

  if(sKey=="cut_off")
  {
    dCutGamma=dValueNew;
    dCutElectron=dValueNew;
    dCutPositron=dValueNew;
    SetCutValue(dCutGamma*m,"gamma");
    SetCutValue(dCutElectron*m,"e+");
    SetCutValue(dCutPositron*m,"e-");

  }
  else if(sKey=="cut_off.e-")
  {
    dCutElectron=dValueNew;
    SetCutValue(dCutPositron*m,"e-");
  }
  else if(sKey=="cut_off.e+")
  {
    dCutPositron=dValueNew;
    SetCutValue(dCutElectron*m,"e+");
  }
  else if(sKey=="cut_off.gamma")
  {
    dCutGamma=dValueNew;
    SetCutValue(dCutGamma*m,"gamma");
  }
  else 
  {
    std::cout<<"GPPhysicsList: The sKey is not exist: "+sKey<<std::endl;
    return;
  }

  //Update();
  std::cout<<"GPPhysicsList: Set "<<sKey<<": "<< sValueOrg<<" "<<sUnit<<std::endl;
}
