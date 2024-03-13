ifndef GO4SYS
GO4SYS = $(shell go4-config --go4sys)
endif

include $(GO4SYS)/Makefile.config

## normally should be like this for every module, but can be specific

ifdef GO4PACKAGE
TUT_DIR         = Go4ExampleSimple
else
TUT_DIR         = .
endif

TUT_LINKDEF     = $(TUT_DIR)/LinkDef.$(HedSuf)
TUT_LIBNAME     = $(GO4_USERLIBNAME)

## must be similar for every module

TUT_DICT        = $(TUT_DIR)/$(DICT_PREFIX)
TUT_DH          = $(TUT_DICT).$(HedSuf)
TUT_DS          = $(TUT_DICT).$(SrcSuf)
TUT_DO          = $(TUT_DICT).$(ObjSuf)

TUT_H           = $(filter-out $(TUT_DH) $(TUT_LINKDEF), $(wildcard $(TUT_DIR)/*.$(HedSuf)))
TUT_S           = $(filter-out $(TUT_DS), $(wildcard $(TUT_DIR)/*.$(SrcSuf)))
TUT_O           = $(TUT_S:.$(SrcSuf)=.$(ObjSuf))

TUT_DEP         = $(TUT_O:.$(ObjSuf)=.$(DepSuf))
TUT_DDEP        = $(TUT_DO:.$(ObjSuf)=.$(DepSuf))

TUT_LIB         = $(TUT_DIR)/$(TUT_LIBNAME).$(DllSuf)

# used in the main Makefile

EXAMPDEPENDENCS    += $(TUT_DEP) $(TUT_DDEP) $(TUT_EDEP)

ifdef DOPACKAGE
DISTRFILES         += $(TUT_S) $(TUT_H) $(TUT_LINKDEF)
DISTRFILES         += $(TUT_DIR)/Readme.txt
endif

##### local rules #####

all::  $(TUT_LIB)

$(TUT_LIB):   $(TUT_O) $(TUT_DO)
	@$(MakeLibrary) $(TUT_LIBNAME) "$(TUT_O) $(TUT_DO)" $(TUT_DIR) $(TUT_LINKDEF) "$(ANAL_LIB_DEP)"

$(TUT_DS): $(TUT_H)  $(TUT_LINKDEF)
	@$(ROOTCINTGO4) $(TUT_H) $(TUT_LINKDEF)

clean-bin::
	@rm -f $(TUT_O) $(TUT_DO)
	@rm -f $(TUT_DEP) $(TUT_DDEP) $(TUT_DS) $(TUT_DH)
	@rm -f libGo4User*.h 
	@rm -f G__.cxx_tmp*

clean:: clean-bin
	@$(CleanLib) $(TUT_LIBNAME) $(TUT_DIR)

include $(GO4SYS)/Makefile.rules
