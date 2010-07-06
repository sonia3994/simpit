//
// $Id: GPTrackInformation.cc,v 1.2 2006/06/29 17:44:25 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#include "GPTrackInformation.hh"
#include "G4ios.hh"

G4Allocator<GPTrackInformation> GPTrackInformationAllocator;

GPTrackInformation::GPTrackInformation()
{
    originalTrackID = 0;
    particleDefinition = 0;
    originalPosition = G4ThreeVector(0.,0.,0.);
    originalMomentum = G4ThreeVector(0.,0.,0.);
    originalEnergy = 0.;
    originalTime = 0.;
    trackingStatus = 1;
    sourceTrackID = -1;
    sourceTrackID = -1;
    sourceDefinition = 0;
    sourcePosition = G4ThreeVector(0.,0.,0.);
    sourceMomentum = G4ThreeVector(0.,0.,0.);
    sourceEnergy = 0.;
    sourceTime = 0.;
}

GPTrackInformation::GPTrackInformation(const G4Track* aTrack)
{
    originalTrackID = aTrack->GetTrackID();
    particleDefinition = aTrack->GetDefinition();
    originalPosition = aTrack->GetPosition();
    originalMomentum = aTrack->GetMomentum();
    originalEnergy = aTrack->GetTotalEnergy();
    originalTime = aTrack->GetGlobalTime();
    trackingStatus = 1;
    sourceTrackID = -1;
    sourceDefinition = 0;
    sourcePosition = G4ThreeVector(0.,0.,0.);
    sourceMomentum = G4ThreeVector(0.,0.,0.);
    sourceEnergy = 0.;
    sourceTime = 0.;
}

GPTrackInformation::GPTrackInformation(const GPTrackInformation* aTrackInfo)
{
    originalTrackID = aTrackInfo->originalTrackID;
    particleDefinition = aTrackInfo->particleDefinition;
    originalPosition = aTrackInfo->originalPosition;
    originalMomentum = aTrackInfo->originalMomentum;
    originalEnergy = aTrackInfo->originalEnergy;
    originalTime = aTrackInfo->originalTime;
    trackingStatus = aTrackInfo->trackingStatus;
    sourceTrackID = aTrackInfo->sourceTrackID;
    sourceDefinition = aTrackInfo->sourceDefinition;
    sourcePosition = aTrackInfo->sourcePosition;
    sourceMomentum = aTrackInfo->sourceMomentum;
    sourceEnergy = aTrackInfo->sourceEnergy;
    sourceTime = aTrackInfo->sourceTime;
}

GPTrackInformation::~GPTrackInformation()
{ ; }

GPTrackInformation& GPTrackInformation::operator =(const GPTrackInformation& aTrackInfo)
{
    originalTrackID = aTrackInfo.originalTrackID;
    particleDefinition = aTrackInfo.particleDefinition;
    originalPosition = aTrackInfo.originalPosition;
    originalMomentum = aTrackInfo.originalMomentum;
    originalEnergy = aTrackInfo.originalEnergy;
    originalTime = aTrackInfo.originalTime;
    trackingStatus = aTrackInfo.trackingStatus;
    sourceTrackID = aTrackInfo.sourceTrackID;
    sourceDefinition = aTrackInfo.sourceDefinition;
    sourcePosition = aTrackInfo.sourcePosition;
    sourceMomentum = aTrackInfo.sourceMomentum;
    sourceEnergy = aTrackInfo.sourceEnergy;
    sourceTime = aTrackInfo.sourceTime;

    return *this;
}

void GPTrackInformation::SetSourceTrackInformation(const G4Track* aTrack)
{
    sourceTrackID = aTrack->GetTrackID();
    sourceDefinition = aTrack->GetDefinition();
    sourcePosition = aTrack->GetPosition();
    sourceMomentum = aTrack->GetMomentum();
    sourceEnergy = aTrack->GetTotalEnergy();
    sourceTime = aTrack->GetGlobalTime();
}

void GPTrackInformation::Print() const
{
    G4cout 
     << "Source track ID " << sourceTrackID << " (" << sourceDefinition->GetParticleName() << ","
     << sourceEnergy/GeV << "[GeV]) at " << sourcePosition << G4endl;
    G4cout
     << "Original primary track ID " << originalTrackID << " (" << particleDefinition->GetParticleName() << ","
     << originalEnergy/GeV << "[GeV])" << G4endl;
}

