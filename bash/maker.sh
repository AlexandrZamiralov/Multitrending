#!/bin/bash
i=0
while [ $i -lt 34 ]
do
touch a$i
touch b$i
touch c$i
i=$[$i+1]
done

