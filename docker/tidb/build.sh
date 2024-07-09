#!/bin/bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
set -euo pipefail

# Build docker image
docker build --tag=ghcr.io/umbra-db/colibri-vldb2024/tidb:latest --ulimit nofile=2048 .
docker login ghcr.io
docker push ghcr.io/umbra-db/colibri-vldb2024/tidb:latest
