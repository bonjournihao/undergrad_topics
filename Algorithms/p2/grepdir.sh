#! /bin/bash

ERROR="usage: grepdir.sh directory pattern [-grep option]*"
OPTIONS=""

if [[ $# -lt 2 ]]; then #if command length shorter than 2
	echo $ERROR
elif [ ! -d $1 ]; then #if invalid directory
	echo $ERROR
elif [[ $# -gt 2 ]]; then #if length greater than 2, append options to a string
	for argNum in ${*:3}; do
			OPTIONS="$OPTIONS $argNum"
	done #make option string for appending in grep command
	find $1 -type f -exec grep $2 $OPTIONS {} \; 2> error #make an error file
	if [[ `ls -l error | awk '{print $5}'`  -ne 0 ]]; then #if size of error file is not 0
		echo errorrrrr
		echo $ERROR #error message
	fi # check all options are legal
else
	if [[ $# -eq 2 ]]; then #if $# eq 2
		find $1 -type f -exec grep $2 {} \; 
	else # if $# gt 2
		find $1 -type f -exec grep $OPTIONS $2 {} \;
	fi # if $# ge 2
fi #end

