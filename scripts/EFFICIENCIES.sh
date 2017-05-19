#!/bin/bash
## Efficiencies
./trigeff.sh
./seleff.sh
./vetoeff.sh 2>&1| tee log/vetoeff.log
./plotttrigeff.sh 2>&1| tee log/plotttrigeff.log


