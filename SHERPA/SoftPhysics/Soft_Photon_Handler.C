#include "SHERPA/SoftPhysics/Soft_Photon_Handler.H"
#include "ATOOLS/Org/Data_Reader.H"
#include "PHOTONS++/Main/Photons.H"

using namespace SHERPA;
using namespace ATOOLS;
using namespace PHOTONS;
using namespace std;


Soft_Photon_Handler::Soft_Photon_Handler(string path,string datfile) :
  m_name(""), m_mode(softphotons::off), p_yfs(NULL)
{
  Data_Reader * dataread = new Data_Reader(" ",";","!","=");
  dataread->AddComment("#");
  dataread->AddWordSeparator("\t");
  dataread->SetInputPath(path);
  dataread->SetInputFile(datfile);

  m_mode = softphotons::code(dataread->GetValue<int>("YFS_MODE",2));
  if (m_mode != softphotons::off)
    p_yfs  = new Photons(dataread);
  if (p_yfs) m_name=p_yfs->Name();
  delete dataread;
}

Soft_Photon_Handler::~Soft_Photon_Handler() 
{
  if (p_yfs) { delete p_yfs; p_yfs = NULL; }
}

bool Soft_Photon_Handler::AddRadiation(Blob * blob)
{
  if (m_mode==softphotons::off) {
    blob->UnsetStatus(blob_status::needs_extraQED);
    return true;
  }
  p_yfs->AddRadiation(blob);
  blob->UnsetStatus(blob_status::needs_extraQED);
  m_photonsadded=p_yfs->AddedAnything();
  return p_yfs->DoneSuccessfully();
}
