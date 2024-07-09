#!/bin/bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

# increase the memlock limit for io uring
ulimit -l 8388608

# Create the local user
groupadd --gid ${HOST_GID} local
useradd --uid ${HOST_UID} --gid ${HOST_GID} local

ln -s /db /home/local/.tiup/data
chown -R "${HOST_UID}:${HOST_GID}" /home/local

# Switch to local user
gosu local "$@"
