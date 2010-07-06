//
// $Id: GPTrackingAction.hh,v 1.2 2006/06/29 17:43:28 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPTrackingAction_h
#define GPTrackingAction_h 1

#include "G4UserTrackingAction.hh"


class GPTrackingAction : public G4UserTrackingAction {

  public:
    GPTrackingAction(){};
    virtual ~GPTrackingAction(){};
   
    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

};

#endif
