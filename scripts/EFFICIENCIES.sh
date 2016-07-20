#!/bin/bash
## Efficiencies
./trigeff.sh
./seleff.sh
./vetoeff.sh | tee log/vetoeff.log
./plotttrigeff.sh
exit 0

