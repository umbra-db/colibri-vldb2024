create table customer
(
    c_id           integer not null,
    c_d_id         integer not null,
    c_w_id         integer not null,
    c_first        varchar(16),
    c_middle       char(2),
    c_last         varchar(16),
    c_street_1     varchar(20),
    c_street_2     varchar(20),
    c_city         varchar(20),
    c_state        char(2),
    c_zip          char(9),
    c_phone        char(16),
    c_since        date,
    c_credit       char(2),
    c_credit_lim   decimal(12, 2),
    c_discount     decimal(4, 4),
    c_balance      decimal(12, 2),
    c_ytd_payment  decimal(12, 2),
    c_payment_cnt  integer,
    c_delivery_cnt integer,
    c_data         text,
    primary key (c_w_id, c_d_id, c_id)
) partition by hash (c_w_id) with (%%relationConfig%%);