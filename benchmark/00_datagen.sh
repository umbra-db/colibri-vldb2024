#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
set -euo pipefail

./benchmark/datagen/datagen-tatp.sh
./benchmark/datagen/datagen-tpcc.sh
./benchmark/datagen/datagen-tpcch.sh
./benchmark/datagen/datagen-tpch.sh
./benchmark/datagen/datagen-tpch.sh 100 1000
./benchmark/datagen/datagen-tpch.sh 1000 500
./benchmark/datagen/datagen-hattrick.sh
./benchmark/datagen/datagen-hattrick.sh 100
./benchmark/datagen/datagen-hybench.sh 10
./benchmark/datagen/datagen-hybench.sh 100
