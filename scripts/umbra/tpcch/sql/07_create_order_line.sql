create table order_line
(
    ol_o_id        integer not null,
    ol_d_id        integer not null,
    ol_w_id        integer not null,
    ol_number      integer not null,
    ol_i_id        integer,
    ol_supply_w_id integer,
    ol_delivery_d  date,
    ol_quantity    integer,
    ol_amount      decimal(6, 2),
    ol_dist_info   char(24),
    primary key (ol_w_id, ol_d_id, ol_o_id, ol_number, ol_i_id)
) partition by hash (ol_w_id) with (%%relationConfig%%);
