create table partsupp
(
    ps_partkey    bigint         not null,
    ps_suppkey    bigint         not null,
    ps_availqty   integer        not null,
    ps_supplycost decimal(12, 2) not null,
    ps_comment    varchar(199)   not null,
    primary key (ps_partkey, ps_suppkey)
) with (%%relationConfig%%);
