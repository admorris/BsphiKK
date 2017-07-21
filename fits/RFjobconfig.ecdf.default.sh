#!/bin/bash
# RapidFit location
export ParallelEnv="#$ -pe sharedmem 4"
# Load ROOT and TexLive
export SetupEnvironment=". /etc/profile.d/modules.sh\nmodule load root\nmodule load igmm/apps/texlive"

