# Colibri Benchmark Suite

Copyright (c) 2024 TUM. All rights reserved.

## Dependencies

The actual benchmark code is encapsulated within multiple Docker containers that are orchestrated using Docker Compose.
We host pre-built Docker images for convenience, i.e. there is no need to manually build Docker containers. The `docker`
subdirectory contains the build scripts used to generate the Docker images. To run the benchmarks, the
following dependencies are required.

- bash
- Docker
- Docker Compose

## Data generation

Before running any benchmarks, the respective data sets have to be generated once. This can be achieved as follows,
assuming that the current working directory contains this README file.

`./benchmark/00_datagen.sh`

The data will be generated into `data` under the root folder.
After successful data generation, this directory should contain the following subdirectories:

```
data
├─── tatp
|    └─── ...               Multiple .csv files
├─── tpcc
|    └─── ...               Multiple .csv files
└─── tpch
     ├─── delete
     |    └─── ...          Multiple delete.N files
     ├─── update
     |    └─── ...          Multiple lineitem.tbl.uN and orders.tbl.uN files
     └─── ...               Multiple .tbl files
```

## Running benchmarks

Login to the container registry using: `docker login gitlab.db.in.tum.de:5005 -u test -p glpat-sgXycvrv8ctxUMko7NAJ`

Once data has been generated, the benchmarks can be run as follows. Once again, the listed commands assume that the
working directory contains this README file.

1. `./benchmark/03_benchmark_postgres.sh`
2. `./benchmark/05_benchmark_umbra.sh`
3. `./benchmark/06_benchmark_umbra_column.sh`
4. `./benchmark/07_benchmark_umbra_async.sh`
5. `./benchmark/11_benchmark_umbra_aws_tpcc.sh`

All files created by the database will be stored in `db` under the root folder.
It can be linked to an SSD RAID, an EBS device, or any other storage device.

Benchmark results will be placed into the directories `output/postgres`, `output/umbra`, or `output/singlestore` for the
respective system.

SingleStore requires a license key: update the `benchmark/singlestore/docker-compose.yml` file.
For running Umbra with Amazon S3 update the `benchmark/umbra_cloud/config.json` file with the S3 credentials and bucket name.

## Plots

The scripts under `plots` will generate the plots from the paper, the results are under `results`.
