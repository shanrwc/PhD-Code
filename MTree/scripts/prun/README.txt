######################################################################
#######
####### Instructions for running prun jobs for D3PD --> MTree conversion
#######
######################################################################

(0) General description
The submission of these jobs is done differently for data/MC input.  This is
related to the addition of truth object information, as well as the current
implementation of reading branches from the input D3PDs.  In the future, I
hope to consolidate.  But, for now, be aware of the following directories:

MTree/share/prun_data
MTree/share/prun_mc

If you wish to work with data or MC, make sure the respective directories
above are moved to the MTree main directory.  It is from this directory you
will call 'prun.'

THE EXAMPLE BELOW CORRESPONDS TO THE SUBMISSION OF A D3PD CONVERSION DONE ON A
DATA SAMPLE.  FOR MC, SIMPLY REPLACE THE STRING 'data' WITH 'mc' in what
follows.

(1) Configure your work area
If you haven't already, copy the prun_data directory to the main directory of
the MTree package:

$> cp -r $TestArea/16.0.2/MTree/share/prun_data $TestArea/16.0.2/MTree

Also, copy the prun_submit.py script from the MTree/scripts directory:

$> cp $TestArea/16.0.2/MTree/scripts/prun/prun_submit.py $TestArea/16.0.2/MTree

(2) Submit the job
At this point, assuming you have a working dataset as input, simply call the
following command:

$> ./prun_submit.py <dataType> <inDS> <suffix>

  - The first argument should be either 'data' or 'mc' ('data' for this
    example)
  - The second argument is the input dataset name, in its entirety
  - The third argument is an optional suffix (e.g., 'ver1.0')
