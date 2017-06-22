#!/bin/bash
find $1 -name "*SimpleTable.tex" -exec grep "$2" {} \; | sort

