#!/bin/bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

# increase the memlock limit for io uring
ulimit -l 8388608

# Create the local user
groupadd --gid ${HOST_GID} local
useradd --uid ${HOST_UID} --gid ${HOST_GID} local

# Switch to local user
gosu local "$@"
