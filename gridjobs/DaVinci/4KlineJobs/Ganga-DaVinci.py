# Works with Ganga 6.1.14... sometimes
# May get 'AttributeError: 'SubJobXMLList' object has no attribute 'append''
# If so, just resubmit

import os
os.system('cp DaVinci-Opts.py DaVinci-Opts_12.py') 
os.system("sed -i 's/IsMC = True/IsMC = False/g' DaVinci-Opts_12.py")
os.system('cp DaVinci-Opts_12.py DaVinci-Opts_11.py')
os.system("sed -i 's/= \'2012\'/= \'2011\'/g' DaVinci-Opts_11.py")

def submit_job(name, optionsfile, data):
  dataset = BKQuery(data,dqflag=['OK']).getDataset()
  dv = DaVinci()
  dv.version = 'v41r3'
#  dv.user_release_area = '/afs/cern.ch/user/a/admorris/cmtuser'
#  dv.platform='x86_64-slc6-gcc48-opt'
  dv.optsfile = [File ( optionsfile )]
  j = Job (
    name         = name,
    application  = dv,
    splitter     = SplitByFiles(filesPerJob = 100, maxFiles = -1, ignoremissing = True, bulksubmit=False),
    backend      = Dirac(),
    outputfiles  = [ DiracFile('BsphiKK.root'), DiracFile('dummy.root'), LocalFile('summary.xml')],
    inputdata    = dataset
  )
  j.do_auto_resubmit = True
  j.submit()

magnet = ['Down', 'Up']
Year = ['11', '12']
Energy = ['3500', '4000']
Strip = ['21r1', '21']

# MC

## Bs to phi phi
#DSTpaths = []
#for i in range (2):
#  DSTpaths.append('/MC/2012/13104013/Beam4000GeV-2012-Mag' + magnet[i] + '-Nu2.5-Pythia6/Sim08a/Digi13/Trig0x409f0045/Reco14a/Stripping20NoPrescalingFlagged/ALLSTREAMS.DST')
#  DSTpaths.append('/MC/2012/13104013/Beam4000GeV-2012-Mag' + magnet[i] + '-Nu2.5-Pythia8/Sim08a/Digi13/Trig0x409f0045/Reco14a/Stripping20NoPrescalingFlagged/ALLSTREAMS.DST')
#submit_job(name = 'Bsphiphi', optionsfile = './DaVinci-Opts.py', data = DSTpaths)
## Bs to phi KK
#DSTpaths = []
#for i in range (2):
#  DSTpaths.append('/MC/2012/13104024/Beam4000GeV-2012-Mag' + magnet[i] + '-Nu2.5-Pythia8/Sim08a/Digi13/Trig0x409f0045/Reco14a/Stripping20NoPrescalingFlagged/ALLSTREAMS.DST')
#  DSTpaths.append('/MC/2012/13104024/Beam4000GeV-2012-Mag' + magnet[i] + '-Nu2.5-Pythia8/Sim08i/Digi13/Trig0x409f0045/Reco14c/Stripping20NoPrescalingFlagged/ALLSTREAMS.DST')
#submit_job(name = 'BsphiKK', optionsfile = './DaVinci-Opts.py', data = DSTpaths)
## Bd to phi K*
#DSTpaths = []
#for i in range (2):
#  DSTpaths.append('/MC/2012/11104021/Beam4000GeV-2012-Mag' + magnet[i] + '-Nu2.5-Pythia8/Sim08a/Digi13/Trig0x409f0045/Reco14a/Stripping20NoPrescalingFlagged/ALLSTREAMS.DST')
#submit_job(name = 'BdphiKst', optionsfile = './DaVinci-Opts.py', data = DSTpaths)
## Bs to phi pi pi
#DSTpaths = []
#for i in range (2):
#  DSTpaths.append('/MC/2012/13104081/Beam4000GeV-2012-Mag' + magnet[i] + '-Nu-2.5-Pythia8/Sim08e/Digi13/Trig0x409f0045/Reco14a/Stripping20NoPrescalingFlagged/ALLSTREAMS.DST')
#submit_job(name = 'Bsphipipi', optionsfile = './DaVinci-Opts.py', data = DSTpaths)
## Lb to phi K p
#DSTpaths = []
#for i in range (2):
#  DSTpaths.append('/MC/2012/15104020/Beam4000GeV-2012-Mag' + magnet[i] + '-Nu2.5-Pythia6/Sim08e/Digi13/Trig0x409f0045/Reco14a/Stripping20NoPrescalingFlagged/ALLSTREAMS.DST')
#  DSTpaths.append('/MC/2012/15104020/Beam4000GeV-2012-Mag' + magnet[i] + '-Nu2.5-Pythia8/Sim08e/Digi13/Trig0x409f0045/Reco14a/Stripping20NoPrescalingFlagged/ALLSTREAMS.DST')
#submit_job(name = 'LbphiKp', optionsfile = './DaVinci-Opts.py', data = DSTpaths)

# Data
for i in range (2):
  for j in range (2):
    DSTpath = ('/LHCb/Collision' + Year[i] + '/Beam' + Energy[i] + 'GeV-VeloClosed-Mag' + magnet[j] + '/Real Data/Reco14/Stripping' + Strip[i] + '/90000000/BHADRON.MDST')
    submit_job(name = 'data_'+Year[i]+"_"+magnet[i], optionsfile = './DaVinci-Opts_'+Year[i]+'.py', data = DSTpath)
