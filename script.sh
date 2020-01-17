#!/bin/bash
FILE="${1}"

if [[ ! -e $FILE ]]; then
   echo "Path doesn't exist"
   exit 1
elif [[ ! -f $FILE ]]; then
    echo "It's not a file"
    exit 1
fi

shift

for var in "$@"
do
  re='[+-]?^[0-9]+$'
  if ! [[ $var =~ $re ]] ; then
    exit 1
  elif [[ $var -gt 999 || $var -lt 1 ]]; then
    exit 1
  else
    awk -F ";" -v id="$var" '{if ($1 == id){ sum=0; for (col=3; col<=NF; col+=3) sum+=$col; print id";"sum;}}' $FILE
  fi
done
