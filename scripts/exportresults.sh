#!/bin/bash
for table in `ls tables/*.csv | sed 's/tables\///g' | sed 's/\.csv//g'`
do
	../bin/ExportResults tables/${table}.csv ../latex/results/${table}.tex
done
