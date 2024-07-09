#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
set -euo pipefail

variant=$1

cores="1 2 4 8 16 24 32 48 64"

export USEDIRECTIO=1
export BUFFERSIZE=384G
export RELATION_COLUMN_SCANPOLICY=asynchronous

# ----------------------------
# TPCC experiments
# ----------------------------
for i in ${cores}; do
  "./benchmark/umbra_cloud/umbra_${variant}.sh" $i 0 TPCC tpcc 100
done
