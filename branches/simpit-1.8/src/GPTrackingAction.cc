//
// $Id: GPTrackingAction.cc,v 1.2 2006/06/29 17:44:34 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//



#include "GPTrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "GPTrajectory.hh"
#include "GPTrackInformation.hh"

GPTrackingAction::GPTrackingAction()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTrackingAction::GPTrackingAction()"<<G4endl;
#endif
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTrackingAction::GPTrackingAction()"<<G4endl;
#endif

}
GPTrackingAction::~GPTrackingAction()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTrackingAction::~GPTrackingAction()"<<G4endl;
#endif
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTrackingAction::~GPTrackingAction()"<<G4endl;
#endif
}
void GPTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTrackingAction::PreUserTrackingAction(const G4Track*)"<<G4endl;
#endif
  GPTrackInformation* trackInfo = (GPTrackInformation*)(aTrack->GetUserInformation());
  if(!trackInfo)
  {
    trackInfo = new GPTrackInformation(aTrack);
	fpTrackingManager->SetUserTrackInformation(trackInfo);
  }
  G4String particleName=aTrack->GetDefinition()->GetParticleName();

  // Create trajectory only for track in tracking region
  if((particleName=="e+"||particleName=="e-")&&trackInfo->GetTrackingStatus() > 0)
  {
    fpTrackingManager->SetStoreTrajectory(true);
    GPTrajectory* trajectory = new GPTrajectory(aTrack);
    fpTrackingManager->SetTrajectory(trajectory);
  }
  else
  { 
    fpTrackingManager->SetStoreTrajectory(false); 
  }
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTrackingAction::PreUserTrackingAction(const G4Track*)"<<G4endl;
#endif
}

void GPTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTrackingAction::PostUserTrackingAction(const G4Track*)"<<G4endl;
#endif
  G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
  if(secondaries)
  {
    GPTrackInformation* info = (GPTrackInformation*)(aTrack->GetUserInformation());
    size_t nSeco = secondaries->size();
    if(nSeco>0)
    {
      for(size_t i=0;i<nSeco;i++)
      { 
        GPTrackInformation* infoNew = new GPTrackInformation(info);
        (*secondaries)[i]->SetUserInformation(infoNew);
      }
    }
  }
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTrackingAction::PostUserTrackingAction(const G4Track*)"<<G4endl;
#endif
}


