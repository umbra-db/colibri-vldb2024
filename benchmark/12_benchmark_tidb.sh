#!/usr/bin/env bash
#---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
#---------------------------------------------------------------------------
set -euo pipefail

cores="1 2 4 8 16 24 32 40 48 56 64"
cores0="0 1 2 4 8 16 32 64"
cores100="0 1 2 4 8 16 32 64"
cores1000="0 1 2 4 8"

# ----------------------------
# TPCH experiments
# ----------------------------
#for i in ${cores0}; do
#  for j in ${cores0}; do
#    if [[ $i -ne 0 || $j -ne 0 ]]; then
#      ./benchmark/tidb/tidb.sh $i $j TPCH tpch 10
#    fi
#  done
#done
#for i in ${cores100}; do
#  for j in ${cores100}; do
#    if [[ $i -ne 0 || $j -ne 0 ]]; then
#      ./benchmark/tidb/tidb.sh $i $j TPCH tpch 100
#    fi
#  done
#done
#for i in ${cores1000}; do
#  for j in ${cores1000}; do
#    if [[ $i -ne 0 || $j -ne 0 ]]; then
#      ./benchmark/tidb/tidb.sh $i $j TPCH1000 tpch 1000
#    fi
#  done
#done


# ----------------------------
# HATTRICK experiments
# ----------------------------
#for i in ${cores0}; do
#  for j in ${cores0}; do
#    if [[ $i -ne 0 || $j -ne 0 ]]; then
#      ./benchmark/tidb/tidb.sh $i $j HATTRICK hattrick 10
#    fi
#  done
#done
#for i in ${cores100}; do
#  for j in ${cores100}; do
#    if [[ $i -ne 0 || $j -ne 0 ]]; then
#      ./benchmark/tidb/tidb.sh $i $j HATTRICK hattrick 100
#    fi
#  done
#done
for i in ${cores100}; do
  if [[ $i -ne 0 ]]; then
    ./benchmark/tidb/tidb.sh $i 0 HATTRICK hattrick 100
  fi
done
for i in ${cores100}; do
  if [[ $i -ne 0 ]]; then
    ./benchmark/tidb/tidb.sh 0 $i HATTRICK hattrick 100
  fi
done
