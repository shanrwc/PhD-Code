#-- start of make_header -----------------

#====================================
#  Library MTree
#
#   Generated Fri Oct  7 15:26:31 2011  by swalch
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_MTree_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_MTree_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_MTree

MTree_tag = $(tag)

#cmt_local_tagfile_MTree = $(MTree_tag)_MTree.make
cmt_local_tagfile_MTree = $(bin)$(MTree_tag)_MTree.make

else

tags      = $(tag),$(CMTEXTRATAGS)

MTree_tag = $(tag)

#cmt_local_tagfile_MTree = $(MTree_tag).make
cmt_local_tagfile_MTree = $(bin)$(MTree_tag).make

endif

include $(cmt_local_tagfile_MTree)
#-include $(cmt_local_tagfile_MTree)

ifdef cmt_MTree_has_target_tag

cmt_final_setup_MTree = $(bin)setup_MTree.make
#cmt_final_setup_MTree = $(bin)MTree_MTreesetup.make
cmt_local_MTree_makefile = $(bin)MTree.make

else

cmt_final_setup_MTree = $(bin)setup.make
#cmt_final_setup_MTree = $(bin)MTreesetup.make
cmt_local_MTree_makefile = $(bin)MTree.make

endif

cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)MTreesetup.make

#MTree :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'MTree'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = MTree/
#MTree::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

MTreelibname   = $(bin)$(library_prefix)MTree$(library_suffix)
MTreelib       = $(MTreelibname).a
MTreestamp     = $(bin)MTree.stamp
MTreeshstamp   = $(bin)MTree.shstamp

MTree :: dirs  MTreeLIB
	$(echo) "MTree ok"

#-- end of libary_header ----------------
#-- start of libary ----------------------

MTreeLIB :: $(MTreelib) $(MTreeshstamp)
	@/bin/echo "------> MTree : library ok"

$(MTreelib) :: $(bin)Muon.o $(bin)PrimaryVertex.o $(bin)HLTTriggerROI.o $(bin)TruthParticle.o $(bin)TriggerObject.o $(bin)SmearingClass.o $(bin)EventInfo.o $(bin)Lepton.o $(bin)MissingET.o $(bin)Trigger.o $(bin)Track.o $(bin)MTreeIO.o $(bin)Photon.o $(bin)L1TriggerROI.o $(bin)EnergyRescaler.o $(bin)Electron.o $(bin)Jet.o $(bin)ConvertAODToMTree.o $(bin)MTree_entries.o $(bin)MTree_load.o
	$(lib_echo) library
	$(lib_silent) cd $(bin); \
	  $(ar) $(MTreelib) $?
	$(lib_silent) $(ranlib) $(MTreelib)
	$(lib_silent) cat /dev/null >$(MTreestamp)

#------------------------------------------------------------------
#  Future improvement? to empty the object files after
#  storing in the library
#
##	  for f in $?; do \
##	    rm $${f}; touch $${f}; \
##	  done
#------------------------------------------------------------------

#
# We add one level of dependency upon the true shared library 
# (rather than simply upon the stamp file)
# this is for cases where the shared library has not been built
# while the stamp was created (error??) 
#

$(MTreelibname).$(shlibsuffix) :: $(MTreelib)
	$(lib_silent) cd $(bin); QUIET=$(QUIET); $(make_shlib) "$(tags)" MTree $(MTree_shlibflags)

$(MTreeshstamp) :: $(MTreelibname).$(shlibsuffix)
	@if test -f $(MTreelibname).$(shlibsuffix) ; then cat /dev/null >$(MTreeshstamp) ; fi

MTreeclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)Muon.o $(bin)PrimaryVertex.o $(bin)HLTTriggerROI.o $(bin)TruthParticle.o $(bin)TriggerObject.o $(bin)SmearingClass.o $(bin)EventInfo.o $(bin)Lepton.o $(bin)MissingET.o $(bin)Trigger.o $(bin)Track.o $(bin)MTreeIO.o $(bin)Photon.o $(bin)L1TriggerROI.o $(bin)EnergyRescaler.o $(bin)Electron.o $(bin)Jet.o $(bin)ConvertAODToMTree.o $(bin)MTree_entries.o $(bin)MTree_load.o

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/${libdirname}
MTreeinstallname = $(library_prefix)MTree$(library_suffix).$(shlibsuffix)

MTree :: MTreeinstall

install :: MTreeinstall

MTreeinstall :: $(install_dir)/$(MTreeinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then\
	  echo "installation done"; \
	fi

$(install_dir)/$(MTreeinstallname) :: $(bin)$(MTreeinstallname)
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  CMTINSTALLAREA=${CMTINSTALLAREA}; export CMTINSTALLAREA; \
	  $(cmt_install_action) "$(PACKAGE_ROOT)/$(tag)" "$(MTreeinstallname)" "$(install_dir)" "$(cmt_install_area_command)" ; \
	fi

MTreeclean :: MTreeuninstall

uninstall :: MTreeuninstall

MTreeuninstall ::
	@if test ! "${CMTINSTALLAREA}" = ""; then \
	  CMTINSTALLAREA=${CMTINSTALLAREA}; export CMTINSTALLAREA; \
	  $(cmt_uninstall_action) "$(PACKAGE_ROOT)/$(tag)" "$(MTreeinstallname)" "$(install_dir)" ; \
	fi


#-- end of libary -----------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Muon.d

$(bin)$(binobj)Muon.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)Muon.d : $(src)Muon.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/Muon.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(Muon_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(Muon_cppflags) $(Muon_cxx_cppflags)  $(src)Muon.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/Muon.o $(src)Muon.cxx $(@D)/Muon.dep
endif
endif

$(bin)$(binobj)Muon.o : $(src)Muon.cxx
else
$(bin)MTree_dependencies.make : $(Muon_cxx_dependencies)

$(bin)$(binobj)Muon.o : $(Muon_cxx_dependencies)
endif
	$(cpp_echo) $(src)Muon.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(Muon_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(Muon_cppflags) $(Muon_cxx_cppflags)  $(src)Muon.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)PrimaryVertex.d

$(bin)$(binobj)PrimaryVertex.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)PrimaryVertex.d : $(src)PrimaryVertex.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/PrimaryVertex.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(PrimaryVertex_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(PrimaryVertex_cppflags) $(PrimaryVertex_cxx_cppflags)  $(src)PrimaryVertex.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/PrimaryVertex.o $(src)PrimaryVertex.cxx $(@D)/PrimaryVertex.dep
endif
endif

$(bin)$(binobj)PrimaryVertex.o : $(src)PrimaryVertex.cxx
else
$(bin)MTree_dependencies.make : $(PrimaryVertex_cxx_dependencies)

$(bin)$(binobj)PrimaryVertex.o : $(PrimaryVertex_cxx_dependencies)
endif
	$(cpp_echo) $(src)PrimaryVertex.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(PrimaryVertex_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(PrimaryVertex_cppflags) $(PrimaryVertex_cxx_cppflags)  $(src)PrimaryVertex.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)HLTTriggerROI.d

$(bin)$(binobj)HLTTriggerROI.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)HLTTriggerROI.d : $(src)HLTTriggerROI.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/HLTTriggerROI.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(HLTTriggerROI_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(HLTTriggerROI_cppflags) $(HLTTriggerROI_cxx_cppflags)  $(src)HLTTriggerROI.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/HLTTriggerROI.o $(src)HLTTriggerROI.cxx $(@D)/HLTTriggerROI.dep
endif
endif

$(bin)$(binobj)HLTTriggerROI.o : $(src)HLTTriggerROI.cxx
else
$(bin)MTree_dependencies.make : $(HLTTriggerROI_cxx_dependencies)

$(bin)$(binobj)HLTTriggerROI.o : $(HLTTriggerROI_cxx_dependencies)
endif
	$(cpp_echo) $(src)HLTTriggerROI.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(HLTTriggerROI_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(HLTTriggerROI_cppflags) $(HLTTriggerROI_cxx_cppflags)  $(src)HLTTriggerROI.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)TruthParticle.d

$(bin)$(binobj)TruthParticle.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)TruthParticle.d : $(src)TruthParticle.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/TruthParticle.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(TruthParticle_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(TruthParticle_cppflags) $(TruthParticle_cxx_cppflags)  $(src)TruthParticle.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/TruthParticle.o $(src)TruthParticle.cxx $(@D)/TruthParticle.dep
endif
endif

$(bin)$(binobj)TruthParticle.o : $(src)TruthParticle.cxx
else
$(bin)MTree_dependencies.make : $(TruthParticle_cxx_dependencies)

$(bin)$(binobj)TruthParticle.o : $(TruthParticle_cxx_dependencies)
endif
	$(cpp_echo) $(src)TruthParticle.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(TruthParticle_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(TruthParticle_cppflags) $(TruthParticle_cxx_cppflags)  $(src)TruthParticle.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)TriggerObject.d

$(bin)$(binobj)TriggerObject.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)TriggerObject.d : $(src)TriggerObject.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/TriggerObject.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(TriggerObject_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(TriggerObject_cppflags) $(TriggerObject_cxx_cppflags)  $(src)TriggerObject.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/TriggerObject.o $(src)TriggerObject.cxx $(@D)/TriggerObject.dep
endif
endif

$(bin)$(binobj)TriggerObject.o : $(src)TriggerObject.cxx
else
$(bin)MTree_dependencies.make : $(TriggerObject_cxx_dependencies)

$(bin)$(binobj)TriggerObject.o : $(TriggerObject_cxx_dependencies)
endif
	$(cpp_echo) $(src)TriggerObject.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(TriggerObject_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(TriggerObject_cppflags) $(TriggerObject_cxx_cppflags)  $(src)TriggerObject.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)SmearingClass.d

$(bin)$(binobj)SmearingClass.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)SmearingClass.d : $(src)SmearingClass.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/SmearingClass.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(SmearingClass_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(SmearingClass_cppflags) $(SmearingClass_cxx_cppflags)  $(src)SmearingClass.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/SmearingClass.o $(src)SmearingClass.cxx $(@D)/SmearingClass.dep
endif
endif

$(bin)$(binobj)SmearingClass.o : $(src)SmearingClass.cxx
else
$(bin)MTree_dependencies.make : $(SmearingClass_cxx_dependencies)

$(bin)$(binobj)SmearingClass.o : $(SmearingClass_cxx_dependencies)
endif
	$(cpp_echo) $(src)SmearingClass.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(SmearingClass_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(SmearingClass_cppflags) $(SmearingClass_cxx_cppflags)  $(src)SmearingClass.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)EventInfo.d

$(bin)$(binobj)EventInfo.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)EventInfo.d : $(src)EventInfo.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/EventInfo.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(EventInfo_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(EventInfo_cppflags) $(EventInfo_cxx_cppflags)  $(src)EventInfo.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/EventInfo.o $(src)EventInfo.cxx $(@D)/EventInfo.dep
endif
endif

$(bin)$(binobj)EventInfo.o : $(src)EventInfo.cxx
else
$(bin)MTree_dependencies.make : $(EventInfo_cxx_dependencies)

$(bin)$(binobj)EventInfo.o : $(EventInfo_cxx_dependencies)
endif
	$(cpp_echo) $(src)EventInfo.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(EventInfo_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(EventInfo_cppflags) $(EventInfo_cxx_cppflags)  $(src)EventInfo.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Lepton.d

$(bin)$(binobj)Lepton.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)Lepton.d : $(src)Lepton.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/Lepton.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(Lepton_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(Lepton_cppflags) $(Lepton_cxx_cppflags)  $(src)Lepton.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/Lepton.o $(src)Lepton.cxx $(@D)/Lepton.dep
endif
endif

$(bin)$(binobj)Lepton.o : $(src)Lepton.cxx
else
$(bin)MTree_dependencies.make : $(Lepton_cxx_dependencies)

$(bin)$(binobj)Lepton.o : $(Lepton_cxx_dependencies)
endif
	$(cpp_echo) $(src)Lepton.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(Lepton_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(Lepton_cppflags) $(Lepton_cxx_cppflags)  $(src)Lepton.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)MissingET.d

$(bin)$(binobj)MissingET.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)MissingET.d : $(src)MissingET.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/MissingET.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(MissingET_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(MissingET_cppflags) $(MissingET_cxx_cppflags)  $(src)MissingET.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/MissingET.o $(src)MissingET.cxx $(@D)/MissingET.dep
endif
endif

$(bin)$(binobj)MissingET.o : $(src)MissingET.cxx
else
$(bin)MTree_dependencies.make : $(MissingET_cxx_dependencies)

$(bin)$(binobj)MissingET.o : $(MissingET_cxx_dependencies)
endif
	$(cpp_echo) $(src)MissingET.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(MissingET_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(MissingET_cppflags) $(MissingET_cxx_cppflags)  $(src)MissingET.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Trigger.d

$(bin)$(binobj)Trigger.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)Trigger.d : $(src)Trigger.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/Trigger.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(Trigger_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(Trigger_cppflags) $(Trigger_cxx_cppflags)  $(src)Trigger.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/Trigger.o $(src)Trigger.cxx $(@D)/Trigger.dep
endif
endif

$(bin)$(binobj)Trigger.o : $(src)Trigger.cxx
else
$(bin)MTree_dependencies.make : $(Trigger_cxx_dependencies)

$(bin)$(binobj)Trigger.o : $(Trigger_cxx_dependencies)
endif
	$(cpp_echo) $(src)Trigger.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(Trigger_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(Trigger_cppflags) $(Trigger_cxx_cppflags)  $(src)Trigger.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Track.d

$(bin)$(binobj)Track.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)Track.d : $(src)Track.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/Track.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(Track_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(Track_cppflags) $(Track_cxx_cppflags)  $(src)Track.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/Track.o $(src)Track.cxx $(@D)/Track.dep
endif
endif

$(bin)$(binobj)Track.o : $(src)Track.cxx
else
$(bin)MTree_dependencies.make : $(Track_cxx_dependencies)

$(bin)$(binobj)Track.o : $(Track_cxx_dependencies)
endif
	$(cpp_echo) $(src)Track.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(Track_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(Track_cppflags) $(Track_cxx_cppflags)  $(src)Track.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)MTreeIO.d

$(bin)$(binobj)MTreeIO.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)MTreeIO.d : $(src)MTreeIO.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/MTreeIO.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(MTreeIO_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(MTreeIO_cppflags) $(MTreeIO_cxx_cppflags)  $(src)MTreeIO.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/MTreeIO.o $(src)MTreeIO.cxx $(@D)/MTreeIO.dep
endif
endif

$(bin)$(binobj)MTreeIO.o : $(src)MTreeIO.cxx
else
$(bin)MTree_dependencies.make : $(MTreeIO_cxx_dependencies)

$(bin)$(binobj)MTreeIO.o : $(MTreeIO_cxx_dependencies)
endif
	$(cpp_echo) $(src)MTreeIO.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(MTreeIO_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(MTreeIO_cppflags) $(MTreeIO_cxx_cppflags)  $(src)MTreeIO.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Photon.d

$(bin)$(binobj)Photon.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)Photon.d : $(src)Photon.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/Photon.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(Photon_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(Photon_cppflags) $(Photon_cxx_cppflags)  $(src)Photon.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/Photon.o $(src)Photon.cxx $(@D)/Photon.dep
endif
endif

$(bin)$(binobj)Photon.o : $(src)Photon.cxx
else
$(bin)MTree_dependencies.make : $(Photon_cxx_dependencies)

$(bin)$(binobj)Photon.o : $(Photon_cxx_dependencies)
endif
	$(cpp_echo) $(src)Photon.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(Photon_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(Photon_cppflags) $(Photon_cxx_cppflags)  $(src)Photon.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)L1TriggerROI.d

$(bin)$(binobj)L1TriggerROI.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)L1TriggerROI.d : $(src)L1TriggerROI.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/L1TriggerROI.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(L1TriggerROI_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(L1TriggerROI_cppflags) $(L1TriggerROI_cxx_cppflags)  $(src)L1TriggerROI.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/L1TriggerROI.o $(src)L1TriggerROI.cxx $(@D)/L1TriggerROI.dep
endif
endif

$(bin)$(binobj)L1TriggerROI.o : $(src)L1TriggerROI.cxx
else
$(bin)MTree_dependencies.make : $(L1TriggerROI_cxx_dependencies)

$(bin)$(binobj)L1TriggerROI.o : $(L1TriggerROI_cxx_dependencies)
endif
	$(cpp_echo) $(src)L1TriggerROI.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(L1TriggerROI_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(L1TriggerROI_cppflags) $(L1TriggerROI_cxx_cppflags)  $(src)L1TriggerROI.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)EnergyRescaler.d

$(bin)$(binobj)EnergyRescaler.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)EnergyRescaler.d : $(src)EnergyRescaler.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/EnergyRescaler.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(EnergyRescaler_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(EnergyRescaler_cppflags) $(EnergyRescaler_cxx_cppflags)  $(src)EnergyRescaler.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/EnergyRescaler.o $(src)EnergyRescaler.cxx $(@D)/EnergyRescaler.dep
endif
endif

$(bin)$(binobj)EnergyRescaler.o : $(src)EnergyRescaler.cxx
else
$(bin)MTree_dependencies.make : $(EnergyRescaler_cxx_dependencies)

$(bin)$(binobj)EnergyRescaler.o : $(EnergyRescaler_cxx_dependencies)
endif
	$(cpp_echo) $(src)EnergyRescaler.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(EnergyRescaler_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(EnergyRescaler_cppflags) $(EnergyRescaler_cxx_cppflags)  $(src)EnergyRescaler.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Electron.d

$(bin)$(binobj)Electron.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)Electron.d : $(src)Electron.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/Electron.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(Electron_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(Electron_cppflags) $(Electron_cxx_cppflags)  $(src)Electron.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/Electron.o $(src)Electron.cxx $(@D)/Electron.dep
endif
endif

$(bin)$(binobj)Electron.o : $(src)Electron.cxx
else
$(bin)MTree_dependencies.make : $(Electron_cxx_dependencies)

$(bin)$(binobj)Electron.o : $(Electron_cxx_dependencies)
endif
	$(cpp_echo) $(src)Electron.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(Electron_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(Electron_cppflags) $(Electron_cxx_cppflags)  $(src)Electron.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Jet.d

$(bin)$(binobj)Jet.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)Jet.d : $(src)Jet.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/Jet.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(Jet_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(Jet_cppflags) $(Jet_cxx_cppflags)  $(src)Jet.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/Jet.o $(src)Jet.cxx $(@D)/Jet.dep
endif
endif

$(bin)$(binobj)Jet.o : $(src)Jet.cxx
else
$(bin)MTree_dependencies.make : $(Jet_cxx_dependencies)

$(bin)$(binobj)Jet.o : $(Jet_cxx_dependencies)
endif
	$(cpp_echo) $(src)Jet.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(Jet_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(Jet_cppflags) $(Jet_cxx_cppflags)  $(src)Jet.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)ConvertAODToMTree.d

$(bin)$(binobj)ConvertAODToMTree.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)ConvertAODToMTree.d : $(src)ConvertAODToMTree.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/ConvertAODToMTree.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(ConvertAODToMTree_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(ConvertAODToMTree_cppflags) $(ConvertAODToMTree_cxx_cppflags)  $(src)ConvertAODToMTree.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/ConvertAODToMTree.o $(src)ConvertAODToMTree.cxx $(@D)/ConvertAODToMTree.dep
endif
endif

$(bin)$(binobj)ConvertAODToMTree.o : $(src)ConvertAODToMTree.cxx
else
$(bin)MTree_dependencies.make : $(ConvertAODToMTree_cxx_dependencies)

$(bin)$(binobj)ConvertAODToMTree.o : $(ConvertAODToMTree_cxx_dependencies)
endif
	$(cpp_echo) $(src)ConvertAODToMTree.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(ConvertAODToMTree_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(ConvertAODToMTree_cppflags) $(ConvertAODToMTree_cxx_cppflags)  $(src)ConvertAODToMTree.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)MTree_entries.d

$(bin)$(binobj)MTree_entries.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)MTree_entries.d : $(src)components/MTree_entries.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/MTree_entries.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(MTree_entries_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(MTree_entries_cppflags) $(MTree_entries_cxx_cppflags) -I../src/components $(src)components/MTree_entries.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/MTree_entries.o $(src)components/MTree_entries.cxx $(@D)/MTree_entries.dep
endif
endif

$(bin)$(binobj)MTree_entries.o : $(src)components/MTree_entries.cxx
else
$(bin)MTree_dependencies.make : $(MTree_entries_cxx_dependencies)

$(bin)$(binobj)MTree_entries.o : $(MTree_entries_cxx_dependencies)
endif
	$(cpp_echo) $(src)components/MTree_entries.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(MTree_entries_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(MTree_entries_cppflags) $(MTree_entries_cxx_cppflags) -I../src/components $(src)components/MTree_entries.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq ($(cppdepflags),)

ifneq ($(MAKECMDGOALS),MTreeclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)MTree_load.d

$(bin)$(binobj)MTree_load.d : $(use_requirements) $(cmt_final_setup_MTree)

$(bin)$(binobj)MTree_load.d : $(src)components/MTree_load.cxx
	$(dep_echo) $@
	$(cpp_silent) $(cppcomp) $(cppdepflags) -o $(@D)/MTree_load.dep $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(MTree_load_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(MTree_load_cppflags) $(MTree_load_cxx_cppflags) -I../src/components $(src)components/MTree_load.cxx
	$(cpp_silent) $(format_dependencies) $@ $(@D)/MTree_load.o $(src)components/MTree_load.cxx $(@D)/MTree_load.dep
endif
endif

$(bin)$(binobj)MTree_load.o : $(src)components/MTree_load.cxx
else
$(bin)MTree_dependencies.make : $(MTree_load_cxx_dependencies)

$(bin)$(binobj)MTree_load.o : $(MTree_load_cxx_dependencies)
endif
	$(cpp_echo) $(src)components/MTree_load.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(MTree_pp_cppflags) $(lib_MTree_pp_cppflags) $(MTree_load_pp_cppflags) $(use_cppflags) $(MTree_cppflags) $(lib_MTree_cppflags) $(MTree_load_cppflags) $(MTree_load_cxx_cppflags) -I../src/components $(src)components/MTree_load.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: MTreeclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(MTree.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
	if echo $@ | grep '$(package)setup\.make$$' >/dev/null; then\
	 echo "$(CMTMSGPREFIX)" "(MTree.make): $@: File no longer generated" >&2; exit 0; fi
else
.DEFAULT::
	$(echo) "(MTree.make) PEDANTIC: $@: No rule for such target" >&2
	if echo $@ | grep '$(package)setup\.make$$' >/dev/null; then\
	 echo "$(CMTMSGPREFIX)" "(MTree.make): $@: File no longer generated" >&2; exit 0;\
	 elif test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_MTree)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MTree.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr $@ : '.*/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MTree.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(MTree.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

MTreeclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library MTree
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(library_prefix)MTree$(library_suffix).a $(library_prefix)MTree$(library_suffix).s? MTree.stamp MTree.shstamp
#-- end of cleanup_library ---------------
