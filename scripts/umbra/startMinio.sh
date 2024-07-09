#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
set -euo pipefail

export MINIO_ROOT_USER=minio
export MINIO_ROOT_PASSWORD=password
export AWS_ACCESS_KEY_ID=${MINIO_ROOT_USER}
export AWS_SECRET_ACCESS_KEY=${MINIO_ROOT_PASSWORD}
export AWS_EC2_METADATA_DISABLED=true

WORKING_DIR=$(pwd)

DB_DIR=${DB_DIR:-${WORKING_DIR}/db/minio}

# Clear the database directory
rm -rf "${DB_DIR}"
mkdir -p "${DB_DIR}"

# Start the minio server
minio server "${DB_DIR}" -console-address ":9001" &> "${DB_DIR}/minio.log" &

# Create the bucket
aws --endpoint-url http://127.0.0.1:9000/ s3 mb s3://umbra

tail -f "${DB_DIR}/minio.log"
