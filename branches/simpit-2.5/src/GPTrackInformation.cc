//
// $Id: GPTrackInformation.cc,v 1.2 2006/06/29 17:44:25 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#include "GPTrackInformation.hh"
#include "G4ios.hh"

G4Allocator<GPTrackInformation> GPTrackInformationAllocator;

GPTrackInformation::GPTrackInformation()
{
    iOriginalTrackID = 0;
    particleDefinition = 0;
    vecOriginalPosition = G4ThreeVector(0.,0.,0.);
    vecOriginalMomentum = G4ThreeVector(0.,0.,0.);
    dOriginalEnergy = 0.;
    dOriginalTime = 0.;
    iTrackingStatus = 1;
    iSourceTrackID = -1;
    iSourceTrackID = -1;
    sourceDefinition = 0;
    vecSourcePosition = G4ThreeVector(0.,0.,0.);
    vecSourceMomentum = G4ThreeVector(0.,0.,0.);
    dSourceEnergy = 0.;
    dSourceTime = 0.;
}

GPTrackInformation::GPTrackInformation(const G4Track* aTrack)
{
    iOriginalTrackID = aTrack->GetTrackID();
    particleDefinition = aTrack->GetDefinition();
    vecOriginalPosition = aTrack->GetPosition();
    vecOriginalMomentum = aTrack->GetMomentum();
    dOriginalEnergy = aTrack->GetTotalEnergy();
    dOriginalTime = aTrack->GetGlobalTime();
    iTrackingStatus = 1;
    iSourceTrackID = -1;
    sourceDefinition = 0;
    vecSourcePosition = G4ThreeVector(0.,0.,0.);
    vecSourceMomentum = G4ThreeVector(0.,0.,0.);
    dSourceEnergy = 0.;
    dSourceTime = 0.;
}

GPTrackInformation::GPTrackInformation(const GPTrackInformation* aTrackInfo)
{
    iOriginalTrackID = aTrackInfo->iOriginalTrackID;
    particleDefinition = aTrackInfo->particleDefinition;
    vecOriginalPosition = aTrackInfo->vecOriginalPosition;
    vecOriginalMomentum = aTrackInfo->vecOriginalMomentum;
    dOriginalEnergy = aTrackInfo->dOriginalEnergy;
    dOriginalTime = aTrackInfo->dOriginalTime;
    iTrackingStatus = aTrackInfo->iTrackingStatus;
    iSourceTrackID = aTrackInfo->iSourceTrackID;
    sourceDefinition = aTrackInfo->sourceDefinition;
    vecSourcePosition = aTrackInfo->vecSourcePosition;
    vecSourceMomentum = aTrackInfo->vecSourceMomentum;
    dSourceEnergy = aTrackInfo->dSourceEnergy;
    dSourceTime = aTrackInfo->dSourceTime;
}

GPTrackInformation::~GPTrackInformation()
{ 
}

GPTrackInformation& GPTrackInformation::operator =(const GPTrackInformation& aTrackInfo)
{
    iOriginalTrackID = aTrackInfo.iOriginalTrackID;
    particleDefinition = aTrackInfo.particleDefinition;
    vecOriginalPosition = aTrackInfo.vecOriginalPosition;
    vecOriginalMomentum = aTrackInfo.vecOriginalMomentum;
    dOriginalEnergy = aTrackInfo.dOriginalEnergy;
    dOriginalTime = aTrackInfo.dOriginalTime;
    iTrackingStatus = aTrackInfo.iTrackingStatus;
    iSourceTrackID = aTrackInfo.iSourceTrackID;
    sourceDefinition = aTrackInfo.sourceDefinition;
    vecSourcePosition = aTrackInfo.vecSourcePosition;
    vecSourceMomentum = aTrackInfo.vecSourceMomentum;
    dSourceEnergy = aTrackInfo.dSourceEnergy;
    dSourceTime = aTrackInfo.dSourceTime;

    return *this;
}

void GPTrackInformation::SetSourceTrackInformation(const G4Track* aTrack)
{
    iSourceTrackID = aTrack->GetTrackID();
    sourceDefinition = aTrack->GetDefinition();
    vecSourcePosition = aTrack->GetPosition();
    vecSourceMomentum = aTrack->GetMomentum();
    dSourceEnergy = aTrack->GetTotalEnergy();
    dSourceTime = aTrack->GetGlobalTime();
}

void GPTrackInformation::Print() const
{
    G4cout 
     << "Source track ID " << iSourceTrackID << " (" << sourceDefinition->GetParticleName() << ","
     << dSourceEnergy/GeV << "[GeV]) at " << vecSourcePosition << G4endl;
    G4cout
     << "Original primary track ID " << iOriginalTrackID << " (" << particleDefinition->GetParticleName() << ","
     << dOriginalEnergy/GeV << "[GeV])" << G4endl;
}

