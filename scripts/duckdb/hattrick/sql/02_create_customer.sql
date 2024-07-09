create table customer
(
    c_custkey    int         not null primary key,
    c_name       varchar(25) not null,
    c_address    varchar(25),
    c_city       char(10),
    c_nation     varchar(15),
    c_region     varchar(12),
    c_phone      char(15),
    c_mktsegment varchar(10),
    c_paymentcnt integer
);
create index c_name_idx on hattrick.customer (c_name);
