#!/usr/bin/env bash
# timeusage -- Measure timings of processes
# Usage: timusage COMMAND [ARGS]...
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
outfile="timeprofile.txt"                                                  #
############################################################################

set -um

# check input
[[ $# -gt 0 ]] || { sed -n '2,/^#$/ s/^# //p' <"$0"; exit 1; }

#Kill process on script exit
trap "kill $pgid 2> /dev/null" EXIT

#Measuring timings of program
/usr/bin/time -o $outfile -v "$@"
sed -i '1s/^/Timing Summary:\n/' $outfile
