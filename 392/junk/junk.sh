###############################################################################
# Author: Yakov Kazinets
# Date: Feb 7,2020
# Pledge: I pledge my honor that I have abided by the Stevens Honor System.
# Description: Write the junk function to recycle files. wooooooooo! its late and i need to drive home, sorry if it doesn't work -l -p stuff is annoying
###############################################################################
#!/bin/bash

size_flag=$#
readonly JUNK="home/shared/392/.junk"
readonly file="home/shared/392/junk/junk.sh"
dhelper(){
	cat<<HELP
    Usage: $(basename $file) [-hlp] [list of files]
        -h:Display help.
        -l:List junked files.
        -p:Purge all files.
        [list of files] with no other arguments to junk those files.

HELP 
}

if [ "$size_flag" -eq 0 ]; then
	dhelper
	exit 1
fi

if [ ! -d "$JUNK"]; then
	mkdir /home/shared/392/.junk
fi


while getopts ":hlp" option; do  #getopts: get the options from the command line
	 case "$option" in
		h)  if ["$size_flag" -gt 1]; then
				printf "Error: Too many options enabled.\n" $OPTARG >&2
				dhelper
				exit 1
			else
			    dhelper
				exit 0
			fi;;
		l)  if ["$size_flag" -gt 1]; then
				printf "Error: Too many options enabled.\n" $OPTARG >&2
				dhelper
				exit 1
			else
				ls "$JUNK" -lAF
				exit 0
			fi;;
		p)  if ["$size_flag" -gt 1]; then
				printf "Error: Too many options enabled.\n" $OPTARG >&2
				dhelper
				exit 1
			else
				rm -rf  $JUNK/.* 2>/dev/null
                rm -rf  $JUNK/* 2>/dev/null
				exit 0
			fi;;
		?) printf "Error: Unknown option '-%s'.\n" $OPTARG >&2
			dhelper
			exit 1
			;;
	 esac
done


processer(){
	if [! -a "$1"]&&[! -d "$1"]&&[! -f "$1"]; then
		printf "Warning: '"$1"' not found.%s\n" $OPTARG >&2
	else  mv "$1" "$JUNK"
	fi
}

for f in $@; do
	processer "$f"
done

declare -a farr
i=0
shift "$((OPTIND-1))"

for f in $@; do
	farr[$i]=$f
	((i++))
done

exit 0 