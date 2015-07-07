# echo "Setting MTree MTree-16-01-15 in /home/swalch/devArea/testarea/16.6.5"

if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/cern.ch/atlas/software/releases/16.6.5/CMT/v1r22; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtMTreetempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtMTreetempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=MTree -version=MTree-16-01-15 -path=/home/swalch/devArea/testarea/16.6.5  -no_cleanup $* >${cmtMTreetempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt setup -sh -pack=MTree -version=MTree-16-01-15 -path=/home/swalch/devArea/testarea/16.6.5  -no_cleanup $* >${cmtMTreetempfile}"
  cmtsetupstatus=2
  /bin/rm -f ${cmtMTreetempfile}
  unset cmtMTreetempfile
  return $cmtsetupstatus
fi
cmtsetupstatus=0
. ${cmtMTreetempfile}
if test $? != 0 ; then
  cmtsetupstatus=2
fi
/bin/rm -f ${cmtMTreetempfile}
unset cmtMTreetempfile
return $cmtsetupstatus

