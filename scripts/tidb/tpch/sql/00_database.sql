DROP DATABASE IF EXISTS tpch;
CREATE DATABASE tpch;
set global tidb_mem_quota_query = 128 << 30;
set global tidb_gc_life_time = "24h";