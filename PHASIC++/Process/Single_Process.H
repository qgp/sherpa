#ifndef PHASIC_Process_Single_Process_H
#define PHASIC_Process_Single_Process_H

#include "PHASIC++/Process/Process_Base.H"

namespace PHASIC {

  class Single_Process: public Process_Base {
  protected:

    double m_lastxs, m_lastbxs;
    bool   m_zero;
    int    m_nloct;

    ATOOLS::ME_wgtinfo m_wgtinfo;

    struct BVI_Wgt {
      double m_w, m_c;
      inline BVI_Wgt(const double &w=1.0,
		     const double &c=0.0):
	m_w(w), m_c(c) {}
    };

  public:

    Single_Process();
    virtual ~Single_Process();
    
    size_t Size() const;
    Process_Base *operator[](const size_t &i);

    Weight_Info *OneEvent(const int wmode,const int mode=0);

    double KFactor() const;

    double NLOCounterTerms() const;
    double CollinearCounterTerms
    (const int i,const ATOOLS::Flavour &fl,const ATOOLS::Vec4D &p,
     const double &z,const double &t1,const double &t2) const;
    double GetX(const ATOOLS::Vec4D &p,const int i) const;

    BVI_Wgt BeamISRWeight(const double &Q2,const int mode,
			  const ATOOLS::ClusterAmplitude_Vector &ampls) const;
    void BeamISRWeight(ATOOLS::NLO_subevtlist *const subs,
		       const int mode) const;

    double Differential(const ATOOLS::Vec4D_Vector &p);

    bool CalculateTotalXSec(const std::string &resultpath,
			    const bool create);
    void SetLookUp(const bool lookup);

    void SetScale(const Scale_Setter_Arguments &args);
    void SetKFactor(const KFactor_Setter_Arguments &args);

    ATOOLS::ME_wgtinfo *GetMEwgtinfo();

    ATOOLS::Cluster_Amplitude *Cluster(const ATOOLS::Vec4D_Vector &p,
				       const size_t &mode=0);

    virtual double Partonic(const ATOOLS::Vec4D_Vector &p,
			    const int mode) = 0;

    virtual bool Combinable(const size_t &idi,
			    const size_t &idj);
    virtual const ATOOLS::Flavour_Vector &
    CombinedFlavour(const size_t &idij);

    virtual ATOOLS::Flavour ReMap
    (const ATOOLS::Flavour &fl,const size_t &id) const;

    inline void ResetLastXS() { m_lastxs=0.0; }

    inline double LastXS() const { return m_lastxs; }

    inline bool Zero() const { return m_zero; }

  };// end of class Single_Process

}// end of namespace PHASIC

#endif
