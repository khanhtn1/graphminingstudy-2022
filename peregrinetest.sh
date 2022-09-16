#!/usr/bin/env bash
# testsuite -- Run a testsuite on a set of patterns locaated in the directory
# Usage: testsuite PATTERN_DIR COMMAND [ARGS]...
#
# Author: Wyatt McGinnis and Anuj Ketkar
# Created: 2022-07-16
############################################################################
# Copyright 2022 Wyatt McGinnis.                                           #
#                                                                          #
# Licensed under the Apache License, Version 2.0 (the "License");          #
# you may not use this file except in compliance with the License.         #
# You may obtain a copy of the License at                                  #
#                                                                          #
#     http://www.apache.org/licenses/LICENSE-2.0                           #
#                                                                          #
# Unless required by applicable law or agreed to in writing, software      #
# distributed under the License is distributed on an "AS IS" BASIS,        #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. #
# See the License for the specific language governing permissions and      #
# limitations under the License.                                           #
############################################################################
#Config                                                                    #
#                                                                          #
#Filename                                                                  #
outfile="peregrine_results.txt"                                            # 
memfile="memprofile.txt"                                                   #
numthreads=24                                                              #
############################################################################

set -um

#Check input
[[ $# -gt 0 ]] || { sed -n '2,/^#$/ s/^# //p' <"$0"; exit 1; }

echo "Pattern_____________|__RSS__|_Dirty_|_Time_" > $outfile

run=0
time_avg=0
for entry in `ls $1`; do
    run="memusage.sh ${@: 2} $1/$entry $numthreads"
    time=$(./$run | grep -ioP 'all patterns finished after\D*\K\d[\d.]*')
    time1=`echo $time | cut -d ' ' -f1`
    time2=`echo $time | cut -d ' ' -f2`
    time3=`echo $time | cut -d ' ' -f3`
    time_avg=`echo "scale=5; ($time1 + $time2 + $time3) / 3" | bc`
    mem=$(cat memprofile.txt | grep 'Average' | grep -oP "[0-9]+.*"| tr '|' ' ' | sed "s/\s\+/ /g") 
    rss=`echo $mem | cut -d ' ' -f1`
    dirty=`echo $mem | cut -d ' ' -f2`
    printf "%-20s|%+7s|%+7s|%+6s\n" $entry $rss $dirty $time_avg >> $outfile
    echo $entry done...
done