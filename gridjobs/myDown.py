from Gaudi.Configuration import * 
from Configurables import DecayTreeTuple, TupleToolTrigger, SubstitutePID, DaVinci, TupleToolTagging, TupleToolTISTOS, CheckPV, FilterDesktop 
#from StrippingSelections.StrippingBs2Q2B import Bs2Q2Body4piConf
from StrippingSelections.StrippingBs2KKhh import BsPhiRhoConf
from DecayTreeTuple.Configuration import *
from PhysSelPython.Wrappers import Selection, SelectionSequence, DataOnDemand, AutomaticData
from Configurables import  LoKi__Hybrid__TupleTool
from Configurables import DaVinci,HltSelReportsDecoder,HltVertexReportsDecoder,HltDecReportsDecoder, BackgroundCategory, TupleToolRecoStats, TupleToolTrackIsolation
from Configurables import GaudiSequencer
from Configurables import EventNodeKiller

myDecayType = 3 
#myDecayType : (1 for Bsphirho, 2 for Bsphif0, 3 for Bsphiphi) 
DataYear = "2012"
IsMC = True
if not IsMC: 
    myDecayType = 0
    MySequencer = GaudiSequencer('Sequence')
    eventNodeKiller = EventNodeKiller('DAQkiller')
    eventNodeKiller.Nodes = ['DAQ','pRec']
    MySequencer.Members+=[eventNodeKiller]

userAlgos = []

from Configurables import TrackScaleState as SCALER
scaler = SCALER('StateScale')
from Configurables import TrackSmearState as SMEAR
smear = SMEAR('StateSmear')

mydataLocation = "/Event/AllStreams/Phys/BsPhiRhoLine/Particles"
_strippingOutput = AutomaticData(Location = mydataLocation)

if myDecayType and IsMC:
  if myDecayType == 1:
      newDecay = "DECTREE('B_s0 -> (phi(1020) -> K+ K-) (rho(770)0 -> pi+ pi-)')"
      _SubstituteB0 = SubstitutePID ( name = "_SubstituteB0", 
        Code = " DECTREE('B0 -> (phi(1020) -> K+ K-) (rho(770)0 -> pi+ pi-)') " , 
        Substitutions = {'Beauty -> (phi(1020) -> K+ K-) (rho(770)0 -> pi+ pi-)' : 'B_s0'})
  elif myDecayType == 2:
      newDecay = "DECTREE('B_s0 -> (phi(1020) -> K+ K-) (f_0(980) -> pi+ pi-)')"
      _SubstituteB0 = SubstitutePID ( name = "_SubstituteB0",
        Code = " DECTREE('B0 -> (phi(1020) -> K+ K-) (rho(770)0 -> pi+ pi-)') " ,
        Substitutions = {'Beauty -> (phi(1020) -> K+ K-) (rho(770)0 -> pi+ pi-)' : 'B_s0',
                         'Beauty -> (phi(1020) -> K+ K-) ^(rho(770)0 -> pi+ pi-)' : 'f_0(980)'})
  elif myDecayType == 3:
      newDecay = "DECTREE('B_s0 -> (phi(1020) -> K+ K-) (phi(1020) -> K+ K-)')"
      _SubstituteB0 = SubstitutePID ( name = "_SubstituteB0",
        Code = " DECTREE('B0 -> (phi(1020) -> K+ K-) (rho(770)0 -> pi+ pi-)') " ,
        Substitutions = {'Beauty -> (phi(1020) -> K+ K-) ^(rho(770)0 -> X+ X-)' : 'phi(1020)',
                         'Beauty -> (phi(1020) -> K+ K-) (X0 -> ^pi+ X-)' : 'K+',
                         'Beauty -> (phi(1020) -> K+ K-) (X0 -> X+ ^pi-)' : 'K-',
                         'Beauty -> (phi(1020) -> K+ K-) (X0 -> X+ X-)' : 'B_s0'})

  _ddChangeSel = Selection( "_ddChangeSel",
                            Algorithm = _SubstituteB0,
                            RequiredSelections = [_strippingOutput]) #[DataOnDemand(Location = "/Event/AllStreams/Phys/BsPhiRhoLine/Particles")])
  selSeq = Selection("selSeq",
                      Algorithm = FilterDesktop("BsPhiRhoFilter", Code = newDecay),
                      RequiredSelections = [_ddChangeSel])
#  selSeq = SelectionSequence("selseqname", TopSelection = _ddChangeSel)

tuple = DecayTreeTuple()

if not IsMC:
    tuple.Inputs = [ "/Event/Bhadron/Phys/BsPhiRhoLine/Particles" ]
elif not myDecayType and IsMC:
    tuple.Inputs = [ "/Event/AllStreams/Phys/BsPhiRhoLine/Particles" ]
elif myDecayType and IsMC:
  tuple.Inputs = [ '/Event/Phys/selSeq/Particles' ]
#  tuple.Inputs = [ selSeq.outputLocation() ]

tuple.ToolList +=  [
#    "TupleToolMCTruth",
    "TupleToolMCBackgroundInfo",
    "TupleToolGeometry"
    , "TupleToolKinematic"
    , "TupleToolPropertime"
    , "TupleToolPrimaries"
    , "TupleToolPid"
    , "TupleToolTrackInfo"
    , "TupleToolTISTOS"
    , "TupleToolTrigger"  
#    , "TupleToolTagging"
    , "TupleToolRecoStats"
#    , "TupleToolTrackIsolation"
 ]

if IsMC:
    tuple.ToolList += [ "TupleToolMCTruth", "TupleToolTagging", "TupleToolTrackIsolation"]

# Trigger Lines interested in
tlist = ["L0HadronDecision","L0MuonDecision","L0DiMuonDecision","L0ElectronDecision","L0PhotonDecision","Hlt1SingleHadronDecision","Hlt1MBNoBiasDecision","Hlt1DiHadronDecision","Hlt1L0AnyDecision","Hlt1TrackAllL0Decision","Hlt2TopoOSTF4BodyDecision","Hlt2IncPhiDecision","Hlt2Topo4BodySimpleDecision","Hlt2Topo3BodySimpleDecision","Hlt2Topo3BodyBBDTDecision","Hlt2Topo2BodySimpleDecision","Hlt2Topo2BodyBBDTDecision","Hlt2Topo4BodyBBDTDecision","Hlt2TopoMu4BodyBBDTDecision","Hlt2IncPhiSidebandsDecision","Hlt2B2HHDecision"]

# Get trigger info
tuple.addTool(TupleToolTrigger, name="TupleToolTrigger")
tuple.TupleToolTrigger.Verbose = True
tuple.TupleToolTrigger.TriggerList = tlist

# Get TISTOS info
tuple.addTool(TupleToolTISTOS, name="TupleToolTISTOS")
tuple.TupleToolTISTOS.Verbose = True
tuple.TupleToolTISTOS.TriggerList = tlist

if IsMC:
    tuple.addTool(TupleToolTrackIsolation, name="TupleToolTrackIsolation")
    tuple.TupleToolTrackIsolation.Verbose = True
    tuple.ToolList+=["TupleToolTrackIsolation/TupleToolTrackIsolation"]

tuple.Decay = "[B0 -> ^(phi(1020) -> ^K+ ^K-) ^(rho(770)0 -> ^pi+ ^pi-)]CC"
if myDecayType == 1:
  tuple.Decay = "[B_s0 -> ^(phi(1020) -> ^K+ ^K-) ^(rho(770)0 -> ^pi+ ^pi-)]CC"
elif myDecayType ==2 :
  tuple.Decay = "[B_s0 -> ^(phi(1020) -> ^K+ ^K-) ^(f_0(980) -> ^pi+ ^pi-)]CC"
elif myDecayType == 3:
  tuple.Decay = "[B_s0 -> ^(phi(1020) -> ^K+ ^K-) ^(phi(1020) -> ^K+ ^K-)]CC"

if myDecayType == 1:
    tuple.addBranches({
       "Bs" : "^([B_s0 -> (phi(1020) -> K+ K-) (rho(770)0 -> pi+ pi-)]CC)",
       "f0" : " [B_s0 -> (phi(1020) -> K+ K-) ^(rho(770)0 -> pi+ pi-)]CC "
    })
elif myDecayType == 2:
    tuple.addBranches({
       "Bs" : "^([B_s0 -> (phi(1020) -> K+ K-) (f_0(980) -> pi+ pi-)]CC)",
       "f0" : " [B_s0 -> (phi(1020) -> K+ K-) ^(f_0(980) -> pi+ pi-)]CC "
    })
elif myDecayType == 3:
    tuple.addBranches({
       "Bs" : "^([B_s0 -> (phi(1020) -> K+ K-) (phi(1020) -> K+ K-)]CC)",
       "f0" : " [B_s0 -> (phi(1020) -> K+ K-) ^(phi(1020) -> K+ K-)]CC ",
       "piplus" : " [B_s0 -> (phi(1020) -> K+ K-) (phi(1020) -> ^K+ K-)]CC ",
       "piminus" : " [B_s0 -> (phi(1020) -> K+ K-) (phi(1020) -> K+ ^K-)]CC "
    })
else:
    tuple.addBranches({
       "Bs" : "^([B0 -> (phi(1020) -> K+ K-) (rho(770)0 -> pi+ pi-)]CC)"
       ,"f0" : " [B0 -> (phi(1020) -> K+ K-) ^(rho(770)0 -> pi+ pi-)]CC "
    })

#if IsMC:
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

    "PX_pion1": "DTF_FUN(CHILD(PX, 2,1), True)",
    "PY_pion1": "DTF_FUN(CHILD(PY, 2,1), True)",
    "PZ_pion1": "DTF_FUN(CHILD(PZ, 2,1), True)",

    "PX_pion2": "DTF_FUN(CHILD(PX, 2,2), True)",
    "PY_pion2": "DTF_FUN(CHILD(PY, 2,2), True)",
    "PZ_pion2": "DTF_FUN(CHILD(PZ, 2,2), True)",

    "BCON_LOKI_Mass" : "DTF_FUN(M, True, strings(['B_s0']))",
    "BCON_LOKI_Chi2" : "DTF_CHI2(True, strings(['B_s0']))",
    "BCON_LOKI_ndof" : "DTF_NDOF(True, strings(['B_s0']))",
    "BCON_PX_kaon1": "DTF_FUN(CHILD(PX, 1,1), True, strings(['B_s0']))",
    "BCON_PY_kaon1": "DTF_FUN(CHILD(PY, 1,1), True, strings(['B_s0']))",
    "BCON_PZ_kaon1": "DTF_FUN(CHILD(PZ, 1,1), True, strings(['B_s0']))",

    "BCON_PX_kaon2": "DTF_FUN(CHILD(PX, 1,2), True, strings(['B_s0']))",
    "BCON_PY_kaon2": "DTF_FUN(CHILD(PY, 1,2), True, strings(['B_s0']))",
    "BCON_PZ_kaon2": "DTF_FUN(CHILD(PZ, 1,2), True, strings(['B_s0']))",

    "BCON_PX_pion1": "DTF_FUN(CHILD(PX, 2,1), True, strings(['B_s0']))",
    "BCON_PY_pion1": "DTF_FUN(CHILD(PY, 2,1), True, strings(['B_s0']))",
    "BCON_PZ_pion1": "DTF_FUN(CHILD(PZ, 2,1), True, strings(['B_s0']))",

    "BCON_PX_pion2": "DTF_FUN(CHILD(PX, 2,2), True, strings(['B_s0']))",
    "BCON_PY_pion2": "DTF_FUN(CHILD(PY, 2,2), True, strings(['B_s0']))",
    "BCON_PZ_pion2": "DTF_FUN(CHILD(PZ, 2,2), True, strings(['B_s0']))"
    }

tuple.addTool(LoKiVariables2 , name = 'LoKiVariables2' )
tuple.ToolList   += [ 'LoKi::Hybrid::TupleTool/LoKiVariables2']
#LoKi_B=tuple.B_0.addTupleTool("LoKi::Hybrid::TupleTool/LoKi_B")
#LoKi_B.Variables =  {
#       "PID" : "ID"
#    , "BPVDIRA" : "BPVDIRA"
#    , "MassDiff_Bs0" : "DMASS('B_s0')"
#}

#######################################################
from Configurables import CondDB, CondDBAccessSvc
CondDB().LatestGlobalTagByDataType = DataYear 
#CondDB(UseOracle = True, IgnoreHeartBeat = True)

############################################
#from Configurables import *
from Configurables import LHCbApp
LHCbApp().XMLSummary='summary.xml'

from Configurables import DataOnDemandSvc
from Configurables import L0SelReportsMaker, L0DecReportsMaker
DataOnDemandSvc().AlgMap["HltLikeL0/DecReports"] = L0DecReportsMaker( OutputLevel = 4 )
DataOnDemandSvc().AlgMap["HltLikeL0/SelReports"] = L0SelReportsMaker( OutputLevel = 4 )

if myDecayType and IsMC:
    BsPhiRho_Sequence = GaudiSequencer("BsPhiRho_Sequence")
    SeqBsPhiRho = SelectionSequence("SeqBsPhiRho", TopSelection = selSeq)
    BsPhiRho_Sequence.Members += [SeqBsPhiRho.sequence()]
    BsPhiRho_Sequence.Members += [smear]
    BsPhiRho_Sequence.Members += [tuple]
    userAlgos.append(BsPhiRho_Sequence)
elif not myDecayType and IsMC:
    userAlgos.append( smear )
    userAlgos.append( tuple )
elif not IsMC:
    MySequencer.Members += [scaler]
    MySequencer.Members += [tuple]
    userAlgos.append(MySequencer)

from Configurables import EventTuple
etuple = EventTuple()
userAlgos.append( HltSelReportsDecoder() )
userAlgos.append( HltVertexReportsDecoder() )
userAlgos.append( HltDecReportsDecoder() )
userAlgos.append( etuple )
#etuple.ToolList = [ "TupleToolEventInfo "]

myTupleName = 'Bsphirho.root'
'''
myTuple = ['Bdphirho', 'Bsphirho', 'Bsphif0', 'Bsphiphi']
myTupleName = myTuple[myDecayType] + '.root'
if not IsMC: myTupleName = "RE" + myTuple[1] + '.root'
# Necessary DaVinci parameters #################

if ( DataYear == "2012" and not IsMC):
    DDDBtag = 'dddb-20120831'
    CondDBtag = 'cond-20120831'
elif ( DataYear == "2011" and not IsMC):
    DDDBtag = 'dddb-20130111'
    CondDBtag = 'cond-20130114'
if IsMC:
    DDDBtag = "dddb-20120831"
    CondDBtag = "sim-20121025-vc-md100"
'''
dv = DaVinci( 
#    DDDBtag  = DDDBtag,
#    CondDBtag = CondDBtag,
    HistogramFile = 'dummy.root',
    TupleFile = myTupleName,
    UserAlgorithms = userAlgos,# if Line == None else [stripSeq],
    DataType  = DataYear,
    EvtMax = -1,
    InputType = 'DST' if IsMC else 'MDST',
    PrintFreq  = 1000,
    Simulation = IsMC,
    Lumi = False if IsMC else True )
'''
if not IsMC:
    dv.DDDBtag  = DDDBtag
    dv.CondDBtag = CondDBtag
'''
#dv.UserAlgorithms = userAlgos #BsPhiRho_Sequence]# scaler , tuple ]
##########################################################
#importOptions("/afs/cern.ch/user/h/hluo/cmtuser/MC2011/Bsphirho/MC201213104051Beam4000GeV-2012-MagDown-Nu25-Pythia8Sim08aDigi13Trig0x409f0045Reco14Stripping20NoPrescalingFlaggedSTREAMSDST.py")
#importOptions("/afs/cern.ch/user/h/hluo/cmtuser/MC2011/Bsphirho/Bsphirho_PFN.py")
##########################################################
