#-- start of make_header -----------------

#====================================
#  Document MTreeConf
#
#   Generated Fri Oct  7 15:28:16 2011  by swalch
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_MTreeConf_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MTreeConf_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_MTreeConf

MTree_tag = $(tag)

#cmt_local_tagfile_MTreeConf = $(MTree_tag)_MTreeConf.make
cmt_local_tagfile_MTreeConf = $(bin)$(MTree_tag)_MTreeConf.make

else

tags      = $(tag),$(CMTEXTRATAGS)

MTree_tag = $(tag)

#cmt_local_tagfile_MTreeConf = $(MTree_tag).make
cmt_local_tagfile_MTreeConf = $(bin)$(MTree_tag).make

endif

include $(cmt_local_tagfile_MTreeConf)
#-include $(cmt_local_tagfile_MTreeConf)

ifdef cmt_MTreeConf_has_target_tag

cmt_final_setup_MTreeConf = $(bin)setup_MTreeConf.make
#cmt_final_setup_MTreeConf = $(bin)MTree_MTreeConfsetup.make
cmt_local_MTreeConf_makefile = $(bin)MTreeConf.make

else

cmt_final_setup_MTreeConf = $(bin)setup.make
#cmt_final_setup_MTreeConf = $(bin)MTreesetup.make
cmt_local_MTreeConf_makefile = $(bin)MTreeConf.make

endif

cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)MTreesetup.make

#MTreeConf :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'MTreeConf'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = MTreeConf/
#MTreeConf::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
# File: cmt/fragments/genconfig_header
# Author: Wim Lavrijsen (WLavrijsen@lbl.gov)

# Use genconf.exe to create configurables python modules, then have the
# normal python install procedure take over.

.PHONY: MTreeConf MTreeConfclean

confpy  := MTreeConf.py
conflib := $(bin)$(library_prefix)MTree.$(shlibsuffix)
dbpy    := MTree_confDb.py
instdir := $(CMTINSTALLAREA)/python/$(package)
product := $(instdir)/$(confpy)
initpy  := $(instdir)/__init__.py

ifdef GENCONF_ECHO
genconf_silent =
else
genconf_silent = $(silent)
endif

MTreeConf :: MTreeConfinstall

install :: MTreeConfinstall

MTreeConfinstall : /home/swalch/testarea/16.6.5/MTree/genConf/MTree/$(confpy)
	@echo "Installing /home/swalch/testarea/16.6.5/MTree/genConf/MTree in /net/s3_data_home/swalch/testarea/16.6.5/InstallArea/python" ; \
	 $(install_command) --exclude="*.py?" --exclude="__init__.py" /home/swalch/testarea/16.6.5/MTree/genConf/MTree /net/s3_data_home/swalch/testarea/16.6.5/InstallArea/python ; \

/home/swalch/testarea/16.6.5/MTree/genConf/MTree/$(confpy) : $(conflib) /home/swalch/testarea/16.6.5/MTree/genConf/MTree
	$(genconf_silent) $(genconfig_cmd)  -o /home/swalch/testarea/16.6.5/MTree/genConf/MTree -p $(package) \
	  --configurable-module=GaudiKernel.Proxy \
	  --configurable-default-name=Configurable.DefaultName \
	  --configurable-algorithm=ConfigurableAlgorithm \
	  --configurable-algtool=ConfigurableAlgTool \
	  --configurable-auditor=ConfigurableAuditor \
          --configurable-service=ConfigurableService \
	  -i ../$(tag)/$(library_prefix)MTree.$(shlibsuffix) 

/home/swalch/testarea/16.6.5/MTree/genConf/MTree:
	@ if [ ! -d /home/swalch/testarea/16.6.5/MTree/genConf/MTree ] ; then mkdir -p /home/swalch/testarea/16.6.5/MTree/genConf/MTree ; fi ; 

MTreeConfclean :: MTreeConfuninstall
	$(cleanup_silent) $(remove_command) /home/swalch/testarea/16.6.5/MTree/genConf/MTree/$(confpy) /home/swalch/testarea/16.6.5/MTree/genConf/MTree/$(dbpy)

uninstall :: MTreeConfuninstall

MTreeConfuninstall :: 
	@$(uninstall_command) /net/s3_data_home/swalch/testarea/16.6.5/InstallArea/python
libMTree_so_dependencies = ../i686-slc5-gcc43-opt/libMTree.so
#-- start of cleanup_header --------------

clean :: MTreeConfclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(MTreeConf.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
	if echo $@ | grep '$(package)setup\.make$$' >/dev/null; then\
	 echo "$(CMTMSGPREFIX)" "(MTreeConf.make): $@: File no longer generated" >&2; exit 0; fi
else
.DEFAULT::
	$(echo) "(MTreeConf.make) PEDANTIC: $@: No rule for such target" >&2
	if echo $@ | grep '$(package)setup\.make$$' >/dev/null; then\
	 echo "$(CMTMSGPREFIX)" "(MTreeConf.make): $@: File no longer generated" >&2; exit 0;\
	 elif test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_MTreeConf)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MTreeConf.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr $@ : '.*/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MTreeConf.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MTreeConf.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

MTreeConfclean ::
#-- end of cleanup_header ---------------
