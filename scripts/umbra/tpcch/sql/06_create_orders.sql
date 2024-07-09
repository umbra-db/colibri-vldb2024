create table orders
(
    o_id         integer not null,
    o_c_id       integer not null,
    o_d_id       integer not null,
    o_w_id       integer not null,
    o_entry_d    date,
    o_carrier_id integer,
    o_ol_cnt     integer,
    o_all_local  integer,
    primary key (o_w_id, o_d_id, o_id)
) partition by hash (o_w_id) with (%%relationConfig%%);
