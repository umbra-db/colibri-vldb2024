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

WORKING_DIR=$(pwd)
SCRIPT_DIR="${WORKING_DIR}/benchmark/${SYSTEM_NAME}"
CONFIG_DIR=${CONFIG_DIR:-${SCRIPT_DIR}}

# Find the default config file
DEFAULT_CONFIG_FILE=config.json
BENCHMARK_LOWERCASE="${BENCHMARK,,}"
if [[ -e "${CONFIG_DIR}/${BENCHMARK_LOWERCASE}${SCALE_FACTOR}-config.json" ]]; then
  DEFAULT_CONFIG_FILE="${BENCHMARK_LOWERCASE}${SCALE_FACTOR}-config.json"
elif [[ -e "${CONFIG_DIR}/${BENCHMARK_LOWERCASE}-config.json" ]]; then
  DEFAULT_CONFIG_FILE="${BENCHMARK_LOWERCASE}-config.json"
fi

CONFIG_FILE=${8:-${DEFAULT_CONFIG_FILE}}

DB_DIR=${DB_DIR:-${WORKING_DIR}/db/${SYSTEM_NAME}}
DATA_DIR=${DATA_DIR:-${WORKING_DIR}/data/${BENCHMARK_DIR}/sf${SCALE_FACTOR}}

OUTPUT_DIR=${OUTPUT_DIR:-${WORKING_DIR}/output}

# Clear the database directory
rm -rf "${DB_DIR}"
mkdir -p "${DB_DIR}"

# Create the output directory
mkdir -p "${OUTPUT_DIR}"

rm -rf "${WORKING_DIR}/txbench"
mkdir -p "${WORKING_DIR}/txbench"

ln -s "${DB_DIR}" "${WORKING_DIR}/txbench/db"
ln -s "${DATA_DIR}" "${WORKING_DIR}/txbench/data"
ln -s "${OUTPUT_DIR}" "${WORKING_DIR}/txbench/output"
ln -s "${CONFIG_DIR}" "${WORKING_DIR}/txbench/config"
ln -s "${WORKING_DIR}/scripts" "${WORKING_DIR}/txbench/scripts"

# Prepare the environment read by the docker-compose file
export HOST_UID
export HOST_GID
export WORKING_DIR

TRACE=${TRACE:-0}
export TRACE

# Run the benchmark
docker compose -f "${CONFIG_DIR}/docker-compose.yml" --project-directory $WORKING_DIR run --rm txbench txbench $SYSTEM $SYSTEM_NAME $BENCHMARK /txbench "/txbench/config/${CONFIG_FILE}" $TRANSACTIONAL_CLIENTS $ANALYTICAL_CLIENTS $SCALE_FACTOR

# Stop the container
while true; do
  if docker compose -f "${CONFIG_DIR}/docker-compose.yml" --project-directory $WORKING_DIR down --remove-orphans; then
    break
  else
    echo "Retrying docker-compose down failed, retrying..."
    sleep 5
  fi
done

# Clear the database directory
rm -rf "${DB_DIR}"
rm -rf "${WORKING_DIR}/txbench"
