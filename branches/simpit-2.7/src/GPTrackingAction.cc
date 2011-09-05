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

}
GPTrackingAction::~GPTrackingAction()
{
}
void GPTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  GPTrackInformation* trackInfo = (GPTrackInformation*)(aTrack->GetUserInformation());
  if(!trackInfo)
  {
    trackInfo = new GPTrackInformation(aTrack);
	fpTrackingManager->SetUserTrackInformation(trackInfo);
  }
  G4String particleName=aTrack->GetDefinition()->GetParticleName();

  
  /*
  // Create trajectory only for track in tracking region
  //if((particleName=="e+"||particleName=="e-"||particleName=="gamma")&&trackInfo->GetTrackingStatus() > 0)
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
  */
  
}

void GPTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
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
}


