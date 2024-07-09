create table lineorder
(
    lo_orderkey      integer not null,
    lo_linenumber    integer not null,
    lo_custkey       int     not null,
    lo_partkey       int     not null,
    lo_suppkey       int     not null,
    lo_orderdate     int     not null,
    lo_ordpriority   char(15),
    lo_shippriority  char(1),
    lo_quantity      integer,
    lo_extendedprice decimal,
    lo_discount      integer,
    lo_revenue       decimal,
    lo_supplycost    decimal,
    lo_tax           integer,
    lo_commitdate    int     not null,
    lo_shipmode      char(10),
    primary key (lo_orderkey, lo_linenumber),
    index (lo_custkey) using btree
);
