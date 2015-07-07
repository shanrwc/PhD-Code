#-- start of make_header -----------------

#====================================
#  Document MTreeMergeMap
#
#   Generated Fri Oct  7 15:28:40 2011  by swalch
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_MTreeMergeMap_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MTreeMergeMap_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_MTreeMergeMap

MTree_tag = $(tag)

#cmt_local_tagfile_MTreeMergeMap = $(MTree_tag)_MTreeMergeMap.make
cmt_local_tagfile_MTreeMergeMap = $(bin)$(MTree_tag)_MTreeMergeMap.make

else

tags      = $(tag),$(CMTEXTRATAGS)

MTree_tag = $(tag)

#cmt_local_tagfile_MTreeMergeMap = $(MTree_tag).make
cmt_local_tagfile_MTreeMergeMap = $(bin)$(MTree_tag).make

endif

include $(cmt_local_tagfile_MTreeMergeMap)
#-include $(cmt_local_tagfile_MTreeMergeMap)

ifdef cmt_MTreeMergeMap_has_target_tag

cmt_final_setup_MTreeMergeMap = $(bin)setup_MTreeMergeMap.make
#cmt_final_setup_MTreeMergeMap = $(bin)MTree_MTreeMergeMapsetup.make
cmt_local_MTreeMergeMap_makefile = $(bin)MTreeMergeMap.make

else

cmt_final_setup_MTreeMergeMap = $(bin)setup.make
#cmt_final_setup_MTreeMergeMap = $(bin)MTreesetup.make
cmt_local_MTreeMergeMap_makefile = $(bin)MTreeMergeMap.make

endif

cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)MTreesetup.make

#MTreeMergeMap :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'MTreeMergeMap'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = MTreeMergeMap/
#MTreeMergeMap::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
# File: cmt/fragments/merge_rootmap_header
# Author: Sebastien Binet (binet@cern.ch)

# Makefile fragment to merge a <library>.rootmap file into a single
# <project>.rootmap file in the (lib) install area
# If no InstallArea is present the fragment is dummy


.PHONY: MTreeMergeMap MTreeMergeMapclean

# default is already '#'
#genmap_comment_char := "'#'"

rootMapRef    := ../$(tag)/MTree.rootmap

ifdef CMTINSTALLAREA
rootMapDir    := ${CMTINSTALLAREA}/$(tag)/lib
mergedRootMap := $(rootMapDir)/$(project).rootmap
stampRootMap  := $(rootMapRef).stamp
else
rootMapDir    := ../$(tag)
mergedRootMap := 
stampRootMap  :=
endif

MTreeMergeMap :: $(stampRootMap) $(mergedRootMap)
	@:

.NOTPARALLEL : $(stampRootMap) $(mergedRootMap)

$(stampRootMap) $(mergedRootMap) :: $(rootMapRef)
	@echo "Running merge_rootmap  MTreeMergeMap" 
	$(merge_rootmap_cmd) --do-merge \
         --input-file $(rootMapRef) --merged-file $(mergedRootMap)

MTreeMergeMapclean ::
	$(cleanup_silent) $(merge_rootmap_cmd) --un-merge \
         --input-file $(rootMapRef) --merged-file $(mergedRootMap) ;
	$(cleanup_silent) $(remove_command) $(stampRootMap)
libMTree_so_dependencies = ../i686-slc5-gcc43-opt/libMTree.so
#-- start of cleanup_header --------------

clean :: MTreeMergeMapclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(MTreeMergeMap.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
	if echo $@ | grep '$(package)setup\.make$$' >/dev/null; then\
	 echo "$(CMTMSGPREFIX)" "(MTreeMergeMap.make): $@: File no longer generated" >&2; exit 0; fi
else
.DEFAULT::
	$(echo) "(MTreeMergeMap.make) PEDANTIC: $@: No rule for such target" >&2
	if echo $@ | grep '$(package)setup\.make$$' >/dev/null; then\
	 echo "$(CMTMSGPREFIX)" "(MTreeMergeMap.make): $@: File no longer generated" >&2; exit 0;\
	 elif test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_MTreeMergeMap)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MTreeMergeMap.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr $@ : '.*/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MTreeMergeMap.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MTreeMergeMap.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

MTreeMergeMapclean ::
#-- end of cleanup_header ---------------
