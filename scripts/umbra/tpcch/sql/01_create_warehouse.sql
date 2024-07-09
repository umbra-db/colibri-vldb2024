create table warehouse
(
    w_id       integer not null,
    w_name     varchar(10),
    w_street_1 varchar(20),
    w_street_2 varchar(20),
    w_city     varchar(20),
    w_state    char(2),
    w_zip      char(9),
    w_tax      decimal(4, 4),
    w_ytd      decimal(12, 2),
    primary key (w_id)
) partition by hash (w_id) with (%%relationConfig%%);
