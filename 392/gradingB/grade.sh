#!/bin/bash

readonly ARCHIVE="gcd_assignments.zip"
readonly TEST_SCRIPT="test_gcd.sh"

process_file() {
	# Functions arguements are $1, $2, etc.
	echo "Processing $1."
	
	# Extract the name of the file without the extension.
	local base=${1%.*}
	
	# cut expects either file input or data on standard in
	# <<< places the values of the variable on standard in
	# and is called a 'here string'
	local first="$(cut -d '_' -f3 <<<$base)"
	local last="$(cut -d '_' -f2 <<<$base)"
	
	echo "Author: $first $last"
	local dirname= "$last"_"$first"
	mkdir -p "$dirname"
	mv "$1" "$dirname" 
	cp "$TEST_SCRIPT" "$dirname"
	cd "$dirname"
	unzip -o "$1"
	rm "$1"
	
	bash "$TEST_SCRIPT" | tee grade.txt
	
	cd - > /dev/null
}

unzip -o "$ARCHIVE"
for f in *.zip; do 
	if [ "$f" != "$ARCHIVE" ]; then
		process_file "$f"
	fi 
done
