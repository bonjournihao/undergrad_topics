#! /bin/bash

ERROR="usage: cpdirs.sh source_directory1 source_directory2 dest_directory"
if [ ! -d $1 ] || [ ! -d $2 ] || [ ! -d $3 ]; then
        echo $ERROR
elif [[ ! $# -eq 3 ]]; then
        echo $ERROR
else
        echo -n `find $2 -maxdepth 1 -type f -exec cp {} $3 \;`
        for file1 in $(find $1 -maxdepth 1 -type f -printf "%f\n"); do
                if [ -f $2/$file1 ]; then
                        if [[ $(stat -c %Y $1/$file1) -gt $(stat -c %Y $2/$file1) ]]; then
                                echo -n `cp $1/$file1 $3`
                        fi
                else
                        echo -n `cp $1/$file1 $3`
                fi
        done
fi
