#!/bin/bash
gaudirun.py $GAUSSOPTS/Gauss-2012.py \
            13104027.opts \
            $LBPYTHIAROOT/options/Pythia.py \
            $GAUSSOPTS/GenStandAlone.py \
            $GAUSSOPTS/Gauss-Job.py
