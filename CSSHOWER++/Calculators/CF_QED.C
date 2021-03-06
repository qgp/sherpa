#include "CSSHOWER++/Showers/Splitting_Function_Base.H"

#include "MODEL/Interaction_Models/Single_Vertex.H"
#include "ATOOLS/Org/Run_Parameter.H"
#include "MODEL/Main/Model_Base.H"
#include "ATOOLS/Org/Exception.H"

namespace CSSHOWER {
  
  class CF_QED: public SF_Coupling {
  protected:

    ATOOLS::Function_Base *p_cpl;
    ATOOLS::Flavour m_cfl;

    double m_q;

  public:

    inline CF_QED(const SF_Key &key):
      SF_Coupling(key), m_cfl(key.p_v->in[0])
    {
      if (key.m_type==cstp::IF || key.m_type==cstp::II)
	m_cfl=key.p_v->in[key.m_mode==0?1:2];
      m_q=ATOOLS::dabs(m_cfl.IntCharge()?m_cfl.Charge():
		       key.p_v->in[key.m_mode==0?2:1].Charge());
      if (m_q==0.0) THROW(fatal_error,"Internal error");
    }

    bool SetCoupling(MODEL::Model_Base *md,
		     const double &k0sqi,const double &k0sqf,
		     const double &isfac,const double &fsfac);
    double Coupling(const double &scale,const int pol);
    bool AllowSpec(const ATOOLS::Flavour &fl);

  };

}

using namespace CSSHOWER;
using namespace ATOOLS;

bool CF_QED::SetCoupling(MODEL::Model_Base *md,
			 const double &k0sqi,const double &k0sqf,
			 const double &isfac,const double &fsfac)
{
  p_cpl=md->GetScalarFunction("alpha_QED");
  m_cplfac=1.0;
  m_cplmax.push_back((*p_cpl)(rpa->gen.CplScale())*m_q);
  m_cplmax.push_back(0.0);
  return true;
}

double CF_QED::Coupling(const double &scale,const int pol)
{
  if (pol!=0) return 0.0;
  if (scale<0.0) return
    m_cplmax.front()*m_q*dabs(p_lf->FlSpec().Charge());
  double scl(CplFac(scale)*scale);
  return (*p_cpl)(scl)*m_q*dabs(p_lf->FlSpec().Charge());
}

bool CF_QED::AllowSpec(const ATOOLS::Flavour &fl) 
{
  if (!fl.Strong() && fl.Mass()>10.0) return false;
  if (m_cfl.IntCharge()==0) return fl.Charge();
  
  switch (m_type) {
  case cstp::FF:
  case cstp::II:
    return fl.IntCharge()*m_cfl.IntCharge()<0;
  case cstp::FI:
  case cstp::IF:
    return fl.IntCharge()*m_cfl.IntCharge()>0;
  default:
    return false;
  }
}

namespace CSSHOWER {

DECLARE_CPL_GETTER(CF_QED_Getter);

SF_Coupling *CF_QED_Getter::operator()
  (const Parameter_Type &args) const
{
  return new CF_QED(args);
}

void CF_QED_Getter::PrintInfo
(std::ostream &str,const size_t width) const
{
  str<<"electromagnetic coupling";
}

}

DECLARE_GETTER(CF_QED_Getter,"SF_QED_Fill",
	       void,SFC_Filler_Key);

void *ATOOLS::Getter<void,SFC_Filler_Key,CF_QED_Getter>::
operator()(const SFC_Filler_Key &key) const
{
  if (!Flavour(kf_photon).IsOn()) return NULL;
  std::string ptag("{"+Flavour(kf_photon).IDName()+"}");
  for (int i(1);i<=16;++i) {
    if (i==7) i=11;
    Flavour f((kf_code)i);
    if (!f.IsOn() || f.IntCharge()==0) continue;
    std::string qtag("{"+f.IDName()+"}");
    std::string qbtag ("{"+f.Bar().IDName()+"}");
    key.p_gets->push_back(new CF_QED_Getter(ptag+qtag+qbtag));
    key.p_gets->push_back(new CF_QED_Getter(qbtag+qbtag+ptag));
    key.p_gets->push_back(new CF_QED_Getter(qtag+qtag+ptag));
  }
  return NULL;
}

void ATOOLS::Getter<void,SFC_Filler_Key,CF_QED_Getter>::
PrintInfo(std::ostream &str,const size_t width) const
{
  str<<"qed coupling filler";
}


