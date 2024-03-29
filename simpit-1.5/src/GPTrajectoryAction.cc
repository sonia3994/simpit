//
// $Id: GPTrajectoryAction.cc,v 1.2 2006/06/29 17:44:34 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//



#include "GPTrajectoryAction.hh"
#include "GPTrajectory.hh"
#include "G4TrajectoryContainer.hh"
//#include "G4TrackingManager.hh"
//#include "G4Track.hh"
//#include "GPTrajectory.hh"
//#include "GPTrackInformation.hh"

GPTrajectoryAction* GPTrajectoryAction::claGPTrajectoryAction = 0;

GPTrajectoryAction* GPTrajectoryAction::GetGPTrajectoryAction()
{
  if(claGPTrajectoryAction)
    return claGPTrajectoryAction;
  else
    claGPTrajectoryAction = new GPTrajectoryAction();
  return claGPTrajectoryAction;
}

GPTrajectoryAction::GPTrajectoryAction()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTrajectoryAction::GPTrajectoryAction()"<<G4endl;
#endif
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTrajectoryAction::GPTrajectoryAction()"<<G4endl;
#endif
}

GPTrajectoryAction::~GPTrajectoryAction()
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTrajectoryAction::~GPTrajectoryAction()"<<G4endl;
#endif
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTrajectoryAction::~GPTrajectoryAction()"<<G4endl;
#endif
}

void GPTrajectoryAction::ProcessTrajectory(G4TrajectoryContainer* trajectoryContainer)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTrajectoryAction::PreUserTrackingAction(const G4Track*)"<<G4endl;
#endif
  G4int iTrajectories = trajectoryContainer->entries();
  //extract the trajectories and process. 
  for(G4int i=0; i<iTrajectories; i++) 
  {
    GPTrajectory* trj = (GPTrajectory*)((*trajectoryContainer)[i]);
    if(TrajectoryFilter(trj))
    {
      //trj->ProcessTrajectory(0);
      trj->SaveAs(ofstream);
    }
  }
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Exit GPTrajectoryAction::PreUserTrackingAction(const G4Track*)"<<G4endl;
#endif
}

G4bool GPTrajectoryAction::TrajectoryFilter(GPTrajectory* trajectory)
{
  //G4double dLimitZ=100;
  G4double dLimitR=10;
  G4double dLimitT=20;
  G4double dTrackLength;
  //G4double dZLength;
  G4double dRValue;
  G4int pdg;
  pdg=trajectory->GetPDGEncoding();
  //dZLength=trajectory->GetZLength();
  dTrackLength=trajectory->GetTrackLength();
  dRValue=trajectory->GetRAtZ(83);

  if(pdg==-11||pdg==11)
  {
    if(dTrackLength>dLimitT)
      return TRUE;
    //if(dZLength<dLimitZ)
    //return FALSE;
    if(dRValue>0&&dRValue<dLimitR)
      return TRUE;
  }

  return FALSE;
}
/*
void GPTrajectoryAction::PostUserTrackingAction(const G4Track* aTrack)
{
#ifdef GP_DEBUG
  G4cout<<"GP_DEBUG: Enter GPTrajectoryAction::PostUserTrackingAction(const G4Track*)"<<G4endl;
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
  G4cout<<"GP_DEBUG: Exit GPTrajectoryAction::PostUserTrackingAction(const G4Track*)"<<G4endl;
#endif
}

*/

