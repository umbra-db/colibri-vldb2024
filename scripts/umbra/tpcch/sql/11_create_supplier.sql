create table supplier
(
    su_suppkey   integer        not null,
    su_name      char(25)       not null,
    su_address   varchar(40)    not null,
    su_nationkey integer        not null,
    su_phone     char(15)       not null,
    su_acctbal   decimal(12, 2) not null,
    su_comment   char(101)      not null,
    primary key (su_suppkey)
) with (%%relationConfig%%);
