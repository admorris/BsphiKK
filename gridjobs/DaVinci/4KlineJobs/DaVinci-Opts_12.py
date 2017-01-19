from Gaudi.Configuration import *
from Configurables import DaVinci

# Flags for year and Data/MC
DataYear = "2012"
IsMC = False

# Decay descriptor
decay = "[B_s0 -> ^(phi(1020) -> ^K+ ^K-) ^(phi(1020) -> ^K+ ^K-)]CC"

# Root in TES and stripping line
rootintes = "/Event/AllStreams" if IsMC else "/Event/Bhadron"
strippingline = "Phys/Bs2Q2Body4KLine/Particles"

# Define selection
from GaudiConfUtils.ConfigurableGenerators import DecayTreeTuple
from DecayTreeTuple.Configuration import *
dtt = DecayTreeTuple()
#from PhysSelPython.Wrappers import SimpleSelection, SelectionSequence, DataOnDemand, AutomaticData

#strippingOutput = AutomaticData(Location = strippingline)

#sel = SimpleSelection("DecayTreeTuple", DecayTreeTuple, [strippingOutput], Decay = decay, ToolList = tupletools)
#selAlg = sel.algorithm()
#selAlg.addTool(TISTOSTool)
#selAlg.addTool(LoKiVariables2)
#selSeq = SelectionSequence("selseq", sel)

dtt.Inputs = [rootintes+"/"+strippingline,]
dtt.Decay = decay

# Tools and triggers
from Configurables import BackgroundCategory, TupleToolRecoStats, TupleToolTrackIsolation, TupleToolTrigger, TupleToolTISTOS, TriggerTisTos

# Universal tools
tupletools = [
#  "TupleToolGeometry",
#  "TupleToolKinematic",
#  "TupleToolPropertime",
#  "TupleToolPid",
  "TupleToolTrackInfo",
#  "TupleToolTISTOS",
  "TupleToolTrigger",
  "TupleToolRecoStats"
]

# MC-only tools
if IsMC:
  tupletools += [
    "TupleToolMCBackgroundInfo",
    "TupleToolMCTruth",
    "TupleToolTagging",
    "TupleToolPrimaries"
  ]

for tool in tupletools:
  dtt.addTupleTool(tool)

# Trigger Lines
triggerlist = [
  "L0HadronDecision",
  "L0MuonDecision",
  "L0DiMuonDecision",
  "L0ElectronDecision",
  "L0PhotonDecision",
  "Hlt1SingleHadronDecision",
  "Hlt1MBNoBiasDecision",
  "Hlt1DiHadronDecision",
  "Hlt1L0AnyDecision",
  "Hlt1TrackAllL0Decision",
  "Hlt2TopoOSTF4BodyDecision",
  "Hlt2IncPhiDecision",
  "Hlt2Topo4BodySimpleDecision",
  "Hlt2Topo3BodySimpleDecision",
  "Hlt2Topo3BodyBBDTDecision",
  "Hlt2Topo2BodySimpleDecision",
  "Hlt2Topo2BodyBBDTDecision",
  "Hlt2Topo4BodyBBDTDecision",
  "Hlt2TopoMu4BodyBBDTDecision",
  "Hlt2IncPhiSidebandsDecision",
  "Hlt2B2HHDecision"
]

# Get TISTOS info
#TISTOSTool = TupleToolTISTOS("TISTOSTool")
TISTOSTool = dtt.addTupleTool("TupleToolTISTOS")
TISTOSTool.VerboseL0   = True
TISTOSTool.VerboseHlt1 = True
TISTOSTool.VerboseHlt2 = True
TISTOSTool.TriggerList = triggerlist
TISTOSTool.addTool(TriggerTisTos, name="TriggerTisTos")

# LOKI Vairables
from Configurables import  LoKi__Hybrid__TupleTool
#LoKiVariables2 = LoKi__Hybrid__TupleTool("LoKiVariables2")
LoKiVariables2 = dtt.addTupleTool("LoKi::Hybrid::TupleTool/LoKiVariables2")
LoKiVariables2.Variables = {
  "LOKI_Mass"       : "DTF_FUN(M, True)",
  "LOKI_Chi2"       : "DTF_CHI2(True)",
  "LOKI_ndof"       : "DTF_NDOF(True)",
  "LOKI_MassError2" : "DTF_FUN(M2ERR2, True)",
  "LOKI_DTF_CTAU"   : "DTF_CTAU( 0, True )",
  "LOKI_DTF_CTAUS"  : "DTF_CTAUSIGNIFICANCE( 0, True )",
  "LOKI_DTF_CTAUERR": "DTF_CTAUERR( 0, True )",

  "PX_kaon0": "DTF_FUN(CHILD(PX, 1,1), True)",
  "PY_kaon0": "DTF_FUN(CHILD(PY, 1,1), True)",
  "PZ_kaon0": "DTF_FUN(CHILD(PZ, 1,1), True)",
  "ID_kaon0": "DTF_FUN(CHILD(ID, 1,1), True)",

  "PX_kaon1": "DTF_FUN(CHILD(PX, 1,2), True)",
  "PY_kaon1": "DTF_FUN(CHILD(PY, 1,2), True)",
  "PZ_kaon1": "DTF_FUN(CHILD(PZ, 1,2), True)",
  "ID_kaon1": "DTF_FUN(CHILD(ID, 1,2), True)",

  "PX_kaon2": "DTF_FUN(CHILD(PX, 2,1), True)",
  "PY_kaon2": "DTF_FUN(CHILD(PY, 2,1), True)",
  "PZ_kaon2": "DTF_FUN(CHILD(PZ, 2,1), True)",
  "ID_kaon2": "DTF_FUN(CHILD(ID, 2,1), True)",

  "PX_kaon3": "DTF_FUN(CHILD(PX, 2,2), True)",
  "PY_kaon3": "DTF_FUN(CHILD(PY, 2,2), True)",
  "PZ_kaon3": "DTF_FUN(CHILD(PZ, 2,2), True)",
  "ID_kaon3": "DTF_FUN(CHILD(ID, 2,2), True)",

  "BCON_LOKI_Mass": "DTF_FUN(M, True, strings(['B_s0']))",
  "BCON_LOKI_Chi2": "DTF_CHI2(True, strings(['B_s0']))",
  "BCON_LOKI_ndof": "DTF_NDOF(True, strings(['B_s0']))",

  "BCON_PX_kaon0": "DTF_FUN(CHILD(PX, 1,1), True, strings(['B_s0']))",
  "BCON_PY_kaon0": "DTF_FUN(CHILD(PY, 1,1), True, strings(['B_s0']))",
  "BCON_PZ_kaon0": "DTF_FUN(CHILD(PZ, 1,1), True, strings(['B_s0']))",

  "BCON_PX_kaon1": "DTF_FUN(CHILD(PX, 1,2), True, strings(['B_s0']))",
  "BCON_PY_kaon1": "DTF_FUN(CHILD(PY, 1,2), True, strings(['B_s0']))",
  "BCON_PZ_kaon1": "DTF_FUN(CHILD(PZ, 1,2), True, strings(['B_s0']))",

  "BCON_PX_kaon2": "DTF_FUN(CHILD(PX, 2,1), True, strings(['B_s0']))",
  "BCON_PY_kaon2": "DTF_FUN(CHILD(PY, 2,1), True, strings(['B_s0']))",
  "BCON_PZ_kaon2": "DTF_FUN(CHILD(PZ, 2,1), True, strings(['B_s0']))",

  "BCON_PX_kaon3": "DTF_FUN(CHILD(PX, 2,2), True, strings(['B_s0']))",
  "BCON_PY_kaon3": "DTF_FUN(CHILD(PY, 2,2), True, strings(['B_s0']))",
  "BCON_PZ_kaon3": "DTF_FUN(CHILD(PZ, 2,2), True, strings(['B_s0']))"
}




from Configurables import CondDB, CondDBAccessSvc
CondDB().LatestGlobalTagByDataType = DataYear

from Configurables import LHCbApp
LHCbApp().XMLSummary="summary.xml"

# User Algorithms
from Configurables import GaudiSequencer
userAlgos = GaudiSequencer("userAlgos")
userAlgos.Members = []
if  IsMC:
  from Configurables import TrackSmearState as SMEAR
  smear = SMEAR("StateSmear")
  userAlgos.Members.append(smear)
else:
  #
  from Configurables import CheckPV
  checkpv = CheckPV("CheckPV")
  checkpv.MinPVs = 1
  userAlgos.Members.append(checkpv)
  from Configurables import TrackScaleState as SCALER
  scaler = SCALER("StateScale")
  userAlgos.Members.append(scaler)
#userAlgos.Members.append(selSeq.sequence())

#from Configurables import EventTuple
#etuple = EventTuple()
#userAlgos.Members.append( etuple )

myTupleName = "BsphiKK.root"

dv = DaVinci(
    HistogramFile = "dummy.root",
    TupleFile = myTupleName,
    UserAlgorithms = [dtt,userAlgos],
    DataType  = DataYear,
    EvtMax = 1000,
    InputType = "DST" if IsMC else "MDST",
    RootInTES = rootintes,
    PrintFreq  = 1000,
    Simulation = IsMC,
    Lumi = not IsMC )
