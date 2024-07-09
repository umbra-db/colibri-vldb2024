#!/usr/bin/python3
# ---------------------------------------------------------------------------
# Copyright (c) 2022 TUM. All rights reserved.
# ---------------------------------------------------------------------------

import subprocess
import os

cores = 8
cores0 = 4
cores100 = 2
cores1000 = 2

benchmarks = {
    "tatp": f"{cores} 0 TATP tatp 10000000",
    "tpcc": f"{cores} 0 TPCC tpcc 100",
    "tpcch": f"{cores0} {cores0} TPCCH tpcch 100",
    "tpch10": f"{cores0} {cores0} TPCH tpch 10",
    "tpch100": f"{cores100} {cores100} TPCH tpch 100",
    "tpch1000": f"{cores1000} {cores1000} TPCH1000 tpch 1000",
    "hattrick10": f"{cores0} {cores0} HATTRICK hattrick 10",
    "hattrick100": f"{cores100} {cores100} HATTRICK hattrick 100",
    "hybench10": f"{cores0} {cores0} HYBENCH hybench 10",
    "hybench100": f"{cores100} {cores100} HYBENCH hybench 100",
}

systems = {
    "0": ("All", None, None),
    "4": ("Postgres", "./benchmark/postgres/postgres.sh",
          ["tatp", "tpcc", "tpcch", "tpch10", "tpch100", "tpch1000", "hattrick10", "hattrick100", "hybench10", "hybench100"]),
    "5": ("Umbra", "./benchmark/umbra/umbra.sh",
          ["tatp", "tpcc", "tpcch", "tpch10", "tpch100", "tpch1000", "hattrick10", "hattrick100", "hybench10", "hybench100"]),
    "6": ("Umbra Column", "./benchmark/umbra_column/umbra_column.sh",
          ["tatp", "tpcc", "tpcch", "tpch10", "tpch100", "tpch1000", "hattrick10", "hattrick100", "hybench10", "hybench100"]),
    "7": ("Umbra Async", "./benchmark/umbra_async/umbra_async.sh",
          ["tatp", "tpcc", "tpcch", "tpch10", "tpch100", "tpch1000", "hattrick10", "hattrick100", "hybench10", "hybench100"]),
    "8": ("SingleStore", "./benchmark/singlestore/singlestore.sh",
          ["tpch10", "tpch100", "tpch1000", "hattrick10", "hattrick100"]),
    "11": ("TiDB", "./benchmark/tidb/tidb.sh",
           ["tpch10", "tpch100", "tpch1000", "hattrick10", "hattrick100"]),
    "12": ("DuckDB", "./benchmark/duckdb/duckdb.sh",
           ["tpch10", "tpch100", "tpch1000", "hattrick10", "hattrick100"]),
    "13": ("AlloyDB", "./benchmark/alloydb/alloydb.sh",
           ["tatp", "tpcc", "tpcch", "tpch10", "tpch100", "tpch1000", "hattrick10", "hattrick100", "hybench10", "hybench100"]),
}


def exec(command, quiet=False):
    if not quiet:
        print(command)
    subprocess.run(command, shell=True, check=True)


def execute_benchmark(system):
    name, command, benchs = systems[system.removesuffix(".")]
    if command is not None:
        if "." in system:
            print(f"\nChoose a benchmark for {name}:")
            indexes = {}
            for idx, benchmark in enumerate(benchs, start=1):
                indexes[idx] = benchmark
                print(f"({idx}) {benchmark}")

            user_input = input("Enter a number: ")
            selected_benchmark = benchmarks[indexes[int(user_input)]]
            exec(command + " " + selected_benchmark)
        else:
            for benchmark in benchs:
                exec(command + " " + benchmarks[benchmark])
    else:
        for k in systems.keys():
            if k is not "0":
                execute_benchmark(k)


def main():
    print("Choose a benchmark to run:")
    for key, (name, _, _) in systems.items():
        print(f"({key}) {name}")

    user_input = input("Enter a number or a comma-separated list (e.g., 1,5,8): ")
    selected_options = user_input.split(',')

    if os.path.exists("output"):
        exec("mv output output.bck", quiet=True)

    exec("mkdir -p output.test", quiet=True)
    exec("mv output.test output", quiet=True)

    for option in selected_options:
        execute_benchmark(option)

    exec("mv output output.test", quiet=True)
    if os.path.exists("output.bck"):
        exec("mv output.bck output", quiet=True)


if __name__ == "__main__":
    main()
