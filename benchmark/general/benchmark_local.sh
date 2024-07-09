#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
set -euo pipefail

HOST_UID=$(id -u "${USER}")
HOST_GID=$(id -g "${USER}")

TRANSACTIONAL_CLIENTS=${1}
ANALYTICAL_CLIENTS=${2}
BENCHMARK=${3}
BENCHMARK_DIR=${4}
SCALE_FACTOR=${5}
SYSTEM=${6}
SYSTEM_NAME=${7}
CONFIG_NAME=${8:-${SYSTEM_NAME}}

WORKING_DIR=$(pwd)
SCRIPT_DIR="${WORKING_DIR}/benchmark/${CONFIG_NAME}"

# Find the default config file
CONFIG_FILE=config.json
BENCHMARK_LOWERCASE="${BENCHMARK,,}"
if [[ -e "${SCRIPT_DIR}/${BENCHMARK_LOWERCASE}${SCALE_FACTOR}-config.json" ]]; then
  CONFIG_FILE="${BENCHMARK_LOWERCASE}${SCALE_FACTOR}-config.json"
elif [[ -e "${SCRIPT_DIR}/${BENCHMARK_LOWERCASE}-config.json" ]]; then
  CONFIG_FILE="${BENCHMARK_LOWERCASE}-config.json"
fi

BIN_DIR=${BIN_DIR:-${WORKING_DIR}/bin}

# Compile txbench
mkdir -p ${BIN_DIR}
(
  cd ${BIN_DIR}
  cmake -DCMAKE_BUILD_TYPE=Release ${WORKING_DIR}
  make -j$(nproc)
  ln -sf tools/txbench/txbench
)

TRACE=${TRACE:-0}
export TRACE

$BIN_DIR/txbench $SYSTEM $SYSTEM_NAME $BENCHMARK $WORKING_DIR "${SCRIPT_DIR}/${CONFIG_FILE}" $TRANSACTIONAL_CLIENTS $ANALYTICAL_CLIENTS $SCALE_FACTOR

