#!/bin/bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

# Create the local user
groupadd --gid ${HOST_GID} local
useradd --uid ${HOST_UID} --gid ${HOST_GID} local

# Switch to local user
gosu local "$@"
