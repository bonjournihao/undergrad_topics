#! /bin/bash

OPTIONS=""
filename=""
compilePrefix="\tg++ -ansi -Wall -g -c$OPTIONS" #must use echo -e to recognize "\t"

if [[ $# -lt 1 ]]; then
	echo -ne "Executable name required.\nusage: makemake.sh executable_name"
	exit 1
fi #if no executable on the command, print usage

if [[ $# -gt 1 ]]; then
	for arg in ${*:2}; do
		OPTIONS="$OPTIONS $arg"
	done #copy all options into a string
fi #if command line length > 1, all options stored in string OPTIONS

#first line
echo -n "$1 : " > Makefile # output the .out filename to Makefile
numOfFiles=`find . -maxdepth 1 -name "*.cpp" | wc -l`
for i in $(seq 1 $numOfFiles); do
	filename=`find . -maxdepth 1 -name "*.cpp" | sort | head -$i | tail -1` #read filenames 1 at a time
	filename=$"${filename#\./}" #delete the "./" in the front of the filename
	filename=$"${filename/.cpp/.o}" #replace ".cpp" with ".o"
	if [[ $i -eq $numOfFiles ]]; then
		echo -e "$filename" >> Makefile # if is the last file
		break 
	else #append filenames in the form of "***.o" onto the first line in Makefile
		echo -n "$filename " >> Makefile
	fi #finish appending .o filenames
done

#second line
echo -ne "\tg++ -ansi -Wall -g -o $1$OPTIONS " >> Makefile #2nd line: link all .o files

for i in $(seq 1 $numOfFiles); do #repeat the same action
	filename=`find . -maxdepth 1 -name "*.cpp" | sort | head -$i | tail -1`
	filename=$"${filename#\./}" 
	filename=$"${filename/.cpp/.o}" 
	if [[ $i -eq $numOfFiles ]]; then
		echo -e "$filename\n" >> Makefile 
		break 
	else 
		echo -n "$filename " >> Makefile 
	fi #finish appending .o filenames
done

#the middle compilations
for i in $(seq 1 $numOfFiles); do
	filename=`find . -maxdepth 1 -name "*.cpp" | sort | head -$i | tail -1`
	filename=$"${filename#\./}"
	echo -n $"${filename/.cpp/.o} : " >> Makefile #append .cpp filename
	echo -n "$filename " >> Makefile
	echo `awk -F'"' '/include \"/ {print $2}' $filename` >> Makefile # append .h header filename
	echo -e "\tg++ -ansi -Wall -g -c$OPTIONS $filename\n" >> Makefile
done

#clean files
echo -ne "clean :\n\trm -f $1 " >> Makefile #add prefix for clean + .out file
for i in $(seq 1 $numOfFiles); do
	filename=`find . -maxdepth 1 -name "*.cpp" | sort | head -$i | tail -1`
	filename=$"${filename#\./}" 
	filename=$"${filename/.cpp/.o}" 
	if [[ $i -eq $numOfFiles ]]; then #if is the last one
		echo -n "$filename" >> Makefile
		break 
	else
		echo -n "$filename " >> Makefile
	fi 
done #finish appending .o filenames
