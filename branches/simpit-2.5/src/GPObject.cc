// $Id: GPObject.cc,v 1.9 2006/06/29 17:47:19 gunter Granular $
// GEANT4 tag $Name: geant4-09-02 $
//

#include "GPObject.hh"
GPObject::GPObject()
{}

GPObject::~GPObject()
{}
void GPObject::Delete(){;}

// Set or get parameter, the parameter maybe not exists.
void GPObject::SetParameter(std::string, std::string)
{}
double GPObject::GetParameter(std::string, std::string)const 
{
  return 0;
}

// Set or get object's name.
std::string GPObject::GetName()const
{
  return sName;
}
void GPObject::SetName(std::string sValue)
{
  sName=sValue;
}

// Set or get object's father name.
std::string GPObject::GetFatherName()const
{
  return sFatherName;
}
void GPObject::SetFatherName(std::string sValue)
{
  sFatherName=sValue;
}

// Set or get object's active flag.
int GPObject::IsActive()const
{
  return iActiveFlag;
}
void GPObject::SetActive(int i)
{
  iActiveFlag=i;
}

// For manager class.
GPObject* GPObject::FindObject(std::string)const
{
  return NULL;
}

// For manager class.
void GPObject::SetObject(std::string,GPObject*)
{}

// Print on the screen or into a file.
void GPObject::Print()
{}
void GPObject::Print(std::ofstream&)
{}

// Read from a file.
void GPObject::Read(std::string)
{}

//Update .
void GPObject::Update()
{}
void GPObject::Init()
{}
int GPObject::GetVerbose()const
{
}
void GPObject::SetVerbose(int i)
{
  iVerbose = i;
}
