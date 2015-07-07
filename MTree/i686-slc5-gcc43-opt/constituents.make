
#-- start of constituents_header ------

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

tags      = $(tag),$(CMTEXTRATAGS)

MTree_tag = $(tag)

#cmt_local_tagfile = $(MTree_tag).make
cmt_local_tagfile = $(bin)$(MTree_tag).make

#-include $(cmt_local_tagfile)
include $(cmt_local_tagfile)

#cmt_local_setup = $(bin)setup$$$$.make
#cmt_local_setup = $(bin)$(package)setup$$$$.make
#cmt_final_setup = $(bin)MTreesetup.make
cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)$(package)setup.make

#--------------------------------------------------------

#cmt_lock_setup = /tmp/lock$(cmt_lock_pid).make
#cmt_temp_tag = /tmp/tag$(cmt_lock_pid).make

#first :: $(cmt_local_tagfile)
#	@echo $(cmt_local_tagfile) ok
#ifndef QUICK
#first :: $(cmt_final_setup) ;
#else
#first :: ;
#endif

##	@bin=`$(cmtexe) show macro_value bin`

#$(cmt_local_tagfile) : $(cmt_lock_setup)
#	@echo "#CMT> Error: $@: No such file" >&2; exit 1
#$(cmt_local_tagfile) :
#	@echo "#CMT> Warning: $@: No such file" >&2; exit
#	@echo "#CMT> Info: $@: No need to rebuild file" >&2; exit

#$(cmt_final_setup) : $(cmt_local_tagfile) 
#	$(echo) "(constituents.make) Rebuilding $@"
#	@if test ! -d $(@D); then $(mkdir) -p $(@D); fi; \
#	  if test -f $(cmt_local_setup); then /bin/rm -f $(cmt_local_setup); fi; \
#	  trap '/bin/rm -f $(cmt_local_setup)' 0 1 2 15; \
#	  $(cmtexe) -tag=$(tags) show setup >>$(cmt_local_setup); \
#	  if test ! -f $@; then \
#	    mv $(cmt_local_setup) $@; \
#	  else \
#	    if /usr/bin/diff $(cmt_local_setup) $@ >/dev/null ; then \
#	      : ; \
#	    else \
#	      mv $(cmt_local_setup) $@; \
#	    fi; \
#	  fi

#	@/bin/echo $@ ok   

#config :: checkuses
#	@exit 0
#checkuses : ;

env.make ::
	printenv >env.make.tmp; $(cmtexe) check files env.make.tmp env.make

ifndef QUICK
all :: build_library_links
	$(echo) "(constituents.make) all done"
endif

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

dirs :: requirements
	@if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi
#	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
#	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

requirements :
	@if test ! -r requirements ; then echo "No requirements file" ; fi

build_library_links : dirs
	$(echo) "(constituents.make) Rebuilding library links"; \
	 $(build_library_links)
#	if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi; \
#	$(build_library_links)

makefiles : ;

.DEFAULT ::
	$(echo) "(constituents.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: Using default commands" >&2; exit

#	@if test "$@" = "$(cmt_lock_setup)"; then \
	#  /bin/rm -f $(cmt_lock_setup); \
	 # touch $(cmt_lock_setup); \
	#fi

#-- end of constituents_header ------
#-- start of group ------

all_groups :: all

all :: $(all_dependencies)  $(all_pre_constituents) $(all_constituents)  $(all_post_constituents)
	$(echo) "all ok."

#	@/bin/echo " all ok."

clean :: allclean

allclean ::  $(all_constituentsclean)
	$(echo) $(all_constituentsclean)
	$(echo) "allclean ok."

#	@echo $(all_constituentsclean)
#	@/bin/echo " allclean ok."

allclean :: makefilesclean

#-- end of group ------
#-- start of group ------

all_groups :: cmt_actions

cmt_actions :: $(cmt_actions_dependencies)  $(cmt_actions_pre_constituents) $(cmt_actions_constituents)  $(cmt_actions_post_constituents)
	$(echo) "cmt_actions ok."

#	@/bin/echo " cmt_actions ok."

clean :: allclean

cmt_actionsclean ::  $(cmt_actions_constituentsclean)
	$(echo) $(cmt_actions_constituentsclean)
	$(echo) "cmt_actionsclean ok."

#	@echo $(cmt_actions_constituentsclean)
#	@/bin/echo " cmt_actionsclean ok."

cmt_actionsclean :: makefilesclean

#-- end of group ------
#-- start of group ------

all_groups :: rulechecker

rulechecker :: $(rulechecker_dependencies)  $(rulechecker_pre_constituents) $(rulechecker_constituents)  $(rulechecker_post_constituents)
	$(echo) "rulechecker ok."

#	@/bin/echo " rulechecker ok."

clean :: allclean

rulecheckerclean ::  $(rulechecker_constituentsclean)
	$(echo) $(rulechecker_constituentsclean)
	$(echo) "rulecheckerclean ok."

#	@echo $(rulechecker_constituentsclean)
#	@/bin/echo " rulecheckerclean ok."

rulecheckerclean :: makefilesclean

#-- end of group ------
#-- start of group ------

all_groups :: ctest

ctest :: $(ctest_dependencies)  $(ctest_pre_constituents) $(ctest_constituents)  $(ctest_post_constituents)
	$(echo) "ctest ok."

#	@/bin/echo " ctest ok."

clean :: allclean

ctestclean ::  $(ctest_constituentsclean)
	$(echo) $(ctest_constituentsclean)
	$(echo) "ctestclean ok."

#	@echo $(ctest_constituentsclean)
#	@/bin/echo " ctestclean ok."

ctestclean :: makefilesclean

#-- end of group ------
#-- start of constituent ------

cmt_MTree_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MTree_has_target_tag

#cmt_local_tagfile_MTree = $(MTree_tag)_MTree.make
cmt_local_tagfile_MTree = $(bin)$(MTree_tag)_MTree.make
cmt_local_setup_MTree = $(bin)setup_MTree$$$$.make
cmt_final_setup_MTree = $(bin)setup_MTree.make
#cmt_final_setup_MTree = $(bin)MTree_MTreesetup.make
cmt_local_MTree_makefile = $(bin)MTree.make

MTree_extratags = -tag_add=target_MTree

#$(cmt_local_tagfile_MTree) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_MTree) ::
else
$(cmt_local_tagfile_MTree) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_MTree); then /bin/rm -f $(cmt_local_tagfile_MTree); fi ; \
	  $(cmtexe) -tag=$(tags) $(MTree_extratags) build tag_makefile >>$(cmt_local_tagfile_MTree)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_MTree)"; \
	  test ! -f $(cmt_local_setup_MTree) || \rm -f $(cmt_local_setup_MTree); \
	  trap '\rm -f $(cmt_local_setup_MTree)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(MTree_extratags) show setup >$(cmt_local_setup_MTree) && \
	  if [ -f $(cmt_final_setup_MTree) ] && \
	    \cmp -s $(cmt_final_setup_MTree) $(cmt_local_setup_MTree); then \
	    \rm $(cmt_local_setup_MTree); else \
	    \mv -f $(cmt_local_setup_MTree) $(cmt_final_setup_MTree); fi

else

#cmt_local_tagfile_MTree = $(MTree_tag).make
cmt_local_tagfile_MTree = $(bin)$(MTree_tag).make
cmt_final_setup_MTree = $(bin)setup.make
#cmt_final_setup_MTree = $(bin)MTreesetup.make
cmt_local_MTree_makefile = $(bin)MTree.make

endif

ifdef STRUCTURED_OUTPUT
MTreedirs :
	@if test ! -d $(bin)MTree; then $(mkdir) -p $(bin)MTree; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)MTree
else
MTreedirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# MTreedirs ::
#	@if test ! -d $(bin)MTree; then $(mkdir) -p $(bin)MTree; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)MTree
#
#$(cmt_local_MTree_makefile) :: $(MTree_dependencies) $(cmt_local_tagfile_MTree) build_library_links dirs MTreedirs
#else
#$(cmt_local_MTree_makefile) :: $(MTree_dependencies) $(cmt_local_tagfile_MTree) build_library_links dirs
#endif
#else
#$(cmt_local_MTree_makefile) :: $(cmt_local_tagfile_MTree)
#endif

makefiles : $(cmt_local_MTree_makefile)

ifndef QUICK
$(cmt_local_MTree_makefile) : $(MTree_dependencies) build_library_links
#$(cmt_local_MTree_makefile) : $(MTree_dependencies) $(cmt_local_tagfile_MTree) build_library_links
	$(echo) "(constituents.make) Building MTree.make"; \
	  $(cmtexe) -tag=$(tags) $(MTree_extratags) build constituent_config -out=$(cmt_local_MTree_makefile) MTree
else
$(cmt_local_MTree_makefile) : $(MTree_dependencies)
#$(cmt_local_MTree_makefile) : $(cmt_local_tagfile_MTree)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_MTree) ] || \
	  [ ! -f $(cmt_final_setup_MTree) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building MTree.make"; \
	  $(cmtexe) -tag=$(tags) $(MTree_extratags) build constituent_config -out=$(cmt_local_MTree_makefile) MTree; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(MTree_extratags) build constituent_makefile -out=$(cmt_local_MTree_makefile) MTree

MTree :: $(MTree_dependencies) $(cmt_local_MTree_makefile) dirs MTreedirs
	$(echo) "(constituents.make) Starting MTree"
	@$(MAKE) -f $(cmt_local_MTree_makefile) MTree
	$(echo) "(constituents.make) MTree done"

clean :: MTreeclean

MTreeclean :: $(MTreeclean_dependencies) ##$(cmt_local_MTree_makefile)
	$(echo) "(constituents.make) Starting MTreeclean"
	@-if test -f $(cmt_local_MTree_makefile); then \
	  $(MAKE) -f $(cmt_local_MTree_makefile) MTreeclean; \
	fi
	$(echo) "(constituents.make) MTreeclean done"
#	@-$(MAKE) -f $(cmt_local_MTree_makefile) MTreeclean

##	  /bin/rm -f $(cmt_local_MTree_makefile) $(bin)MTree_dependencies.make

install :: MTreeinstall

MTreeinstall :: $(MTree_dependencies) $(cmt_local_MTree_makefile)
	$(echo) "(constituents.make) Starting install MTree"
	@-$(MAKE) -f $(cmt_local_MTree_makefile) install
	$(echo) "(constituents.make) install MTree done"

uninstall :: MTreeuninstall

$(foreach d,$(MTree_dependencies),$(eval $(d)uninstall_dependencies += MTreeuninstall))

MTreeuninstall :: $(MTreeuninstall_dependencies) $(cmt_local_MTree_makefile)
	$(echo) "(constituents.make) Starting uninstall MTree"
	@$(MAKE) -f $(cmt_local_MTree_makefile) uninstall
	$(echo) "(constituents.make) uninstall MTree done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ MTree"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ MTree done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_MTreeConf_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MTreeConf_has_target_tag

#cmt_local_tagfile_MTreeConf = $(MTree_tag)_MTreeConf.make
cmt_local_tagfile_MTreeConf = $(bin)$(MTree_tag)_MTreeConf.make
cmt_local_setup_MTreeConf = $(bin)setup_MTreeConf$$$$.make
cmt_final_setup_MTreeConf = $(bin)setup_MTreeConf.make
#cmt_final_setup_MTreeConf = $(bin)MTree_MTreeConfsetup.make
cmt_local_MTreeConf_makefile = $(bin)MTreeConf.make

MTreeConf_extratags = -tag_add=target_MTreeConf

#$(cmt_local_tagfile_MTreeConf) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_MTreeConf) ::
else
$(cmt_local_tagfile_MTreeConf) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_MTreeConf); then /bin/rm -f $(cmt_local_tagfile_MTreeConf); fi ; \
	  $(cmtexe) -tag=$(tags) $(MTreeConf_extratags) build tag_makefile >>$(cmt_local_tagfile_MTreeConf)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_MTreeConf)"; \
	  test ! -f $(cmt_local_setup_MTreeConf) || \rm -f $(cmt_local_setup_MTreeConf); \
	  trap '\rm -f $(cmt_local_setup_MTreeConf)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(MTreeConf_extratags) show setup >$(cmt_local_setup_MTreeConf) && \
	  if [ -f $(cmt_final_setup_MTreeConf) ] && \
	    \cmp -s $(cmt_final_setup_MTreeConf) $(cmt_local_setup_MTreeConf); then \
	    \rm $(cmt_local_setup_MTreeConf); else \
	    \mv -f $(cmt_local_setup_MTreeConf) $(cmt_final_setup_MTreeConf); fi

else

#cmt_local_tagfile_MTreeConf = $(MTree_tag).make
cmt_local_tagfile_MTreeConf = $(bin)$(MTree_tag).make
cmt_final_setup_MTreeConf = $(bin)setup.make
#cmt_final_setup_MTreeConf = $(bin)MTreesetup.make
cmt_local_MTreeConf_makefile = $(bin)MTreeConf.make

endif

ifdef STRUCTURED_OUTPUT
MTreeConfdirs :
	@if test ! -d $(bin)MTreeConf; then $(mkdir) -p $(bin)MTreeConf; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)MTreeConf
else
MTreeConfdirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# MTreeConfdirs ::
#	@if test ! -d $(bin)MTreeConf; then $(mkdir) -p $(bin)MTreeConf; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)MTreeConf
#
#$(cmt_local_MTreeConf_makefile) :: $(MTreeConf_dependencies) $(cmt_local_tagfile_MTreeConf) build_library_links dirs MTreeConfdirs
#else
#$(cmt_local_MTreeConf_makefile) :: $(MTreeConf_dependencies) $(cmt_local_tagfile_MTreeConf) build_library_links dirs
#endif
#else
#$(cmt_local_MTreeConf_makefile) :: $(cmt_local_tagfile_MTreeConf)
#endif

makefiles : $(cmt_local_MTreeConf_makefile)

ifndef QUICK
$(cmt_local_MTreeConf_makefile) : $(MTreeConf_dependencies) build_library_links
#$(cmt_local_MTreeConf_makefile) : $(MTreeConf_dependencies) $(cmt_local_tagfile_MTreeConf) build_library_links
	$(echo) "(constituents.make) Building MTreeConf.make"; \
	  $(cmtexe) -tag=$(tags) $(MTreeConf_extratags) build constituent_config -out=$(cmt_local_MTreeConf_makefile) MTreeConf
else
$(cmt_local_MTreeConf_makefile) : $(MTreeConf_dependencies)
#$(cmt_local_MTreeConf_makefile) : $(cmt_local_tagfile_MTreeConf)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_MTreeConf) ] || \
	  [ ! -f $(cmt_final_setup_MTreeConf) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building MTreeConf.make"; \
	  $(cmtexe) -tag=$(tags) $(MTreeConf_extratags) build constituent_config -out=$(cmt_local_MTreeConf_makefile) MTreeConf; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(MTreeConf_extratags) build constituent_makefile -out=$(cmt_local_MTreeConf_makefile) MTreeConf

MTreeConf :: $(MTreeConf_dependencies) $(cmt_local_MTreeConf_makefile) dirs MTreeConfdirs
	$(echo) "(constituents.make) Starting MTreeConf"
	@$(MAKE) -f $(cmt_local_MTreeConf_makefile) MTreeConf
	$(echo) "(constituents.make) MTreeConf done"

clean :: MTreeConfclean

MTreeConfclean :: $(MTreeConfclean_dependencies) ##$(cmt_local_MTreeConf_makefile)
	$(echo) "(constituents.make) Starting MTreeConfclean"
	@-if test -f $(cmt_local_MTreeConf_makefile); then \
	  $(MAKE) -f $(cmt_local_MTreeConf_makefile) MTreeConfclean; \
	fi
	$(echo) "(constituents.make) MTreeConfclean done"
#	@-$(MAKE) -f $(cmt_local_MTreeConf_makefile) MTreeConfclean

##	  /bin/rm -f $(cmt_local_MTreeConf_makefile) $(bin)MTreeConf_dependencies.make

install :: MTreeConfinstall

MTreeConfinstall :: $(MTreeConf_dependencies) $(cmt_local_MTreeConf_makefile)
	$(echo) "(constituents.make) Starting install MTreeConf"
	@-$(MAKE) -f $(cmt_local_MTreeConf_makefile) install
	$(echo) "(constituents.make) install MTreeConf done"

uninstall :: MTreeConfuninstall

$(foreach d,$(MTreeConf_dependencies),$(eval $(d)uninstall_dependencies += MTreeConfuninstall))

MTreeConfuninstall :: $(MTreeConfuninstall_dependencies) $(cmt_local_MTreeConf_makefile)
	$(echo) "(constituents.make) Starting uninstall MTreeConf"
	@$(MAKE) -f $(cmt_local_MTreeConf_makefile) uninstall
	$(echo) "(constituents.make) uninstall MTreeConf done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ MTreeConf"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ MTreeConf done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_MTree_python_init_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MTree_python_init_has_target_tag

#cmt_local_tagfile_MTree_python_init = $(MTree_tag)_MTree_python_init.make
cmt_local_tagfile_MTree_python_init = $(bin)$(MTree_tag)_MTree_python_init.make
cmt_local_setup_MTree_python_init = $(bin)setup_MTree_python_init$$$$.make
cmt_final_setup_MTree_python_init = $(bin)setup_MTree_python_init.make
#cmt_final_setup_MTree_python_init = $(bin)MTree_MTree_python_initsetup.make
cmt_local_MTree_python_init_makefile = $(bin)MTree_python_init.make

MTree_python_init_extratags = -tag_add=target_MTree_python_init

#$(cmt_local_tagfile_MTree_python_init) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_MTree_python_init) ::
else
$(cmt_local_tagfile_MTree_python_init) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_MTree_python_init); then /bin/rm -f $(cmt_local_tagfile_MTree_python_init); fi ; \
	  $(cmtexe) -tag=$(tags) $(MTree_python_init_extratags) build tag_makefile >>$(cmt_local_tagfile_MTree_python_init)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_MTree_python_init)"; \
	  test ! -f $(cmt_local_setup_MTree_python_init) || \rm -f $(cmt_local_setup_MTree_python_init); \
	  trap '\rm -f $(cmt_local_setup_MTree_python_init)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(MTree_python_init_extratags) show setup >$(cmt_local_setup_MTree_python_init) && \
	  if [ -f $(cmt_final_setup_MTree_python_init) ] && \
	    \cmp -s $(cmt_final_setup_MTree_python_init) $(cmt_local_setup_MTree_python_init); then \
	    \rm $(cmt_local_setup_MTree_python_init); else \
	    \mv -f $(cmt_local_setup_MTree_python_init) $(cmt_final_setup_MTree_python_init); fi

else

#cmt_local_tagfile_MTree_python_init = $(MTree_tag).make
cmt_local_tagfile_MTree_python_init = $(bin)$(MTree_tag).make
cmt_final_setup_MTree_python_init = $(bin)setup.make
#cmt_final_setup_MTree_python_init = $(bin)MTreesetup.make
cmt_local_MTree_python_init_makefile = $(bin)MTree_python_init.make

endif

ifdef STRUCTURED_OUTPUT
MTree_python_initdirs :
	@if test ! -d $(bin)MTree_python_init; then $(mkdir) -p $(bin)MTree_python_init; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)MTree_python_init
else
MTree_python_initdirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# MTree_python_initdirs ::
#	@if test ! -d $(bin)MTree_python_init; then $(mkdir) -p $(bin)MTree_python_init; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)MTree_python_init
#
#$(cmt_local_MTree_python_init_makefile) :: $(MTree_python_init_dependencies) $(cmt_local_tagfile_MTree_python_init) build_library_links dirs MTree_python_initdirs
#else
#$(cmt_local_MTree_python_init_makefile) :: $(MTree_python_init_dependencies) $(cmt_local_tagfile_MTree_python_init) build_library_links dirs
#endif
#else
#$(cmt_local_MTree_python_init_makefile) :: $(cmt_local_tagfile_MTree_python_init)
#endif

makefiles : $(cmt_local_MTree_python_init_makefile)

ifndef QUICK
$(cmt_local_MTree_python_init_makefile) : $(MTree_python_init_dependencies) build_library_links
#$(cmt_local_MTree_python_init_makefile) : $(MTree_python_init_dependencies) $(cmt_local_tagfile_MTree_python_init) build_library_links
	$(echo) "(constituents.make) Building MTree_python_init.make"; \
	  $(cmtexe) -tag=$(tags) $(MTree_python_init_extratags) build constituent_config -out=$(cmt_local_MTree_python_init_makefile) MTree_python_init
else
$(cmt_local_MTree_python_init_makefile) : $(MTree_python_init_dependencies)
#$(cmt_local_MTree_python_init_makefile) : $(cmt_local_tagfile_MTree_python_init)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_MTree_python_init) ] || \
	  [ ! -f $(cmt_final_setup_MTree_python_init) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building MTree_python_init.make"; \
	  $(cmtexe) -tag=$(tags) $(MTree_python_init_extratags) build constituent_config -out=$(cmt_local_MTree_python_init_makefile) MTree_python_init; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(MTree_python_init_extratags) build constituent_makefile -out=$(cmt_local_MTree_python_init_makefile) MTree_python_init

MTree_python_init :: $(MTree_python_init_dependencies) $(cmt_local_MTree_python_init_makefile) dirs MTree_python_initdirs
	$(echo) "(constituents.make) Starting MTree_python_init"
	@$(MAKE) -f $(cmt_local_MTree_python_init_makefile) MTree_python_init
	$(echo) "(constituents.make) MTree_python_init done"

clean :: MTree_python_initclean

MTree_python_initclean :: $(MTree_python_initclean_dependencies) ##$(cmt_local_MTree_python_init_makefile)
	$(echo) "(constituents.make) Starting MTree_python_initclean"
	@-if test -f $(cmt_local_MTree_python_init_makefile); then \
	  $(MAKE) -f $(cmt_local_MTree_python_init_makefile) MTree_python_initclean; \
	fi
	$(echo) "(constituents.make) MTree_python_initclean done"
#	@-$(MAKE) -f $(cmt_local_MTree_python_init_makefile) MTree_python_initclean

##	  /bin/rm -f $(cmt_local_MTree_python_init_makefile) $(bin)MTree_python_init_dependencies.make

install :: MTree_python_initinstall

MTree_python_initinstall :: $(MTree_python_init_dependencies) $(cmt_local_MTree_python_init_makefile)
	$(echo) "(constituents.make) Starting install MTree_python_init"
	@-$(MAKE) -f $(cmt_local_MTree_python_init_makefile) install
	$(echo) "(constituents.make) install MTree_python_init done"

uninstall :: MTree_python_inituninstall

$(foreach d,$(MTree_python_init_dependencies),$(eval $(d)uninstall_dependencies += MTree_python_inituninstall))

MTree_python_inituninstall :: $(MTree_python_inituninstall_dependencies) $(cmt_local_MTree_python_init_makefile)
	$(echo) "(constituents.make) Starting uninstall MTree_python_init"
	@$(MAKE) -f $(cmt_local_MTree_python_init_makefile) uninstall
	$(echo) "(constituents.make) uninstall MTree_python_init done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ MTree_python_init"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ MTree_python_init done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_MTreeConfDbMerge_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MTreeConfDbMerge_has_target_tag

#cmt_local_tagfile_MTreeConfDbMerge = $(MTree_tag)_MTreeConfDbMerge.make
cmt_local_tagfile_MTreeConfDbMerge = $(bin)$(MTree_tag)_MTreeConfDbMerge.make
cmt_local_setup_MTreeConfDbMerge = $(bin)setup_MTreeConfDbMerge$$$$.make
cmt_final_setup_MTreeConfDbMerge = $(bin)setup_MTreeConfDbMerge.make
#cmt_final_setup_MTreeConfDbMerge = $(bin)MTree_MTreeConfDbMergesetup.make
cmt_local_MTreeConfDbMerge_makefile = $(bin)MTreeConfDbMerge.make

MTreeConfDbMerge_extratags = -tag_add=target_MTreeConfDbMerge

#$(cmt_local_tagfile_MTreeConfDbMerge) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_MTreeConfDbMerge) ::
else
$(cmt_local_tagfile_MTreeConfDbMerge) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_MTreeConfDbMerge); then /bin/rm -f $(cmt_local_tagfile_MTreeConfDbMerge); fi ; \
	  $(cmtexe) -tag=$(tags) $(MTreeConfDbMerge_extratags) build tag_makefile >>$(cmt_local_tagfile_MTreeConfDbMerge)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_MTreeConfDbMerge)"; \
	  test ! -f $(cmt_local_setup_MTreeConfDbMerge) || \rm -f $(cmt_local_setup_MTreeConfDbMerge); \
	  trap '\rm -f $(cmt_local_setup_MTreeConfDbMerge)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(MTreeConfDbMerge_extratags) show setup >$(cmt_local_setup_MTreeConfDbMerge) && \
	  if [ -f $(cmt_final_setup_MTreeConfDbMerge) ] && \
	    \cmp -s $(cmt_final_setup_MTreeConfDbMerge) $(cmt_local_setup_MTreeConfDbMerge); then \
	    \rm $(cmt_local_setup_MTreeConfDbMerge); else \
	    \mv -f $(cmt_local_setup_MTreeConfDbMerge) $(cmt_final_setup_MTreeConfDbMerge); fi

else

#cmt_local_tagfile_MTreeConfDbMerge = $(MTree_tag).make
cmt_local_tagfile_MTreeConfDbMerge = $(bin)$(MTree_tag).make
cmt_final_setup_MTreeConfDbMerge = $(bin)setup.make
#cmt_final_setup_MTreeConfDbMerge = $(bin)MTreesetup.make
cmt_local_MTreeConfDbMerge_makefile = $(bin)MTreeConfDbMerge.make

endif

ifdef STRUCTURED_OUTPUT
MTreeConfDbMergedirs :
	@if test ! -d $(bin)MTreeConfDbMerge; then $(mkdir) -p $(bin)MTreeConfDbMerge; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)MTreeConfDbMerge
else
MTreeConfDbMergedirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# MTreeConfDbMergedirs ::
#	@if test ! -d $(bin)MTreeConfDbMerge; then $(mkdir) -p $(bin)MTreeConfDbMerge; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)MTreeConfDbMerge
#
#$(cmt_local_MTreeConfDbMerge_makefile) :: $(MTreeConfDbMerge_dependencies) $(cmt_local_tagfile_MTreeConfDbMerge) build_library_links dirs MTreeConfDbMergedirs
#else
#$(cmt_local_MTreeConfDbMerge_makefile) :: $(MTreeConfDbMerge_dependencies) $(cmt_local_tagfile_MTreeConfDbMerge) build_library_links dirs
#endif
#else
#$(cmt_local_MTreeConfDbMerge_makefile) :: $(cmt_local_tagfile_MTreeConfDbMerge)
#endif

makefiles : $(cmt_local_MTreeConfDbMerge_makefile)

ifndef QUICK
$(cmt_local_MTreeConfDbMerge_makefile) : $(MTreeConfDbMerge_dependencies) build_library_links
#$(cmt_local_MTreeConfDbMerge_makefile) : $(MTreeConfDbMerge_dependencies) $(cmt_local_tagfile_MTreeConfDbMerge) build_library_links
	$(echo) "(constituents.make) Building MTreeConfDbMerge.make"; \
	  $(cmtexe) -tag=$(tags) $(MTreeConfDbMerge_extratags) build constituent_config -out=$(cmt_local_MTreeConfDbMerge_makefile) MTreeConfDbMerge
else
$(cmt_local_MTreeConfDbMerge_makefile) : $(MTreeConfDbMerge_dependencies)
#$(cmt_local_MTreeConfDbMerge_makefile) : $(cmt_local_tagfile_MTreeConfDbMerge)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_MTreeConfDbMerge) ] || \
	  [ ! -f $(cmt_final_setup_MTreeConfDbMerge) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building MTreeConfDbMerge.make"; \
	  $(cmtexe) -tag=$(tags) $(MTreeConfDbMerge_extratags) build constituent_config -out=$(cmt_local_MTreeConfDbMerge_makefile) MTreeConfDbMerge; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(MTreeConfDbMerge_extratags) build constituent_makefile -out=$(cmt_local_MTreeConfDbMerge_makefile) MTreeConfDbMerge

MTreeConfDbMerge :: $(MTreeConfDbMerge_dependencies) $(cmt_local_MTreeConfDbMerge_makefile) dirs MTreeConfDbMergedirs
	$(echo) "(constituents.make) Starting MTreeConfDbMerge"
	@$(MAKE) -f $(cmt_local_MTreeConfDbMerge_makefile) MTreeConfDbMerge
	$(echo) "(constituents.make) MTreeConfDbMerge done"

clean :: MTreeConfDbMergeclean

MTreeConfDbMergeclean :: $(MTreeConfDbMergeclean_dependencies) ##$(cmt_local_MTreeConfDbMerge_makefile)
	$(echo) "(constituents.make) Starting MTreeConfDbMergeclean"
	@-if test -f $(cmt_local_MTreeConfDbMerge_makefile); then \
	  $(MAKE) -f $(cmt_local_MTreeConfDbMerge_makefile) MTreeConfDbMergeclean; \
	fi
	$(echo) "(constituents.make) MTreeConfDbMergeclean done"
#	@-$(MAKE) -f $(cmt_local_MTreeConfDbMerge_makefile) MTreeConfDbMergeclean

##	  /bin/rm -f $(cmt_local_MTreeConfDbMerge_makefile) $(bin)MTreeConfDbMerge_dependencies.make

install :: MTreeConfDbMergeinstall

MTreeConfDbMergeinstall :: $(MTreeConfDbMerge_dependencies) $(cmt_local_MTreeConfDbMerge_makefile)
	$(echo) "(constituents.make) Starting install MTreeConfDbMerge"
	@-$(MAKE) -f $(cmt_local_MTreeConfDbMerge_makefile) install
	$(echo) "(constituents.make) install MTreeConfDbMerge done"

uninstall :: MTreeConfDbMergeuninstall

$(foreach d,$(MTreeConfDbMerge_dependencies),$(eval $(d)uninstall_dependencies += MTreeConfDbMergeuninstall))

MTreeConfDbMergeuninstall :: $(MTreeConfDbMergeuninstall_dependencies) $(cmt_local_MTreeConfDbMerge_makefile)
	$(echo) "(constituents.make) Starting uninstall MTreeConfDbMerge"
	@$(MAKE) -f $(cmt_local_MTreeConfDbMerge_makefile) uninstall
	$(echo) "(constituents.make) uninstall MTreeConfDbMerge done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ MTreeConfDbMerge"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ MTreeConfDbMerge done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_MTreeRootMap_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MTreeRootMap_has_target_tag

#cmt_local_tagfile_MTreeRootMap = $(MTree_tag)_MTreeRootMap.make
cmt_local_tagfile_MTreeRootMap = $(bin)$(MTree_tag)_MTreeRootMap.make
cmt_local_setup_MTreeRootMap = $(bin)setup_MTreeRootMap$$$$.make
cmt_final_setup_MTreeRootMap = $(bin)setup_MTreeRootMap.make
#cmt_final_setup_MTreeRootMap = $(bin)MTree_MTreeRootMapsetup.make
cmt_local_MTreeRootMap_makefile = $(bin)MTreeRootMap.make

MTreeRootMap_extratags = -tag_add=target_MTreeRootMap

#$(cmt_local_tagfile_MTreeRootMap) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_MTreeRootMap) ::
else
$(cmt_local_tagfile_MTreeRootMap) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_MTreeRootMap); then /bin/rm -f $(cmt_local_tagfile_MTreeRootMap); fi ; \
	  $(cmtexe) -tag=$(tags) $(MTreeRootMap_extratags) build tag_makefile >>$(cmt_local_tagfile_MTreeRootMap)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_MTreeRootMap)"; \
	  test ! -f $(cmt_local_setup_MTreeRootMap) || \rm -f $(cmt_local_setup_MTreeRootMap); \
	  trap '\rm -f $(cmt_local_setup_MTreeRootMap)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(MTreeRootMap_extratags) show setup >$(cmt_local_setup_MTreeRootMap) && \
	  if [ -f $(cmt_final_setup_MTreeRootMap) ] && \
	    \cmp -s $(cmt_final_setup_MTreeRootMap) $(cmt_local_setup_MTreeRootMap); then \
	    \rm $(cmt_local_setup_MTreeRootMap); else \
	    \mv -f $(cmt_local_setup_MTreeRootMap) $(cmt_final_setup_MTreeRootMap); fi

else

#cmt_local_tagfile_MTreeRootMap = $(MTree_tag).make
cmt_local_tagfile_MTreeRootMap = $(bin)$(MTree_tag).make
cmt_final_setup_MTreeRootMap = $(bin)setup.make
#cmt_final_setup_MTreeRootMap = $(bin)MTreesetup.make
cmt_local_MTreeRootMap_makefile = $(bin)MTreeRootMap.make

endif

ifdef STRUCTURED_OUTPUT
MTreeRootMapdirs :
	@if test ! -d $(bin)MTreeRootMap; then $(mkdir) -p $(bin)MTreeRootMap; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)MTreeRootMap
else
MTreeRootMapdirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# MTreeRootMapdirs ::
#	@if test ! -d $(bin)MTreeRootMap; then $(mkdir) -p $(bin)MTreeRootMap; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)MTreeRootMap
#
#$(cmt_local_MTreeRootMap_makefile) :: $(MTreeRootMap_dependencies) $(cmt_local_tagfile_MTreeRootMap) build_library_links dirs MTreeRootMapdirs
#else
#$(cmt_local_MTreeRootMap_makefile) :: $(MTreeRootMap_dependencies) $(cmt_local_tagfile_MTreeRootMap) build_library_links dirs
#endif
#else
#$(cmt_local_MTreeRootMap_makefile) :: $(cmt_local_tagfile_MTreeRootMap)
#endif

makefiles : $(cmt_local_MTreeRootMap_makefile)

ifndef QUICK
$(cmt_local_MTreeRootMap_makefile) : $(MTreeRootMap_dependencies) build_library_links
#$(cmt_local_MTreeRootMap_makefile) : $(MTreeRootMap_dependencies) $(cmt_local_tagfile_MTreeRootMap) build_library_links
	$(echo) "(constituents.make) Building MTreeRootMap.make"; \
	  $(cmtexe) -tag=$(tags) $(MTreeRootMap_extratags) build constituent_config -out=$(cmt_local_MTreeRootMap_makefile) MTreeRootMap
else
$(cmt_local_MTreeRootMap_makefile) : $(MTreeRootMap_dependencies)
#$(cmt_local_MTreeRootMap_makefile) : $(cmt_local_tagfile_MTreeRootMap)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_MTreeRootMap) ] || \
	  [ ! -f $(cmt_final_setup_MTreeRootMap) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building MTreeRootMap.make"; \
	  $(cmtexe) -tag=$(tags) $(MTreeRootMap_extratags) build constituent_config -out=$(cmt_local_MTreeRootMap_makefile) MTreeRootMap; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(MTreeRootMap_extratags) build constituent_makefile -out=$(cmt_local_MTreeRootMap_makefile) MTreeRootMap

MTreeRootMap :: $(MTreeRootMap_dependencies) $(cmt_local_MTreeRootMap_makefile) dirs MTreeRootMapdirs
	$(echo) "(constituents.make) Starting MTreeRootMap"
	@$(MAKE) -f $(cmt_local_MTreeRootMap_makefile) MTreeRootMap
	$(echo) "(constituents.make) MTreeRootMap done"

clean :: MTreeRootMapclean

MTreeRootMapclean :: $(MTreeRootMapclean_dependencies) ##$(cmt_local_MTreeRootMap_makefile)
	$(echo) "(constituents.make) Starting MTreeRootMapclean"
	@-if test -f $(cmt_local_MTreeRootMap_makefile); then \
	  $(MAKE) -f $(cmt_local_MTreeRootMap_makefile) MTreeRootMapclean; \
	fi
	$(echo) "(constituents.make) MTreeRootMapclean done"
#	@-$(MAKE) -f $(cmt_local_MTreeRootMap_makefile) MTreeRootMapclean

##	  /bin/rm -f $(cmt_local_MTreeRootMap_makefile) $(bin)MTreeRootMap_dependencies.make

install :: MTreeRootMapinstall

MTreeRootMapinstall :: $(MTreeRootMap_dependencies) $(cmt_local_MTreeRootMap_makefile)
	$(echo) "(constituents.make) Starting install MTreeRootMap"
	@-$(MAKE) -f $(cmt_local_MTreeRootMap_makefile) install
	$(echo) "(constituents.make) install MTreeRootMap done"

uninstall :: MTreeRootMapuninstall

$(foreach d,$(MTreeRootMap_dependencies),$(eval $(d)uninstall_dependencies += MTreeRootMapuninstall))

MTreeRootMapuninstall :: $(MTreeRootMapuninstall_dependencies) $(cmt_local_MTreeRootMap_makefile)
	$(echo) "(constituents.make) Starting uninstall MTreeRootMap"
	@$(MAKE) -f $(cmt_local_MTreeRootMap_makefile) uninstall
	$(echo) "(constituents.make) uninstall MTreeRootMap done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ MTreeRootMap"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ MTreeRootMap done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_MTreeMergeMap_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MTreeMergeMap_has_target_tag

#cmt_local_tagfile_MTreeMergeMap = $(MTree_tag)_MTreeMergeMap.make
cmt_local_tagfile_MTreeMergeMap = $(bin)$(MTree_tag)_MTreeMergeMap.make
cmt_local_setup_MTreeMergeMap = $(bin)setup_MTreeMergeMap$$$$.make
cmt_final_setup_MTreeMergeMap = $(bin)setup_MTreeMergeMap.make
#cmt_final_setup_MTreeMergeMap = $(bin)MTree_MTreeMergeMapsetup.make
cmt_local_MTreeMergeMap_makefile = $(bin)MTreeMergeMap.make

MTreeMergeMap_extratags = -tag_add=target_MTreeMergeMap

#$(cmt_local_tagfile_MTreeMergeMap) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_MTreeMergeMap) ::
else
$(cmt_local_tagfile_MTreeMergeMap) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_MTreeMergeMap); then /bin/rm -f $(cmt_local_tagfile_MTreeMergeMap); fi ; \
	  $(cmtexe) -tag=$(tags) $(MTreeMergeMap_extratags) build tag_makefile >>$(cmt_local_tagfile_MTreeMergeMap)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_MTreeMergeMap)"; \
	  test ! -f $(cmt_local_setup_MTreeMergeMap) || \rm -f $(cmt_local_setup_MTreeMergeMap); \
	  trap '\rm -f $(cmt_local_setup_MTreeMergeMap)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(MTreeMergeMap_extratags) show setup >$(cmt_local_setup_MTreeMergeMap) && \
	  if [ -f $(cmt_final_setup_MTreeMergeMap) ] && \
	    \cmp -s $(cmt_final_setup_MTreeMergeMap) $(cmt_local_setup_MTreeMergeMap); then \
	    \rm $(cmt_local_setup_MTreeMergeMap); else \
	    \mv -f $(cmt_local_setup_MTreeMergeMap) $(cmt_final_setup_MTreeMergeMap); fi

else

#cmt_local_tagfile_MTreeMergeMap = $(MTree_tag).make
cmt_local_tagfile_MTreeMergeMap = $(bin)$(MTree_tag).make
cmt_final_setup_MTreeMergeMap = $(bin)setup.make
#cmt_final_setup_MTreeMergeMap = $(bin)MTreesetup.make
cmt_local_MTreeMergeMap_makefile = $(bin)MTreeMergeMap.make

endif

ifdef STRUCTURED_OUTPUT
MTreeMergeMapdirs :
	@if test ! -d $(bin)MTreeMergeMap; then $(mkdir) -p $(bin)MTreeMergeMap; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)MTreeMergeMap
else
MTreeMergeMapdirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# MTreeMergeMapdirs ::
#	@if test ! -d $(bin)MTreeMergeMap; then $(mkdir) -p $(bin)MTreeMergeMap; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)MTreeMergeMap
#
#$(cmt_local_MTreeMergeMap_makefile) :: $(MTreeMergeMap_dependencies) $(cmt_local_tagfile_MTreeMergeMap) build_library_links dirs MTreeMergeMapdirs
#else
#$(cmt_local_MTreeMergeMap_makefile) :: $(MTreeMergeMap_dependencies) $(cmt_local_tagfile_MTreeMergeMap) build_library_links dirs
#endif
#else
#$(cmt_local_MTreeMergeMap_makefile) :: $(cmt_local_tagfile_MTreeMergeMap)
#endif

makefiles : $(cmt_local_MTreeMergeMap_makefile)

ifndef QUICK
$(cmt_local_MTreeMergeMap_makefile) : $(MTreeMergeMap_dependencies) build_library_links
#$(cmt_local_MTreeMergeMap_makefile) : $(MTreeMergeMap_dependencies) $(cmt_local_tagfile_MTreeMergeMap) build_library_links
	$(echo) "(constituents.make) Building MTreeMergeMap.make"; \
	  $(cmtexe) -tag=$(tags) $(MTreeMergeMap_extratags) build constituent_config -out=$(cmt_local_MTreeMergeMap_makefile) MTreeMergeMap
else
$(cmt_local_MTreeMergeMap_makefile) : $(MTreeMergeMap_dependencies)
#$(cmt_local_MTreeMergeMap_makefile) : $(cmt_local_tagfile_MTreeMergeMap)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_MTreeMergeMap) ] || \
	  [ ! -f $(cmt_final_setup_MTreeMergeMap) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building MTreeMergeMap.make"; \
	  $(cmtexe) -tag=$(tags) $(MTreeMergeMap_extratags) build constituent_config -out=$(cmt_local_MTreeMergeMap_makefile) MTreeMergeMap; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(MTreeMergeMap_extratags) build constituent_makefile -out=$(cmt_local_MTreeMergeMap_makefile) MTreeMergeMap

MTreeMergeMap :: $(MTreeMergeMap_dependencies) $(cmt_local_MTreeMergeMap_makefile) dirs MTreeMergeMapdirs
	$(echo) "(constituents.make) Starting MTreeMergeMap"
	@$(MAKE) -f $(cmt_local_MTreeMergeMap_makefile) MTreeMergeMap
	$(echo) "(constituents.make) MTreeMergeMap done"

clean :: MTreeMergeMapclean

MTreeMergeMapclean :: $(MTreeMergeMapclean_dependencies) ##$(cmt_local_MTreeMergeMap_makefile)
	$(echo) "(constituents.make) Starting MTreeMergeMapclean"
	@-if test -f $(cmt_local_MTreeMergeMap_makefile); then \
	  $(MAKE) -f $(cmt_local_MTreeMergeMap_makefile) MTreeMergeMapclean; \
	fi
	$(echo) "(constituents.make) MTreeMergeMapclean done"
#	@-$(MAKE) -f $(cmt_local_MTreeMergeMap_makefile) MTreeMergeMapclean

##	  /bin/rm -f $(cmt_local_MTreeMergeMap_makefile) $(bin)MTreeMergeMap_dependencies.make

install :: MTreeMergeMapinstall

MTreeMergeMapinstall :: $(MTreeMergeMap_dependencies) $(cmt_local_MTreeMergeMap_makefile)
	$(echo) "(constituents.make) Starting install MTreeMergeMap"
	@-$(MAKE) -f $(cmt_local_MTreeMergeMap_makefile) install
	$(echo) "(constituents.make) install MTreeMergeMap done"

uninstall :: MTreeMergeMapuninstall

$(foreach d,$(MTreeMergeMap_dependencies),$(eval $(d)uninstall_dependencies += MTreeMergeMapuninstall))

MTreeMergeMapuninstall :: $(MTreeMergeMapuninstall_dependencies) $(cmt_local_MTreeMergeMap_makefile)
	$(echo) "(constituents.make) Starting uninstall MTreeMergeMap"
	@$(MAKE) -f $(cmt_local_MTreeMergeMap_makefile) uninstall
	$(echo) "(constituents.make) uninstall MTreeMergeMap done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ MTreeMergeMap"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ MTreeMergeMap done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_MTreeCLIDDB_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MTreeCLIDDB_has_target_tag

#cmt_local_tagfile_MTreeCLIDDB = $(MTree_tag)_MTreeCLIDDB.make
cmt_local_tagfile_MTreeCLIDDB = $(bin)$(MTree_tag)_MTreeCLIDDB.make
cmt_local_setup_MTreeCLIDDB = $(bin)setup_MTreeCLIDDB$$$$.make
cmt_final_setup_MTreeCLIDDB = $(bin)setup_MTreeCLIDDB.make
#cmt_final_setup_MTreeCLIDDB = $(bin)MTree_MTreeCLIDDBsetup.make
cmt_local_MTreeCLIDDB_makefile = $(bin)MTreeCLIDDB.make

MTreeCLIDDB_extratags = -tag_add=target_MTreeCLIDDB

#$(cmt_local_tagfile_MTreeCLIDDB) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_MTreeCLIDDB) ::
else
$(cmt_local_tagfile_MTreeCLIDDB) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_MTreeCLIDDB); then /bin/rm -f $(cmt_local_tagfile_MTreeCLIDDB); fi ; \
	  $(cmtexe) -tag=$(tags) $(MTreeCLIDDB_extratags) build tag_makefile >>$(cmt_local_tagfile_MTreeCLIDDB)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_MTreeCLIDDB)"; \
	  test ! -f $(cmt_local_setup_MTreeCLIDDB) || \rm -f $(cmt_local_setup_MTreeCLIDDB); \
	  trap '\rm -f $(cmt_local_setup_MTreeCLIDDB)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(MTreeCLIDDB_extratags) show setup >$(cmt_local_setup_MTreeCLIDDB) && \
	  if [ -f $(cmt_final_setup_MTreeCLIDDB) ] && \
	    \cmp -s $(cmt_final_setup_MTreeCLIDDB) $(cmt_local_setup_MTreeCLIDDB); then \
	    \rm $(cmt_local_setup_MTreeCLIDDB); else \
	    \mv -f $(cmt_local_setup_MTreeCLIDDB) $(cmt_final_setup_MTreeCLIDDB); fi

else

#cmt_local_tagfile_MTreeCLIDDB = $(MTree_tag).make
cmt_local_tagfile_MTreeCLIDDB = $(bin)$(MTree_tag).make
cmt_final_setup_MTreeCLIDDB = $(bin)setup.make
#cmt_final_setup_MTreeCLIDDB = $(bin)MTreesetup.make
cmt_local_MTreeCLIDDB_makefile = $(bin)MTreeCLIDDB.make

endif

ifdef STRUCTURED_OUTPUT
MTreeCLIDDBdirs :
	@if test ! -d $(bin)MTreeCLIDDB; then $(mkdir) -p $(bin)MTreeCLIDDB; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)MTreeCLIDDB
else
MTreeCLIDDBdirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# MTreeCLIDDBdirs ::
#	@if test ! -d $(bin)MTreeCLIDDB; then $(mkdir) -p $(bin)MTreeCLIDDB; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)MTreeCLIDDB
#
#$(cmt_local_MTreeCLIDDB_makefile) :: $(MTreeCLIDDB_dependencies) $(cmt_local_tagfile_MTreeCLIDDB) build_library_links dirs MTreeCLIDDBdirs
#else
#$(cmt_local_MTreeCLIDDB_makefile) :: $(MTreeCLIDDB_dependencies) $(cmt_local_tagfile_MTreeCLIDDB) build_library_links dirs
#endif
#else
#$(cmt_local_MTreeCLIDDB_makefile) :: $(cmt_local_tagfile_MTreeCLIDDB)
#endif

makefiles : $(cmt_local_MTreeCLIDDB_makefile)

ifndef QUICK
$(cmt_local_MTreeCLIDDB_makefile) : $(MTreeCLIDDB_dependencies) build_library_links
#$(cmt_local_MTreeCLIDDB_makefile) : $(MTreeCLIDDB_dependencies) $(cmt_local_tagfile_MTreeCLIDDB) build_library_links
	$(echo) "(constituents.make) Building MTreeCLIDDB.make"; \
	  $(cmtexe) -tag=$(tags) $(MTreeCLIDDB_extratags) build constituent_config -out=$(cmt_local_MTreeCLIDDB_makefile) MTreeCLIDDB
else
$(cmt_local_MTreeCLIDDB_makefile) : $(MTreeCLIDDB_dependencies)
#$(cmt_local_MTreeCLIDDB_makefile) : $(cmt_local_tagfile_MTreeCLIDDB)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_MTreeCLIDDB) ] || \
	  [ ! -f $(cmt_final_setup_MTreeCLIDDB) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building MTreeCLIDDB.make"; \
	  $(cmtexe) -tag=$(tags) $(MTreeCLIDDB_extratags) build constituent_config -out=$(cmt_local_MTreeCLIDDB_makefile) MTreeCLIDDB; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(MTreeCLIDDB_extratags) build constituent_makefile -out=$(cmt_local_MTreeCLIDDB_makefile) MTreeCLIDDB

MTreeCLIDDB :: $(MTreeCLIDDB_dependencies) $(cmt_local_MTreeCLIDDB_makefile) dirs MTreeCLIDDBdirs
	$(echo) "(constituents.make) Starting MTreeCLIDDB"
	@$(MAKE) -f $(cmt_local_MTreeCLIDDB_makefile) MTreeCLIDDB
	$(echo) "(constituents.make) MTreeCLIDDB done"

clean :: MTreeCLIDDBclean

MTreeCLIDDBclean :: $(MTreeCLIDDBclean_dependencies) ##$(cmt_local_MTreeCLIDDB_makefile)
	$(echo) "(constituents.make) Starting MTreeCLIDDBclean"
	@-if test -f $(cmt_local_MTreeCLIDDB_makefile); then \
	  $(MAKE) -f $(cmt_local_MTreeCLIDDB_makefile) MTreeCLIDDBclean; \
	fi
	$(echo) "(constituents.make) MTreeCLIDDBclean done"
#	@-$(MAKE) -f $(cmt_local_MTreeCLIDDB_makefile) MTreeCLIDDBclean

##	  /bin/rm -f $(cmt_local_MTreeCLIDDB_makefile) $(bin)MTreeCLIDDB_dependencies.make

install :: MTreeCLIDDBinstall

MTreeCLIDDBinstall :: $(MTreeCLIDDB_dependencies) $(cmt_local_MTreeCLIDDB_makefile)
	$(echo) "(constituents.make) Starting install MTreeCLIDDB"
	@-$(MAKE) -f $(cmt_local_MTreeCLIDDB_makefile) install
	$(echo) "(constituents.make) install MTreeCLIDDB done"

uninstall :: MTreeCLIDDBuninstall

$(foreach d,$(MTreeCLIDDB_dependencies),$(eval $(d)uninstall_dependencies += MTreeCLIDDBuninstall))

MTreeCLIDDBuninstall :: $(MTreeCLIDDBuninstall_dependencies) $(cmt_local_MTreeCLIDDB_makefile)
	$(echo) "(constituents.make) Starting uninstall MTreeCLIDDB"
	@$(MAKE) -f $(cmt_local_MTreeCLIDDB_makefile) uninstall
	$(echo) "(constituents.make) uninstall MTreeCLIDDB done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ MTreeCLIDDB"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ MTreeCLIDDB done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_MTreerchk_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MTreerchk_has_target_tag

#cmt_local_tagfile_MTreerchk = $(MTree_tag)_MTreerchk.make
cmt_local_tagfile_MTreerchk = $(bin)$(MTree_tag)_MTreerchk.make
cmt_local_setup_MTreerchk = $(bin)setup_MTreerchk$$$$.make
cmt_final_setup_MTreerchk = $(bin)setup_MTreerchk.make
#cmt_final_setup_MTreerchk = $(bin)MTree_MTreerchksetup.make
cmt_local_MTreerchk_makefile = $(bin)MTreerchk.make

MTreerchk_extratags = -tag_add=target_MTreerchk

#$(cmt_local_tagfile_MTreerchk) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_MTreerchk) ::
else
$(cmt_local_tagfile_MTreerchk) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_MTreerchk); then /bin/rm -f $(cmt_local_tagfile_MTreerchk); fi ; \
	  $(cmtexe) -tag=$(tags) $(MTreerchk_extratags) build tag_makefile >>$(cmt_local_tagfile_MTreerchk)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_MTreerchk)"; \
	  test ! -f $(cmt_local_setup_MTreerchk) || \rm -f $(cmt_local_setup_MTreerchk); \
	  trap '\rm -f $(cmt_local_setup_MTreerchk)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(MTreerchk_extratags) show setup >$(cmt_local_setup_MTreerchk) && \
	  if [ -f $(cmt_final_setup_MTreerchk) ] && \
	    \cmp -s $(cmt_final_setup_MTreerchk) $(cmt_local_setup_MTreerchk); then \
	    \rm $(cmt_local_setup_MTreerchk); else \
	    \mv -f $(cmt_local_setup_MTreerchk) $(cmt_final_setup_MTreerchk); fi

else

#cmt_local_tagfile_MTreerchk = $(MTree_tag).make
cmt_local_tagfile_MTreerchk = $(bin)$(MTree_tag).make
cmt_final_setup_MTreerchk = $(bin)setup.make
#cmt_final_setup_MTreerchk = $(bin)MTreesetup.make
cmt_local_MTreerchk_makefile = $(bin)MTreerchk.make

endif

ifdef STRUCTURED_OUTPUT
MTreerchkdirs :
	@if test ! -d $(bin)MTreerchk; then $(mkdir) -p $(bin)MTreerchk; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)MTreerchk
else
MTreerchkdirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# MTreerchkdirs ::
#	@if test ! -d $(bin)MTreerchk; then $(mkdir) -p $(bin)MTreerchk; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)MTreerchk
#
#$(cmt_local_MTreerchk_makefile) :: $(MTreerchk_dependencies) $(cmt_local_tagfile_MTreerchk) build_library_links dirs MTreerchkdirs
#else
#$(cmt_local_MTreerchk_makefile) :: $(MTreerchk_dependencies) $(cmt_local_tagfile_MTreerchk) build_library_links dirs
#endif
#else
#$(cmt_local_MTreerchk_makefile) :: $(cmt_local_tagfile_MTreerchk)
#endif

makefiles : $(cmt_local_MTreerchk_makefile)

ifndef QUICK
$(cmt_local_MTreerchk_makefile) : $(MTreerchk_dependencies) build_library_links
#$(cmt_local_MTreerchk_makefile) : $(MTreerchk_dependencies) $(cmt_local_tagfile_MTreerchk) build_library_links
	$(echo) "(constituents.make) Building MTreerchk.make"; \
	  $(cmtexe) -tag=$(tags) $(MTreerchk_extratags) build constituent_config -out=$(cmt_local_MTreerchk_makefile) MTreerchk
else
$(cmt_local_MTreerchk_makefile) : $(MTreerchk_dependencies)
#$(cmt_local_MTreerchk_makefile) : $(cmt_local_tagfile_MTreerchk)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_MTreerchk) ] || \
	  [ ! -f $(cmt_final_setup_MTreerchk) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building MTreerchk.make"; \
	  $(cmtexe) -tag=$(tags) $(MTreerchk_extratags) build constituent_config -out=$(cmt_local_MTreerchk_makefile) MTreerchk; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(MTreerchk_extratags) build constituent_makefile -out=$(cmt_local_MTreerchk_makefile) MTreerchk

MTreerchk :: $(MTreerchk_dependencies) $(cmt_local_MTreerchk_makefile) dirs MTreerchkdirs
	$(echo) "(constituents.make) Starting MTreerchk"
	@$(MAKE) -f $(cmt_local_MTreerchk_makefile) MTreerchk
	$(echo) "(constituents.make) MTreerchk done"

clean :: MTreerchkclean

MTreerchkclean :: $(MTreerchkclean_dependencies) ##$(cmt_local_MTreerchk_makefile)
	$(echo) "(constituents.make) Starting MTreerchkclean"
	@-if test -f $(cmt_local_MTreerchk_makefile); then \
	  $(MAKE) -f $(cmt_local_MTreerchk_makefile) MTreerchkclean; \
	fi
	$(echo) "(constituents.make) MTreerchkclean done"
#	@-$(MAKE) -f $(cmt_local_MTreerchk_makefile) MTreerchkclean

##	  /bin/rm -f $(cmt_local_MTreerchk_makefile) $(bin)MTreerchk_dependencies.make

install :: MTreerchkinstall

MTreerchkinstall :: $(MTreerchk_dependencies) $(cmt_local_MTreerchk_makefile)
	$(echo) "(constituents.make) Starting install MTreerchk"
	@-$(MAKE) -f $(cmt_local_MTreerchk_makefile) install
	$(echo) "(constituents.make) install MTreerchk done"

uninstall :: MTreerchkuninstall

$(foreach d,$(MTreerchk_dependencies),$(eval $(d)uninstall_dependencies += MTreerchkuninstall))

MTreerchkuninstall :: $(MTreerchkuninstall_dependencies) $(cmt_local_MTreerchk_makefile)
	$(echo) "(constituents.make) Starting uninstall MTreerchk"
	@$(MAKE) -f $(cmt_local_MTreerchk_makefile) uninstall
	$(echo) "(constituents.make) uninstall MTreerchk done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ MTreerchk"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ MTreerchk done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_myctest_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_myctest_has_target_tag

#cmt_local_tagfile_myctest = $(MTree_tag)_myctest.make
cmt_local_tagfile_myctest = $(bin)$(MTree_tag)_myctest.make
cmt_local_setup_myctest = $(bin)setup_myctest$$$$.make
cmt_final_setup_myctest = $(bin)setup_myctest.make
#cmt_final_setup_myctest = $(bin)MTree_myctestsetup.make
cmt_local_myctest_makefile = $(bin)myctest.make

myctest_extratags = -tag_add=target_myctest

#$(cmt_local_tagfile_myctest) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_myctest) ::
else
$(cmt_local_tagfile_myctest) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_myctest); then /bin/rm -f $(cmt_local_tagfile_myctest); fi ; \
	  $(cmtexe) -tag=$(tags) $(myctest_extratags) build tag_makefile >>$(cmt_local_tagfile_myctest)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_myctest)"; \
	  test ! -f $(cmt_local_setup_myctest) || \rm -f $(cmt_local_setup_myctest); \
	  trap '\rm -f $(cmt_local_setup_myctest)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(myctest_extratags) show setup >$(cmt_local_setup_myctest) && \
	  if [ -f $(cmt_final_setup_myctest) ] && \
	    \cmp -s $(cmt_final_setup_myctest) $(cmt_local_setup_myctest); then \
	    \rm $(cmt_local_setup_myctest); else \
	    \mv -f $(cmt_local_setup_myctest) $(cmt_final_setup_myctest); fi

else

#cmt_local_tagfile_myctest = $(MTree_tag).make
cmt_local_tagfile_myctest = $(bin)$(MTree_tag).make
cmt_final_setup_myctest = $(bin)setup.make
#cmt_final_setup_myctest = $(bin)MTreesetup.make
cmt_local_myctest_makefile = $(bin)myctest.make

endif

ifdef STRUCTURED_OUTPUT
myctestdirs :
	@if test ! -d $(bin)myctest; then $(mkdir) -p $(bin)myctest; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)myctest
else
myctestdirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# myctestdirs ::
#	@if test ! -d $(bin)myctest; then $(mkdir) -p $(bin)myctest; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)myctest
#
#$(cmt_local_myctest_makefile) :: $(myctest_dependencies) $(cmt_local_tagfile_myctest) build_library_links dirs myctestdirs
#else
#$(cmt_local_myctest_makefile) :: $(myctest_dependencies) $(cmt_local_tagfile_myctest) build_library_links dirs
#endif
#else
#$(cmt_local_myctest_makefile) :: $(cmt_local_tagfile_myctest)
#endif

makefiles : $(cmt_local_myctest_makefile)

ifndef QUICK
$(cmt_local_myctest_makefile) : $(myctest_dependencies) build_library_links
#$(cmt_local_myctest_makefile) : $(myctest_dependencies) $(cmt_local_tagfile_myctest) build_library_links
	$(echo) "(constituents.make) Building myctest.make"; \
	  $(cmtexe) -tag=$(tags) $(myctest_extratags) build constituent_config -out=$(cmt_local_myctest_makefile) myctest
else
$(cmt_local_myctest_makefile) : $(myctest_dependencies)
#$(cmt_local_myctest_makefile) : $(cmt_local_tagfile_myctest)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_myctest) ] || \
	  [ ! -f $(cmt_final_setup_myctest) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building myctest.make"; \
	  $(cmtexe) -tag=$(tags) $(myctest_extratags) build constituent_config -out=$(cmt_local_myctest_makefile) myctest; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(myctest_extratags) build constituent_makefile -out=$(cmt_local_myctest_makefile) myctest

myctest :: $(myctest_dependencies) $(cmt_local_myctest_makefile) dirs myctestdirs
	$(echo) "(constituents.make) Starting myctest"
	@$(MAKE) -f $(cmt_local_myctest_makefile) myctest
	$(echo) "(constituents.make) myctest done"

clean :: myctestclean

myctestclean :: $(myctestclean_dependencies) ##$(cmt_local_myctest_makefile)
	$(echo) "(constituents.make) Starting myctestclean"
	@-if test -f $(cmt_local_myctest_makefile); then \
	  $(MAKE) -f $(cmt_local_myctest_makefile) myctestclean; \
	fi
	$(echo) "(constituents.make) myctestclean done"
#	@-$(MAKE) -f $(cmt_local_myctest_makefile) myctestclean

##	  /bin/rm -f $(cmt_local_myctest_makefile) $(bin)myctest_dependencies.make

install :: myctestinstall

myctestinstall :: $(myctest_dependencies) $(cmt_local_myctest_makefile)
	$(echo) "(constituents.make) Starting install myctest"
	@-$(MAKE) -f $(cmt_local_myctest_makefile) install
	$(echo) "(constituents.make) install myctest done"

uninstall :: myctestuninstall

$(foreach d,$(myctest_dependencies),$(eval $(d)uninstall_dependencies += myctestuninstall))

myctestuninstall :: $(myctestuninstall_dependencies) $(cmt_local_myctest_makefile)
	$(echo) "(constituents.make) Starting uninstall myctest"
	@$(MAKE) -f $(cmt_local_myctest_makefile) uninstall
	$(echo) "(constituents.make) uninstall myctest done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ myctest"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ myctest done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_install_includes_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_install_includes_has_target_tag

#cmt_local_tagfile_install_includes = $(MTree_tag)_install_includes.make
cmt_local_tagfile_install_includes = $(bin)$(MTree_tag)_install_includes.make
cmt_local_setup_install_includes = $(bin)setup_install_includes$$$$.make
cmt_final_setup_install_includes = $(bin)setup_install_includes.make
#cmt_final_setup_install_includes = $(bin)MTree_install_includessetup.make
cmt_local_install_includes_makefile = $(bin)install_includes.make

install_includes_extratags = -tag_add=target_install_includes

#$(cmt_local_tagfile_install_includes) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_install_includes) ::
else
$(cmt_local_tagfile_install_includes) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_install_includes); then /bin/rm -f $(cmt_local_tagfile_install_includes); fi ; \
	  $(cmtexe) -tag=$(tags) $(install_includes_extratags) build tag_makefile >>$(cmt_local_tagfile_install_includes)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_install_includes)"; \
	  test ! -f $(cmt_local_setup_install_includes) || \rm -f $(cmt_local_setup_install_includes); \
	  trap '\rm -f $(cmt_local_setup_install_includes)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(install_includes_extratags) show setup >$(cmt_local_setup_install_includes) && \
	  if [ -f $(cmt_final_setup_install_includes) ] && \
	    \cmp -s $(cmt_final_setup_install_includes) $(cmt_local_setup_install_includes); then \
	    \rm $(cmt_local_setup_install_includes); else \
	    \mv -f $(cmt_local_setup_install_includes) $(cmt_final_setup_install_includes); fi

else

#cmt_local_tagfile_install_includes = $(MTree_tag).make
cmt_local_tagfile_install_includes = $(bin)$(MTree_tag).make
cmt_final_setup_install_includes = $(bin)setup.make
#cmt_final_setup_install_includes = $(bin)MTreesetup.make
cmt_local_install_includes_makefile = $(bin)install_includes.make

endif

ifdef STRUCTURED_OUTPUT
install_includesdirs :
	@if test ! -d $(bin)install_includes; then $(mkdir) -p $(bin)install_includes; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)install_includes
else
install_includesdirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# install_includesdirs ::
#	@if test ! -d $(bin)install_includes; then $(mkdir) -p $(bin)install_includes; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)install_includes
#
#$(cmt_local_install_includes_makefile) :: $(install_includes_dependencies) $(cmt_local_tagfile_install_includes) build_library_links dirs install_includesdirs
#else
#$(cmt_local_install_includes_makefile) :: $(install_includes_dependencies) $(cmt_local_tagfile_install_includes) build_library_links dirs
#endif
#else
#$(cmt_local_install_includes_makefile) :: $(cmt_local_tagfile_install_includes)
#endif

makefiles : $(cmt_local_install_includes_makefile)

ifndef QUICK
$(cmt_local_install_includes_makefile) : $(install_includes_dependencies) build_library_links
#$(cmt_local_install_includes_makefile) : $(install_includes_dependencies) $(cmt_local_tagfile_install_includes) build_library_links
	$(echo) "(constituents.make) Building install_includes.make"; \
	  $(cmtexe) -tag=$(tags) $(install_includes_extratags) build constituent_config -out=$(cmt_local_install_includes_makefile) install_includes
else
$(cmt_local_install_includes_makefile) : $(install_includes_dependencies)
#$(cmt_local_install_includes_makefile) : $(cmt_local_tagfile_install_includes)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_install_includes) ] || \
	  [ ! -f $(cmt_final_setup_install_includes) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building install_includes.make"; \
	  $(cmtexe) -tag=$(tags) $(install_includes_extratags) build constituent_config -out=$(cmt_local_install_includes_makefile) install_includes; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(install_includes_extratags) build constituent_makefile -out=$(cmt_local_install_includes_makefile) install_includes

install_includes :: $(install_includes_dependencies) $(cmt_local_install_includes_makefile) dirs install_includesdirs
	$(echo) "(constituents.make) Starting install_includes"
	@$(MAKE) -f $(cmt_local_install_includes_makefile) install_includes
	$(echo) "(constituents.make) install_includes done"

clean :: install_includesclean

install_includesclean :: $(install_includesclean_dependencies) ##$(cmt_local_install_includes_makefile)
	$(echo) "(constituents.make) Starting install_includesclean"
	@-if test -f $(cmt_local_install_includes_makefile); then \
	  $(MAKE) -f $(cmt_local_install_includes_makefile) install_includesclean; \
	fi
	$(echo) "(constituents.make) install_includesclean done"
#	@-$(MAKE) -f $(cmt_local_install_includes_makefile) install_includesclean

##	  /bin/rm -f $(cmt_local_install_includes_makefile) $(bin)install_includes_dependencies.make

install :: install_includesinstall

install_includesinstall :: $(install_includes_dependencies) $(cmt_local_install_includes_makefile)
	$(echo) "(constituents.make) Starting install install_includes"
	@-$(MAKE) -f $(cmt_local_install_includes_makefile) install
	$(echo) "(constituents.make) install install_includes done"

uninstall :: install_includesuninstall

$(foreach d,$(install_includes_dependencies),$(eval $(d)uninstall_dependencies += install_includesuninstall))

install_includesuninstall :: $(install_includesuninstall_dependencies) $(cmt_local_install_includes_makefile)
	$(echo) "(constituents.make) Starting uninstall install_includes"
	@$(MAKE) -f $(cmt_local_install_includes_makefile) uninstall
	$(echo) "(constituents.make) uninstall install_includes done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ install_includes"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ install_includes done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_make_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_make_has_target_tag

#cmt_local_tagfile_make = $(MTree_tag)_make.make
cmt_local_tagfile_make = $(bin)$(MTree_tag)_make.make
cmt_local_setup_make = $(bin)setup_make$$$$.make
cmt_final_setup_make = $(bin)setup_make.make
#cmt_final_setup_make = $(bin)MTree_makesetup.make
cmt_local_make_makefile = $(bin)make.make

make_extratags = -tag_add=target_make

#$(cmt_local_tagfile_make) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_make) ::
else
$(cmt_local_tagfile_make) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_make); then /bin/rm -f $(cmt_local_tagfile_make); fi ; \
	  $(cmtexe) -tag=$(tags) $(make_extratags) build tag_makefile >>$(cmt_local_tagfile_make)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_make)"; \
	  test ! -f $(cmt_local_setup_make) || \rm -f $(cmt_local_setup_make); \
	  trap '\rm -f $(cmt_local_setup_make)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(make_extratags) show setup >$(cmt_local_setup_make) && \
	  if [ -f $(cmt_final_setup_make) ] && \
	    \cmp -s $(cmt_final_setup_make) $(cmt_local_setup_make); then \
	    \rm $(cmt_local_setup_make); else \
	    \mv -f $(cmt_local_setup_make) $(cmt_final_setup_make); fi

else

#cmt_local_tagfile_make = $(MTree_tag).make
cmt_local_tagfile_make = $(bin)$(MTree_tag).make
cmt_final_setup_make = $(bin)setup.make
#cmt_final_setup_make = $(bin)MTreesetup.make
cmt_local_make_makefile = $(bin)make.make

endif

ifdef STRUCTURED_OUTPUT
makedirs :
	@if test ! -d $(bin)make; then $(mkdir) -p $(bin)make; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)make
else
makedirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# makedirs ::
#	@if test ! -d $(bin)make; then $(mkdir) -p $(bin)make; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)make
#
#$(cmt_local_make_makefile) :: $(make_dependencies) $(cmt_local_tagfile_make) build_library_links dirs makedirs
#else
#$(cmt_local_make_makefile) :: $(make_dependencies) $(cmt_local_tagfile_make) build_library_links dirs
#endif
#else
#$(cmt_local_make_makefile) :: $(cmt_local_tagfile_make)
#endif

makefiles : $(cmt_local_make_makefile)

ifndef QUICK
$(cmt_local_make_makefile) : $(make_dependencies) build_library_links
#$(cmt_local_make_makefile) : $(make_dependencies) $(cmt_local_tagfile_make) build_library_links
	$(echo) "(constituents.make) Building make.make"; \
	  $(cmtexe) -tag=$(tags) $(make_extratags) build constituent_config -out=$(cmt_local_make_makefile) make
else
$(cmt_local_make_makefile) : $(make_dependencies)
#$(cmt_local_make_makefile) : $(cmt_local_tagfile_make)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_make) ] || \
	  [ ! -f $(cmt_final_setup_make) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building make.make"; \
	  $(cmtexe) -tag=$(tags) $(make_extratags) build constituent_config -out=$(cmt_local_make_makefile) make; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(make_extratags) build constituent_makefile -out=$(cmt_local_make_makefile) make

make :: $(make_dependencies) $(cmt_local_make_makefile) dirs makedirs
	$(echo) "(constituents.make) Starting make"
	@$(MAKE) -f $(cmt_local_make_makefile) make
	$(echo) "(constituents.make) make done"

clean :: makeclean

makeclean :: $(makeclean_dependencies) ##$(cmt_local_make_makefile)
	$(echo) "(constituents.make) Starting makeclean"
	@-if test -f $(cmt_local_make_makefile); then \
	  $(MAKE) -f $(cmt_local_make_makefile) makeclean; \
	fi
	$(echo) "(constituents.make) makeclean done"
#	@-$(MAKE) -f $(cmt_local_make_makefile) makeclean

##	  /bin/rm -f $(cmt_local_make_makefile) $(bin)make_dependencies.make

install :: makeinstall

makeinstall :: $(make_dependencies) $(cmt_local_make_makefile)
	$(echo) "(constituents.make) Starting install make"
	@-$(MAKE) -f $(cmt_local_make_makefile) install
	$(echo) "(constituents.make) install make done"

uninstall :: makeuninstall

$(foreach d,$(make_dependencies),$(eval $(d)uninstall_dependencies += makeuninstall))

makeuninstall :: $(makeuninstall_dependencies) $(cmt_local_make_makefile)
	$(echo) "(constituents.make) Starting uninstall make"
	@$(MAKE) -f $(cmt_local_make_makefile) uninstall
	$(echo) "(constituents.make) uninstall make done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ make"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ make done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_CompilePython_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_CompilePython_has_target_tag

#cmt_local_tagfile_CompilePython = $(MTree_tag)_CompilePython.make
cmt_local_tagfile_CompilePython = $(bin)$(MTree_tag)_CompilePython.make
cmt_local_setup_CompilePython = $(bin)setup_CompilePython$$$$.make
cmt_final_setup_CompilePython = $(bin)setup_CompilePython.make
#cmt_final_setup_CompilePython = $(bin)MTree_CompilePythonsetup.make
cmt_local_CompilePython_makefile = $(bin)CompilePython.make

CompilePython_extratags = -tag_add=target_CompilePython

#$(cmt_local_tagfile_CompilePython) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_CompilePython) ::
else
$(cmt_local_tagfile_CompilePython) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_CompilePython); then /bin/rm -f $(cmt_local_tagfile_CompilePython); fi ; \
	  $(cmtexe) -tag=$(tags) $(CompilePython_extratags) build tag_makefile >>$(cmt_local_tagfile_CompilePython)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_CompilePython)"; \
	  test ! -f $(cmt_local_setup_CompilePython) || \rm -f $(cmt_local_setup_CompilePython); \
	  trap '\rm -f $(cmt_local_setup_CompilePython)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(CompilePython_extratags) show setup >$(cmt_local_setup_CompilePython) && \
	  if [ -f $(cmt_final_setup_CompilePython) ] && \
	    \cmp -s $(cmt_final_setup_CompilePython) $(cmt_local_setup_CompilePython); then \
	    \rm $(cmt_local_setup_CompilePython); else \
	    \mv -f $(cmt_local_setup_CompilePython) $(cmt_final_setup_CompilePython); fi

else

#cmt_local_tagfile_CompilePython = $(MTree_tag).make
cmt_local_tagfile_CompilePython = $(bin)$(MTree_tag).make
cmt_final_setup_CompilePython = $(bin)setup.make
#cmt_final_setup_CompilePython = $(bin)MTreesetup.make
cmt_local_CompilePython_makefile = $(bin)CompilePython.make

endif

ifdef STRUCTURED_OUTPUT
CompilePythondirs :
	@if test ! -d $(bin)CompilePython; then $(mkdir) -p $(bin)CompilePython; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)CompilePython
else
CompilePythondirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# CompilePythondirs ::
#	@if test ! -d $(bin)CompilePython; then $(mkdir) -p $(bin)CompilePython; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)CompilePython
#
#$(cmt_local_CompilePython_makefile) :: $(CompilePython_dependencies) $(cmt_local_tagfile_CompilePython) build_library_links dirs CompilePythondirs
#else
#$(cmt_local_CompilePython_makefile) :: $(CompilePython_dependencies) $(cmt_local_tagfile_CompilePython) build_library_links dirs
#endif
#else
#$(cmt_local_CompilePython_makefile) :: $(cmt_local_tagfile_CompilePython)
#endif

makefiles : $(cmt_local_CompilePython_makefile)

ifndef QUICK
$(cmt_local_CompilePython_makefile) : $(CompilePython_dependencies) build_library_links
#$(cmt_local_CompilePython_makefile) : $(CompilePython_dependencies) $(cmt_local_tagfile_CompilePython) build_library_links
	$(echo) "(constituents.make) Building CompilePython.make"; \
	  $(cmtexe) -tag=$(tags) $(CompilePython_extratags) build constituent_config -out=$(cmt_local_CompilePython_makefile) CompilePython
else
$(cmt_local_CompilePython_makefile) : $(CompilePython_dependencies)
#$(cmt_local_CompilePython_makefile) : $(cmt_local_tagfile_CompilePython)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_CompilePython) ] || \
	  [ ! -f $(cmt_final_setup_CompilePython) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building CompilePython.make"; \
	  $(cmtexe) -tag=$(tags) $(CompilePython_extratags) build constituent_config -out=$(cmt_local_CompilePython_makefile) CompilePython; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(CompilePython_extratags) build constituent_makefile -out=$(cmt_local_CompilePython_makefile) CompilePython

CompilePython :: $(CompilePython_dependencies) $(cmt_local_CompilePython_makefile) dirs CompilePythondirs
	$(echo) "(constituents.make) Starting CompilePython"
	@$(MAKE) -f $(cmt_local_CompilePython_makefile) CompilePython
	$(echo) "(constituents.make) CompilePython done"

clean :: CompilePythonclean

CompilePythonclean :: $(CompilePythonclean_dependencies) ##$(cmt_local_CompilePython_makefile)
	$(echo) "(constituents.make) Starting CompilePythonclean"
	@-if test -f $(cmt_local_CompilePython_makefile); then \
	  $(MAKE) -f $(cmt_local_CompilePython_makefile) CompilePythonclean; \
	fi
	$(echo) "(constituents.make) CompilePythonclean done"
#	@-$(MAKE) -f $(cmt_local_CompilePython_makefile) CompilePythonclean

##	  /bin/rm -f $(cmt_local_CompilePython_makefile) $(bin)CompilePython_dependencies.make

install :: CompilePythoninstall

CompilePythoninstall :: $(CompilePython_dependencies) $(cmt_local_CompilePython_makefile)
	$(echo) "(constituents.make) Starting install CompilePython"
	@-$(MAKE) -f $(cmt_local_CompilePython_makefile) install
	$(echo) "(constituents.make) install CompilePython done"

uninstall :: CompilePythonuninstall

$(foreach d,$(CompilePython_dependencies),$(eval $(d)uninstall_dependencies += CompilePythonuninstall))

CompilePythonuninstall :: $(CompilePythonuninstall_dependencies) $(cmt_local_CompilePython_makefile)
	$(echo) "(constituents.make) Starting uninstall CompilePython"
	@$(MAKE) -f $(cmt_local_CompilePython_makefile) uninstall
	$(echo) "(constituents.make) uninstall CompilePython done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ CompilePython"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ CompilePython done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_qmtest_run_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_qmtest_run_has_target_tag

#cmt_local_tagfile_qmtest_run = $(MTree_tag)_qmtest_run.make
cmt_local_tagfile_qmtest_run = $(bin)$(MTree_tag)_qmtest_run.make
cmt_local_setup_qmtest_run = $(bin)setup_qmtest_run$$$$.make
cmt_final_setup_qmtest_run = $(bin)setup_qmtest_run.make
#cmt_final_setup_qmtest_run = $(bin)MTree_qmtest_runsetup.make
cmt_local_qmtest_run_makefile = $(bin)qmtest_run.make

qmtest_run_extratags = -tag_add=target_qmtest_run

#$(cmt_local_tagfile_qmtest_run) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_qmtest_run) ::
else
$(cmt_local_tagfile_qmtest_run) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_qmtest_run); then /bin/rm -f $(cmt_local_tagfile_qmtest_run); fi ; \
	  $(cmtexe) -tag=$(tags) $(qmtest_run_extratags) build tag_makefile >>$(cmt_local_tagfile_qmtest_run)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_qmtest_run)"; \
	  test ! -f $(cmt_local_setup_qmtest_run) || \rm -f $(cmt_local_setup_qmtest_run); \
	  trap '\rm -f $(cmt_local_setup_qmtest_run)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(qmtest_run_extratags) show setup >$(cmt_local_setup_qmtest_run) && \
	  if [ -f $(cmt_final_setup_qmtest_run) ] && \
	    \cmp -s $(cmt_final_setup_qmtest_run) $(cmt_local_setup_qmtest_run); then \
	    \rm $(cmt_local_setup_qmtest_run); else \
	    \mv -f $(cmt_local_setup_qmtest_run) $(cmt_final_setup_qmtest_run); fi

else

#cmt_local_tagfile_qmtest_run = $(MTree_tag).make
cmt_local_tagfile_qmtest_run = $(bin)$(MTree_tag).make
cmt_final_setup_qmtest_run = $(bin)setup.make
#cmt_final_setup_qmtest_run = $(bin)MTreesetup.make
cmt_local_qmtest_run_makefile = $(bin)qmtest_run.make

endif

ifdef STRUCTURED_OUTPUT
qmtest_rundirs :
	@if test ! -d $(bin)qmtest_run; then $(mkdir) -p $(bin)qmtest_run; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)qmtest_run
else
qmtest_rundirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# qmtest_rundirs ::
#	@if test ! -d $(bin)qmtest_run; then $(mkdir) -p $(bin)qmtest_run; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)qmtest_run
#
#$(cmt_local_qmtest_run_makefile) :: $(qmtest_run_dependencies) $(cmt_local_tagfile_qmtest_run) build_library_links dirs qmtest_rundirs
#else
#$(cmt_local_qmtest_run_makefile) :: $(qmtest_run_dependencies) $(cmt_local_tagfile_qmtest_run) build_library_links dirs
#endif
#else
#$(cmt_local_qmtest_run_makefile) :: $(cmt_local_tagfile_qmtest_run)
#endif

makefiles : $(cmt_local_qmtest_run_makefile)

ifndef QUICK
$(cmt_local_qmtest_run_makefile) : $(qmtest_run_dependencies) build_library_links
#$(cmt_local_qmtest_run_makefile) : $(qmtest_run_dependencies) $(cmt_local_tagfile_qmtest_run) build_library_links
	$(echo) "(constituents.make) Building qmtest_run.make"; \
	  $(cmtexe) -tag=$(tags) $(qmtest_run_extratags) build constituent_config -out=$(cmt_local_qmtest_run_makefile) qmtest_run
else
$(cmt_local_qmtest_run_makefile) : $(qmtest_run_dependencies)
#$(cmt_local_qmtest_run_makefile) : $(cmt_local_tagfile_qmtest_run)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_qmtest_run) ] || \
	  [ ! -f $(cmt_final_setup_qmtest_run) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building qmtest_run.make"; \
	  $(cmtexe) -tag=$(tags) $(qmtest_run_extratags) build constituent_config -out=$(cmt_local_qmtest_run_makefile) qmtest_run; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(qmtest_run_extratags) build constituent_makefile -out=$(cmt_local_qmtest_run_makefile) qmtest_run

qmtest_run :: $(qmtest_run_dependencies) $(cmt_local_qmtest_run_makefile) dirs qmtest_rundirs
	$(echo) "(constituents.make) Starting qmtest_run"
	@$(MAKE) -f $(cmt_local_qmtest_run_makefile) qmtest_run
	$(echo) "(constituents.make) qmtest_run done"

clean :: qmtest_runclean

qmtest_runclean :: $(qmtest_runclean_dependencies) ##$(cmt_local_qmtest_run_makefile)
	$(echo) "(constituents.make) Starting qmtest_runclean"
	@-if test -f $(cmt_local_qmtest_run_makefile); then \
	  $(MAKE) -f $(cmt_local_qmtest_run_makefile) qmtest_runclean; \
	fi
	$(echo) "(constituents.make) qmtest_runclean done"
#	@-$(MAKE) -f $(cmt_local_qmtest_run_makefile) qmtest_runclean

##	  /bin/rm -f $(cmt_local_qmtest_run_makefile) $(bin)qmtest_run_dependencies.make

install :: qmtest_runinstall

qmtest_runinstall :: $(qmtest_run_dependencies) $(cmt_local_qmtest_run_makefile)
	$(echo) "(constituents.make) Starting install qmtest_run"
	@-$(MAKE) -f $(cmt_local_qmtest_run_makefile) install
	$(echo) "(constituents.make) install qmtest_run done"

uninstall :: qmtest_rununinstall

$(foreach d,$(qmtest_run_dependencies),$(eval $(d)uninstall_dependencies += qmtest_rununinstall))

qmtest_rununinstall :: $(qmtest_rununinstall_dependencies) $(cmt_local_qmtest_run_makefile)
	$(echo) "(constituents.make) Starting uninstall qmtest_run"
	@$(MAKE) -f $(cmt_local_qmtest_run_makefile) uninstall
	$(echo) "(constituents.make) uninstall qmtest_run done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ qmtest_run"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ qmtest_run done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_qmtest_summarize_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_qmtest_summarize_has_target_tag

#cmt_local_tagfile_qmtest_summarize = $(MTree_tag)_qmtest_summarize.make
cmt_local_tagfile_qmtest_summarize = $(bin)$(MTree_tag)_qmtest_summarize.make
cmt_local_setup_qmtest_summarize = $(bin)setup_qmtest_summarize$$$$.make
cmt_final_setup_qmtest_summarize = $(bin)setup_qmtest_summarize.make
#cmt_final_setup_qmtest_summarize = $(bin)MTree_qmtest_summarizesetup.make
cmt_local_qmtest_summarize_makefile = $(bin)qmtest_summarize.make

qmtest_summarize_extratags = -tag_add=target_qmtest_summarize

#$(cmt_local_tagfile_qmtest_summarize) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_qmtest_summarize) ::
else
$(cmt_local_tagfile_qmtest_summarize) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_qmtest_summarize); then /bin/rm -f $(cmt_local_tagfile_qmtest_summarize); fi ; \
	  $(cmtexe) -tag=$(tags) $(qmtest_summarize_extratags) build tag_makefile >>$(cmt_local_tagfile_qmtest_summarize)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_qmtest_summarize)"; \
	  test ! -f $(cmt_local_setup_qmtest_summarize) || \rm -f $(cmt_local_setup_qmtest_summarize); \
	  trap '\rm -f $(cmt_local_setup_qmtest_summarize)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(qmtest_summarize_extratags) show setup >$(cmt_local_setup_qmtest_summarize) && \
	  if [ -f $(cmt_final_setup_qmtest_summarize) ] && \
	    \cmp -s $(cmt_final_setup_qmtest_summarize) $(cmt_local_setup_qmtest_summarize); then \
	    \rm $(cmt_local_setup_qmtest_summarize); else \
	    \mv -f $(cmt_local_setup_qmtest_summarize) $(cmt_final_setup_qmtest_summarize); fi

else

#cmt_local_tagfile_qmtest_summarize = $(MTree_tag).make
cmt_local_tagfile_qmtest_summarize = $(bin)$(MTree_tag).make
cmt_final_setup_qmtest_summarize = $(bin)setup.make
#cmt_final_setup_qmtest_summarize = $(bin)MTreesetup.make
cmt_local_qmtest_summarize_makefile = $(bin)qmtest_summarize.make

endif

ifdef STRUCTURED_OUTPUT
qmtest_summarizedirs :
	@if test ! -d $(bin)qmtest_summarize; then $(mkdir) -p $(bin)qmtest_summarize; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)qmtest_summarize
else
qmtest_summarizedirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# qmtest_summarizedirs ::
#	@if test ! -d $(bin)qmtest_summarize; then $(mkdir) -p $(bin)qmtest_summarize; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)qmtest_summarize
#
#$(cmt_local_qmtest_summarize_makefile) :: $(qmtest_summarize_dependencies) $(cmt_local_tagfile_qmtest_summarize) build_library_links dirs qmtest_summarizedirs
#else
#$(cmt_local_qmtest_summarize_makefile) :: $(qmtest_summarize_dependencies) $(cmt_local_tagfile_qmtest_summarize) build_library_links dirs
#endif
#else
#$(cmt_local_qmtest_summarize_makefile) :: $(cmt_local_tagfile_qmtest_summarize)
#endif

makefiles : $(cmt_local_qmtest_summarize_makefile)

ifndef QUICK
$(cmt_local_qmtest_summarize_makefile) : $(qmtest_summarize_dependencies) build_library_links
#$(cmt_local_qmtest_summarize_makefile) : $(qmtest_summarize_dependencies) $(cmt_local_tagfile_qmtest_summarize) build_library_links
	$(echo) "(constituents.make) Building qmtest_summarize.make"; \
	  $(cmtexe) -tag=$(tags) $(qmtest_summarize_extratags) build constituent_config -out=$(cmt_local_qmtest_summarize_makefile) qmtest_summarize
else
$(cmt_local_qmtest_summarize_makefile) : $(qmtest_summarize_dependencies)
#$(cmt_local_qmtest_summarize_makefile) : $(cmt_local_tagfile_qmtest_summarize)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_qmtest_summarize) ] || \
	  [ ! -f $(cmt_final_setup_qmtest_summarize) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building qmtest_summarize.make"; \
	  $(cmtexe) -tag=$(tags) $(qmtest_summarize_extratags) build constituent_config -out=$(cmt_local_qmtest_summarize_makefile) qmtest_summarize; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(qmtest_summarize_extratags) build constituent_makefile -out=$(cmt_local_qmtest_summarize_makefile) qmtest_summarize

qmtest_summarize :: $(qmtest_summarize_dependencies) $(cmt_local_qmtest_summarize_makefile) dirs qmtest_summarizedirs
	$(echo) "(constituents.make) Starting qmtest_summarize"
	@$(MAKE) -f $(cmt_local_qmtest_summarize_makefile) qmtest_summarize
	$(echo) "(constituents.make) qmtest_summarize done"

clean :: qmtest_summarizeclean

qmtest_summarizeclean :: $(qmtest_summarizeclean_dependencies) ##$(cmt_local_qmtest_summarize_makefile)
	$(echo) "(constituents.make) Starting qmtest_summarizeclean"
	@-if test -f $(cmt_local_qmtest_summarize_makefile); then \
	  $(MAKE) -f $(cmt_local_qmtest_summarize_makefile) qmtest_summarizeclean; \
	fi
	$(echo) "(constituents.make) qmtest_summarizeclean done"
#	@-$(MAKE) -f $(cmt_local_qmtest_summarize_makefile) qmtest_summarizeclean

##	  /bin/rm -f $(cmt_local_qmtest_summarize_makefile) $(bin)qmtest_summarize_dependencies.make

install :: qmtest_summarizeinstall

qmtest_summarizeinstall :: $(qmtest_summarize_dependencies) $(cmt_local_qmtest_summarize_makefile)
	$(echo) "(constituents.make) Starting install qmtest_summarize"
	@-$(MAKE) -f $(cmt_local_qmtest_summarize_makefile) install
	$(echo) "(constituents.make) install qmtest_summarize done"

uninstall :: qmtest_summarizeuninstall

$(foreach d,$(qmtest_summarize_dependencies),$(eval $(d)uninstall_dependencies += qmtest_summarizeuninstall))

qmtest_summarizeuninstall :: $(qmtest_summarizeuninstall_dependencies) $(cmt_local_qmtest_summarize_makefile)
	$(echo) "(constituents.make) Starting uninstall qmtest_summarize"
	@$(MAKE) -f $(cmt_local_qmtest_summarize_makefile) uninstall
	$(echo) "(constituents.make) uninstall qmtest_summarize done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ qmtest_summarize"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ qmtest_summarize done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_TestPackage_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_TestPackage_has_target_tag

#cmt_local_tagfile_TestPackage = $(MTree_tag)_TestPackage.make
cmt_local_tagfile_TestPackage = $(bin)$(MTree_tag)_TestPackage.make
cmt_local_setup_TestPackage = $(bin)setup_TestPackage$$$$.make
cmt_final_setup_TestPackage = $(bin)setup_TestPackage.make
#cmt_final_setup_TestPackage = $(bin)MTree_TestPackagesetup.make
cmt_local_TestPackage_makefile = $(bin)TestPackage.make

TestPackage_extratags = -tag_add=target_TestPackage

#$(cmt_local_tagfile_TestPackage) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_TestPackage) ::
else
$(cmt_local_tagfile_TestPackage) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_TestPackage); then /bin/rm -f $(cmt_local_tagfile_TestPackage); fi ; \
	  $(cmtexe) -tag=$(tags) $(TestPackage_extratags) build tag_makefile >>$(cmt_local_tagfile_TestPackage)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_TestPackage)"; \
	  test ! -f $(cmt_local_setup_TestPackage) || \rm -f $(cmt_local_setup_TestPackage); \
	  trap '\rm -f $(cmt_local_setup_TestPackage)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(TestPackage_extratags) show setup >$(cmt_local_setup_TestPackage) && \
	  if [ -f $(cmt_final_setup_TestPackage) ] && \
	    \cmp -s $(cmt_final_setup_TestPackage) $(cmt_local_setup_TestPackage); then \
	    \rm $(cmt_local_setup_TestPackage); else \
	    \mv -f $(cmt_local_setup_TestPackage) $(cmt_final_setup_TestPackage); fi

else

#cmt_local_tagfile_TestPackage = $(MTree_tag).make
cmt_local_tagfile_TestPackage = $(bin)$(MTree_tag).make
cmt_final_setup_TestPackage = $(bin)setup.make
#cmt_final_setup_TestPackage = $(bin)MTreesetup.make
cmt_local_TestPackage_makefile = $(bin)TestPackage.make

endif

ifdef STRUCTURED_OUTPUT
TestPackagedirs :
	@if test ! -d $(bin)TestPackage; then $(mkdir) -p $(bin)TestPackage; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)TestPackage
else
TestPackagedirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# TestPackagedirs ::
#	@if test ! -d $(bin)TestPackage; then $(mkdir) -p $(bin)TestPackage; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)TestPackage
#
#$(cmt_local_TestPackage_makefile) :: $(TestPackage_dependencies) $(cmt_local_tagfile_TestPackage) build_library_links dirs TestPackagedirs
#else
#$(cmt_local_TestPackage_makefile) :: $(TestPackage_dependencies) $(cmt_local_tagfile_TestPackage) build_library_links dirs
#endif
#else
#$(cmt_local_TestPackage_makefile) :: $(cmt_local_tagfile_TestPackage)
#endif

makefiles : $(cmt_local_TestPackage_makefile)

ifndef QUICK
$(cmt_local_TestPackage_makefile) : $(TestPackage_dependencies) build_library_links
#$(cmt_local_TestPackage_makefile) : $(TestPackage_dependencies) $(cmt_local_tagfile_TestPackage) build_library_links
	$(echo) "(constituents.make) Building TestPackage.make"; \
	  $(cmtexe) -tag=$(tags) $(TestPackage_extratags) build constituent_config -out=$(cmt_local_TestPackage_makefile) TestPackage
else
$(cmt_local_TestPackage_makefile) : $(TestPackage_dependencies)
#$(cmt_local_TestPackage_makefile) : $(cmt_local_tagfile_TestPackage)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_TestPackage) ] || \
	  [ ! -f $(cmt_final_setup_TestPackage) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building TestPackage.make"; \
	  $(cmtexe) -tag=$(tags) $(TestPackage_extratags) build constituent_config -out=$(cmt_local_TestPackage_makefile) TestPackage; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(TestPackage_extratags) build constituent_makefile -out=$(cmt_local_TestPackage_makefile) TestPackage

TestPackage :: $(TestPackage_dependencies) $(cmt_local_TestPackage_makefile) dirs TestPackagedirs
	$(echo) "(constituents.make) Starting TestPackage"
	@$(MAKE) -f $(cmt_local_TestPackage_makefile) TestPackage
	$(echo) "(constituents.make) TestPackage done"

clean :: TestPackageclean

TestPackageclean :: $(TestPackageclean_dependencies) ##$(cmt_local_TestPackage_makefile)
	$(echo) "(constituents.make) Starting TestPackageclean"
	@-if test -f $(cmt_local_TestPackage_makefile); then \
	  $(MAKE) -f $(cmt_local_TestPackage_makefile) TestPackageclean; \
	fi
	$(echo) "(constituents.make) TestPackageclean done"
#	@-$(MAKE) -f $(cmt_local_TestPackage_makefile) TestPackageclean

##	  /bin/rm -f $(cmt_local_TestPackage_makefile) $(bin)TestPackage_dependencies.make

install :: TestPackageinstall

TestPackageinstall :: $(TestPackage_dependencies) $(cmt_local_TestPackage_makefile)
	$(echo) "(constituents.make) Starting install TestPackage"
	@-$(MAKE) -f $(cmt_local_TestPackage_makefile) install
	$(echo) "(constituents.make) install TestPackage done"

uninstall :: TestPackageuninstall

$(foreach d,$(TestPackage_dependencies),$(eval $(d)uninstall_dependencies += TestPackageuninstall))

TestPackageuninstall :: $(TestPackageuninstall_dependencies) $(cmt_local_TestPackage_makefile)
	$(echo) "(constituents.make) Starting uninstall TestPackage"
	@$(MAKE) -f $(cmt_local_TestPackage_makefile) uninstall
	$(echo) "(constituents.make) uninstall TestPackage done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ TestPackage"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ TestPackage done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_TestProject_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_TestProject_has_target_tag

#cmt_local_tagfile_TestProject = $(MTree_tag)_TestProject.make
cmt_local_tagfile_TestProject = $(bin)$(MTree_tag)_TestProject.make
cmt_local_setup_TestProject = $(bin)setup_TestProject$$$$.make
cmt_final_setup_TestProject = $(bin)setup_TestProject.make
#cmt_final_setup_TestProject = $(bin)MTree_TestProjectsetup.make
cmt_local_TestProject_makefile = $(bin)TestProject.make

TestProject_extratags = -tag_add=target_TestProject

#$(cmt_local_tagfile_TestProject) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_TestProject) ::
else
$(cmt_local_tagfile_TestProject) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_TestProject); then /bin/rm -f $(cmt_local_tagfile_TestProject); fi ; \
	  $(cmtexe) -tag=$(tags) $(TestProject_extratags) build tag_makefile >>$(cmt_local_tagfile_TestProject)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_TestProject)"; \
	  test ! -f $(cmt_local_setup_TestProject) || \rm -f $(cmt_local_setup_TestProject); \
	  trap '\rm -f $(cmt_local_setup_TestProject)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(TestProject_extratags) show setup >$(cmt_local_setup_TestProject) && \
	  if [ -f $(cmt_final_setup_TestProject) ] && \
	    \cmp -s $(cmt_final_setup_TestProject) $(cmt_local_setup_TestProject); then \
	    \rm $(cmt_local_setup_TestProject); else \
	    \mv -f $(cmt_local_setup_TestProject) $(cmt_final_setup_TestProject); fi

else

#cmt_local_tagfile_TestProject = $(MTree_tag).make
cmt_local_tagfile_TestProject = $(bin)$(MTree_tag).make
cmt_final_setup_TestProject = $(bin)setup.make
#cmt_final_setup_TestProject = $(bin)MTreesetup.make
cmt_local_TestProject_makefile = $(bin)TestProject.make

endif

ifdef STRUCTURED_OUTPUT
TestProjectdirs :
	@if test ! -d $(bin)TestProject; then $(mkdir) -p $(bin)TestProject; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)TestProject
else
TestProjectdirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# TestProjectdirs ::
#	@if test ! -d $(bin)TestProject; then $(mkdir) -p $(bin)TestProject; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)TestProject
#
#$(cmt_local_TestProject_makefile) :: $(TestProject_dependencies) $(cmt_local_tagfile_TestProject) build_library_links dirs TestProjectdirs
#else
#$(cmt_local_TestProject_makefile) :: $(TestProject_dependencies) $(cmt_local_tagfile_TestProject) build_library_links dirs
#endif
#else
#$(cmt_local_TestProject_makefile) :: $(cmt_local_tagfile_TestProject)
#endif

makefiles : $(cmt_local_TestProject_makefile)

ifndef QUICK
$(cmt_local_TestProject_makefile) : $(TestProject_dependencies) build_library_links
#$(cmt_local_TestProject_makefile) : $(TestProject_dependencies) $(cmt_local_tagfile_TestProject) build_library_links
	$(echo) "(constituents.make) Building TestProject.make"; \
	  $(cmtexe) -tag=$(tags) $(TestProject_extratags) build constituent_config -out=$(cmt_local_TestProject_makefile) TestProject
else
$(cmt_local_TestProject_makefile) : $(TestProject_dependencies)
#$(cmt_local_TestProject_makefile) : $(cmt_local_tagfile_TestProject)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_TestProject) ] || \
	  [ ! -f $(cmt_final_setup_TestProject) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building TestProject.make"; \
	  $(cmtexe) -tag=$(tags) $(TestProject_extratags) build constituent_config -out=$(cmt_local_TestProject_makefile) TestProject; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(TestProject_extratags) build constituent_makefile -out=$(cmt_local_TestProject_makefile) TestProject

TestProject :: $(TestProject_dependencies) $(cmt_local_TestProject_makefile) dirs TestProjectdirs
	$(echo) "(constituents.make) Starting TestProject"
	@$(MAKE) -f $(cmt_local_TestProject_makefile) TestProject
	$(echo) "(constituents.make) TestProject done"

clean :: TestProjectclean

TestProjectclean :: $(TestProjectclean_dependencies) ##$(cmt_local_TestProject_makefile)
	$(echo) "(constituents.make) Starting TestProjectclean"
	@-if test -f $(cmt_local_TestProject_makefile); then \
	  $(MAKE) -f $(cmt_local_TestProject_makefile) TestProjectclean; \
	fi
	$(echo) "(constituents.make) TestProjectclean done"
#	@-$(MAKE) -f $(cmt_local_TestProject_makefile) TestProjectclean

##	  /bin/rm -f $(cmt_local_TestProject_makefile) $(bin)TestProject_dependencies.make

install :: TestProjectinstall

TestProjectinstall :: $(TestProject_dependencies) $(cmt_local_TestProject_makefile)
	$(echo) "(constituents.make) Starting install TestProject"
	@-$(MAKE) -f $(cmt_local_TestProject_makefile) install
	$(echo) "(constituents.make) install TestProject done"

uninstall :: TestProjectuninstall

$(foreach d,$(TestProject_dependencies),$(eval $(d)uninstall_dependencies += TestProjectuninstall))

TestProjectuninstall :: $(TestProjectuninstall_dependencies) $(cmt_local_TestProject_makefile)
	$(echo) "(constituents.make) Starting uninstall TestProject"
	@$(MAKE) -f $(cmt_local_TestProject_makefile) uninstall
	$(echo) "(constituents.make) uninstall TestProject done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ TestProject"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ TestProject done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_post_install_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_post_install_has_target_tag

#cmt_local_tagfile_post_install = $(MTree_tag)_post_install.make
cmt_local_tagfile_post_install = $(bin)$(MTree_tag)_post_install.make
cmt_local_setup_post_install = $(bin)setup_post_install$$$$.make
cmt_final_setup_post_install = $(bin)setup_post_install.make
#cmt_final_setup_post_install = $(bin)MTree_post_installsetup.make
cmt_local_post_install_makefile = $(bin)post_install.make

post_install_extratags = -tag_add=target_post_install

#$(cmt_local_tagfile_post_install) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_post_install) ::
else
$(cmt_local_tagfile_post_install) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_post_install); then /bin/rm -f $(cmt_local_tagfile_post_install); fi ; \
	  $(cmtexe) -tag=$(tags) $(post_install_extratags) build tag_makefile >>$(cmt_local_tagfile_post_install)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_post_install)"; \
	  test ! -f $(cmt_local_setup_post_install) || \rm -f $(cmt_local_setup_post_install); \
	  trap '\rm -f $(cmt_local_setup_post_install)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(post_install_extratags) show setup >$(cmt_local_setup_post_install) && \
	  if [ -f $(cmt_final_setup_post_install) ] && \
	    \cmp -s $(cmt_final_setup_post_install) $(cmt_local_setup_post_install); then \
	    \rm $(cmt_local_setup_post_install); else \
	    \mv -f $(cmt_local_setup_post_install) $(cmt_final_setup_post_install); fi

else

#cmt_local_tagfile_post_install = $(MTree_tag).make
cmt_local_tagfile_post_install = $(bin)$(MTree_tag).make
cmt_final_setup_post_install = $(bin)setup.make
#cmt_final_setup_post_install = $(bin)MTreesetup.make
cmt_local_post_install_makefile = $(bin)post_install.make

endif

ifdef STRUCTURED_OUTPUT
post_installdirs :
	@if test ! -d $(bin)post_install; then $(mkdir) -p $(bin)post_install; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)post_install
else
post_installdirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# post_installdirs ::
#	@if test ! -d $(bin)post_install; then $(mkdir) -p $(bin)post_install; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)post_install
#
#$(cmt_local_post_install_makefile) :: $(post_install_dependencies) $(cmt_local_tagfile_post_install) build_library_links dirs post_installdirs
#else
#$(cmt_local_post_install_makefile) :: $(post_install_dependencies) $(cmt_local_tagfile_post_install) build_library_links dirs
#endif
#else
#$(cmt_local_post_install_makefile) :: $(cmt_local_tagfile_post_install)
#endif

makefiles : $(cmt_local_post_install_makefile)

ifndef QUICK
$(cmt_local_post_install_makefile) : $(post_install_dependencies) build_library_links
#$(cmt_local_post_install_makefile) : $(post_install_dependencies) $(cmt_local_tagfile_post_install) build_library_links
	$(echo) "(constituents.make) Building post_install.make"; \
	  $(cmtexe) -tag=$(tags) $(post_install_extratags) build constituent_config -out=$(cmt_local_post_install_makefile) post_install
else
$(cmt_local_post_install_makefile) : $(post_install_dependencies)
#$(cmt_local_post_install_makefile) : $(cmt_local_tagfile_post_install)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_post_install) ] || \
	  [ ! -f $(cmt_final_setup_post_install) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building post_install.make"; \
	  $(cmtexe) -tag=$(tags) $(post_install_extratags) build constituent_config -out=$(cmt_local_post_install_makefile) post_install; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(post_install_extratags) build constituent_makefile -out=$(cmt_local_post_install_makefile) post_install

post_install :: $(post_install_dependencies) $(cmt_local_post_install_makefile) dirs post_installdirs
	$(echo) "(constituents.make) Starting post_install"
	@$(MAKE) -f $(cmt_local_post_install_makefile) post_install
	$(echo) "(constituents.make) post_install done"

clean :: post_installclean

post_installclean :: $(post_installclean_dependencies) ##$(cmt_local_post_install_makefile)
	$(echo) "(constituents.make) Starting post_installclean"
	@-if test -f $(cmt_local_post_install_makefile); then \
	  $(MAKE) -f $(cmt_local_post_install_makefile) post_installclean; \
	fi
	$(echo) "(constituents.make) post_installclean done"
#	@-$(MAKE) -f $(cmt_local_post_install_makefile) post_installclean

##	  /bin/rm -f $(cmt_local_post_install_makefile) $(bin)post_install_dependencies.make

install :: post_installinstall

post_installinstall :: $(post_install_dependencies) $(cmt_local_post_install_makefile)
	$(echo) "(constituents.make) Starting install post_install"
	@-$(MAKE) -f $(cmt_local_post_install_makefile) install
	$(echo) "(constituents.make) install post_install done"

uninstall :: post_installuninstall

$(foreach d,$(post_install_dependencies),$(eval $(d)uninstall_dependencies += post_installuninstall))

post_installuninstall :: $(post_installuninstall_dependencies) $(cmt_local_post_install_makefile)
	$(echo) "(constituents.make) Starting uninstall post_install"
	@$(MAKE) -f $(cmt_local_post_install_makefile) uninstall
	$(echo) "(constituents.make) uninstall post_install done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ post_install"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ post_install done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_post_merge_rootmap_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_post_merge_rootmap_has_target_tag

#cmt_local_tagfile_post_merge_rootmap = $(MTree_tag)_post_merge_rootmap.make
cmt_local_tagfile_post_merge_rootmap = $(bin)$(MTree_tag)_post_merge_rootmap.make
cmt_local_setup_post_merge_rootmap = $(bin)setup_post_merge_rootmap$$$$.make
cmt_final_setup_post_merge_rootmap = $(bin)setup_post_merge_rootmap.make
#cmt_final_setup_post_merge_rootmap = $(bin)MTree_post_merge_rootmapsetup.make
cmt_local_post_merge_rootmap_makefile = $(bin)post_merge_rootmap.make

post_merge_rootmap_extratags = -tag_add=target_post_merge_rootmap

#$(cmt_local_tagfile_post_merge_rootmap) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_post_merge_rootmap) ::
else
$(cmt_local_tagfile_post_merge_rootmap) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_post_merge_rootmap); then /bin/rm -f $(cmt_local_tagfile_post_merge_rootmap); fi ; \
	  $(cmtexe) -tag=$(tags) $(post_merge_rootmap_extratags) build tag_makefile >>$(cmt_local_tagfile_post_merge_rootmap)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_post_merge_rootmap)"; \
	  test ! -f $(cmt_local_setup_post_merge_rootmap) || \rm -f $(cmt_local_setup_post_merge_rootmap); \
	  trap '\rm -f $(cmt_local_setup_post_merge_rootmap)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(post_merge_rootmap_extratags) show setup >$(cmt_local_setup_post_merge_rootmap) && \
	  if [ -f $(cmt_final_setup_post_merge_rootmap) ] && \
	    \cmp -s $(cmt_final_setup_post_merge_rootmap) $(cmt_local_setup_post_merge_rootmap); then \
	    \rm $(cmt_local_setup_post_merge_rootmap); else \
	    \mv -f $(cmt_local_setup_post_merge_rootmap) $(cmt_final_setup_post_merge_rootmap); fi

else

#cmt_local_tagfile_post_merge_rootmap = $(MTree_tag).make
cmt_local_tagfile_post_merge_rootmap = $(bin)$(MTree_tag).make
cmt_final_setup_post_merge_rootmap = $(bin)setup.make
#cmt_final_setup_post_merge_rootmap = $(bin)MTreesetup.make
cmt_local_post_merge_rootmap_makefile = $(bin)post_merge_rootmap.make

endif

ifdef STRUCTURED_OUTPUT
post_merge_rootmapdirs :
	@if test ! -d $(bin)post_merge_rootmap; then $(mkdir) -p $(bin)post_merge_rootmap; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)post_merge_rootmap
else
post_merge_rootmapdirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# post_merge_rootmapdirs ::
#	@if test ! -d $(bin)post_merge_rootmap; then $(mkdir) -p $(bin)post_merge_rootmap; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)post_merge_rootmap
#
#$(cmt_local_post_merge_rootmap_makefile) :: $(post_merge_rootmap_dependencies) $(cmt_local_tagfile_post_merge_rootmap) build_library_links dirs post_merge_rootmapdirs
#else
#$(cmt_local_post_merge_rootmap_makefile) :: $(post_merge_rootmap_dependencies) $(cmt_local_tagfile_post_merge_rootmap) build_library_links dirs
#endif
#else
#$(cmt_local_post_merge_rootmap_makefile) :: $(cmt_local_tagfile_post_merge_rootmap)
#endif

makefiles : $(cmt_local_post_merge_rootmap_makefile)

ifndef QUICK
$(cmt_local_post_merge_rootmap_makefile) : $(post_merge_rootmap_dependencies) build_library_links
#$(cmt_local_post_merge_rootmap_makefile) : $(post_merge_rootmap_dependencies) $(cmt_local_tagfile_post_merge_rootmap) build_library_links
	$(echo) "(constituents.make) Building post_merge_rootmap.make"; \
	  $(cmtexe) -tag=$(tags) $(post_merge_rootmap_extratags) build constituent_config -out=$(cmt_local_post_merge_rootmap_makefile) post_merge_rootmap
else
$(cmt_local_post_merge_rootmap_makefile) : $(post_merge_rootmap_dependencies)
#$(cmt_local_post_merge_rootmap_makefile) : $(cmt_local_tagfile_post_merge_rootmap)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_post_merge_rootmap) ] || \
	  [ ! -f $(cmt_final_setup_post_merge_rootmap) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building post_merge_rootmap.make"; \
	  $(cmtexe) -tag=$(tags) $(post_merge_rootmap_extratags) build constituent_config -out=$(cmt_local_post_merge_rootmap_makefile) post_merge_rootmap; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(post_merge_rootmap_extratags) build constituent_makefile -out=$(cmt_local_post_merge_rootmap_makefile) post_merge_rootmap

post_merge_rootmap :: $(post_merge_rootmap_dependencies) $(cmt_local_post_merge_rootmap_makefile) dirs post_merge_rootmapdirs
	$(echo) "(constituents.make) Starting post_merge_rootmap"
	@$(MAKE) -f $(cmt_local_post_merge_rootmap_makefile) post_merge_rootmap
	$(echo) "(constituents.make) post_merge_rootmap done"

clean :: post_merge_rootmapclean

post_merge_rootmapclean :: $(post_merge_rootmapclean_dependencies) ##$(cmt_local_post_merge_rootmap_makefile)
	$(echo) "(constituents.make) Starting post_merge_rootmapclean"
	@-if test -f $(cmt_local_post_merge_rootmap_makefile); then \
	  $(MAKE) -f $(cmt_local_post_merge_rootmap_makefile) post_merge_rootmapclean; \
	fi
	$(echo) "(constituents.make) post_merge_rootmapclean done"
#	@-$(MAKE) -f $(cmt_local_post_merge_rootmap_makefile) post_merge_rootmapclean

##	  /bin/rm -f $(cmt_local_post_merge_rootmap_makefile) $(bin)post_merge_rootmap_dependencies.make

install :: post_merge_rootmapinstall

post_merge_rootmapinstall :: $(post_merge_rootmap_dependencies) $(cmt_local_post_merge_rootmap_makefile)
	$(echo) "(constituents.make) Starting install post_merge_rootmap"
	@-$(MAKE) -f $(cmt_local_post_merge_rootmap_makefile) install
	$(echo) "(constituents.make) install post_merge_rootmap done"

uninstall :: post_merge_rootmapuninstall

$(foreach d,$(post_merge_rootmap_dependencies),$(eval $(d)uninstall_dependencies += post_merge_rootmapuninstall))

post_merge_rootmapuninstall :: $(post_merge_rootmapuninstall_dependencies) $(cmt_local_post_merge_rootmap_makefile)
	$(echo) "(constituents.make) Starting uninstall post_merge_rootmap"
	@$(MAKE) -f $(cmt_local_post_merge_rootmap_makefile) uninstall
	$(echo) "(constituents.make) uninstall post_merge_rootmap done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ post_merge_rootmap"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ post_merge_rootmap done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_post_merge_genconfdb_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_post_merge_genconfdb_has_target_tag

#cmt_local_tagfile_post_merge_genconfdb = $(MTree_tag)_post_merge_genconfdb.make
cmt_local_tagfile_post_merge_genconfdb = $(bin)$(MTree_tag)_post_merge_genconfdb.make
cmt_local_setup_post_merge_genconfdb = $(bin)setup_post_merge_genconfdb$$$$.make
cmt_final_setup_post_merge_genconfdb = $(bin)setup_post_merge_genconfdb.make
#cmt_final_setup_post_merge_genconfdb = $(bin)MTree_post_merge_genconfdbsetup.make
cmt_local_post_merge_genconfdb_makefile = $(bin)post_merge_genconfdb.make

post_merge_genconfdb_extratags = -tag_add=target_post_merge_genconfdb

#$(cmt_local_tagfile_post_merge_genconfdb) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_post_merge_genconfdb) ::
else
$(cmt_local_tagfile_post_merge_genconfdb) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_post_merge_genconfdb); then /bin/rm -f $(cmt_local_tagfile_post_merge_genconfdb); fi ; \
	  $(cmtexe) -tag=$(tags) $(post_merge_genconfdb_extratags) build tag_makefile >>$(cmt_local_tagfile_post_merge_genconfdb)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_post_merge_genconfdb)"; \
	  test ! -f $(cmt_local_setup_post_merge_genconfdb) || \rm -f $(cmt_local_setup_post_merge_genconfdb); \
	  trap '\rm -f $(cmt_local_setup_post_merge_genconfdb)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(post_merge_genconfdb_extratags) show setup >$(cmt_local_setup_post_merge_genconfdb) && \
	  if [ -f $(cmt_final_setup_post_merge_genconfdb) ] && \
	    \cmp -s $(cmt_final_setup_post_merge_genconfdb) $(cmt_local_setup_post_merge_genconfdb); then \
	    \rm $(cmt_local_setup_post_merge_genconfdb); else \
	    \mv -f $(cmt_local_setup_post_merge_genconfdb) $(cmt_final_setup_post_merge_genconfdb); fi

else

#cmt_local_tagfile_post_merge_genconfdb = $(MTree_tag).make
cmt_local_tagfile_post_merge_genconfdb = $(bin)$(MTree_tag).make
cmt_final_setup_post_merge_genconfdb = $(bin)setup.make
#cmt_final_setup_post_merge_genconfdb = $(bin)MTreesetup.make
cmt_local_post_merge_genconfdb_makefile = $(bin)post_merge_genconfdb.make

endif

ifdef STRUCTURED_OUTPUT
post_merge_genconfdbdirs :
	@if test ! -d $(bin)post_merge_genconfdb; then $(mkdir) -p $(bin)post_merge_genconfdb; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)post_merge_genconfdb
else
post_merge_genconfdbdirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# post_merge_genconfdbdirs ::
#	@if test ! -d $(bin)post_merge_genconfdb; then $(mkdir) -p $(bin)post_merge_genconfdb; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)post_merge_genconfdb
#
#$(cmt_local_post_merge_genconfdb_makefile) :: $(post_merge_genconfdb_dependencies) $(cmt_local_tagfile_post_merge_genconfdb) build_library_links dirs post_merge_genconfdbdirs
#else
#$(cmt_local_post_merge_genconfdb_makefile) :: $(post_merge_genconfdb_dependencies) $(cmt_local_tagfile_post_merge_genconfdb) build_library_links dirs
#endif
#else
#$(cmt_local_post_merge_genconfdb_makefile) :: $(cmt_local_tagfile_post_merge_genconfdb)
#endif

makefiles : $(cmt_local_post_merge_genconfdb_makefile)

ifndef QUICK
$(cmt_local_post_merge_genconfdb_makefile) : $(post_merge_genconfdb_dependencies) build_library_links
#$(cmt_local_post_merge_genconfdb_makefile) : $(post_merge_genconfdb_dependencies) $(cmt_local_tagfile_post_merge_genconfdb) build_library_links
	$(echo) "(constituents.make) Building post_merge_genconfdb.make"; \
	  $(cmtexe) -tag=$(tags) $(post_merge_genconfdb_extratags) build constituent_config -out=$(cmt_local_post_merge_genconfdb_makefile) post_merge_genconfdb
else
$(cmt_local_post_merge_genconfdb_makefile) : $(post_merge_genconfdb_dependencies)
#$(cmt_local_post_merge_genconfdb_makefile) : $(cmt_local_tagfile_post_merge_genconfdb)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_post_merge_genconfdb) ] || \
	  [ ! -f $(cmt_final_setup_post_merge_genconfdb) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building post_merge_genconfdb.make"; \
	  $(cmtexe) -tag=$(tags) $(post_merge_genconfdb_extratags) build constituent_config -out=$(cmt_local_post_merge_genconfdb_makefile) post_merge_genconfdb; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(post_merge_genconfdb_extratags) build constituent_makefile -out=$(cmt_local_post_merge_genconfdb_makefile) post_merge_genconfdb

post_merge_genconfdb :: $(post_merge_genconfdb_dependencies) $(cmt_local_post_merge_genconfdb_makefile) dirs post_merge_genconfdbdirs
	$(echo) "(constituents.make) Starting post_merge_genconfdb"
	@$(MAKE) -f $(cmt_local_post_merge_genconfdb_makefile) post_merge_genconfdb
	$(echo) "(constituents.make) post_merge_genconfdb done"

clean :: post_merge_genconfdbclean

post_merge_genconfdbclean :: $(post_merge_genconfdbclean_dependencies) ##$(cmt_local_post_merge_genconfdb_makefile)
	$(echo) "(constituents.make) Starting post_merge_genconfdbclean"
	@-if test -f $(cmt_local_post_merge_genconfdb_makefile); then \
	  $(MAKE) -f $(cmt_local_post_merge_genconfdb_makefile) post_merge_genconfdbclean; \
	fi
	$(echo) "(constituents.make) post_merge_genconfdbclean done"
#	@-$(MAKE) -f $(cmt_local_post_merge_genconfdb_makefile) post_merge_genconfdbclean

##	  /bin/rm -f $(cmt_local_post_merge_genconfdb_makefile) $(bin)post_merge_genconfdb_dependencies.make

install :: post_merge_genconfdbinstall

post_merge_genconfdbinstall :: $(post_merge_genconfdb_dependencies) $(cmt_local_post_merge_genconfdb_makefile)
	$(echo) "(constituents.make) Starting install post_merge_genconfdb"
	@-$(MAKE) -f $(cmt_local_post_merge_genconfdb_makefile) install
	$(echo) "(constituents.make) install post_merge_genconfdb done"

uninstall :: post_merge_genconfdbuninstall

$(foreach d,$(post_merge_genconfdb_dependencies),$(eval $(d)uninstall_dependencies += post_merge_genconfdbuninstall))

post_merge_genconfdbuninstall :: $(post_merge_genconfdbuninstall_dependencies) $(cmt_local_post_merge_genconfdb_makefile)
	$(echo) "(constituents.make) Starting uninstall post_merge_genconfdb"
	@$(MAKE) -f $(cmt_local_post_merge_genconfdb_makefile) uninstall
	$(echo) "(constituents.make) uninstall post_merge_genconfdb done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ post_merge_genconfdb"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ post_merge_genconfdb done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_post_build_tpcnvdb_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_post_build_tpcnvdb_has_target_tag

#cmt_local_tagfile_post_build_tpcnvdb = $(MTree_tag)_post_build_tpcnvdb.make
cmt_local_tagfile_post_build_tpcnvdb = $(bin)$(MTree_tag)_post_build_tpcnvdb.make
cmt_local_setup_post_build_tpcnvdb = $(bin)setup_post_build_tpcnvdb$$$$.make
cmt_final_setup_post_build_tpcnvdb = $(bin)setup_post_build_tpcnvdb.make
#cmt_final_setup_post_build_tpcnvdb = $(bin)MTree_post_build_tpcnvdbsetup.make
cmt_local_post_build_tpcnvdb_makefile = $(bin)post_build_tpcnvdb.make

post_build_tpcnvdb_extratags = -tag_add=target_post_build_tpcnvdb

#$(cmt_local_tagfile_post_build_tpcnvdb) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_post_build_tpcnvdb) ::
else
$(cmt_local_tagfile_post_build_tpcnvdb) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_post_build_tpcnvdb); then /bin/rm -f $(cmt_local_tagfile_post_build_tpcnvdb); fi ; \
	  $(cmtexe) -tag=$(tags) $(post_build_tpcnvdb_extratags) build tag_makefile >>$(cmt_local_tagfile_post_build_tpcnvdb)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_post_build_tpcnvdb)"; \
	  test ! -f $(cmt_local_setup_post_build_tpcnvdb) || \rm -f $(cmt_local_setup_post_build_tpcnvdb); \
	  trap '\rm -f $(cmt_local_setup_post_build_tpcnvdb)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(post_build_tpcnvdb_extratags) show setup >$(cmt_local_setup_post_build_tpcnvdb) && \
	  if [ -f $(cmt_final_setup_post_build_tpcnvdb) ] && \
	    \cmp -s $(cmt_final_setup_post_build_tpcnvdb) $(cmt_local_setup_post_build_tpcnvdb); then \
	    \rm $(cmt_local_setup_post_build_tpcnvdb); else \
	    \mv -f $(cmt_local_setup_post_build_tpcnvdb) $(cmt_final_setup_post_build_tpcnvdb); fi

else

#cmt_local_tagfile_post_build_tpcnvdb = $(MTree_tag).make
cmt_local_tagfile_post_build_tpcnvdb = $(bin)$(MTree_tag).make
cmt_final_setup_post_build_tpcnvdb = $(bin)setup.make
#cmt_final_setup_post_build_tpcnvdb = $(bin)MTreesetup.make
cmt_local_post_build_tpcnvdb_makefile = $(bin)post_build_tpcnvdb.make

endif

ifdef STRUCTURED_OUTPUT
post_build_tpcnvdbdirs :
	@if test ! -d $(bin)post_build_tpcnvdb; then $(mkdir) -p $(bin)post_build_tpcnvdb; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)post_build_tpcnvdb
else
post_build_tpcnvdbdirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# post_build_tpcnvdbdirs ::
#	@if test ! -d $(bin)post_build_tpcnvdb; then $(mkdir) -p $(bin)post_build_tpcnvdb; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)post_build_tpcnvdb
#
#$(cmt_local_post_build_tpcnvdb_makefile) :: $(post_build_tpcnvdb_dependencies) $(cmt_local_tagfile_post_build_tpcnvdb) build_library_links dirs post_build_tpcnvdbdirs
#else
#$(cmt_local_post_build_tpcnvdb_makefile) :: $(post_build_tpcnvdb_dependencies) $(cmt_local_tagfile_post_build_tpcnvdb) build_library_links dirs
#endif
#else
#$(cmt_local_post_build_tpcnvdb_makefile) :: $(cmt_local_tagfile_post_build_tpcnvdb)
#endif

makefiles : $(cmt_local_post_build_tpcnvdb_makefile)

ifndef QUICK
$(cmt_local_post_build_tpcnvdb_makefile) : $(post_build_tpcnvdb_dependencies) build_library_links
#$(cmt_local_post_build_tpcnvdb_makefile) : $(post_build_tpcnvdb_dependencies) $(cmt_local_tagfile_post_build_tpcnvdb) build_library_links
	$(echo) "(constituents.make) Building post_build_tpcnvdb.make"; \
	  $(cmtexe) -tag=$(tags) $(post_build_tpcnvdb_extratags) build constituent_config -out=$(cmt_local_post_build_tpcnvdb_makefile) post_build_tpcnvdb
else
$(cmt_local_post_build_tpcnvdb_makefile) : $(post_build_tpcnvdb_dependencies)
#$(cmt_local_post_build_tpcnvdb_makefile) : $(cmt_local_tagfile_post_build_tpcnvdb)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_post_build_tpcnvdb) ] || \
	  [ ! -f $(cmt_final_setup_post_build_tpcnvdb) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building post_build_tpcnvdb.make"; \
	  $(cmtexe) -tag=$(tags) $(post_build_tpcnvdb_extratags) build constituent_config -out=$(cmt_local_post_build_tpcnvdb_makefile) post_build_tpcnvdb; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(post_build_tpcnvdb_extratags) build constituent_makefile -out=$(cmt_local_post_build_tpcnvdb_makefile) post_build_tpcnvdb

post_build_tpcnvdb :: $(post_build_tpcnvdb_dependencies) $(cmt_local_post_build_tpcnvdb_makefile) dirs post_build_tpcnvdbdirs
	$(echo) "(constituents.make) Starting post_build_tpcnvdb"
	@$(MAKE) -f $(cmt_local_post_build_tpcnvdb_makefile) post_build_tpcnvdb
	$(echo) "(constituents.make) post_build_tpcnvdb done"

clean :: post_build_tpcnvdbclean

post_build_tpcnvdbclean :: $(post_build_tpcnvdbclean_dependencies) ##$(cmt_local_post_build_tpcnvdb_makefile)
	$(echo) "(constituents.make) Starting post_build_tpcnvdbclean"
	@-if test -f $(cmt_local_post_build_tpcnvdb_makefile); then \
	  $(MAKE) -f $(cmt_local_post_build_tpcnvdb_makefile) post_build_tpcnvdbclean; \
	fi
	$(echo) "(constituents.make) post_build_tpcnvdbclean done"
#	@-$(MAKE) -f $(cmt_local_post_build_tpcnvdb_makefile) post_build_tpcnvdbclean

##	  /bin/rm -f $(cmt_local_post_build_tpcnvdb_makefile) $(bin)post_build_tpcnvdb_dependencies.make

install :: post_build_tpcnvdbinstall

post_build_tpcnvdbinstall :: $(post_build_tpcnvdb_dependencies) $(cmt_local_post_build_tpcnvdb_makefile)
	$(echo) "(constituents.make) Starting install post_build_tpcnvdb"
	@-$(MAKE) -f $(cmt_local_post_build_tpcnvdb_makefile) install
	$(echo) "(constituents.make) install post_build_tpcnvdb done"

uninstall :: post_build_tpcnvdbuninstall

$(foreach d,$(post_build_tpcnvdb_dependencies),$(eval $(d)uninstall_dependencies += post_build_tpcnvdbuninstall))

post_build_tpcnvdbuninstall :: $(post_build_tpcnvdbuninstall_dependencies) $(cmt_local_post_build_tpcnvdb_makefile)
	$(echo) "(constituents.make) Starting uninstall post_build_tpcnvdb"
	@$(MAKE) -f $(cmt_local_post_build_tpcnvdb_makefile) uninstall
	$(echo) "(constituents.make) uninstall post_build_tpcnvdb done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ post_build_tpcnvdb"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ post_build_tpcnvdb done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_SLC5_Compat_install_action_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_SLC5_Compat_install_action_has_target_tag

#cmt_local_tagfile_SLC5_Compat_install_action = $(MTree_tag)_SLC5_Compat_install_action.make
cmt_local_tagfile_SLC5_Compat_install_action = $(bin)$(MTree_tag)_SLC5_Compat_install_action.make
cmt_local_setup_SLC5_Compat_install_action = $(bin)setup_SLC5_Compat_install_action$$$$.make
cmt_final_setup_SLC5_Compat_install_action = $(bin)setup_SLC5_Compat_install_action.make
#cmt_final_setup_SLC5_Compat_install_action = $(bin)MTree_SLC5_Compat_install_actionsetup.make
cmt_local_SLC5_Compat_install_action_makefile = $(bin)SLC5_Compat_install_action.make

SLC5_Compat_install_action_extratags = -tag_add=target_SLC5_Compat_install_action

#$(cmt_local_tagfile_SLC5_Compat_install_action) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_SLC5_Compat_install_action) ::
else
$(cmt_local_tagfile_SLC5_Compat_install_action) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_SLC5_Compat_install_action); then /bin/rm -f $(cmt_local_tagfile_SLC5_Compat_install_action); fi ; \
	  $(cmtexe) -tag=$(tags) $(SLC5_Compat_install_action_extratags) build tag_makefile >>$(cmt_local_tagfile_SLC5_Compat_install_action)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_SLC5_Compat_install_action)"; \
	  test ! -f $(cmt_local_setup_SLC5_Compat_install_action) || \rm -f $(cmt_local_setup_SLC5_Compat_install_action); \
	  trap '\rm -f $(cmt_local_setup_SLC5_Compat_install_action)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(SLC5_Compat_install_action_extratags) show setup >$(cmt_local_setup_SLC5_Compat_install_action) && \
	  if [ -f $(cmt_final_setup_SLC5_Compat_install_action) ] && \
	    \cmp -s $(cmt_final_setup_SLC5_Compat_install_action) $(cmt_local_setup_SLC5_Compat_install_action); then \
	    \rm $(cmt_local_setup_SLC5_Compat_install_action); else \
	    \mv -f $(cmt_local_setup_SLC5_Compat_install_action) $(cmt_final_setup_SLC5_Compat_install_action); fi

else

#cmt_local_tagfile_SLC5_Compat_install_action = $(MTree_tag).make
cmt_local_tagfile_SLC5_Compat_install_action = $(bin)$(MTree_tag).make
cmt_final_setup_SLC5_Compat_install_action = $(bin)setup.make
#cmt_final_setup_SLC5_Compat_install_action = $(bin)MTreesetup.make
cmt_local_SLC5_Compat_install_action_makefile = $(bin)SLC5_Compat_install_action.make

endif

ifdef STRUCTURED_OUTPUT
SLC5_Compat_install_actiondirs :
	@if test ! -d $(bin)SLC5_Compat_install_action; then $(mkdir) -p $(bin)SLC5_Compat_install_action; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)SLC5_Compat_install_action
else
SLC5_Compat_install_actiondirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# SLC5_Compat_install_actiondirs ::
#	@if test ! -d $(bin)SLC5_Compat_install_action; then $(mkdir) -p $(bin)SLC5_Compat_install_action; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)SLC5_Compat_install_action
#
#$(cmt_local_SLC5_Compat_install_action_makefile) :: $(SLC5_Compat_install_action_dependencies) $(cmt_local_tagfile_SLC5_Compat_install_action) build_library_links dirs SLC5_Compat_install_actiondirs
#else
#$(cmt_local_SLC5_Compat_install_action_makefile) :: $(SLC5_Compat_install_action_dependencies) $(cmt_local_tagfile_SLC5_Compat_install_action) build_library_links dirs
#endif
#else
#$(cmt_local_SLC5_Compat_install_action_makefile) :: $(cmt_local_tagfile_SLC5_Compat_install_action)
#endif

makefiles : $(cmt_local_SLC5_Compat_install_action_makefile)

ifndef QUICK
$(cmt_local_SLC5_Compat_install_action_makefile) : $(SLC5_Compat_install_action_dependencies) build_library_links
#$(cmt_local_SLC5_Compat_install_action_makefile) : $(SLC5_Compat_install_action_dependencies) $(cmt_local_tagfile_SLC5_Compat_install_action) build_library_links
	$(echo) "(constituents.make) Building SLC5_Compat_install_action.make"; \
	  $(cmtexe) -tag=$(tags) $(SLC5_Compat_install_action_extratags) build constituent_config -out=$(cmt_local_SLC5_Compat_install_action_makefile) SLC5_Compat_install_action
else
$(cmt_local_SLC5_Compat_install_action_makefile) : $(SLC5_Compat_install_action_dependencies)
#$(cmt_local_SLC5_Compat_install_action_makefile) : $(cmt_local_tagfile_SLC5_Compat_install_action)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_SLC5_Compat_install_action) ] || \
	  [ ! -f $(cmt_final_setup_SLC5_Compat_install_action) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building SLC5_Compat_install_action.make"; \
	  $(cmtexe) -tag=$(tags) $(SLC5_Compat_install_action_extratags) build constituent_config -out=$(cmt_local_SLC5_Compat_install_action_makefile) SLC5_Compat_install_action; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(SLC5_Compat_install_action_extratags) build constituent_makefile -out=$(cmt_local_SLC5_Compat_install_action_makefile) SLC5_Compat_install_action

SLC5_Compat_install_action :: $(SLC5_Compat_install_action_dependencies) $(cmt_local_SLC5_Compat_install_action_makefile) dirs SLC5_Compat_install_actiondirs
	$(echo) "(constituents.make) Starting SLC5_Compat_install_action"
	@$(MAKE) -f $(cmt_local_SLC5_Compat_install_action_makefile) SLC5_Compat_install_action
	$(echo) "(constituents.make) SLC5_Compat_install_action done"

clean :: SLC5_Compat_install_actionclean

SLC5_Compat_install_actionclean :: $(SLC5_Compat_install_actionclean_dependencies) ##$(cmt_local_SLC5_Compat_install_action_makefile)
	$(echo) "(constituents.make) Starting SLC5_Compat_install_actionclean"
	@-if test -f $(cmt_local_SLC5_Compat_install_action_makefile); then \
	  $(MAKE) -f $(cmt_local_SLC5_Compat_install_action_makefile) SLC5_Compat_install_actionclean; \
	fi
	$(echo) "(constituents.make) SLC5_Compat_install_actionclean done"
#	@-$(MAKE) -f $(cmt_local_SLC5_Compat_install_action_makefile) SLC5_Compat_install_actionclean

##	  /bin/rm -f $(cmt_local_SLC5_Compat_install_action_makefile) $(bin)SLC5_Compat_install_action_dependencies.make

install :: SLC5_Compat_install_actioninstall

SLC5_Compat_install_actioninstall :: $(SLC5_Compat_install_action_dependencies) $(cmt_local_SLC5_Compat_install_action_makefile)
	$(echo) "(constituents.make) Starting install SLC5_Compat_install_action"
	@-$(MAKE) -f $(cmt_local_SLC5_Compat_install_action_makefile) install
	$(echo) "(constituents.make) install SLC5_Compat_install_action done"

uninstall :: SLC5_Compat_install_actionuninstall

$(foreach d,$(SLC5_Compat_install_action_dependencies),$(eval $(d)uninstall_dependencies += SLC5_Compat_install_actionuninstall))

SLC5_Compat_install_actionuninstall :: $(SLC5_Compat_install_actionuninstall_dependencies) $(cmt_local_SLC5_Compat_install_action_makefile)
	$(echo) "(constituents.make) Starting uninstall SLC5_Compat_install_action"
	@$(MAKE) -f $(cmt_local_SLC5_Compat_install_action_makefile) uninstall
	$(echo) "(constituents.make) uninstall SLC5_Compat_install_action done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ SLC5_Compat_install_action"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ SLC5_Compat_install_action done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_doxygen_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_doxygen_has_target_tag

#cmt_local_tagfile_doxygen = $(MTree_tag)_doxygen.make
cmt_local_tagfile_doxygen = $(bin)$(MTree_tag)_doxygen.make
cmt_local_setup_doxygen = $(bin)setup_doxygen$$$$.make
cmt_final_setup_doxygen = $(bin)setup_doxygen.make
#cmt_final_setup_doxygen = $(bin)MTree_doxygensetup.make
cmt_local_doxygen_makefile = $(bin)doxygen.make

doxygen_extratags = -tag_add=target_doxygen

#$(cmt_local_tagfile_doxygen) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_doxygen) ::
else
$(cmt_local_tagfile_doxygen) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_doxygen); then /bin/rm -f $(cmt_local_tagfile_doxygen); fi ; \
	  $(cmtexe) -tag=$(tags) $(doxygen_extratags) build tag_makefile >>$(cmt_local_tagfile_doxygen)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_doxygen)"; \
	  test ! -f $(cmt_local_setup_doxygen) || \rm -f $(cmt_local_setup_doxygen); \
	  trap '\rm -f $(cmt_local_setup_doxygen)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(doxygen_extratags) show setup >$(cmt_local_setup_doxygen) && \
	  if [ -f $(cmt_final_setup_doxygen) ] && \
	    \cmp -s $(cmt_final_setup_doxygen) $(cmt_local_setup_doxygen); then \
	    \rm $(cmt_local_setup_doxygen); else \
	    \mv -f $(cmt_local_setup_doxygen) $(cmt_final_setup_doxygen); fi

else

#cmt_local_tagfile_doxygen = $(MTree_tag).make
cmt_local_tagfile_doxygen = $(bin)$(MTree_tag).make
cmt_final_setup_doxygen = $(bin)setup.make
#cmt_final_setup_doxygen = $(bin)MTreesetup.make
cmt_local_doxygen_makefile = $(bin)doxygen.make

endif

ifdef STRUCTURED_OUTPUT
doxygendirs :
	@if test ! -d $(bin)doxygen; then $(mkdir) -p $(bin)doxygen; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)doxygen
else
doxygendirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# doxygendirs ::
#	@if test ! -d $(bin)doxygen; then $(mkdir) -p $(bin)doxygen; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)doxygen
#
#$(cmt_local_doxygen_makefile) :: $(doxygen_dependencies) $(cmt_local_tagfile_doxygen) build_library_links dirs doxygendirs
#else
#$(cmt_local_doxygen_makefile) :: $(doxygen_dependencies) $(cmt_local_tagfile_doxygen) build_library_links dirs
#endif
#else
#$(cmt_local_doxygen_makefile) :: $(cmt_local_tagfile_doxygen)
#endif

makefiles : $(cmt_local_doxygen_makefile)

ifndef QUICK
$(cmt_local_doxygen_makefile) : $(doxygen_dependencies) build_library_links
#$(cmt_local_doxygen_makefile) : $(doxygen_dependencies) $(cmt_local_tagfile_doxygen) build_library_links
	$(echo) "(constituents.make) Building doxygen.make"; \
	  $(cmtexe) -tag=$(tags) $(doxygen_extratags) build constituent_config -out=$(cmt_local_doxygen_makefile) doxygen
else
$(cmt_local_doxygen_makefile) : $(doxygen_dependencies)
#$(cmt_local_doxygen_makefile) : $(cmt_local_tagfile_doxygen)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_doxygen) ] || \
	  [ ! -f $(cmt_final_setup_doxygen) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building doxygen.make"; \
	  $(cmtexe) -tag=$(tags) $(doxygen_extratags) build constituent_config -out=$(cmt_local_doxygen_makefile) doxygen; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(doxygen_extratags) build constituent_makefile -out=$(cmt_local_doxygen_makefile) doxygen

doxygen :: $(doxygen_dependencies) $(cmt_local_doxygen_makefile) dirs doxygendirs
	$(echo) "(constituents.make) Starting doxygen"
	@$(MAKE) -f $(cmt_local_doxygen_makefile) doxygen
	$(echo) "(constituents.make) doxygen done"

clean :: doxygenclean

doxygenclean :: $(doxygenclean_dependencies) ##$(cmt_local_doxygen_makefile)
	$(echo) "(constituents.make) Starting doxygenclean"
	@-if test -f $(cmt_local_doxygen_makefile); then \
	  $(MAKE) -f $(cmt_local_doxygen_makefile) doxygenclean; \
	fi
	$(echo) "(constituents.make) doxygenclean done"
#	@-$(MAKE) -f $(cmt_local_doxygen_makefile) doxygenclean

##	  /bin/rm -f $(cmt_local_doxygen_makefile) $(bin)doxygen_dependencies.make

install :: doxygeninstall

doxygeninstall :: $(doxygen_dependencies) $(cmt_local_doxygen_makefile)
	$(echo) "(constituents.make) Starting install doxygen"
	@-$(MAKE) -f $(cmt_local_doxygen_makefile) install
	$(echo) "(constituents.make) install doxygen done"

uninstall :: doxygenuninstall

$(foreach d,$(doxygen_dependencies),$(eval $(d)uninstall_dependencies += doxygenuninstall))

doxygenuninstall :: $(doxygenuninstall_dependencies) $(cmt_local_doxygen_makefile)
	$(echo) "(constituents.make) Starting uninstall doxygen"
	@$(MAKE) -f $(cmt_local_doxygen_makefile) uninstall
	$(echo) "(constituents.make) uninstall doxygen done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ doxygen"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ doxygen done"
endif

#-- end of constituent ------
#-- start of constituent ------

cmt_new_rootsys_has_target_tag = 1

#--------------------------------------------------------

ifdef cmt_new_rootsys_has_target_tag

#cmt_local_tagfile_new_rootsys = $(MTree_tag)_new_rootsys.make
cmt_local_tagfile_new_rootsys = $(bin)$(MTree_tag)_new_rootsys.make
cmt_local_setup_new_rootsys = $(bin)setup_new_rootsys$$$$.make
cmt_final_setup_new_rootsys = $(bin)setup_new_rootsys.make
#cmt_final_setup_new_rootsys = $(bin)MTree_new_rootsyssetup.make
cmt_local_new_rootsys_makefile = $(bin)new_rootsys.make

new_rootsys_extratags = -tag_add=target_new_rootsys

#$(cmt_local_tagfile_new_rootsys) : $(cmt_lock_setup)
ifndef QUICK
$(cmt_local_tagfile_new_rootsys) ::
else
$(cmt_local_tagfile_new_rootsys) :
endif
	$(echo) "(constituents.make) Rebuilding $@"; \
	  if test -f $(cmt_local_tagfile_new_rootsys); then /bin/rm -f $(cmt_local_tagfile_new_rootsys); fi ; \
	  $(cmtexe) -tag=$(tags) $(new_rootsys_extratags) build tag_makefile >>$(cmt_local_tagfile_new_rootsys)
	$(echo) "(constituents.make) Rebuilding $(cmt_final_setup_new_rootsys)"; \
	  test ! -f $(cmt_local_setup_new_rootsys) || \rm -f $(cmt_local_setup_new_rootsys); \
	  trap '\rm -f $(cmt_local_setup_new_rootsys)' 0 1 2 15; \
	  $(cmtexe) -tag=$(tags) $(new_rootsys_extratags) show setup >$(cmt_local_setup_new_rootsys) && \
	  if [ -f $(cmt_final_setup_new_rootsys) ] && \
	    \cmp -s $(cmt_final_setup_new_rootsys) $(cmt_local_setup_new_rootsys); then \
	    \rm $(cmt_local_setup_new_rootsys); else \
	    \mv -f $(cmt_local_setup_new_rootsys) $(cmt_final_setup_new_rootsys); fi

else

#cmt_local_tagfile_new_rootsys = $(MTree_tag).make
cmt_local_tagfile_new_rootsys = $(bin)$(MTree_tag).make
cmt_final_setup_new_rootsys = $(bin)setup.make
#cmt_final_setup_new_rootsys = $(bin)MTreesetup.make
cmt_local_new_rootsys_makefile = $(bin)new_rootsys.make

endif

ifdef STRUCTURED_OUTPUT
new_rootsysdirs :
	@if test ! -d $(bin)new_rootsys; then $(mkdir) -p $(bin)new_rootsys; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)new_rootsys
else
new_rootsysdirs : ;
endif

#ifndef QUICK
#ifdef STRUCTURED_OUTPUT
# new_rootsysdirs ::
#	@if test ! -d $(bin)new_rootsys; then $(mkdir) -p $(bin)new_rootsys; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)new_rootsys
#
#$(cmt_local_new_rootsys_makefile) :: $(new_rootsys_dependencies) $(cmt_local_tagfile_new_rootsys) build_library_links dirs new_rootsysdirs
#else
#$(cmt_local_new_rootsys_makefile) :: $(new_rootsys_dependencies) $(cmt_local_tagfile_new_rootsys) build_library_links dirs
#endif
#else
#$(cmt_local_new_rootsys_makefile) :: $(cmt_local_tagfile_new_rootsys)
#endif

makefiles : $(cmt_local_new_rootsys_makefile)

ifndef QUICK
$(cmt_local_new_rootsys_makefile) : $(new_rootsys_dependencies) build_library_links
#$(cmt_local_new_rootsys_makefile) : $(new_rootsys_dependencies) $(cmt_local_tagfile_new_rootsys) build_library_links
	$(echo) "(constituents.make) Building new_rootsys.make"; \
	  $(cmtexe) -tag=$(tags) $(new_rootsys_extratags) build constituent_config -out=$(cmt_local_new_rootsys_makefile) new_rootsys
else
$(cmt_local_new_rootsys_makefile) : $(new_rootsys_dependencies)
#$(cmt_local_new_rootsys_makefile) : $(cmt_local_tagfile_new_rootsys)
	if [ ! -f $@ ] || [ ! -f $(cmt_local_tagfile_new_rootsys) ] || \
	  [ ! -f $(cmt_final_setup_new_rootsys) ]; then \
	  test -z "$(cmtmsg)" || \
	  echo "$(CMTMSGPREFIX)" "(constituents.make) Building new_rootsys.make"; \
	  $(cmtexe) -tag=$(tags) $(new_rootsys_extratags) build constituent_config -out=$(cmt_local_new_rootsys_makefile) new_rootsys; \
	  fi
endif

#	  $(cmtexe) -tag=$(tags) $(new_rootsys_extratags) build constituent_makefile -out=$(cmt_local_new_rootsys_makefile) new_rootsys

new_rootsys :: $(new_rootsys_dependencies) $(cmt_local_new_rootsys_makefile) dirs new_rootsysdirs
	$(echo) "(constituents.make) Starting new_rootsys"
	@$(MAKE) -f $(cmt_local_new_rootsys_makefile) new_rootsys
	$(echo) "(constituents.make) new_rootsys done"

clean :: new_rootsysclean

new_rootsysclean :: $(new_rootsysclean_dependencies) ##$(cmt_local_new_rootsys_makefile)
	$(echo) "(constituents.make) Starting new_rootsysclean"
	@-if test -f $(cmt_local_new_rootsys_makefile); then \
	  $(MAKE) -f $(cmt_local_new_rootsys_makefile) new_rootsysclean; \
	fi
	$(echo) "(constituents.make) new_rootsysclean done"
#	@-$(MAKE) -f $(cmt_local_new_rootsys_makefile) new_rootsysclean

##	  /bin/rm -f $(cmt_local_new_rootsys_makefile) $(bin)new_rootsys_dependencies.make

install :: new_rootsysinstall

new_rootsysinstall :: $(new_rootsys_dependencies) $(cmt_local_new_rootsys_makefile)
	$(echo) "(constituents.make) Starting install new_rootsys"
	@-$(MAKE) -f $(cmt_local_new_rootsys_makefile) install
	$(echo) "(constituents.make) install new_rootsys done"

uninstall :: new_rootsysuninstall

$(foreach d,$(new_rootsys_dependencies),$(eval $(d)uninstall_dependencies += new_rootsysuninstall))

new_rootsysuninstall :: $(new_rootsysuninstall_dependencies) $(cmt_local_new_rootsys_makefile)
	$(echo) "(constituents.make) Starting uninstall new_rootsys"
	@$(MAKE) -f $(cmt_local_new_rootsys_makefile) uninstall
	$(echo) "(constituents.make) uninstall new_rootsys done"

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(constituents.make) Starting $@ new_rootsys"
	$(echo) Using default action for $@
	$(echo) "(constituents.make) $@ new_rootsys done"
endif

#-- end of constituent ------
#-- start of constituents_trailer ------

clean :: remove_library_links

remove_library_links ::
	$(echo) "(constituents.make) Removing library links"; \
	  $(remove_library_links)

makefilesclean ::

###	@/bin/rm -f checkuses

###	/bin/rm -f *.make*

clean :: makefilesclean

binclean :: clean
	$(echo) "(constituents.make) Removing binary directory $(bin)"
	@if test ! "$(bin)" = "./"; then \
	  /bin/rm -rf $(bin); \
	fi

#-- end of constituents_trailer ------
