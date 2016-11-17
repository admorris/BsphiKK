#! /bin/bash
source eos.sh
source cuts.sh
cd ${nTuples_dir}/
cutapplier BsphiKK_data_nocut.root DecayTreeTuple/DecayTree ${sidebandcut} BsphiKK_sideband_nocut.root
cutapplier BsphiKK_data_loosePID.root DecayTreeTuple/DecayTree ${sidebandcut} BsphiKK_sideband_loosePID.root
exit 0
