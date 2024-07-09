#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

BENCHMARK_DIR=${1}
SCALE_FACTOR=${2}
OUTPUT_DIR=${3}

WORKING_DIR=$(pwd)

BIN_DIR=${BIN_DIR:-${WORKING_DIR}/umbra_bin}
DB_DIR=${DB_DIR:-${WORKING_DIR}/db/umbra_local}
DATA_DIR=${DATA_DIR:-${WORKING_DIR}/data/${BENCHMARK_DIR}/sf${SCALE_FACTOR}}

# Check if the trace environment variable is equal to 1
if [ -n "$TRACE" ]; then
  BIN_DIR="${BIN_DIR}/trace"
fi

# Clear the database directory
while true; do
  rm -rf "${DB_DIR}"
    
  # Check if the removal was successful
  if [ $? -eq 0 ]; then
    echo "Directory removed successfully."
    break  # Exit the loop if successful
  else
    echo "Error removing directory. Retrying..."
    sleep 10
  fi
done

# Loop until port is free or maximum retries reached
retry_count=0
while [ $retry_count -lt 120 ]; do
    if ! nc -z localhost 5432; then
        echo "Port 5432 is free."
        break
    fi

    retry_count=$((retry_count+1))
    echo "Port 5432 is still in use. Retrying..."
    sleep 1
done

set -euo pipefail
mkdir -p "${DB_DIR}"

# Create the output directory
mkdir -p "${OUTPUT_DIR}"

rm -rf "${WORKING_DIR}/txbench"
mkdir -p "${WORKING_DIR}/txbench"

ln -s "${DB_DIR}" "${WORKING_DIR}/txbench/db"
ln -s "${DATA_DIR}" "${WORKING_DIR}/txbench/data"
ln -s "${OUTPUT_DIR}" "${WORKING_DIR}/txbench/output"

(
  cd "${WORKING_DIR}/txbench"

  export USEDIRECTIO=1

  # Setup a database
  ${BIN_DIR}/sql -createdb db/umbra.db <<<"ALTER ROLE postgres WITH LOGIN SUPERUSER PASSWORD 'postgres';" &> output/umbra.log || exit 1

  # Start the server
  ${BIN_DIR}/server -createSSLFiles -certFile db/umbra.cert -keyFile db/umbra.pem -address 0.0.0.0 db/umbra.db &> output/umbra.log || exit 1
)

# Clear the database directory
rm -rf "${DB_DIR}"
rm -rf "${WORKING_DIR}/txbench"
