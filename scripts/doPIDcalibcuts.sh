#!/bin/bash
source cuts.sh
source eos.sh
cd ../ntuples
mode=BsphiKK_MC
totalcut="${trigcut}&&${ghstcut}&&${trackisMuoncut}&&${KpTcut}&&${StdTightKaons}&&${KaonIPCHI2cut}&&${phiIPCHI2cut}&&${KKIPCHI2cut}&&${KKEVCHI2cut}&&${BsFDCHI2cut}&&${BsIPCHI2cut}&&${signalBKGCATcut}"
#cutapplier ${nTuples_dir}${mode}_nocut.root DecayTreeTuple/DecayTree "$totalcut" ${mode}_noPIDcuts.root
../bin/ApplyDataDrivenPIDcut ${mode}_noPIDcuts.root ${mode}_PIDcalib_cuts.root ../pidcalib/PerfHists_K_Strip20_MagUp_Laurence_P_ETA.root ../pidcalib/PerfHists_K_Strip20_MagDown_Laurence_P_ETA.root "K_DLLK>0.0 && MC12TuneV2_ProbNNK*(1-MC12TuneV2_ProbNNpi)>0.025_All"

