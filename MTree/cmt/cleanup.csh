if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/cern.ch/atlas/software/releases/16.6.5/CMT/v1r22
endif
source ${CMTROOT}/mgr/setup.csh
set cmtMTreetempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtMTreetempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=MTree -version=MTree-16-01-15 -path=/home/swalch/devArea/testarea/16.6.5 $* >${cmtMTreetempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt cleanup -csh -pack=MTree -version=MTree-16-01-15 -path=/home/swalch/devArea/testarea/16.6.5 $* >${cmtMTreetempfile}"
  set cmtcleanupstatus=2
  /bin/rm -f ${cmtMTreetempfile}
  unset cmtMTreetempfile
  exit $cmtcleanupstatus
endif
set cmtcleanupstatus=0
source ${cmtMTreetempfile}
if ( $status != 0 ) then
  set cmtcleanupstatus=2
endif
/bin/rm -f ${cmtMTreetempfile}
unset cmtMTreetempfile
exit $cmtcleanupstatus

