create table new_orders
(
    no_o_id integer not null,
    no_d_id integer not null,
    no_w_id integer not null,
    primary key (no_w_id, no_d_id, no_o_id)
) partition by hash (no_w_id) with (%%relationConfig%%);
