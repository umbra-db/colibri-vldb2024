#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
set -euo pipefail

HOST_UID=$(id -u "${USER}")
HOST_GID=$(id -g "${USER}")

WORKING_DIR=$(pwd)
SCRIPT_DIR=$(dirname -- "${0}")

SCALE_FACTOR=${1:-10}
HOST_DATA_DIR=${HOST_DATA_DIR:-${WORKING_DIR}/data/hattrick/sf${SCALE_FACTOR}}

if [ ! -d "${HOST_DATA_DIR}" ]; then
  # Create the data directory
  mkdir -p "${HOST_DATA_DIR}"

  # Prepare the environment read by the docker-compose file
  export HOST_UID
  export HOST_GID
  export HOST_DATA_DIR

  # Generate data
  docker compose -f "${SCRIPT_DIR}/docker-compose.yml" --project-directory "${WORKING_DIR}" run --rm txbench txbench-datagen HATTRICK "${SCALE_FACTOR}" /data
  docker compose -f "${SCRIPT_DIR}/docker-compose.yml" --project-directory "${WORKING_DIR}" down --remove-orphans
fi
