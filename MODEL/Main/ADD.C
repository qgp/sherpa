#include "MODEL/Main/ADD.H"
#include "ATOOLS/Org/Message.H"
#include "MODEL/Main/Standard_Model.H"
#include "MODEL/Main/Spectrum_Generator_Base.H"
#include <iomanip>

using namespace MODEL;
using namespace ATOOLS;

DECLARE_GETTER(ADD,"ADD",Model_Base,Model_Arguments);

Model_Base *Getter<Model_Base,Model_Arguments,ADD>::
operator()(const Model_Arguments &args) const
{
  return new ADD(args.m_path,args.m_file,args.m_elementary);
}

void Getter<Model_Base,Model_Arguments,ADD>::
PrintInfo(std::ostream &str,const size_t width) const
{ 
  str<<"The ADD model of large extra dimensions\n";
  str<<std::setw(width+4)<<" "<<"{\n"
     <<std::setw(width+7)<<" "<<"parameter specification [keyword=value]\n"
     <<std::setw(width+7)<<" "<<"- all the SM parameters\n"
     <<std::setw(width+7)<<" "<<"- N_ED (number of extra dimensions)\n"
     <<std::setw(width+7)<<" "<<"- G_NEWTON (Newton's gravity constant)\n"
     <<std::setw(width+7)<<" "<<"- KK_CONVENTION (values 0,1,2,3,4,5, see documentation)\n"
     <<std::setw(width+7)<<" "<<"- M_S (string scale, depends on KK_CONVENTION)\n"
     <<std::setw(width+7)<<" "<<"- M_CUT (cut-off scale for c.m. energy)\n"
     <<std::setw(width+4)<<" "<<"}\n";
}


ADD::ADD(std::string _dir,std::string _file,bool _elementary) :
  Model_Base(_dir,_file,_elementary)
{
  p_sm = new Standard_Model(m_dir,m_file,false);
  ParticleInit();
  if (m_elementary) {
    ATOOLS::OutputParticles(msg->Info());
    ATOOLS::OutputContainers(msg->Info());
  }
}

bool ADD::ModelInit(const PDF::ISR_Handler_Map& isr)
{
  msg_Info()<<"Initialize the ADD from "<<m_dir<<" / "<<m_file<<std::endl;
  m_name      = std::string("ADD");

  p_sm->ModelInit(isr);
  p_numbers   = p_sm->ExtractScalarNumbers();
  p_constants = p_sm->ExtractScalarConstants();
  p_complexconstants = p_sm->ExtractComplexConstants();
  p_functions = p_sm->ExtractScalarFunctions();
  p_matrices  = p_sm->ExtractComplexMatrices();

  delete p_sm;

  FillSpectrum(isr);

  return true;
}

ADD::~ADD()
{
}

void ADD::ParticleInit() {
  //add the graviton and the gravi-scalar
  s_kftable[39] = new Particle_Info(39,100.,0.,0,0,0,4,-1,1,1,1,"graviton","G");
  s_kftable[40] = new Particle_Info(40,100.,0.,0,0,0,0,-1,0,1,1,"gscalar","G_s");

  ReadParticleData();
}

void ADD::FillSpectrum(const PDF::ISR_Handler_Map& isr) {
  p_dataread->RereadInFile();

  
  p_constants->insert(std::make_pair(std::string("G_Newton"), 
				     p_dataread->GetValue<double>("G_NEWTON",6.707E-39)));
  p_numbers->insert(std::make_pair(std::string("ED"), 
				     p_dataread->GetValue<int>("N_ED",2)));
  p_constants->insert(std::make_pair(std::string("M_s"), 
				     p_dataread->GetValue<double>("M_S",0.)));
  p_constants->insert(std::make_pair(std::string("M_cut"), 
				     p_dataread->GetValue<double>("M_CUT",ScalarConstant(std::string("M_s")))));
  p_constants->insert(std::make_pair(std::string("Radius"), 
				     p_dataread->GetValue<double>("RADIUS",0.)));
  p_numbers->insert(std::make_pair(std::string("KK_mode"), 
				     p_dataread->GetValue<int>("KK_CONVENTION",1)));


  int    mode = ScalarNumber(std::string("KK_mode"));
  double rad = ScalarConstant(std::string("Radius"));
  int    ed  = ScalarNumber(std::string("ED"));
  double gn  = ScalarConstant(std::string("G_Newton"));
  double ms  = ScalarConstant(std::string("M_s"));

  switch(mode){
  case 1:case 2:                            //HLZ
    //Calculation of Gamma(ed/2)
    double gam;
    if(ed%2==0) gam=1.;
    else gam=sqrt(M_PI);
    for(int i=2-ed%2;i<ed;i+=2)gam*=0.5*i;
    
    //If Radius is set but not the scale M_s, M_s is calculated
    if (IsZero(ms) && rad > 0.) {
      ms=pow(gam*pow(4.*M_PI,.5*ed)/pow(2.*M_PI*rad,1.*ed)/gn,1./(2.+ed));
      (*p_constants)[std::string("M_s")] = ms;
    }
    
    (*p_constants)[std::string("Radius")] = 
      pow(gam*pow(4.*M_PI,.5*ed)/pow(ms,2.+(double(ed)))/gn/2.,1./(double(ed)))/2/M_PI;
    break;
  case 5:                                   //GRW
    //If Radius is set but not the scale M_s, M_s is calculated
    if (IsZero(ms) && rad > 0.) {
      ms=pow(8.*M_PI*pow(rad,1.*ed)*gn,-1./(2.+ed));
      (*p_constants)[std::string("M_s")] = ms;
    }
    
    (*p_constants)[std::string("Radius")] = pow(8.*M_PI*pow(ms,2.+(double(ed)))*gn,-1./(double(ed)));
  }

  p_constants->insert(std::make_pair(std::string("kappa"),sqrt(8.*M_PI*gn))); 
  p_constants->insert(std::make_pair(std::string("omega"),sqrt(4.*(-1.+ed)/(3.*(2.+ed)))));
  p_constants->insert(std::make_pair(std::string("M2_s"),sqr(ms)));
}


