
pkglib_LTLIBRARIES    =	libPhasicScales.la

SVNTAG = PHASIC++/Scales
include ../../svn.make

libPhasicScales_la_SOURCES    =	\
				SVN_Info.C \
				Scale_Setter_Base.C \
				Core_Scale_Setter.C \
				Default_Core_Scale.C \
                                Higgs_Core_Scale.C \
				KFactor_Setter_Base.C \
				Tag_Setter.C \
				Variable_Scale_Setter.C \
				Variable_Core_Scale.C \
				Fastjet_Scale_Setter.C \
				MPI_Scale_Setter.C \
				QCD_Scale_Setter.C \
				QCD_Core_Scale.C \
				TTBar_Core_Scale.C \
				WBF_Core_Scale.C \
				QQSinglet_Core_Scale.C \
				HTPrime_Core_Scale.C \
				METS_Scale_Setter.C \
				Variable_KFactor_Setter.C \
				No_KFactor_Setter.C

localinc_HEADERS      =	\
			Scale_Setter_Base.H \
			Core_Scale_Setter.H \
			KFactor_Setter_Base.H \
			Tag_Setter.H

libPhasicScales_la_LIBADD     =	@CONDITIONAL_FASTJETLIBS@

INCLUDES      =	@CONDITIONAL_FASTJETINCS@
