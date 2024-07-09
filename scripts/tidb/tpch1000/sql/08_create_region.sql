create table region
(
    r_regionkey integer      not null,
    r_name      char(25)     not null,
    r_comment   varchar(152) not null,
    primary key (r_regionkey)
);
alter table region set tiflash replica 1;
