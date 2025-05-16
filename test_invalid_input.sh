#!/usr/bin/env bash
set -e

EXEC=./find_sig
SIG=test_files/crypty.sig
EMPTY_SIG=empty.sig
EMPTY_DIR=test_empty_dir
NONEXIST=nonexist_dir

echo "1) Nonexistent root_dir"
output=$($EXEC $NONEXIST $SIG 2>&1 || echo "EXIT\$?")
[[ "$output" == *"Provided path does not exist."* ]] && echo "Test 1 passed" || echo "Test 1 failed: $output"

echo "2) sig_path not a file"
output=$($EXEC test_files $NONEXIST 2>&1 || echo "EXIT\$?")
[[ "$output" == *"Provided signature file path is not a file."* ]] && echo "Test 2 passed" || echo "Test 2 failed: $output"

echo "3) Empty signature file"
# create empty sig
rm -f $EMPTY_SIG
touch $EMPTY_SIG
output=$($EXEC test_files $EMPTY_SIG 2>&1 || echo "EXIT\$?")
[[ "$output" == *"Provided signature file is empty."* ]] && echo "Test 3 passed" || echo "Test 3 failed: $output"

echo "4) Empty directory"
rm -rf $EMPTY_DIR
mkdir $EMPTY_DIR
output=$($EXEC $EMPTY_DIR $SIG 2>&1 || echo "EXIT\$?")
[[ "$output" == *"Provided directory is empty."* ]] && echo "Test 4 passed" || echo "Test 4 failed: $output"

echo "5) root_dir is not directory"
output=$($EXEC $SIG $SIG 2>&1 || echo "EXIT\$?")
[[ "$output" == *"Provided path is not a directory."* ]] && echo "Test 5 passed" || echo "Test 5 failed: $output"

# Clean up
rm -f $EMPTY_SIG
rmdir $EMPTY_DIR
