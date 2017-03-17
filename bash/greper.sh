#!/bin/bash
echo "type word to found"
read a
cat SimpleText.txt | grep -i $a >> Search.txt

