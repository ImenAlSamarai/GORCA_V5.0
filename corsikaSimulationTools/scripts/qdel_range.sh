#! /bin/bash
#
# delete a range of jobs
#
# Gernot Maier (gm@ast.leeds.ac.uk) 17/01/2006
#

if [ ${#1} -eq 0 ]
then
   echo usage: qdel_range.sh startID stopID
   exit
fi
if [ ${#2} -eq 0 ]
then
   echo usage: qdel_range.sh startID stopID
   exit
fi

JTART=$1
JTOPP=$2

for (( i = JTART; i <= JTOPP; i++))    # <= to get last input file with ANEV showers
do
   qdel $i
done

exit
