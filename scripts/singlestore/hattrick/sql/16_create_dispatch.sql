create procedure dispatch(txn integer, orderkey int, custname varchar(25), partkey1 int, partkey2 int,
                          partkey3 int, partkey4 int, suppname1 varchar(25), suppname2 varchar(25),
                          suppname3 varchar(25), suppname4 varchar(25), date1 varchar(18), date2 varchar(18),
                          date3 varchar(18), date4 varchar(18), ordpriority char(15), shippriority char(1),
                          quantity int, extendedprice decimal, discount int, revenue decimal, supplycost decimal,
                          tax int, shipmode char(10))
as
begin
    if txn = 0 then
        call neworder(orderkey, custname, partkey1, partkey2, partkey3, partkey4, suppname1, suppname2,
                      suppname3, suppname4, date1, date2, date3, date4, ordpriority, shippriority, quantity,
                      extendedprice, discount, revenue, supplycost, tax, shipmode);
    elsif txn = 1 then
        call payment(partkey1, partkey2, revenue, orderkey);
    elsif txn = 2 then
        call countorders(custname);
    end if;
end
