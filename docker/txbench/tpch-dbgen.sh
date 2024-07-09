#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------

DATA_PATH=${1}
SCALE_FACTOR=${2:-1}
UPDATES=${3:-4000}

# Generate initial data population
mkdir "${DATA_PATH}/temp"
DSS_PATH=${DATA_PATH}/temp tpch-dbgen -vf -s "${SCALE_FACTOR}"
(
  pushd "${DATA_PATH}/temp" || exit 1
  for table in ./*.tbl; do
    chmod 600 "$table"
    sed 's/|$//' "$table" >"../$table"
    rm "$table"
  done
  popd || exit 1
)

# Generate updates (a full cycle consists of 4000 update/delete pairs but our benchmarks do not run that long)
DSS_PATH=${DATA_PATH}/temp tpch-dbgen -vf -s "${SCALE_FACTOR}" -U "${UPDATES}"
(
  pushd "${DATA_PATH}/temp" || exit 1
  for table in ./delete.*; do
    sed 's/|$//' "$table" >"../delete/$table"
    rm "$table"
  done
  for table in ./*.tbl.*; do
    sed 's/|$//' "$table" >"../update/$table"
    rm "$table"
  done
  popd || exit 1
)

rm -rf "${DATA_PATH}/temp"
