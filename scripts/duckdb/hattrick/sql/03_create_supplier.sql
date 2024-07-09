create table supplier
(
    s_suppkey integer     not null primary key,
    s_name    varchar(25) not null,
    s_address varchar(25),
    s_city    char(10),
    s_nation  varchar(15),
    s_region  char(12),
    s_phone   char(15),
    s_ytd     decimal
);
create index s_name_idx on hattrick.supplier (s_name);
