create table date
(
    d_datekey          integer     not null primary key,
    d_date             varchar(18) not null,
    d_dateofweek       varchar(9),
    d_month            varchar(9),
    d_year             integer,
    d_yearmonthnum     integer,
    d_yearmonth        char(7),
    d_daynuminweek     integer,
    d_daynuminmonth    integer,
    d_daynuminyear     integer,
    d_monthnuminyear   integer,
    d_weeknuminyear    integer,
    d_sellingseason    char(15),
    d_lastdayinweekfl  bit,
    d_lastdayinmonthfl bit,
    d_holidayfl        bit,
    d_weekdayfl        bit,
    index (d_date) using btree
);
alter table date set tiflash replica 1;
