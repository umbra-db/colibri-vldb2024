#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
set -euo pipefail

BENCHMARK_SCRIPT="$(dirname -- "${0}")/../general/benchmark.sh"

TRANSACTIONAL_CLIENTS=${1}
ANALYTICAL_CLIENTS=${2}
BENCHMARK=${3}
BENCHMARK_DIR=${4}
SCALE_FACTOR=${5}

$BENCHMARK_SCRIPT $TRANSACTIONAL_CLIENTS $ANALYTICAL_CLIENTS $BENCHMARK $BENCHMARK_DIR $SCALE_FACTOR Umbra umbra
