#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
set -euo pipefail

HOST_UID=$(id -u "${USER}")
HOST_GID=$(id -g "${USER}")

BENCHMARK_DIR=${1}
SCALE_FACTOR=${2}
SYSTEM_NAME=${3}

WORKING_DIR=$(pwd)
SCRIPT_DIR="${WORKING_DIR}/benchmark/${SYSTEM_NAME}"

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
ln -s "${SCRIPT_DIR}" "${WORKING_DIR}/txbench/config"

# Prepare the environment read by the docker-compose file
export HOST_UID
export HOST_GID
export WORKING_DIR

TRACE=${TRACE:-0}
export TRACE

# Run the benchmark
docker compose -f "${SCRIPT_DIR}/docker-compose.yml" --project-directory $WORKING_DIR run --service-ports --rm "txbench-${SYSTEM_NAME}" || true

# Stop the container
while true; do
  if docker compose -f "${SCRIPT_DIR}/docker-compose.yml" --project-directory $WORKING_DIR down --remove-orphans; then
    break
  else
    echo "Retrying docker-compose down failed, retrying..."
    sleep 5
  fi
done

# Clear the database directory
rm -rf "${DB_DIR}"
rm -rf "${WORKING_DIR}/txbench"
