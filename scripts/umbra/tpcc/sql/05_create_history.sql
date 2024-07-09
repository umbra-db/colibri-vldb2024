create table history
(
    h_c_id   integer,
    h_c_d_id integer,
    h_c_w_id integer,
    h_d_id   integer,
    h_w_id   integer,
    h_date   date,
    h_amount decimal(6, 2),
    h_data   varchar(24)
) partition by hash (h_w_id) with (%%relationConfig%%);
