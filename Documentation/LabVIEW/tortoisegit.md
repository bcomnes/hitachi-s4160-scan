# Notes on using labview with git

These are preliminary notes on using tortoise-git with labview to perform Diffs and Merges.

For Diffs:
"C:\Program Files (x86)\National Instruments\Shared\LabVIEW Compare\LVCompare.exe" %mine %base -nobdcosm -nobdpos
for the file format .vi seems to do the trick

For merges
"C:\Program Files (x86)\National Instruments\Shared\LabVIEW Merge\LVMerge.exe" %base %theirs %mine %merged
for .vi seems to also do the trick

We will probrably need to add a few other filters for .ctl files as well as others possibly.