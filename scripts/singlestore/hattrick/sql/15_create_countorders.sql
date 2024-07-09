create procedure countorders(custname varchar(25))
as
declare
    custkey integer;
    result integer;
begin
    start transaction;

    select c_custkey into custkey
    from customer
    where c_name = custname;

    select count(*) into result
    from lineorder
    where lo_custkey = custkey;

    commit;

exception
    when others then
        rollback;
end;
