if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/cern.ch/atlas/software/releases/16.6.5/CMT/v1r22; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtMTreetempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtMTreetempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=MTree -version=MTree-16-01-15 -path=/home/swalch/devArea/testarea/16.6.5 $* >${cmtMTreetempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt cleanup -sh -pack=MTree -version=MTree-16-01-15 -path=/home/swalch/devArea/testarea/16.6.5 $* >${cmtMTreetempfile}"
  cmtcleanupstatus=2
  /bin/rm -f ${cmtMTreetempfile}
  unset cmtMTreetempfile
  return $cmtcleanupstatus
fi
cmtcleanupstatus=0
. ${cmtMTreetempfile}
if test $? != 0 ; then
  cmtcleanupstatus=2
fi
/bin/rm -f ${cmtMTreetempfile}
unset cmtMTreetempfile
return $cmtcleanupstatus

