create table history
(
    h_orderkey integer not null,
    h_custkey  integer not null,
    h_amount   decimal not null,
    primary key (h_orderkey, h_custkey, h_amount)
);
alter table history set tiflash replica 1;
