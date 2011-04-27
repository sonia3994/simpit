//
// $Id: GPObject.hh,v 1.6 
// GEANT4 tag $Name: geant4-09-02 $
//

#ifndef GPOBJECT_H
#define GPOBJECT_H 1
#include <string>
#include <sstream>
#include <fstream>


class GPObject
{
  public:

    GPObject();
    virtual ~GPObject();

    // Should be an another method of "destruction".
    virtual void Delete(){;};

    // Set or get parameter, the parameter maybe not exists.
    virtual void SetParameter(std::string, std::string){;};
    virtual double GetParameter(std::string, std::string)const {return 0;};

    // Set or get object's name.
    inline std::string GetName(){return sName;};
    inline void SetName(std::string sValue){sName=sValue;};

    // Set or get object's father name.
    inline std::string GetFatherName(){return sFatherName;};
    inline void SetFatherName(std::string sValue){sFatherName=sValue;};
    
    // Set or get object's active flag.
    inline int IsActive(){return iActiveFlag;};
    inline void SetActive(int i){iActiveFlag=i;};

    // For manager class.
    virtual GPObject* FindObject(std::string)const{return NULL;};

    // For manager class.
    virtual void SetObject(std::string,GPObject*){;};

    // Print on the screen or into a file.
    virtual void Print(){;};
    virtual void Print(std::ofstream&){;};

    // Read from a file.
    virtual void Read(std::string){;};

    //Update .
    virtual void Update(){;};
  protected:
    virtual void Init(){;};
  protected:
    int iActiveFlag;
    std::string sName;
    std::string sFatherName;
};

#endif

