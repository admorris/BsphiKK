# Tested with Ganga 6.1.11

import sys
import os

GaussVersion = "v49r0"

GaussDir = "/afs/cern.ch/lhcb/software/releases/GAUSS/GAUSS_"+GaussVersion
OptnsDir = GaussDir+"/Sim/Gauss/options"
PythiDir = GaussDir+"/Gen/LbPythia"
CurntDir = os.getcwd()

if len(sys.argv) is not 2:
  sys.exit("Please provide an event type file.")

j = Job(application=Gauss(version=GaussVersion))
j.application.optsfile = [
    OptnsDir+"/Gauss-2012.py"
  , CurntDir+"/"+sys.argv[1]
  , PythiDir+"/options/Pythia.py"
  , OptnsDir+"/GenStandAlone.py"
  , OptnsDir+"/Gauss-Job.py"
  ]
j.name = "Default Generation"
j.splitter = GaussSplitter(numberOfJobs=200,eventsPerJob=25000)
j.outputfiles = [DiracFile("*.xgen"),DiracFile("*.root"),DiracFile("*.xml")]
j.backend = Dirac()
j.submit()
