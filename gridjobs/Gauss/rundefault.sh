#!/bin/bash
gaudirun.py $GAUSSOPTS/Gauss-2012.py \
            13104024.py \
            $LBPYTHIAROOT/options/Pythia.py \
            $GAUSSOPTS/GenStandAlone.py \
            $GAUSSOPTS/Gauss-Job.py
