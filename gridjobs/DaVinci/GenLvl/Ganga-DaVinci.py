DaVinciVersion = 'v36r1'
myJobName = 'BsPhiKKGen'
myFile = './tupleResult.py' 
myApplication = DaVinci()
myApplication.version = DaVinciVersion
myApplication.user_release_area = '/afs/cern.ch/user/a/admorris/cmtuser'
myApplication.platform='x86_64-slc6-gcc48-opt'
#optFile =  [ './DaVinci-Opts.py', './myUp.py']

mySplitter = SplitByFiles(filesPerJob = 20, maxFiles = -1, ignoremissing = True, bulksubmit=True)
myOutput = [ DiracFile('*.root'), LocalFile('summary.xml')]
myBackend = Dirac()
dirc = '~/work/BsphiKK/gridjobs/'
InputName.append('LFNs.py')
opts_names = [File ( myFile[nameLoop] ) ]
opts_names += [File ( dirc + InputName[2*nameLoop + i] ) ]
myApplication.optsfile = opts_names
j = Job (
name         = myJobName,
application  = myApplication,
splitter     = mySplitter,
backend      = myBackend,
outputfiles  = myOutput
)
j.do_auto_resubmit = True
j.submit()
