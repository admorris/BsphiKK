JobName  = 'BsPhiPhiGen'
LFNsFile = 'PwaveLFNs.txt'
Application = DaVinci()
Application.version = 'v41r0'

debug = False

Splitter = SplitByFiles(ignoremissing = True, bulksubmit=False)
Output   = [ DiracFile('DVntuple.root'), LocalFile('histo.root'), LocalFile('summary.xml') ]
Backend  = Dirac()
Input    = []
LFNs = [line.rstrip('\n') for line in open(LFNsFile)]
if not debug:
  for line in LFNs:
    Input.append(DiracFile(lfn=line))
else:
  Input.append(DiracFile(lfn=LFNs[0]))
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
