#ifndef PHASIC_Process_Process_Base_H
#define PHASIC_Process_Process_Base_H

#include "PHASIC++/Process/Process_Info.H"
#include "PHASIC++/Selectors/Selector.H"
#include "PHASIC++/Selectors/Cut_Data.H"
#include "PHASIC++/Scales/Scale_Setter_Base.H"
#include "PHASIC++/Scales/KFactor_Setter_Base.H"
#include "ATOOLS/Phys/NLO_Subevt.H"

namespace ATOOLS { 
  class Cluster_Leg;
  class Cluster_Amplitude; 
  class Histogram;
  struct Decay_Info;
  typedef std::vector<Decay_Info* > DecayInfo_Vector;
}

namespace BEAM { class Beam_Spectra_Handler; }

namespace PDF { 
  class ISR_Handler;
  class Shower_Base;
}

namespace METOOLS { class Spin_Amplitudes; }

namespace PHASIC {

  typedef std::map<int,int> FMMap;

  class Process_Integrator;
  class Phase_Space_Handler;
  class Combined_Selector;
  class ME_Generator_Base;

  struct Weight_Info;

  class Process_Base;
  class BBar_Multi_Channel;

  typedef std::vector<PHASIC::Process_Base*> Process_Vector;

  typedef std::map<std::string,Process_Base*> StringProcess_Map;

  typedef std::map<nlo_type::code,StringProcess_Map*>
  NLOTypeStringProcessMap_Map;

  class Process_Base {
  protected:

    Process_Base       *p_parent, *p_selected, *p_mapproc, *p_sproc;
    Process_Integrator *p_int;
    Combined_Selector  *p_selector;
    Cut_Data           *p_cuts;
    ME_Generator_Base  *p_gen;

    PDF::Shower_Base   *p_shower;
    BBar_Multi_Channel *p_mc;


    Scale_Setter_Base   *p_scale;
    KFactor_Setter_Base *p_kfactor;

    size_t      m_nin, m_nout, m_oqcd, m_oew, m_tinfo, m_mcmode, m_cmode;
    std::string m_name;

    ATOOLS::Flavour_Vector m_flavs;
    Process_Info           m_pinfo;
    ATOOLS::DecayInfo_Vector m_decins;

    double m_last, m_lastb, m_symfac;
    bool   m_lookup, m_use_biweight;

    MODEL::Coupling_Map m_cpls;

    NLOTypeStringProcessMap_Map *p_apmap;

    static void SortFlavours(Subprocess_Info &info,FMMap *const fmm=0);
    static void SortFlavours(std::vector<ATOOLS::Cluster_Leg*> &legs,
			     FMMap *const fmm=0);

  public:

    Process_Base();
    virtual ~Process_Base();

    // member functions
    Process_Base *Parent();
    Process_Base *Selected();

    bool SetSelected(Process_Base *const proc);

    double LastPlus();
    double LastMinus();

    void SetSProc(Process_Base *sproc);
    void SetBBarMC(BBar_Multi_Channel *mc);

    virtual void FillProcessMap(NLOTypeStringProcessMap_Map *apmap);

    virtual void SetScale(const Scale_Setter_Arguments &args) = 0;
    virtual void SetKFactor(const KFactor_Setter_Arguments &args) = 0;

    virtual void SetFixedScale(const std::vector<double> &s);
    virtual void SetSelectorOn(const bool on);
    virtual void SetUseBIWeight(bool on);

    virtual size_t SetMCMode(const size_t mcmode);
    virtual size_t SetClusterMode(const size_t mcmode);

    virtual size_t Size() const = 0;
    virtual Process_Base *operator[](const size_t &i) = 0;

    virtual Weight_Info *OneEvent(const int wmode,const int mode=0) = 0;

    virtual double Differential(const ATOOLS::Vec4D_Vector &p) = 0;
    virtual double Differential(const ATOOLS::Cluster_Amplitude &ampl,
                                int mode=0);

    virtual bool GeneratePoint();
    virtual void AddPoint(const double &value);
    virtual bool ReadIn(const std::string &pid);
    virtual void WriteOut(const std::string &pid);
    virtual void EndOptimize();
    virtual void MPISync();

    virtual bool IsGroup() const;

    virtual bool CalculateTotalXSec(const std::string &resultpath,
				    const bool create=false) = 0;
    virtual void SetLookUp(const bool lookup) = 0;

    virtual void Init(const Process_Info &pi,
		      BEAM::Beam_Spectra_Handler *const beamhandler,
		      PDF::ISR_Handler *const isrhandler,const int mode=0);

    virtual bool FillIntegrator(Phase_Space_Handler *const psh);
    virtual bool InitIntegrator(Phase_Space_Handler *const psh);
    virtual void UpdateIntegrator(Phase_Space_Handler *const psh);

    virtual void SetGenerator(ME_Generator_Base *const gen);
    virtual void SetShower(PDF::Shower_Base *const ps);
    virtual void SetSelector(const Selector_Key &key);
    virtual void SetUpThreading();

    virtual void InitCuts(Cut_Data *const cuts);
    virtual void BuildCuts(Cut_Data *const cuts);

    virtual bool Trigger(const ATOOLS::Vec4D_Vector &p);

    virtual ATOOLS::NLO_subevtlist *GetSubevtList();
    virtual ATOOLS::NLO_subevtlist *GetRSSubevtList();
    virtual ATOOLS::ME_wgtinfo     *GetMEwgtinfo();

    virtual void SetRBMap(ATOOLS::Cluster_Amplitude *ampl);
    virtual void InitPSHandler(const double &maxerror,
			       const std::string eobs,
			       const std::string efunc);

    virtual void FillOnshellConditions();

    static void SortFlavours(ATOOLS::Cluster_Amplitude *const ampl,
			     const int mode=0);
    static void SortFlavours(Process_Info &pi,const int mode=0);

    static std::string GenerateName(const ATOOLS::Cluster_Amplitude *ampl);
    static std::string GenerateName(const ATOOLS::NLO_subevt *sub,
				    const size_t &nin);

    static std::string GenerateName(const Subprocess_Info &info);
    static std::string GenerateName(const Subprocess_Info &ii,
				    const Subprocess_Info &fi);

    // inline functions
    inline void SetParent(Process_Base *const proc) { p_parent=proc; }

    inline size_t NIn() const  { return m_nin;  }
    inline size_t NOut() const { return m_nout; }

    inline const ATOOLS::Flavour_Vector &Flavours() const { return m_flavs; }

    inline const std::string &Name() const { return m_name; }

    inline void SetOrderQCD(const size_t &oqcd) { m_oqcd=oqcd; }
    inline void SetOrderEW(const size_t &oew)   { m_oew=oew;   }

    inline size_t OrderQCD() const { return m_oqcd; }
    inline size_t OrderEW() const  { return m_oew;  }

    PDF::Shower_Base *Shower() const { return p_shower; }

    inline Process_Info       &Info()       { return m_pinfo; }
    inline const Process_Info &Info() const { return m_pinfo; }

    inline ATOOLS::DecayInfo_Vector       &DecayInfos()       {return m_decins;}
    inline const ATOOLS::DecayInfo_Vector &DecayInfos() const {return m_decins;}

    inline Process_Integrator *Integrator() const { return p_int; }

    virtual void FillAmplitudes(std::vector<METOOLS::Spin_Amplitudes>& amps,
                                std::vector<std::vector<Complex> >& cols);

    inline double Last() const { return m_last; }
    inline double LastB() const { return m_lastb; }

    inline bool LookUp() const { return m_lookup; }

    inline double SymFac() const { return m_symfac; }

    inline Scale_Setter_Base *ScaleSetter(const int map=0) const
    { return map?(p_mapproc?p_mapproc->p_scale:p_scale):p_scale; }
    inline KFactor_Setter_Base *KFactorSetter(const int map=0) const
    { return map?(p_mapproc?p_mapproc->p_kfactor:p_kfactor):p_kfactor; }

    inline bool IsMapped() const { return p_mapproc; }

    inline size_t ThreadInfo() const { return m_tinfo; }

    inline size_t MCMode() const      { return m_mcmode; }
    inline size_t ClusterMode() const { return m_cmode;  }

    inline ME_Generator_Base *Generator() const { return p_gen; }

    inline Process_Base *MapProc() const  { return p_mapproc; }

    inline Cut_Data          * Cuts() const { return p_cuts; }
    inline Combined_Selector * Selector() const 
    { return p_mapproc?p_mapproc->p_selector:p_selector; }

    template <class PType> inline PType *Get() 
    { return dynamic_cast<PType*>(this); }
    template <class PType> inline const PType *Get() const 
    { return dynamic_cast<PType*>(this); }

    inline NLOTypeStringProcessMap_Map *AllProcs() { return p_apmap; }

    inline BBar_Multi_Channel *BBarMC() const { return p_mc; }

  };// end of class Process_Base

}// end of namespace ATOOLS

#endif
