#!/bin/bash

operands_regex='^[+-]?[0-9]+(.[0-9]+)?$'

read -p "Enter first operator: " op1
read -p "Enter operand: " operand
read -p "Enter second operator: " op2

# Checking whether arguments are numeric

	if [[ ! "$op1" =~ $operands_regex ]]; then
		echo "\"$op1\" is not a real value."
		exit 1
	fi

	if [[ ! "$op2" =~ $operands_regex ]]; then
		echo "\"$op2\" is not a real value."
		exit 1
	fi


echo -n "Result: "
case $operand in
	+)
	        echo "$op1 + $op2" | bc ;;
	-)
		echo "$op1 - $op2" | bc ;;
	\*)
		echo "$op1 * $op2" | bc ;;
	/)
		echo "$op1 / $op2" | bc ;;
esac


