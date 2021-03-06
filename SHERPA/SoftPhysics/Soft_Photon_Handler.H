#ifndef SHERPA_SoftPhysics_Soft_Photon_Handler_H
#define SHERPA_SoftPhysics_Soft_Photon_Handler_H

#include "ATOOLS/Phys/Blob.H"
#include <string>

namespace PHOTONS {
  class Photons;
}

namespace SHERPA {
  struct softphotons {
    enum code {
      off        = 0,
      exp        = 1,
      exp_order1 = 2,
      exp_order2 = 3
    };
  };

  class Soft_Photon_Handler {
  private:
    bool                m_photonsadded;
    std::string         m_name;
    softphotons::code   m_mode;
    PHOTONS::Photons  * p_yfs;
  public :
    // constructor
    Soft_Photon_Handler(std::string path=std::string("./"),
			std::string datfile=std::string("Fragmentation.dat"));
    // destructor
    ~Soft_Photon_Handler();
    bool AddRadiation(ATOOLS::Blob *);

    inline bool        IsOn()             { return m_mode; }
    inline std::string SoftQEDGenerator() { return m_name; }
    inline size_t      AddedAnything()    { return m_photonsadded; }
  };// end of class Soft_Photon_Handler

}// end of namespace SHERPA

#endif

