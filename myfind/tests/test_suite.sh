#!/bin/bash

# Define a function to test myfind
test_myfind() {
    local input=$1
    local verbose=$2 # Set to 1 to print the output of myfind
    local passed=0 # Set to 1 if the test passed

    # Run the function with the input
    local output=$(./myfind $input)
    local expected_output=$(find $input)

    echo -e -n ">> [\e[0;34m$input\e[0m] >> "

    # Check if the output matches the expected output
    if [ "$output" == "$expected_output" ]; then
        echo -e "[\e[1;32mOK\e[0m]"
    else
        echo -e "[\e[1;31mKO\e[0m]"
        verbose=1
    fi

    if [ "$verbose" == "1" ]; then
        echo "|"
        echo "|--> Expected output:"
        echo -e "\e[0;34m$expected_output\e[0m"
        echo "|--> Actual output:"
        echo -e "\e[0;36m$output\e[0m"
        echo "---------"
    else
        echo ""
    fi
}

# Run tests

# Add more tests as needed

echo "-------------------- Basic --------------------"

test_myfind "" 0
test_myfind "." 0
test_myfind "tests" 0
test_myfind "tests/" 0
test_myfind "tests/." 0
test_myfind "tests/foo" 0
test_myfind "tests/foo/" 0
test_myfind "../src/engine" 0
test_myfind "tests/foo/fae tests/foo/fao" 0

echo "-------------------- Print --------------------"

test_myfind "-print" 0
test_myfind "-print -print" 0
test_myfind "-print -print -print" 0

echo "-------------------- Name --------------------"

test_myfind "-name foo" 0
test_myfind "-name foo -name foo" 0
test_myfind "-name foo -name baz" 0
test_myfind "-name foo -o -name baz" 0
test_myfind "-name foo -a -name baz" 0
test_myfind "-name foo -o -name baz -o -name fae" 0
test_myfind "-name f??" 0

echo "-------------------- Type --------------------"

test_myfind "tests/foo/fao -type f" 0
test_myfind "tests/foo/fao -type d" 0
test_myfind "tests/foo/fao -type l" 0
test_myfind "tests/foo/fao -type p" 0
test_myfind "tests/foo/fao -type c" 0
test_myfind "tests/foo/fao -type b" 0
test_myfind "tests/foo/fao -type s" 0

echo "-------------------- Perm --------------------"

# Create files with different permissions
touch tests/foo/file1
chmod 644 tests/foo/file1
test_myfind "tests/foo/file1 -perm 644" 0
test_myfind "tests/foo/file1 -perm -644" 0  # All bits set
test_myfind "tests/foo/file1 -perm /644" 0  # Any bit set
test_myfind "tests/foo/file1 -perm 755" 0  # Test for a wrong permission
test_myfind "tests/foo/file1 -perm 0" 0  # Test for a wrong permission

touch tests/foo/file2
chmod 755 tests/foo/file2
test_myfind "tests/foo/file2 -perm 755" 0
test_myfind "tests/foo/file2 -perm -755" 0  # All bits set
test_myfind "tests/foo/file2 -perm /755" 0  # Any bit set
test_myfind "tests/foo/file2 -perm 644" 0  # Test for a wrong permission

touch tests/foo/file3
chmod 777 tests/foo/file3
test_myfind "tests/foo/file3 -perm 777" 0
test_myfind "tests/foo/file3 -perm 777" 0  # Exact match
test_myfind "tests/foo/file3 -perm -777" 0  # All bits set
test_myfind "tests/foo/file3 -perm 644" 0  # Test for a wrong permission

echo "-------------------- Newer --------------------"

# Create two files with different timestamps
touch -t 202201011200 tests/foo/file1
sleep 1
touch -t 202201011201 tests/foo/file2

# file2 is newer than file1
test_myfind "tests/foo/file2 -newer tests/foo/file1" 0

# file1 is not newer than file2
test_myfind "tests/foo/file1 -newer tests/foo/file2" 0

echo "-------------------- Newer (nanosecond precision) --------------------"

# Create two files with different timestamps
touch -d '2022-01-01 12:00:00.000000001' tests/foo/file1
sleep 0.000000001
touch -d '2022-01-01 12:00:00.000000002' tests/foo/file2

# file2 is newer than file1
test_myfind "tests/foo/file2 -newer tests/foo/file1" 0

# file1 is not newer than file2
test_myfind "tests/foo/file1 -newer tests/foo/file2" 0

# echo "-------------------- Directory Order (-d) --------------------"

# # Create a directory with a file
# mkdir -p tests/foo/dir
# touch tests/foo/dir/file
# test_myfind "-d tests/foo/dir" 0

# ln -s tests/foo/dir tests/foo/sym/sydir
# ln -s tests/foo/fae/clef tests/foo/dir/syclef
# ln -s tests/foo/dir/file tests/foo/sym/syfile

# echo "-------------------- Symbolic Links --------------------"

# test_myfind "-H tests/foo/sym/syfile" 0
# test_myfind "-H tests/foo/sym/sydir" 0

# test_myfind "-L tests/foo/sym/syfile" 0
# test_myfind "-L tests/foo/sym/sydir" 0

# test_myfind "-P tests/foo/sym/syfile" 0
# test_myfind "-P tests/foo/sym/sydir" 0

# test_myfind "tests/foo/sym/syfile" 0
# test_myfind "tests/foo/sym/sydir" 0

echo "-------------------- Priority --------------------"

test_myfind "-name src -print" 0
test_myfind "-print -name src" 0
test_myfind "-name src -print -name src" 0
test_myfind "-name foo -print -name foo -print" 0
# Test operator priority without parentheses
test_myfind "-name foo -o -name bar -a -name baz" 0

# Test operator priority with parentheses
test_myfind "\\( -name foo -o -name bar \\) -a -name baz" 0
test_myfind "-name foo -o \\( -name bar -a -name baz \\)" 0

echo "-------------------- Parentheses --------------------"

# Test parentheses
test_myfind "\\( -name foo -o -name bar \\) -a -name baz" 0
test_myfind "\\( -name foo -a -name bar \\) -o -name baz" 0
test_myfind "\\( -name foo -o -name bar -a -name baz \\)" 0

