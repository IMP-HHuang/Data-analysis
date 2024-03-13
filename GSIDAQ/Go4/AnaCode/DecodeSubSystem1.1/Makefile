ifndef GO4SYS
GO4SYS = $(shell go4-config --go4sys)
endif

include $(GO4SYS)/Makefile.config
#CXXFLAGS += -std=c++11 -fdiagnostics-color -g -Wno-unused-variable -Wno-unused-but-set-variable $(shell root-config --libs --cflags)
CXXFLAGS += -fdiagnostics-color -g -Wno-unused-variable -Wno-unused-but-set-variable $(shell root-config --libs --cflags)

## normally should be like this for every module, but can be specific

ifdef GO4PACKAGE
TSCNSTEP_DIR         = finalproject_v2
else
TSCNSTEP_DIR         = .
endif

TSCNSTEP_LINKDEF     = $(TSCNSTEP_DIR)/LinkDef.$(HedSuf)
TSCNSTEP_LIBNAME     = $(GO4_USERLIBNAME)

## must be similar for every module

TSCNSTEP_DICT        = $(TSCNSTEP_DIR)/$(DICT_PREFIX)
TSCNSTEP_DH          = $(TSCNSTEP_DICT).$(HedSuf)
TSCNSTEP_DS          = $(TSCNSTEP_DICT).$(SrcSuf)
TSCNSTEP_DO          = $(TSCNSTEP_DICT).$(ObjSuf)

TSCNSTEP_H           = $(filter-out $(TSCNSTEP_DH) $(TSCNSTEP_LINKDEF), $(wildcard $(TSCNSTEP_DIR)/*.$(HedSuf)))
TSCNSTEP_S           = $(filter-out $(TSCNSTEP_DS), $(wildcard $(TSCNSTEP_DIR)/*.$(SrcSuf)))
TSCNSTEP_O           = $(TSCNSTEP_S:.$(SrcSuf)=.$(ObjSuf))

TSCNSTEP_DEP         =  $(TSCNSTEP_O:.$(ObjSuf)=.$(DepSuf))
TSCNSTEP_DDEP        =  $(TSCNSTEP_DO:.$(ObjSuf)=.$(DepSuf))

TSCNSTEP_LIB         =  $(TSCNSTEP_DIR)/$(TSCNSTEP_LIBNAME).$(DllSuf)

# used in the main Makefile

EXAMPDEPENDENCS    += $(TSCNSTEP_DEP) $(TSCNSTEP_DDEP)

ifdef DOPACKAGE
DISTRFILES         += $(TSCNSTEP_S) $(TSCNSTEP_H) $(TSCNSTEP_LINKDEF)
DISTRFILES         += $(TSCNSTEP_DIR)/Readme.txt $(TSCNSTEP_DIR)/Makefile.win
DISTRFILES         += $(TSCNSTEP_DIR)/setparam.C
endif

##### local rules #####

all::  $(TSCNSTEP_LIB)

$(TSCNSTEP_LIB):   $(TSCNSTEP_O) $(TSCNSTEP_DO) $(ANAL_LIB_DEP)
	@$(MakeLibrary) $(TSCNSTEP_LIBNAME) "$(TSCNSTEP_O) $(TSCNSTEP_DO)" $(TSCNSTEP_DIR) $(TSCNSTEP_LINKDEF) "$(ANAL_LIB_DEP)" $(TSCNSTEP_DS) "$(TSCNSTEP_H)"


$(TSCNSTEP_DS): $(TSCNSTEP_H)  $(TSCNSTEP_LINKDEF)
	@$(ROOTCINTGO4) $(TSCNSTEP_LIB) $(TSCNSTEP_H) $(TSCNSTEP_LINKDEF)

clean-bin::
	@rm -f $(TSCNSTEP_O) $(TSCNSTEP_DO)
	@rm -f $(TSCNSTEP_DEP) $(TSCNSTEP_DDEP) $(TSCNSTEP_DS) $(TSCNSTEP_DH)

clean:: clean-bin
	@$(CleanLib) $(TSCNSTEP_LIBNAME) $(TSCNSTEP_DIR)
#	@rm -f *.root
	@rm -f *.pcm
	@rm -f *.rootmap
	@rm -f *output_dataset.txt
	@rm -f libGo4User*.h 
	@rm -f G__.cxx_tmp*
	@rm -f .fuse_hidden*

include $(GO4SYS)/Makefile.rules
