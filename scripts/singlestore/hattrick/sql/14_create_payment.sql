create procedure payment(custkey int, suppkey int, amount decimal, orderkey int)
as
begin
    start transaction;

    update customer
    set c_paymentcnt = c_paymentcnt + 1
    where c_custkey = custkey;

    update supplier
    set s_ytd = s_ytd + amount
    where s_suppkey = suppkey;

    insert into history(h_orderkey, h_custkey, h_amount)
    values (orderkey, custkey, amount);

    commit;

exception
    when others then
        rollback;
end;
