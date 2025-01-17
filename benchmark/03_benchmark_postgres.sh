#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
set -euo pipefail

cores="1 2 4 8 16 24 32 40 48 56 64"
cores0="0 1 2 4 8 16 32 64"
cores100="1 2 4 8 16 32 64"
cores1000="1 2"

# ----------------------------
# TATP experiments
# ----------------------------
for i in ${cores}; do
  ./benchmark/postgres/postgres.sh $i 0 TATP tatp 10000000
done

# ----------------------------
# TPCC experiments
# ----------------------------
for i in ${cores}; do
  ./benchmark/postgres/postgres.sh $i 0 TPCC tpcc 100
done

# ----------------------------
# TPCCH experiments
# ----------------------------
for i in ${cores0}; do
  for j in ${cores0}; do
    if [[ $i -ne 0 || $j -ne 0 ]]; then
      ./benchmark/postgres/postgres.sh $i $j TPCCH tpcch 100
    fi
  done
done

# ----------------------------
# TPCH experiments
# ----------------------------
for i in ${cores0}; do
  for j in ${cores0}; do
    if [[ $i -ne 0 || $j -ne 0 ]]; then
      ./benchmark/postgres/postgres.sh $i $j TPCH tpch 10
    fi
  done
done
for i in ${cores100}; do
  ./benchmark/postgres/postgres.sh $i 0 TPCH tpch 100
done
for i in ${cores100}; do
  ./benchmark/postgres/postgres.sh 0 $i TPCH tpch 100
done
for i in ${cores}; do
  ./benchmark/postgres/postgres.sh $i 0 TPCH1000 tpch 1000
done
for i in ${cores}; do
  ./benchmark/postgres/postgres.sh 0 $i TPCH1000 tpch 1000
done

# ----------------------------
# HATTRICK experiments
# ----------------------------
for i in ${cores0}; do
  for j in ${cores0}; do
    if [[ $i -ne 0 || $j -ne 0 ]]; then
      ./benchmark/postgres/postgres.sh $i $j HATTRICK hattrick 10
    fi
  done
done
for i in ${cores100}; do
  ./benchmark/postgres/postgres.sh $i 0 HATTRICK hattrick 100
done
for i in ${cores100}; do
  ./benchmark/postgres/postgres.sh 0 $i HATTRICK hattrick 100
done
