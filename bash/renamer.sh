#!/bin/bash
i=0
for d in b*
do
mv  $d e$i
i=$[$i+1]
done
