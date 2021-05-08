#!/bin/bash

read -p "Enter file name: " file
read -p "Enter the starting line number: " x
read -p "Enter the stopping line number: " y

head -n $y $file | tail -n $(($y - $x + 1))

