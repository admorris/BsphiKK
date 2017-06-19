#!/bin/bash
hline=$(for dummy in $(seq $(tput cols)); do printf "_"; done)
for jid in $(qstat | grep " r " | sed -r "s/^\s*([0-9]*).*$/\1/")
	do echo -e "job $jid:\n"
	tail -1 $(find . -name "*.o$jid")
	echo -e "\n$hline\n"
done
