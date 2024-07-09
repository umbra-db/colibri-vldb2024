create table customer
(
    c_custkey    bigint         not null,
    c_name       varchar(25)    not null,
    c_address    varchar(40)    not null,
    c_nationkey  integer        not null,
    c_phone      char(15)       not null,
    c_acctbal    decimal(12, 2) not null,
    c_mktsegment char(10)       not null,
    c_comment    varchar(117)   not null,
    primary key (c_custkey)
);
alter table customer set tiflash replica 1;
