#!/bin/bash
find results/datafits/1800isobar/ -name "*SimpleTable.tex" -exec grep "$1" {} \; | sort

