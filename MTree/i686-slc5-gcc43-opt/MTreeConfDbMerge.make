#-- start of make_header -----------------

#====================================
#  Document MTreeConfDbMerge
#
#   Generated Fri Oct  7 15:28:34 2011  by swalch
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_MTreeConfDbMerge_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MTreeConfDbMerge_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_MTreeConfDbMerge

MTree_tag = $(tag)

#cmt_local_tagfile_MTreeConfDbMerge = $(MTree_tag)_MTreeConfDbMerge.make
cmt_local_tagfile_MTreeConfDbMerge = $(bin)$(MTree_tag)_MTreeConfDbMerge.make

else

tags      = $(tag),$(CMTEXTRATAGS)

MTree_tag = $(tag)

#cmt_local_tagfile_MTreeConfDbMerge = $(MTree_tag).make
cmt_local_tagfile_MTreeConfDbMerge = $(bin)$(MTree_tag).make

endif

include $(cmt_local_tagfile_MTreeConfDbMerge)
#-include $(cmt_local_tagfile_MTreeConfDbMerge)

ifdef cmt_MTreeConfDbMerge_has_target_tag

cmt_final_setup_MTreeConfDbMerge = $(bin)setup_MTreeConfDbMerge.make
#cmt_final_setup_MTreeConfDbMerge = $(bin)MTree_MTreeConfDbMergesetup.make
cmt_local_MTreeConfDbMerge_makefile = $(bin)MTreeConfDbMerge.make

else

cmt_final_setup_MTreeConfDbMerge = $(bin)setup.make
#cmt_final_setup_MTreeConfDbMerge = $(bin)MTreesetup.make
cmt_local_MTreeConfDbMerge_makefile = $(bin)MTreeConfDbMerge.make

endif

cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)MTreesetup.make

#MTreeConfDbMerge :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'MTreeConfDbMerge'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = MTreeConfDbMerge/
#MTreeConfDbMerge::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
# File: cmt/fragments/merge_genconfDb_header
# Author: Sebastien Binet (binet@cern.ch)

# Makefile fragment to merge a <library>_confDb.py file into a single
# <project>_merged_confDb.py file in the (python) install area
# Note that <project> is massaged to get a valid python module name 
# (ie: remove dots and dashes)
 
.PHONY: MTreeConfDbMerge MTreeConfDbMergeclean

# default is already '#'
#genconfDb_comment_char := "'#'"

instdir      := ${CMTINSTALLAREA}
confDbRef    := /home/swalch/testarea/16.6.5/MTree/genConf/MTree/MTree_confDb.py
stampConfDb  := $(confDbRef).stamp
mergedConfDb := $(instdir)/python/$(subst .,_,$(subst -,_,$(project)))_merged_confDb.py

MTreeConfDbMerge :: $(stampConfDb) $(mergedConfDb)
	@:

.NOTPARALLEL : $(stampConfDb) $(mergedConfDb)

$(stampConfDb) $(mergedConfDb) :: $(confDbRef)
	@echo "Running merge_genconfDb  MTreeConfDbMerge"
	$(merge_genconfDb_cmd) \
          --do-merge \
          --input-file $(confDbRef) \
          --merged-file $(mergedConfDb)	  

MTreeConfDbMergeclean ::
	$(cleanup_silent) $(merge_genconfDb_cmd) \
          --un-merge \
          --input-file $(confDbRef) \
          --merged-file $(mergedConfDb)	;
	$(cleanup_silent) $(remove_command) $(stampConfDb)
libMTree_so_dependencies = ../i686-slc5-gcc43-opt/libMTree.so
#-- start of cleanup_header --------------

clean :: MTreeConfDbMergeclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(MTreeConfDbMerge.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
	if echo $@ | grep '$(package)setup\.make$$' >/dev/null; then\
	 echo "$(CMTMSGPREFIX)" "(MTreeConfDbMerge.make): $@: File no longer generated" >&2; exit 0; fi
else
.DEFAULT::
	$(echo) "(MTreeConfDbMerge.make) PEDANTIC: $@: No rule for such target" >&2
	if echo $@ | grep '$(package)setup\.make$$' >/dev/null; then\
	 echo "$(CMTMSGPREFIX)" "(MTreeConfDbMerge.make): $@: File no longer generated" >&2; exit 0;\
	 elif test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_MTreeConfDbMerge)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MTreeConfDbMerge.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr $@ : '.*/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MTreeConfDbMerge.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MTreeConfDbMerge.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

MTreeConfDbMergeclean ::
#-- end of cleanup_header ---------------
