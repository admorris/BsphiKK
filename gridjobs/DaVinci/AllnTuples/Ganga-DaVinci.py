import os
os.system("cp DaVinci-Opts.py DaVinci-Opts_12.py") 
os.system("sed -i 's/IsMC = True/IsMC = False/g' DaVinci-Opts_12.py")
os.system("cp DaVinci-Opts_12.py DaVinci-Opts_11.py")
os.system("sed -i 's/= \"2012\"/= \"2011\"/g' DaVinci-Opts_11.py")

DaVinciVersion = 'v36r1'

myApplication = DaVinci()
myApplication.version = DaVinciVersion
myApplication.user_release_area = '/afs/cern.ch/user/a/admorris/cmtuser'
myApplication.platform='x86_64-slc6-gcc48-opt'

mySplitter = SplitByFiles(filesPerJob = 20, maxFiles = -1, ignoremissing = True, bulksubmit=True)
myOutput = [ DiracFile('BsphiKK.root'), DiracFile('dummy.root'), LocalFile('summary.xml')]
myBackend = Dirac()
dirc = '~/work/BsphiKK/gridjobs/DaVinci/AllnTuples/'

def submit_job(name, optionsfile, DSTLFNs):
  opts_names = [File ( optionsfile ) ]
  opts_names += [File ( dirc + DSTLFNs ) ]
  myApplication.optsfile = opts_names
  j = Job (
    name         = name,
    application  = myApplication,
    splitter     = mySplitter,
    backend      = myBackend,
    outputfiles  = myOutput
  )
  j.do_auto_resubmit = True
  j.submit()

magnet = ['Down', 'Up']
# Bs to phi phi
for i in range (2):
    dataName = 'MC201213104013Beam4000GeV-2012-Mag' + magnet[i] + '-Nu2.5-Pythia8Sim08aDigi13Trig0x409f0045Reco14aStripping20NoPrescalingFlaggedALLSTREAMSDST.py'
#    submit_job(name = "Bsphiphi", optionsfile = "./DaVinci-Opts.py", DSTLFNs = dataName)
# Bs to phi KK
for i in range (2):
    dataName = 'MC201213104024Beam4000GeV-2012-Mag' + magnet[i] + '-Nu2.5-Pythia8Sim08aDigi13Trig0x409f0045Reco14aStripping20NoPrescalingFlaggedALLSTREAMSDST.py'
    submit_job(name = "BsphiKK", optionsfile = "./DaVinci-Opts.py", DSTLFNs = dataName)
for i in range (2):
    dataName = 'MC201213104024Beam4000GeV-2012-Mag' + magnet[i] + '-Nu2.5-Pythia8Sim08iDigi13Trig0x409f0045Reco14cStripping20NoPrescalingFlaggedALLSTREAMSDST.py'
    submit_job(name = "BsphiKK", optionsfile = "./DaVinci-Opts.py", DSTLFNs = dataName)
# Bd to phi K*
for i in range (2):
    dataName = 'MC201211104021Beam4000GeV-2012-Mag' + magnet[i] + '-Nu2.5-Pythia8Sim08aDigi13Trig0x409f0045Reco14aStripping20NoPrescalingFlaggedALLSTREAMS.DST.py'
#    submit_job(name = "BdphiKst", optionsfile = "./DaVinci-Opts.py", DSTLFNs = dataName)
# Bs to phi pi pi
for i in range (2):
    dataName = 'MC201213104081Beam4000GeV-2012-Mag' + magnet[i] + '-Nu-2.5-Pythia8Sim08eDigi13Trig0x409f0045Reco14aStripping20NoPrescalingFlaggedALLSTREAMS.DST.py'
#    submit_job(name = "Bsphipipi", optionsfile = "./DaVinci-Opts.py", DSTLFNs = dataName)
# Lb to phi K p
for i in range (2):
    dataName = 'MC201215104020Beam4000GeV-2012-Mag' + magnet[i] + '-Nu2.5-Pythia6+8Sim08eDigi13Trig0x409f0045Reco14aStripping20NoPrescalingFlaggedALLSTREAMS.DST.py'
#    submit_job(name = "LbphiKp", optionsfile = "./DaVinci-Opts.py", DSTLFNs = dataName)
# Data
Year = ['11', '12']
Energy = ['3500', '4000']
Strip = ['20r1', '20']
for i in range (2):
    for j in range (2):
        dataName = 'LHCbCollision' + Year[i]+'90000000Beam' + Energy[i] + 'GeV-VeloClosed-Mag' + magnet[j] + 'RealDataReco14Stripping' + Strip[i] + 'BHADRONMDST.py'
#        submit_job(name = "phiKK_"+Year[i], optionsfile = './DaVinci-Opts_'+Year[i]+'.py', DSTLFNs = dataName)

