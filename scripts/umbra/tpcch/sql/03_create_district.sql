create table district
(
    d_id        integer not null,
    d_w_id      integer not null,
    d_name      varchar(10),
    d_street_1  varchar(20),
    d_street_2  varchar(20),
    d_city      varchar(20),
    d_state     char(2),
    d_zip       char(9),
    d_tax       decimal(4, 4),
    d_ytd       decimal(12, 2),
    d_next_o_id integer,
    primary key (d_w_id, d_id)
) partition by hash (d_w_id) with (%%relationConfig%%);
