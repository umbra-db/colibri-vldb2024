#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

export AWS_ACCESS_KEY_ID=${MINIO_ROOT_USER}
export AWS_SECRET_ACCESS_KEY=${MINIO_ROOT_PASSWORD}
export AWS_EC2_METADATA_DISABLED=true

mkdir /db/minio

# Start the minio server
minio server /db/minio -console-address ":9001" &> /output/minio.log &

# Create the bucket
aws --endpoint-url http://127.0.0.1:9000/ s3 mb s3://umbra

tail -f /output/minio.log
