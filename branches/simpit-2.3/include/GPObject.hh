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
    virtual void Delete();

    // Set or get parameter, the parameter maybe not exists.
    virtual void SetParameter(std::string, std::string);
    virtual double GetParameter(std::string, std::string)const;

    // Set or get object's name.
    std::string GetName();
    void SetName(std::string sValue);

    // Set or get object's father name.
    std::string GetFatherName();
    void SetFatherName(std::string sValue);
    
    // Set or get object's active flag.
    int IsActive();
    void SetActive(int i);

    int GetVerbose();
    void SetVerbose(int i);

    // For manager class.
    virtual GPObject* FindObject(std::string)const;

    // For manager class.
    virtual void SetObject(std::string,GPObject*);

    // Print on the screen or into a file.
    virtual void Print();
    virtual void Print(std::ofstream&);

    // Read from a file.
    virtual void Read(std::string);

    //Update .
    virtual void Update();
  protected:
    virtual void Init();
  private:
    int iActiveFlag;
    int iVerbose;
    std::string sName;
    std::string sFatherName;
};

#endif

