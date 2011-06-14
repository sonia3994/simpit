//
// $Id: GPTrajectoryAction.hh,v 1.2 2006/06/29 17:43:28 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPTRAJECTORYACTION_H
#define GPTRAJECTORYACTION_H 1

#include "G4Types.hh"
#include <fstream>
class G4TrajectoryContainer;
class GPTrajectory;

class GPTrajectoryAction 
{

  public:
    static GPTrajectoryAction* GetGPTrajectoryAction();
    virtual ~GPTrajectoryAction();
   
    virtual void ProcessTrajectory(G4TrajectoryContainer*);
    void SetOfstream(std::ofstream* ofs){ofstream=ofs;};
  protected:
    GPTrajectoryAction();
    static GPTrajectoryAction* claGPTrajectoryAction;
    G4bool TrajectoryFilter(GPTrajectory*);
    std::ofstream* ofstream;

};

#endif
