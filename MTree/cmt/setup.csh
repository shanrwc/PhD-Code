# echo "Setting MTree MTree-16-01-15 in /home/swalch/devArea/testarea/16.6.5"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/cern.ch/atlas/software/releases/16.6.5/CMT/v1r22
endif
source ${CMTROOT}/mgr/setup.csh
set cmtMTreetempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtMTreetempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=MTree -version=MTree-16-01-15 -path=/home/swalch/devArea/testarea/16.6.5  -no_cleanup $* >${cmtMTreetempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt setup -csh -pack=MTree -version=MTree-16-01-15 -path=/home/swalch/devArea/testarea/16.6.5  -no_cleanup $* >${cmtMTreetempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtMTreetempfile}
  unset cmtMTreetempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtMTreetempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtMTreetempfile}
unset cmtMTreetempfile
exit $cmtsetupstatus

