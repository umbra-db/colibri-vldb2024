create table supplier
(
    s_suppkey   bigint         not null,
    s_name      char(25)       not null,
    s_address   varchar(40)    not null,
    s_nationkey integer        not null,
    s_phone     char(15)       not null,
    s_acctbal   decimal(12, 2) not null,
    s_comment   varchar(101)   not null,
    primary key (s_suppkey)
);
alter table supplier set tiflash replica 1;
