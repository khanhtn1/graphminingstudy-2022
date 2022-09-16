#!/usr/bin/env bash
# memusage -- Measure memory usage of processes
# Usage: memusage COMMAND [ARGS]...
#
# Author: Wyatt McGinnis
# Created: 2022-06-23
# Based on: memusage.sh by Jaeho Shin <netj@sparcs.org>
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
outfile="memprofile.txt"                                                   #
runs=2                                                                     #
############################################################################

set -um

#Check input
[[ $# -gt 0 ]] || { sed -n '2,/^#$/ s/^# //p' <"$0"; exit 1; }

#Set file
echo "Time________________|__RSS__|_Dirty_|_Total_|_Run_" > $outfile

#Global variables
glbpeakrss=0; glbpeakdirty=0; glbpeaktotal=0;
glbsumrss=0; glbsumdirty=0; glbsumtotal=0;

for i in $(seq 1 1 $runs); do
    $@ &
    pgid=$!

    #Kill process on script exit
    trap "kill $pgid 2> /dev/null" EXIT

    #Monitoring memory usage
    peakrss=0; peakdirty=0; peaktotal=0;
    sumrss=0; sumdirty=0; sumtotal=0; cnt=0;
    while kill -0 $pgid 2> /dev/null; do
        temp=`pmap -x $pgid | grep total | grep -oP "[0-9]+.*"| sed "s/\s\+/ /g"`
        total=`echo $temp | cut -d ' ' -f1`
        rss=`echo $temp | cut -d ' ' -f2`
        dirty=`echo $temp | cut -d ' ' -f3`
        let peaktotal="total > peaktotal ? total : peaktotal"
        let peakdirty="dirty > peakdirty ? dirty : peakdirty"
        let peakrss="rss > peakrss ? rss : peakrss"
        let sumtotal="sumtotal + total"
        let sumdirty="sumdirty + dirty"
        let sumrss="sumrss + rss"
        let cnt="cnt + 1"
        date=`date +20%y-%m-%d-%H-%M-%S`
        if [ ! -z $total ]; then 
            printf "%-20s|%+7s|%+7s|%+7s|%+5s\n" $date $rss $dirty $total $i >> $outfile
        fi
        sleep 1
    done
    printf "%-20s|%+7s|%+7s|%+7s|%+5s\n" Maximum: $peakrss $peakdirty $peaktotal $i >> $outfile
    printf "%-20s|%+7s|%+7s|%+7s|%+5s\n" >> $outfile
    let glbpeaktotal="peaktotal > glbpeaktotal ? peaktotal : glbpeaktotal"
    let glbpeakdirty="peakdirty > glbpeakdirty ? peakdirty : glbpeakdirty"
    let glbpeakrss="peakrss > glbpeakrss ? peakrss : glbpeakrss"
    let glbsumrss="glbsumrss + sumrss / cnt"
    let glbsumdirty="glbsumdirty + sumdirty / cnt"
    let glbsumtotal="glbsumtotal + sumtotal / cnt"
done

    let glbsumrss="glbsumrss / runs"
    let glbsumdirty="glbsumdirty / runs"
    let glbsumtotal="glbsumtotal / runs"

printf "%-20s|%+7s|%+7s|%+7s|%+5s\n" Maximum: $glbpeakrss $glbpeakdirty $glbpeaktotal glb >> $outfile
printf "%-20s|%+7s|%+7s|%+7s|%+5s\n" Average: $glbsumrss $glbsumdirty $glbsumtotal glb >> $outfile
