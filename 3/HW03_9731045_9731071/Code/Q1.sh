#!/bin/bash

# Checking the number of arguments
if [[ $# -ne 2 ]]; then
	echo "Number of arguments must be 2"
	exit 1
fi


# Checking if the arguments are numeric 
regex='^[+-]?[0-9]+$'
for i in "${@}"; do
	if [[ ! $i =~ $regex ]]; then
		echo "\"$i\" is not a numeric value"
		exit 1
	fi
done


# Printing sum of two arguments
echo "Sum of two arguments is: $(($1 + $2))"

# Comparing arguments
if [[ $1 -gt $2 ]]; then
	echo "$1 is greater than $2"

elif [[ $1 -eq $2 ]]; then
	echo "$1 and $2 are equal"
	
else
	echo "$2 is greater than $1"

fi
