create procedure neworder(orderkey integer, custname varchar(25),
                          partkey1 integer, partkey2 integer, partkey3 integer, partkey4 integer,
                          suppname1 varchar(25), suppname2 varchar(25), suppname3 varchar(25), suppname4 varchar(25),
                          date1 varchar(18), date2 varchar(18), date3 varchar(18), date4 varchar(18),
                          ordpriority char(15), shippriority char(1), quantity integer, extendedprice numeric(18, 2),
                          discount integer, revenue numeric(18, 2), supplycost numeric(18, 2), tax integer,
                          shipmode char(10))
as
$$
    select c_custkey as var_custkey from customer where c_name = custname;

    select p_price as price1 from part where p_partkey = partkey1;
    select s_suppkey as var_suppkey1 from supplier where s_name = suppname1;
    select d_datekey as var_datekey1 from "date" where d_date = date1;

    select p_price as price2 from part where p_partkey = partkey2;
    select s_suppkey as var_suppkey2 from supplier where s_name = suppname2;
    select d_datekey as var_datekey2 from "date" where d_date = date2;

    select p_price as price3 from part where p_partkey = partkey3;
    select s_suppkey as var_suppkey3 from supplier where s_name = suppname3;
    select d_datekey as var_datekey3 from "date" where d_date = date3;

    select p_price as price4 from part where p_partkey = partkey4;
    select s_suppkey as var_suppkey4 from supplier where s_name = suppname4;
    select d_datekey as var_datekey4 from "date" where d_date = date4;

    let var_price1 = extendedprice * price1;
    let var_revenue1 = revenue * var_price1;

    let var_price2 = extendedprice * price2;
    let var_revenue2 = revenue * var_price2;

    let var_price3 = extendedprice * price3;
    let var_revenue3 = revenue * var_price3;

    let var_price4 = extendedprice * price4;
    let var_revenue4 = revenue * var_price4;

    insert into lineorder(lo_orderkey, lo_linenumber, lo_custkey, lo_partkey, lo_suppkey, lo_orderdate,
                          lo_ordpriority, lo_shippriority, lo_quantity, lo_extendedprice, lo_discount, lo_revenue,
                          lo_supplycost, lo_tax, lo_commitdate, lo_shipmode)
    values (orderkey, 1, var_custkey, partkey1, var_suppkey1, var_datekey1, ordpriority, shippriority, quantity,
            var_price1, discount, var_revenue1, supplycost, tax, var_datekey1, shipmode),
           (orderkey, 2, var_custkey, partkey2, var_suppkey2, var_datekey2, ordpriority, shippriority, quantity,
            var_price2, discount, var_revenue2, supplycost, tax, var_datekey2, shipmode),
           (orderkey, 3, var_custkey, partkey3, var_suppkey3, var_datekey3, ordpriority, shippriority, quantity,
            var_price3, discount, var_revenue3, supplycost, tax, var_datekey3, shipmode),
           (orderkey, 4, var_custkey, partkey4, var_suppkey4, var_datekey4, ordpriority, shippriority, quantity,
            var_price4, discount, var_revenue4, supplycost, tax, var_datekey4, shipmode)
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    commit;
$$ language 'umbrascript';

create procedure payment(custkey integer, suppkey integer, amount numeric(18, 2), orderkey integer)
as
$$
    update customer
    set c_paymentcnt = c_paymentcnt + 1
    where c_custkey = custkey
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    update supplier
    set s_ytd = s_ytd + amount
    where s_suppkey = suppkey
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    insert into history(h_orderkey, h_custkey, h_amount)
    values (orderkey, custkey, amount)
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    commit;
$$ language 'umbrascript';

create procedure countorders(custname varchar(25))
as
$$
    select c_custkey as var_custkey
    from customer
    where c_name = custname;

    select count(distinct lo_orderkey)
    from lineorder
    where lo_custkey = var_custkey {
        -- do nothing, we just have to select data
    }

    commit;
$$ language 'umbrascript';
