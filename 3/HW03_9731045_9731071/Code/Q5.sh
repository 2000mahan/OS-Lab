#!/bin/bash
argArray=( "$@" )
arrayLen="${#argArray[@]}"

flag=1
for (( i=0; i < arrayLen-1 && flag==1 ; i++ ))
do
    flag=0
    for (( j=0; j<arrayLen-i-1; j++ ))
    do
        if (( ${argArray[$j]} > ${argArray[$j+1]} )); then
            temp=${argArray[$j]}
            argArray[$j]=${argArray[$j+1]}
            argArray[$j+1]=$temp
            flag=1
        fi
    done
done

# printing array
for (( l=0; l<arrayLen; l++))
do
    echo -ne "${argArray[$l]} "
done
echo -e " "
