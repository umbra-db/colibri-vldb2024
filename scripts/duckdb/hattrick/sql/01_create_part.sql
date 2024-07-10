create table part
(
    p_partkey   int not null primary key,
    p_name      varchar(22),
    p_mfgr      char(6),
    p_category  char(7),
    p_brand1    char(9),
    p_color     varchar(11),
    p_type      varchar(25),
    p_size      int,
    p_container varchar(10),
    p_price     decimal
);
