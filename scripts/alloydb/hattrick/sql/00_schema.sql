create table part
(
    p_partkey   int not null,
    p_name      varchar(22),
    p_mfgr      char(6),
    p_category  char(7),
    p_brand1    char(9),
    p_color     varchar(11),
    p_type      varchar(25),
    p_size      int,
    p_container varchar(10),
    p_price     numeric(18, 2),
    primary key (p_partkey)
);

create table customer
(
    c_custkey    int not null,
    c_name       varchar(25),
    c_address    varchar(25),
    c_city       char(10),
    c_nation     varchar(15),
    c_region     varchar(12),
    c_phone      char(15),
    c_mktsegment varchar(10),
    c_paymentcnt integer,
    primary key (c_custkey)
);

create table supplier
(
    s_suppkey integer not null,
    s_name    varchar(25),
    s_address varchar(25),
    s_city    char(10),
    s_nation  varchar(15),
    s_region  char(12),
    s_phone   char(15),
    s_ytd     numeric(18, 2),
    primary key (s_suppkey)
);

create table date
(
    d_datekey          integer not null,
    d_date             varchar(18),
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
    d_lastdayinweekfl  boolean,
    d_lastdayinmonthfl boolean,
    d_holidayfl        boolean,
    d_weekdayfl        boolean,
    primary key (d_datekey)
);

create table lineorder
(
    lo_orderkey      integer not null,
    lo_linenumber    integer not null,
    lo_custkey       integer,
    lo_partkey       integer,
    lo_suppkey       integer,
    lo_orderdate     integer,
    lo_ordpriority   char(15),
    lo_shippriority  char(1),
    lo_quantity      integer,
    lo_extendedprice numeric(18, 2),
    lo_discount      integer,
    lo_revenue       numeric(18, 2),
    lo_supplycost    numeric(18, 2),
    lo_tax           integer,
    lo_commitdate    integer,
    lo_shipmode      char(10),
    primary key (lo_orderkey, lo_linenumber)
);

create table history
(
    h_orderkey integer not null,
    h_custkey  integer not null,
    h_amount   numeric(18, 2)
);
