#!/bin/bash
gaudirun.py $GAUSSOPTS/Gauss-2012.py \
            $1 \
            $LBPYTHIAROOT/options/Pythia.py \
            $GAUSSOPTS/GenStandAlone.py \
            $GAUSSOPTS/Gauss-Job.py
