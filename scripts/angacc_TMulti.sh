#!/bin/bash
cd ..
bin/AngularAcceptance_TMulti ntuples/BsphiKK_MC_mvacut.root
mv -v AutoAngAccMDF.cxx src/lib/AutoAngAcc.cpp
make -j16 # Sorry!
bin/testAngAcc 

