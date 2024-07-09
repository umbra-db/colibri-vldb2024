create or replace procedure neworder(orderkey integer, custname varchar(25),
                                     partkey1 integer, partkey2 integer, partkey3 integer, partkey4 integer,
                                     suppname1 varchar(25), suppname2 varchar(25), suppname3 varchar(25),
                                     suppname4 varchar(25), date1 varchar(18), date2 varchar(18), date3 varchar(18),
                                     date4 varchar(18), ordpriority char(15), shippriority char(1), quantity integer,
                                     extendedprice numeric(18, 2), discount integer, revenue numeric(18, 2),
                                     supplycost numeric(18, 2), tax integer, shipmode char(10))
    language plpgsql
as
$$
declare
    price1   numeric(18, 2);
    price2   numeric(18, 2);
    price3   numeric(18, 2);
    price4   numeric(18, 2);
    revenue1 numeric(18, 2);
    revenue2 numeric(18, 2);
    revenue3 numeric(18, 2);
    revenue4 numeric(18, 2);
    suppkey1 integer;
    suppkey2 integer;
    suppkey3 integer;
    suppkey4 integer;
    datekey1 integer;
    datekey2 integer;
    datekey3 integer;
    datekey4 integer;
    custkey  integer;
begin

    begin
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
        revenue1 = revenue * price1;

        price2 = extendedprice * price2;
        revenue2 = revenue * price2;

        price3 = extendedprice * price3;
        revenue3 = revenue * price3;

        price4 = extendedprice * price4;
        revenue4 = revenue * price4;

        insert into lineorder(lo_orderkey, lo_linenumber, lo_custkey, lo_partkey, lo_suppkey, lo_orderdate,
                              lo_ordpriority, lo_shippriority, lo_quantity, lo_extendedprice, lo_discount, lo_revenue,
                              lo_supplycost, lo_tax, lo_commitdate, lo_shipmode)
        values (orderkey, 1, custkey, partkey1, suppkey1, datekey1, ordpriority, shippriority, quantity, price1,
                discount, revenue1, supplycost, tax, datekey1, shipmode),
               (orderkey, 2, custkey, partkey2, suppkey2, datekey2, ordpriority, shippriority, quantity, price2,
                discount, revenue2, supplycost, tax, datekey2, shipmode),
               (orderkey, 3, custkey, partkey3, suppkey3, datekey3, ordpriority, shippriority, quantity, price3,
                discount, revenue3, supplycost, tax, datekey3, shipmode),
               (orderkey, 4, custkey, partkey4, suppkey4, datekey4, ordpriority, shippriority, quantity, price4,
                discount, revenue4, supplycost, tax, datekey4, shipmode);
    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;

    commit;
end;
$$;

create or replace procedure payment(custkey integer, suppkey integer, amount numeric(18, 2), orderkey integer)
    language plpgsql
as
$$
begin
    begin
        update customer
        set c_paymentcnt = c_paymentcnt + 1
        where c_custkey = custkey;

        update supplier
        set s_ytd = s_ytd + amount
        where s_suppkey = suppkey;

        insert into history(h_orderkey, h_custkey, h_amount)
        values (orderkey, custkey, amount);
    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;

    commit;
end;
$$;

create or replace procedure countorders(custname varchar(25))
    language plpgsql
as
$$
declare
    custkey integer;
begin
    begin
        select c_custkey into custkey from customer where c_name = custname;

        perform count(distinct lo_orderkey) from lineorder where lo_custkey = custkey;

    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;

    commit;
end;
$$;
