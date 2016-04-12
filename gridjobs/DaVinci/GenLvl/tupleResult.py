from Configurables import ( DaVinci
#                          , EventSelector # Uncomment when running locally
                          , MCDecayTreeTuple
                          , LHCbApp
                          )
from DecayTreeTuple.Configuration import *
LHCbApp().XMLSummary='summary.xml'

# Create an MC DTT containing any candidates matching the decay descriptor
mctuple = MCDecayTreeTuple()
mctuple.Decay = "[B_s0 => ^(phi(1020) ==> ^K+ ^K-) ^(phi(1020) ==> ^K+ ^K-)]CC"
mctuple.addBranches({
   "KK" : " [B_s0 => (phi(1020) ==> K+ K-) ^(phi(1020) ==> K+ K-)]CC "
})
mctuple.ToolList = []
mctuple.addTupleTool("MCTupleToolKinematic").Verbose = True


# Name of the .xgen file produced by Gauss
#ev = EventSelector()
#ev.Input = ["DATAFILE='Gauss-13104013-100000ev-20160407.xgen' TYP='POOL_ROOTTREE' Opt='READ'"]

# Configure DaVinci
dv = DaVinci( HistogramFile = "histo.root"
            , TupleFile = "DVntuple.root"
            , UserAlgorithms = [mctuple]
            , Simulation = True
            , Lumi = False
            , DataType = "2012"
            , EvtMax = -1
            )
