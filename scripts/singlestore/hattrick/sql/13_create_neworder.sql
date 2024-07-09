create procedure neworder(orderkey int, custname varchar(25), partkey1 int, partkey2 int, partkey3 int,
                          partkey4 int, suppname1 varchar(25), suppname2 varchar(25), suppname3 varchar(25),
                          suppname4 varchar(25), date1 varchar(18), date2 varchar(18), date3 varchar(18),
                          date4 varchar(18), ordpriority char(15), shippriority char(1), quantity int,
                          extendedprice decimal, discount int, revenue decimal, supplycost decimal, tax int,
                          shipmode char(10))
as
declare
    price1 decimal;
    price2 decimal;
    price3 decimal;
    price4 decimal;
    suppkey1 integer;
    suppkey2 integer;
    suppkey3 integer;
    suppkey4 integer;
    datekey1 integer;
    datekey2 integer;
    datekey3 integer;
    datekey4 integer;
    revenue1 decimal;
    revenue2 decimal;
    revenue3 decimal;
    revenue4 decimal;
    custkey integer;
begin
    start transaction;

    select c_custkey into custkey from customer where c_name = custname;

    select p_price into price1 from part where p_partkey = partkey1;
    select s_suppkey into suppkey1 from supplier where s_name = suppname1;
    select d_datekey into datekey1 from date where d_date = date1;

    select p_price into price2 from part where p_partkey = partkey2;
    select s_suppkey into suppkey2 from supplier where s_name = suppname2;
    select d_datekey into datekey2 from date where d_date = date2;

    select p_price into price3 from part where p_partkey = partkey3;
    select s_suppkey into suppkey3 from supplier where s_name = suppname3;
    select d_datekey into datekey3 from date where d_date = date3;

    select p_price into price4 from part where p_partkey = partkey4;
    select s_suppkey into suppkey4 from supplier where s_name = suppname4;
    select d_datekey into datekey4 from date where d_date = date4;

    price1 = extendedprice * price1;
    revenue1 = revenue * extendedprice;

    price2 = extendedprice * price2;
    revenue2 = revenue * extendedprice;

    price3 = extendedprice * price3;
    revenue3 = revenue * extendedprice;

    price4 = extendedprice * price4;
    revenue4 = revenue * extendedprice;

    insert into lineorder(lo_orderkey, lo_linenumber, lo_custkey, lo_partkey, lo_suppkey, lo_orderdate,
                          lo_ordpriority, lo_shippriority, lo_quantity, lo_extendedprice, lo_discount, lo_revenue,
                          lo_supplycost, lo_tax, lo_commitdate, lo_shipmode)
    values (orderkey, 1, custkey, partkey1, suppkey1, datekey1, ordpriority, shippriority, quantity,
            price1, discount, revenue1, supplycost, tax, datekey1, shipmode),
           (orderkey, 2, custkey, partkey2, suppkey2, datekey2, ordpriority, shippriority, quantity,
            price2, discount, revenue2, supplycost, tax, datekey2, shipmode),
           (orderkey, 3, custkey, partkey3, suppkey3, datekey3, ordpriority, shippriority, quantity,
            price3, discount, revenue3, supplycost, tax, datekey3, shipmode),
           (orderkey, 4, custkey, partkey4, suppkey4, datekey4, ordpriority, shippriority, quantity,
            price4, discount, revenue4, supplycost, tax, datekey4, shipmode);

    commit;
exception
    when others then
        rollback;
end;

