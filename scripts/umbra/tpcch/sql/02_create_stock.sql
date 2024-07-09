create table stock
(
    s_i_id       integer not null,
    s_w_id       integer not null,
    s_quantity   numeric(4),
    s_dist_01    char(24),
    s_dist_02    char(24),
    s_dist_03    char(24),
    s_dist_04    char(24),
    s_dist_05    char(24),
    s_dist_06    char(24),
    s_dist_07    char(24),
    s_dist_08    char(24),
    s_dist_09    char(24),
    s_dist_10    char(24),
    s_ytd        integer,
    s_order_cnt  integer,
    s_remote_cnt integer,
    s_data       varchar(50),
    primary key (s_w_id, s_i_id)
) partition by hash (s_w_id) with (%%relationConfig%%);
