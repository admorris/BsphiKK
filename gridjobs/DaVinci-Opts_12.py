from Gaudi.Configuration import * 
from Configurables import DaVinci

# Flags for year and Data/MC
DataYear = "2012"
IsMC = False

# Define selection ############################################################
from PhysSelPython.Wrappers import Selection, SelectionSequence, DataOnDemand, AutomaticData
from Configurables import SubstitutePID
from StrippingSelections.StrippingBs2KKhh import BsPhiRhoConf

# Choose appropriate stripping line
strippingline = "Phys/BsPhiRhoLine/Particles" if not IsMC else "/Event/AllStreams/Phys/BsPhiRhoLine/Particles"
_strippingOutput = AutomaticData(Location = strippingline)

# Change mass hypothesis
newDecay = "DECTREE('B_s0 -> (phi(1020) -> K+ K-) (f_0(980) -> K+ K-)')"
_SubstituteB0 = SubstitutePID( name = "_SubstituteB0"
                             , Code = " DECTREE('B0 -> (phi(1020) -> K+ K-) (rho(770)0 -> pi+ pi-)') " 
                             , Substitutions = { 'Beauty -> (phi(1020) -> K+ K-) ^(rho(770)0 -> X+ X-)' : 'f_0(980)'
                                               , 'Beauty -> (phi(1020) -> K+ K-) (X0 -> ^pi+ X-)' : 'K+'
                                               , 'Beauty -> (phi(1020) -> K+ K-) (X0 -> X+ ^pi-)' : 'K-'
                                               , 'Beauty -> (phi(1020) -> K+ K-) (X0 -> X+ X-)' : 'B_s0'
                                               }
                             )
_ddChangeSel = Selection( "_ddChangeSel"
                        , Algorithm = _SubstituteB0
                        , RequiredSelections = [_strippingOutput]
                        )
selSeq = SelectionSequence("selseqname", TopSelection = _ddChangeSel)
# Create Decay Tree Tuple #####################################################
from Configurables import DecayTreeTuple
from DecayTreeTuple.Configuration import *
tuple = DecayTreeTuple()
tuple.Inputs = [ selSeq.outputLocation() ]
#tuple.Inputs = [ strippingline ]

from Configurables import BackgroundCategory
from Configurables import TupleToolTrigger, TupleToolRecoStats, TupleToolTrackIsolation, TupleToolTagging, TupleToolTISTOS

# Universal tools
tuple.ToolList += [ "TupleToolGeometry"
                  , "TupleToolKinematic"
                  , "TupleToolPropertime"
#                  , "TupleToolPrimaries"
                  , "TupleToolPid"
                  , "TupleToolTrackInfo"
                  , "TupleToolTISTOS"
                  , "TupleToolTrigger"  
                  , "TupleToolRecoStats"
                  ]

# MC-only tools
if IsMC:
    tuple.ToolList += [ "TupleToolMCBackgroundInfo"
                      , "TupleToolMCTruth"
                      , "TupleToolTagging"
#                      , "TupleToolTrackIsolation" # Doesn't work
                      , "TupleToolPrimaries"
                      ]


# Trigger Lines
tlist = [ "L0HadronDecision"
        , "L0MuonDecision"
        , "L0DiMuonDecision"
        , "L0ElectronDecision"
        , "L0PhotonDecision"
        , "Hlt1SingleHadronDecision"
        , "Hlt1MBNoBiasDecision"
        , "Hlt1DiHadronDecision"
        , "Hlt1L0AnyDecision"
        , "Hlt1TrackAllL0Decision"
        , "Hlt2TopoOSTF4BodyDecision"
        , "Hlt2IncPhiDecision"
        , "Hlt2Topo4BodySimpleDecision"
        , "Hlt2Topo3BodySimpleDecision"
        , "Hlt2Topo3BodyBBDTDecision"
        , "Hlt2Topo2BodySimpleDecision"
        , "Hlt2Topo2BodyBBDTDecision"
        , "Hlt2Topo4BodyBBDTDecision"
        , "Hlt2TopoMu4BodyBBDTDecision"
        , "Hlt2IncPhiSidebandsDecision"
        , "Hlt2B2HHDecision"
        ]
# Get trigger info
tuple.addTool(TupleToolTrigger, name="TupleToolTrigger")
tuple.TupleToolTrigger.Verbose = True
tuple.TupleToolTrigger.TriggerList = tlist

# Get TISTOS info
tuple.addTool(TupleToolTISTOS, name="TupleToolTISTOS")
tuple.TupleToolTISTOS.Verbose = True
tuple.TupleToolTISTOS.TriggerList = tlist
# Decay descriptor
tuple.Decay = "[B_s0 -> ^(phi(1020) -> ^K+ ^K-) ^(f_0(980) -> ^K+ ^K-)]CC"
#tuple.Decay = "[B0 -> ^(phi(1020) -> ^K+ ^K-) ^(rho(770)0 -> ^pi+ ^pi-)]CC" # original
# LOKI Vairables
if IsMC:
  from Configurables import  LoKi__Hybrid__TupleTool
  LoKiVariables2 = LoKi__Hybrid__TupleTool('LoKiVariables2')
  LoKiVariables2.Variables = {
    "LOKI_Mass" : "DTF_FUN(M, True)",
    "LOKI_Chi2" : "DTF_CHI2(True)",
    "LOKI_ndof" : "DTF_NDOF(True)",
    "LOKI_MassError2" : "DTF_FUN(M2ERR2, True)",
    "LOKI_DTF_CTAU"        : "DTF_CTAU( 0, True )",
    "LOKI_DTF_CTAUS"       : "DTF_CTAUSIGNIFICANCE( 0, True )",
    "LOKI_DTF_CTAUERR"     : "DTF_CTAUERR( 0, True )",

    "PX_kaon1": "DTF_FUN(CHILD(PX, 1,1), True)",
    "PY_kaon1": "DTF_FUN(CHILD(PY, 1,1), True)",
    "PZ_kaon1": "DTF_FUN(CHILD(PZ, 1,1), True)",

    "PX_kaon2": "DTF_FUN(CHILD(PX, 1,2), True)",
    "PY_kaon2": "DTF_FUN(CHILD(PY, 1,2), True)",
    "PZ_kaon2": "DTF_FUN(CHILD(PZ, 1,2), True)",

    "PX_kaon3": "DTF_FUN(CHILD(PX, 2,1), True)",
    "PY_kaon3": "DTF_FUN(CHILD(PY, 2,1), True)",
    "PZ_kaon3": "DTF_FUN(CHILD(PZ, 2,1), True)",

    "PX_kaon4": "DTF_FUN(CHILD(PX, 2,2), True)",
    "PY_kaon4": "DTF_FUN(CHILD(PY, 2,2), True)",
    "PZ_kaon4": "DTF_FUN(CHILD(PZ, 2,2), True)",

    "BCON_LOKI_Mass" : "DTF_FUN(M, True, strings(['B_s0']))",
    "BCON_LOKI_Chi2" : "DTF_CHI2(True, strings(['B_s0']))",
    "BCON_LOKI_ndof" : "DTF_NDOF(True, strings(['B_s0']))",
    "BCON_PX_kaon1": "DTF_FUN(CHILD(PX, 1,1), True, strings(['B_s0']))",
    "BCON_PY_kaon1": "DTF_FUN(CHILD(PY, 1,1), True, strings(['B_s0']))",
    "BCON_PZ_kaon1": "DTF_FUN(CHILD(PZ, 1,1), True, strings(['B_s0']))",

    "BCON_PX_kaon2": "DTF_FUN(CHILD(PX, 1,2), True, strings(['B_s0']))",
    "BCON_PY_kaon2": "DTF_FUN(CHILD(PY, 1,2), True, strings(['B_s0']))",
    "BCON_PZ_kaon2": "DTF_FUN(CHILD(PZ, 1,2), True, strings(['B_s0']))",

    "BCON_PX_kaon3": "DTF_FUN(CHILD(PX, 2,1), True, strings(['B_s0']))",
    "BCON_PY_kaon3": "DTF_FUN(CHILD(PY, 2,1), True, strings(['B_s0']))",
    "BCON_PZ_kaon3": "DTF_FUN(CHILD(PZ, 2,1), True, strings(['B_s0']))",

    "BCON_PX_kaon4": "DTF_FUN(CHILD(PX, 2,2), True, strings(['B_s0']))",
    "BCON_PY_kaon4": "DTF_FUN(CHILD(PY, 2,2), True, strings(['B_s0']))",
    "BCON_PZ_kaon4": "DTF_FUN(CHILD(PZ, 2,2), True, strings(['B_s0']))"
  }

  tuple.addTool(LoKiVariables2 , name = 'LoKiVariables2' )
  tuple.ToolList   += [ 'LoKi::Hybrid::TupleTool/LoKiVariables2']

##################################################
from Configurables import CondDB, CondDBAccessSvc
CondDB().LatestGlobalTagByDataType = DataYear
##################################################
from Configurables import LHCbApp
LHCbApp().XMLSummary='summary.xml'

#from Configurables import DataOnDemandSvc, L0SelReportsMaker, L0DecReportsMaker
#DataOnDemandSvc().AlgMap["HltLikeL0/DecReports"] = L0DecReportsMaker( OutputLevel = 4 )
#DataOnDemandSvc().AlgMap["HltLikeL0/SelReports"] = L0SelReportsMaker( OutputLevel = 4 )

# User Algorithms
from Configurables import GaudiSequencer
userAlgos = GaudiSequencer("userAlgos")
userAlgos.Members = []
#userAlgos=[]
if  IsMC:
  from Configurables import TrackSmearState as SMEAR
  smear = SMEAR('StateSmear')
  userAlgos.Members.append(smear)
else:
  #
  from Configurables import CheckPV
  checkpv = CheckPV("CheckPV")
  checkpv.MinPVs = 1
  userAlgos.Members.append(checkpv)
  from Configurables import TrackScaleState as SCALER
  scaler = SCALER('StateScale')
  userAlgos.Members.append(scaler)  
userAlgos.Members.append(selSeq.sequence())
userAlgos.Members.append(tuple)
  
#from Configurables import HltSelReportsDecoder, HltVertexReportsDecoder, HltDecReportsDecoder
#userAlgos.Members.append( HltSelReportsDecoder() )
#userAlgos.Members.append( HltVertexReportsDecoder() )
#userAlgos.Members.append( HltDecReportsDecoder() )

from Configurables import EventTuple
etuple = EventTuple()
userAlgos.Members.append( etuple )

myTupleName = 'BsphiKK.root'

dv = DaVinci( 
    HistogramFile = 'dummy.root',
    TupleFile = myTupleName,
    UserAlgorithms = [userAlgos],
    DataType  = DataYear,
    EvtMax = -1,
    InputType = 'DST' if IsMC else 'MDST',
    RootInTES = "/Event/AllStreams" if IsMC else '/Event/Bhadron',
    PrintFreq  = 1000,
    Simulation = IsMC,
    Lumi = False if IsMC else True )
