import os
os.system("cp DaVinci-Opts.py DaVinci-Opts_12.py") 
os.system("sed -i 's/IsMC = True/IsMC = False/g' DaVinci-Opts_12.py")
os.system("cp DaVinci-Opts_12.py DaVinci-Opts_11.py")
os.system("sed -i 's/= \"2012\"/= \"2011\"/g' DaVinci-Opts_11.py")

DaVinciVersion = 'v36r1'
myJobName = ['Bsphiphi', 'BsphiKK', 'phiKK_11', 'phiKK_12'] #'Bdphikst', 'Bdkstrho', 'Bsphikst', 'Bsphi3pi', 'Bsphietap']
myFile = ['./DaVinci-Opts.py', './DaVinci-Opts.py', './DaVinci-Opts_11.py', './DaVinci-Opts_12.py']
myApplication = DaVinci()
myApplication.version = DaVinciVersion
myApplication.user_release_area = '/afs/cern.ch/user/a/admorris/cmtuser'
myApplication.platform='x86_64-slc6-gcc48-opt'
#optFile =  [ './DaVinci-Opts.py', './myUp.py']

mySplitter = SplitByFiles(filesPerJob = 40, maxFiles = -1, ignoremissing = True, bulksubmit=True)
myOutput = [ DiracFile('BsphiKK.root'), DiracFile('dummy.root'), LocalFile('summary.xml')]
myBackend = Dirac()
dirc = '~/public/phiKK/'
magnet = ['Down', 'Up']
InputName = []
for i in range (2):
    dataName = 'MC201213104013Beam4000GeV-2012-Mag' + magnet[i] + '-Nu25-Pythia8Sim08aDigi13Trig0x409f0045Reco14aStripping20NoPrescalingFlaggedSTREAMSDST.py'
    InputName.append(dataName)
    
for i in range (2):
    dataName = 'MC201213104024Beam4000GeV-2012-Mag' + magnet[i] + '-Nu25-Pythia8Sim08aDigi13Trig0x409f0045Reco14aStripping20NoPrescalingFlaggedSTREAMSDST.py'
    InputName.append(dataName)

Year = ['11', '12']
Energy = ['3500', '4000']
Strip = ['20r1', '20']
for i in range (2):
    for j in range (2):
        dataName = 'LHCbCollision' + Year[i]+'90000000Beam' + Energy[i] + 'GeV-VeloClosed-Mag' + magnet[j] + 'RealDataReco14Stripping' + Strip[i] + 'BHADRONMDST.py'
        InputName.append(dataName)

for i in range (len(InputName) ):
   print InputName[i]

#decayType = ['15104020','13144004'] #'11104021', '11104041', '13104021', '13104401', '13104231']
#simType = ['Sim08e'] #'Sim08a', 'Sim08f']
#RecoType = ['Reco14', 'Reco14a']

for nameLoop in range(2): #myJobName:
#  opts_names = [File ('./DaVinci-Opts.py')]
  for i in range (2):
    opts_names = [File ( myFile[nameLoop] ) ]
    opts_names += [File ( dirc + InputName[2*nameLoop + i] ) ]
#    opts_names += [File ( InputName[2*index1 + i] ) ]
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
#lxplus0009 screen session test jobs

