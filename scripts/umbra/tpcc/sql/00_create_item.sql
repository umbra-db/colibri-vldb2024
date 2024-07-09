create table item
(
    i_id    integer not null,
    i_im_id integer,
    i_name  varchar(24),
    i_price decimal(5, 2),
    i_data  varchar(50),
    primary key (i_id)
) with (%%relationConfig%%);
