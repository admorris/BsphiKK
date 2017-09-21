#!/bin/bash
export ParallelEnv="#$ -pe sharedmem 4"
export SetupEnvironment=". /etc/profile.d/modules.sh\nmodule load root\nmodule load igmm/apps/texlive"

