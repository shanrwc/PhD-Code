#-- start of make_header -----------------

#====================================
#  Document MTreeCLIDDB
#
#   Generated Fri Oct  7 15:28:43 2011  by swalch
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_MTreeCLIDDB_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MTreeCLIDDB_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_MTreeCLIDDB

MTree_tag = $(tag)

#cmt_local_tagfile_MTreeCLIDDB = $(MTree_tag)_MTreeCLIDDB.make
cmt_local_tagfile_MTreeCLIDDB = $(bin)$(MTree_tag)_MTreeCLIDDB.make

else

tags      = $(tag),$(CMTEXTRATAGS)

MTree_tag = $(tag)

#cmt_local_tagfile_MTreeCLIDDB = $(MTree_tag).make
cmt_local_tagfile_MTreeCLIDDB = $(bin)$(MTree_tag).make

endif

include $(cmt_local_tagfile_MTreeCLIDDB)
#-include $(cmt_local_tagfile_MTreeCLIDDB)

ifdef cmt_MTreeCLIDDB_has_target_tag

cmt_final_setup_MTreeCLIDDB = $(bin)setup_MTreeCLIDDB.make
#cmt_final_setup_MTreeCLIDDB = $(bin)MTree_MTreeCLIDDBsetup.make
cmt_local_MTreeCLIDDB_makefile = $(bin)MTreeCLIDDB.make

else

cmt_final_setup_MTreeCLIDDB = $(bin)setup.make
#cmt_final_setup_MTreeCLIDDB = $(bin)MTreesetup.make
cmt_local_MTreeCLIDDB_makefile = $(bin)MTreeCLIDDB.make

endif

cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)MTreesetup.make

#MTreeCLIDDB :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'MTreeCLIDDB'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = MTreeCLIDDB/
#MTreeCLIDDB::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
# File: cmt/fragments/genCLIDDB_header
# Author: Paolo Calafiura
# derived from genconf_header

# Use genCLIDDB_cmd to create package clid.db files

.PHONY: MTreeCLIDDB MTreeCLIDDBclean

outname := clid.db
cliddb  := MTree_$(outname)
instdir := $(CMTINSTALLAREA)/share
result  := $(instdir)/$(cliddb)
product := $(instdir)/$(outname)
conflib := $(bin)$(library_prefix)MTree.$(shlibsuffix)

MTreeCLIDDB :: $(product)

$(instdir) :
	$(mkdir) -p $(instdir)

$(result) : $(conflib) $(instdir)
	@$(genCLIDDB_cmd) -p MTree -i$(product) -o $(result)

$(product) : $(result)
	cp $(result) $(instdir)/$(outname)

MTreeCLIDDBclean ::
	$(cleanup_silent) $(uninstall_command) $(product) $(result)
	$(cleanup_silent) $(cmt_uninstallarea_command) $(product) $(result)

#-- start of cleanup_header --------------

clean :: MTreeCLIDDBclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(MTreeCLIDDB.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
	if echo $@ | grep '$(package)setup\.make$$' >/dev/null; then\
	 echo "$(CMTMSGPREFIX)" "(MTreeCLIDDB.make): $@: File no longer generated" >&2; exit 0; fi
else
.DEFAULT::
	$(echo) "(MTreeCLIDDB.make) PEDANTIC: $@: No rule for such target" >&2
	if echo $@ | grep '$(package)setup\.make$$' >/dev/null; then\
	 echo "$(CMTMSGPREFIX)" "(MTreeCLIDDB.make): $@: File no longer generated" >&2; exit 0;\
	 elif test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_MTreeCLIDDB)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MTreeCLIDDB.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr $@ : '.*/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MTreeCLIDDB.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MTreeCLIDDB.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

MTreeCLIDDBclean ::
#-- end of cleanup_header ---------------
