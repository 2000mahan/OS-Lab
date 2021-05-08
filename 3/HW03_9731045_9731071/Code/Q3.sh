#!/bin/bash

function sum() {
	local number=$1
	local sumOfDigits=0
	while [[ $number -ne 0 ]]; do
		((sumOfDigits+=number%10))
		((number/=10))
	done
	echo $sumOfDigits
}

read -p "Enter a number: " number
while [[ "$number" -ne 0 ]]; do
	echo "Sum of digits is $(sum $number)"
	echo "Reverse $(echo $number | rev)"

	read -p "Enter a number: " number
done

