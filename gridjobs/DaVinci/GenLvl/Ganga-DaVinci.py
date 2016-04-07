JobName  = 'BsPhiPhiGen'
LFNsFile = 'PwaveLFNs.txt'
#JobName  = 'BsPhiKKGen'
#LFNsFile = 'PhaseSpaceLFNs.txt'
Application = DaVinci()
Application.version = 'v37r0'

Splitter = SplitByFiles(ignoremissing = True, bulksubmit=True)
Output   = [ DiracFile('*.root'), LocalFile('summary.xml') ]
Backend  = Dirac()
Input    = []
LFNs = [line.rstrip('\n') for line in open(LFNsFile)]
for line in LFNs:
  Input.append(DiracFile(lfn=line))
Options  = [ File ( './tupleResult.py' ) ]
Application.optsfile = Options
j = Job ()
j.name         = JobName
j.application  = Application
j.splitter     = Splitter
j.backend      = Backend
j.outputfiles  = Output
j.inputdata    = Input
j.do_auto_resubmit = True
j.prepare()
j.submit()
