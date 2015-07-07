#-- start of make_header -----------------

#====================================
#  Document MTreeRootMap
#
#   Generated Fri Oct  7 15:28:37 2011  by swalch
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_MTreeRootMap_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MTreeRootMap_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_MTreeRootMap

MTree_tag = $(tag)

#cmt_local_tagfile_MTreeRootMap = $(MTree_tag)_MTreeRootMap.make
cmt_local_tagfile_MTreeRootMap = $(bin)$(MTree_tag)_MTreeRootMap.make

else

tags      = $(tag),$(CMTEXTRATAGS)

MTree_tag = $(tag)

#cmt_local_tagfile_MTreeRootMap = $(MTree_tag).make
cmt_local_tagfile_MTreeRootMap = $(bin)$(MTree_tag).make

endif

include $(cmt_local_tagfile_MTreeRootMap)
#-include $(cmt_local_tagfile_MTreeRootMap)

ifdef cmt_MTreeRootMap_has_target_tag

cmt_final_setup_MTreeRootMap = $(bin)setup_MTreeRootMap.make
#cmt_final_setup_MTreeRootMap = $(bin)MTree_MTreeRootMapsetup.make
cmt_local_MTreeRootMap_makefile = $(bin)MTreeRootMap.make

else

cmt_final_setup_MTreeRootMap = $(bin)setup.make
#cmt_final_setup_MTreeRootMap = $(bin)MTreesetup.make
cmt_local_MTreeRootMap_makefile = $(bin)MTreeRootMap.make

endif

cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)MTreesetup.make

#MTreeRootMap :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'MTreeRootMap'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = MTreeRootMap/
#MTreeRootMap::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
##
rootmapfile = MTree.rootmap
ROOTMAP_DIR = ../$(tag)
fulllibname = libMTree.$(shlibsuffix)

MTreeRootMap :: ${ROOTMAP_DIR}/$(rootmapfile)
	@:

${ROOTMAP_DIR}/$(rootmapfile) :: $(bin)$(fulllibname)
	@echo 'Generating rootmap file for $(fulllibname)'
	cd ../$(tag);$(genmap_cmd) -i $(fulllibname) -o ${ROOTMAP_DIR}/$(rootmapfile) $(MTreeRootMap_genmapflags)

install :: MTreeRootMapinstall
MTreeRootMapinstall :: MTreeRootMap

uninstall :: MTreeRootMapuninstall
MTreeRootMapuninstall :: MTreeRootMapclean

MTreeRootMapclean ::
	@echo 'Deleting $(rootmapfile)'
	@rm -f ${ROOTMAP_DIR}/$(rootmapfile)

#-- start of cleanup_header --------------

clean :: MTreeRootMapclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(MTreeRootMap.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
	if echo $@ | grep '$(package)setup\.make$$' >/dev/null; then\
	 echo "$(CMTMSGPREFIX)" "(MTreeRootMap.make): $@: File no longer generated" >&2; exit 0; fi
else
.DEFAULT::
	$(echo) "(MTreeRootMap.make) PEDANTIC: $@: No rule for such target" >&2
	if echo $@ | grep '$(package)setup\.make$$' >/dev/null; then\
	 echo "$(CMTMSGPREFIX)" "(MTreeRootMap.make): $@: File no longer generated" >&2; exit 0;\
	 elif test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_MTreeRootMap)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MTreeRootMap.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr $@ : '.*/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MTreeRootMap.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MTreeRootMap.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

MTreeRootMapclean ::
#-- end of cleanup_header ---------------
