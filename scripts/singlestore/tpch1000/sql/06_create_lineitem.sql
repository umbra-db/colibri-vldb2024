create table lineitem
(
    l_orderkey      bigint         not null,
    l_partkey       bigint         not null,
    l_suppkey       bigint         not null,
    l_linenumber    integer        not null,
    l_quantity      decimal(12, 2) not null,
    l_extendedprice decimal(12, 2) not null,
    l_discount      decimal(12, 2) not null,
    l_tax           decimal(12, 2) not null,
    l_returnflag    char(1)        not null,
    l_linestatus    char(1)        not null,
    l_shipdate      date           not null,
    l_commitdate    date           not null,
    l_receiptdate   date           not null,
    l_shipinstruct  char(25)       not null,
    l_shipmode      char(10)       not null,
    l_comment       varchar(44)    not null,
    primary key (l_orderkey, l_linenumber)
);
