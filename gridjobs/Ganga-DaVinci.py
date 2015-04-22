import os
os.system("cp DaVinci-Opts.py DaVinci-Opts_12.py") 
os.system("sed -i 's/IsMC = True/IsMC = False/g' DaVinci-Opts_12.py")
os.system("cp DaVinci-Opts_12.py DaVinci-Opts_11.py")
os.system("sed -i 's/= \"2012\"/= \"2011\"/g' DaVinci-Opts_11.py")

DaVinciVersion = 'v36r1'
myJobName = ['Bsphiphi', 'BsphiKK', 'BsphiKst', 'Bdphipipi', 'LbphiKp', 'phiKK_11', 'phiKK_12']
myFile = ['./DaVinci-Opts.py', './DaVinci-Opts.py', './DaVinci-Opts.py', './DaVinci-Opts.py', './DaVinci-Opts.py', './DaVinci-Opts_11.py', './DaVinci-Opts_12.py']
myApplication = DaVinci()
myApplication.version = DaVinciVersion
myApplication.user_release_area = '/afs/cern.ch/user/a/admorris/cmtuser'
myApplication.platform='x86_64-slc6-gcc48-opt'
#optFile =  [ './DaVinci-Opts.py', './myUp.py']

mySplitter = SplitByFiles(filesPerJob = 40, maxFiles = -1, ignoremissing = True, bulksubmit=True)
myOutput = [ DiracFile('BsphiKK.root'), DiracFile('dummy.root'), LocalFile('summary.xml')]
myBackend = Dirac()
dirc = '~/work/BsphiKK/gridjobs/'
magnet = ['Down', 'Up']
InputName = []
# Bs to phi phi
for i in range (2):
    dataName = 'MC201213104013Beam4000GeV-2012-Mag' + magnet[i] + '-Nu25-Pythia8Sim08aDigi13Trig0x409f0045Reco14aStripping20NoPrescalingFlaggedSTREAMSDST.py'
    InputName.append(dataName)
# Bs to phi KK
for i in range (2):
    dataName = 'MC201213104024Beam4000GeV-2012-Mag' + magnet[i] + '-Nu25-Pythia8Sim08aDigi13Trig0x409f0045Reco14aStripping20NoPrescalingFlaggedSTREAMSDST.py'
    InputName.append(dataName)
# Bs to phi K*
for i in range (2):
    dataName = 'MC201211104021Beam4000GeV-2012-Mag' + magnet[i] + '-Nu25-Pythia8Sim08aDigi13Trig0x409f0045Reco14aStripping20NoPrescalingFlaggedALLSTREAMS.DST.py'
    InputName.append(dataName)
# Bd to phi pi pi
for i in range (2):
    dataName = 'MC201213104081Beam4000GeV2012Mag' + magnet[i] + '-Nu25-Pythia8Sim08aDigi13Trig0x409f0045Reco14aStripping20NoPrescalingFlaggedALLSTREAMS.DST.py'
    InputName.append(dataName)
# Lb to phi K p
for i in range (2):
    dataName = 'MC201215104020Beam4000GeV-2012-Mag' + magnet[i] + '-Nu25-Pythia6+8Sim08aDigi13Trig0x409f0045Reco14aStripping20NoPrescalingFlaggedALLSTREAMS.DST.py'
    InputName.append(dataName)
# Data
Year = ['11', '12']
Energy = ['3500', '4000']
Strip = ['20r1', '20']
for i in range (2):
    for j in range (2):
        dataName = 'LHCbCollision' + Year[i]+'90000000Beam' + Energy[i] + 'GeV-VeloClosed-Mag' + magnet[j] + 'RealDataReco14Stripping' + Strip[i] + 'BHADRONMDST.py'
        InputName.append(dataName)

for i in range (len(InputName) ):
   print InputName[i]
   
if len(InputName) == 2*len(myFile):
  for nameLoop in range(len(myFile)):
    for i in range (2):
      opts_names = [File ( myFile[nameLoop] ) ]
      opts_names += [File ( dirc + InputName[2*nameLoop + i] ) ]
      myApplication.optsfile = opts_names
      j = Job (
        name         = myJobName[nameLoop],
        application  = myApplication,
        splitter     = mySplitter,
        backend      = myBackend,
        outputfiles  = myOutput
      )
      j.do_auto_resubmit = True
      j.submit()
