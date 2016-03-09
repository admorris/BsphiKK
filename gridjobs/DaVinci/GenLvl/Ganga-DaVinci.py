# Does not immediately work on any current version of Ganga (6.1.11 or before)
# It should hopefully work with the next version
# https://github.com/ganga-devs/ganga/commit/30e0ef56537ce8f5b2ac6b89a9666cd8024b137b
JobName = 'BsPhiKKGen'
Application = DaVinci()
Application.version = 'v37r0'

Splitter = SplitByFiles(filesPerJob = 20, maxFiles = -1, ignoremissing = True, bulksubmit=True)
Output   = [ DiracFile('*.root'), LocalFile('summary.xml') ]
Backend  = Dirac()
#Input    = [ File ( './LFNs.py' ) ]
Options = [ File ( './tupleResult.py' ), File ( './PwaveLFNs.py' ) ] # This isn't the proper way to do this
Application.optsfile = Options
j = Job ()
j.name         = JobName
j.application  = Application
j.splitter     = Splitter
j.backend      = Backend
j.outputfiles  = Output
#j.inputdata    = j.application.readInputData(Options) # This crashes for some obscure reason.
j.do_auto_resubmit = True
j.prepare()
j.submit()
