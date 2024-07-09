create or replace function random(upper integer) returns integer
as
$$
begin
    return trunc(random() * upper);
end
$$ language 'plpgsql' strict;

create or replace function random_date(lower timestamp, upper timestamp) returns timestamp
as
$$
begin
    return lower + ((upper - lower) / 10000 * random(10000));
end
$$
    language 'plpgsql'
    strict;


create or replace procedure tp1(var_custid int)
as
$$
begin
    perform *
    from customer
    where custid = var_custid;

    commit;
end
$$
    language 'plpgsql';

create or replace procedure tp2(var_companyid int)
as
$$
begin
    perform *
    from company
    where companyid = var_companyid;

    commit;
end
$$ language 'plpgsql';

create or replace procedure tp3(var_accountid int)
as
$$
begin
    perform *
    from savingaccount
    where accountid = var_accountid;

    commit;
end
$$ language 'plpgsql';

create or replace procedure tp4(var_accountid int)
as
$$
begin
    perform *
    from checkingaccount
    where accountid = var_accountid;

    commit;
end
$$ language 'plpgsql';

create or replace procedure tp5(var_accountid int)
as
$$
begin
    perform *
    from transfer
    where sourceid = var_accountid
       or targetid = var_accountid
    order by timestamp desc
    limit 1;

    commit;
end
$$ language 'plpgsql';

create or replace procedure tp6(var_accountid int)
as
$$
begin
    perform *
    from checking
    where sourceid = var_accountid
       or targetid = var_accountid
    order by timestamp desc
    limit 1;

    commit;
end
$$ language 'plpgsql';

create or replace procedure tp7(var_applicantid int)
as
$$
begin
    perform *
    from loanapps
    where applicantid = var_applicantid
    order by timestamp desc
    limit 1;

    commit;
end
$$ language 'plpgsql';

create or replace procedure tp8(var_applicantid int)
as
$$
begin
    perform *
    from loantrans
    where applicantid = var_applicantid
    order by timestamp desc
    limit 1;

    commit;
end
$$ language 'plpgsql';

create or replace procedure tp9(var_transferid bigint, var_sourceid int, var_targetid int, var_amount_bi bigint, var_type text, var_ts text)
as
$$
declare
    var_amount          real;
    var_balance         real;
    var_should_rollback boolean := false;
begin
    begin
        var_amount = var_amount_bi::real / 100;

        select balance into var_balance from savingaccount where accountid = var_sourceid;

        if var_balance < var_amount then
            var_should_rollback = true;
        else
            update savingaccount
            set balance = balance - var_amount
            where accountid = var_sourceid;

            update savingaccount
            set balance = balance + var_amount
            where accountid = var_targetid;

            insert into transfer values (var_transferid, var_sourceid, var_targetid, var_amount::real / 100, var_type, var_ts::timestamp);
        end if;
    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;

    commit;
end
$$ language 'plpgsql';

create or replace procedure tp10(var_transferid bigint, var_sourceid int, var_salary_bi bigint, var_type text, var_ts text)
as
$$
declare
    var_salary     real;
    l              record;
    var_row_number int;
begin
    begin
        var_salary = var_salary_bi::real / 100;
        var_row_number = 0;

        for l in
            select custid from customer where companyid = var_sourceid
            loop
                var_row_number = var_row_number + 1;

                update savingaccount
                set balance = balance - var_salary
                where accountid = var_sourceid;

                update savingaccount
                set balance = balance + var_salary
                where accountid = l.custid;

                insert into transfer values (var_transferid + var_row_number, var_sourceid, l.custid, var_salary, var_type, var_ts::timestamp);
            end loop;
    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;
    commit;
end
$$ language 'plpgsql';

create or replace procedure tp11(var_checkingid bigint, var_sourceid int, var_targetid int, var_amount_bi bigint, var_type text, var_ts text)
as
$$
declare
    var_amount          real;
    var_balance         real;
    var_should_rollback boolean := false;
begin
    begin
        var_amount = var_amount_bi::real / 100;

        select balance into var_balance from checkingaccount where accountid = var_sourceid;

        if var_balance < var_amount then
            var_should_rollback = true;
        else
            update checkingaccount
            set balance = balance - var_amount
            where accountid = var_sourceid;

            update checkingaccount
            set balance = balance + var_amount
            where accountid = var_targetid;

            insert into checking values (var_checkingid, var_sourceid, var_targetid, var_amount, var_type, var_ts::timestamp);
        end if;
    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;

    if var_should_rollback then
        rollback;
    else
        commit;
    end if;
end
$$ language 'plpgsql';

create or replace procedure tp12(var_appid bigint, var_applicantid int, var_amount_bi bigint, var_duration int, var_status text, var_ts text, var_customercount bigint)
as
$$
declare
    var_amount          real;
    var_balance         real;
    var_should_rollback boolean := false;
begin
    begin
        var_amount = var_amount_bi::real / 100;
        if var_applicantid <= var_customercount then
            select loan_balance into var_balance from customer where custid = var_applicantid;

            if var_balance < var_amount then
                var_should_rollback = true;
            else

                update customer
                set loan_balance = loan_balance - var_amount
                where custid = var_applicantid;

                insert into loanapps values (var_appid, var_applicantid, var_amount, var_duration, var_status, var_ts::timestamp);
            end if;
        else
            select loan_balance into var_balance from company where companyid = var_applicantid;

            if var_balance < var_amount then
                var_should_rollback = true;
            else
                update company
                set loan_balance = loan_balance - var_amount
                where companyid = var_applicantid;

                insert into loanapps values (var_appid, var_applicantid, var_amount, var_duration, var_status, var_ts::timestamp);
            end if;
        end if;
    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;

    if var_should_rollback then
        rollback;
    else
        commit;
    end if;
end
$$ language 'plpgsql';

create or replace procedure tp13(var_transid bigint, var_status text, var_end_ts text, var_customercount bigint)
as
$$
declare
    var_appid       bigint;
    var_applicantid int;
    var_amount      real;
    var_duration    int;
    var_ts          timestamp;
    var_contract_ts timestamp;
begin
    begin
        select id, applicantid, amount, duration, timestamp
        into var_appid, var_applicantid, var_amount, var_duration, var_ts
        from loanapps
        where status = 'under_review'
        order by random()
        limit 1;

        var_contract_ts = random_date(var_ts, var_end_ts::timestamp);
        if var_status = 'accept' then
            insert into loantrans values (var_transid, var_applicantid, var_appid, var_amount, var_status, var_ts, var_duration, var_contract_ts, 0);
        else
            if var_applicantid <= var_customercount then
                update customer
                set loan_balance = loan_balance + var_amount
                where custid = var_applicantid;
            else
                update company
                set loan_balance = loan_balance + var_amount
                where companyid = var_applicantid;
            end if;
        end if;

        update loanapps
        set status = var_status
        where id = var_appid;
    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;
    commit;
end
$$ language 'plpgsql';

create or replace procedure tp14(var_end_ts text)
as
$$
declare
    var_transid     bigint;
    var_applicantid int;
    var_amount      real;
    var_contract_ts timestamp;
    var_ts          timestamp;
begin
    begin
        select id, applicantid, amount, contract_timestamp into var_transid, var_applicantid, var_amount, var_contract_ts from loantrans where status = 'accept' order by random() limit 1;
        var_ts = random_date(var_contract_ts, var_end_ts::timestamp);

        update savingaccount
        set balance = balance + var_amount
        where accountid = var_applicantid;

        update loantrans
        set status    = 'lent',
            timestamp = var_ts
        where id = var_transid;
    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;
    commit;
end
$$ language 'plpgsql';

create or replace procedure tp15(var_current_ts text)
as
$$
declare
    l record;
begin
    begin
        for l in
            select id, duration, timestamp
            from loantrans
            where status = 'lent'
            order by timestamp
            limit 1
            loop
                if l.timestamp + l.duration * interval '1' day < var_current_ts::timestamp then
                    update loantrans
                    set delinquency = 1,
                        timestamp   = var_current_ts::timestamp
                    where id = l.id;
                end if;
            end loop;
    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;
    commit;
end
$$ language 'plpgsql';

create or replace procedure tp16(var_end_ts text)
as
$$
declare
    l                   record;
    var_balance         real;
    var_current_ts      timestamp;
    var_should_rollback boolean := false;
begin
    begin
        for l in
            select id, applicantid, amount, timestamp from loantrans where status = 'lent' order by random() limit 1
            loop
                select balance into var_balance from savingaccount where accountid = l.applicantid;
                if var_balance < l.amount then
                    var_should_rollback = true;
                else
                    var_current_ts = random_date(l.timestamp, var_end_ts::timestamp);
                    update savingaccount
                    set balance = balance - l.amount
                    where accountid = l.applicantid;

                    update loantrans
                    set status    = 'repaid',
                        timestamp = var_current_ts
                    where id = l.id;
                end if;
            end loop;
    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;

    if var_should_rollback then
        rollback;
    else
        commit;
    end if;
end
$$ language 'plpgsql';

create or replace procedure tp17(var_accountid int, var_amount_bi bigint)
as
$$
declare
    var_amount          real;
    var_balance1        real;
    var_isblocked1      int;
    var_isblocked2      int;
    var_should_rollback boolean := false;
begin
    begin
        var_amount = var_amount_bi::real / 100;

        select balance, isblocked
        into var_balance1, var_isblocked1
        from savingaccount
        where accountid = var_accountid;

        if var_balance1 < var_amount or var_isblocked1 = 1 then
            var_should_rollback = true;
        else
            select isblocked
            into var_isblocked2
            from checkingaccount
            where accountid = var_accountid;

            if var_isblocked2 = 1 then
                var_should_rollback = true;
            else
                update savingaccount
                set balance = balance - var_amount
                where accountid = var_accountid;

                update checkingaccount
                set balance = balance + var_amount
                where accountid = var_accountid;
            end if;
        end if;
    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;

    if var_should_rollback then
        rollback;
    else
        commit;
    end if;
end
$$ language 'plpgsql';

create or replace procedure tp18(var_accountid int, var_amount_bi bigint)
as
$$
declare
    var_amount          real;
    var_balance1        real;
    var_isblocked1      int;
    var_isblocked2      int;
    var_should_rollback boolean := false;
begin
    begin
        var_amount = var_amount_bi::real / 100;

        select balance, isblocked
        into var_balance1, var_isblocked1
        from checkingaccount
        where accountid = var_accountid;

        if var_balance1 < var_amount or var_isblocked1 = 1 then
            var_should_rollback = true;
        else
            select isblocked
            into var_isblocked2
            from savingaccount
            where accountid = var_accountid;

            if var_isblocked2 = 1 then
                var_should_rollback = true;
            else
                update checkingaccount
                set balance = balance - var_amount
                where accountid = var_accountid;

                update savingaccount
                set balance = balance + var_amount
                where accountid = var_accountid;
            end if;
        end if;
    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;

    if var_should_rollback then
        rollback;
    else
        commit;
    end if;
end
$$ language 'plpgsql';

create or replace procedure at1(var_transferid bigint, var_sourceid int, var_targetid int, var_amount_bi bigint, var_type text, var_ts text)
as
$$
declare
    var_amount          real;
    var_balance         real;
    var_isblocked       int;
    var_count           bigint;
    var_should_rollback boolean := false;
begin
    begin
        var_amount = var_amount_bi::real / 100;

        select balance, isblocked
        into var_balance, var_isblocked
        from savingaccount
        where accountid = var_sourceid;

        if var_balance < var_amount or var_isblocked = 1 then
            var_should_rollback = true;
        else
            select count(*)
            into var_count
            from transfer t,
                 savingaccount s
            where t.targetid = var_targetid
              and s.isblocked = 1
              and t.sourceid = s.accountid;

            if var_count > 0 then
                var_should_rollback = true;
            else
                update savingaccount
                set balance = balance - var_amount
                where accountid = var_sourceid;

                update savingaccount
                set balance = balance + var_amount
                where accountid = var_targetid;

                insert into transfer values (var_transferid, var_sourceid, var_targetid, var_amount, var_type, var_ts::timestamp);
            end if;
        end if;
    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;

    if var_should_rollback then
        rollback;
    else
        commit;
    end if;
end
$$ language 'plpgsql';

create or replace procedure at2(var_checkingid bigint, var_sourceid int, var_targetid int, var_amount_bi bigint, var_type text, var_ts text)
as
$$
declare
    var_amount          real;
    var_balance         real;
    var_isblocked       int;
    var_count           bigint;
    var_should_rollback boolean := false;
begin
    begin
        var_amount = var_amount_bi::real / 100;

        select balance, isblocked
        into var_balance, var_isblocked
        from checkingaccount
        where accountid = var_sourceid;

        if var_balance < var_amount or var_isblocked = 1 then
            var_should_rollback = true;
        else
            select count(*)
            into var_count
            from checking ch,
                 checkingaccount ca
            where ch.sourceid = var_sourceid
              and ca.isblocked = 1
              and ch.targetid = ca.accountid;

            if var_count > 0 then
                var_should_rollback = true;
            else
                update checkingaccount
                set balance = balance - var_amount
                where accountid = var_sourceid;

                update checkingaccount
                set balance = balance + var_amount
                where accountid = var_targetid;

                insert into checking values (var_checkingid, var_sourceid, var_targetid, var_amount, var_type, var_ts::timestamp);
            end if;
        end if;
    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;

    if var_should_rollback then
        rollback;
    else
        commit;
    end if;
end
$$ language 'plpgsql';

create or replace procedure at3(var_loantransid bigint, var_end_ts text)
as
$$
declare
    var_applicantid int;
    var_appid       bigint;
    var_ts          timestamp;
    var_duration    int;
    var_current_ts  timestamp;
    var_isblocked   int;
    var_amount      real;
begin
    begin
        select applicantid, id, timestamp, duration
        into var_applicantid, var_appid, var_ts, var_duration
        from loanapps
        where status = 'under_review'
        order by random()
        limit 1;
        var_current_ts = random_date(var_ts, var_end_ts::timestamp);

        select isblocked into var_isblocked from savingaccount where accountid = var_applicantid;
        if var_isblocked = 1 then
            update loanapps
            set status    = 'reject',
                timestamp = var_current_ts
            where id = var_appid;
        else
            select (inbound.t_amount - outbound.t_amount)
            into var_amount
            from (select sa.accountid as sid, sum(t.amount) as t_amount
                  from savingaccount sa,
                       transfer t
                  where sa.accountid = var_applicantid
                    and sa.accountid = t.sourceid
                  group by sid) as outbound,
                 (select sa.accountid as sid, sum(t.amount) as t_amount
                  from savingaccount sa,
                       transfer t
                  where sa.accountid = var_applicantid
                    and sa.accountid = t.targetid
                  group by sid) as inbound;

            if var_amount < 0 then
                update loanapps
                set status    = 'reject',
                    timestamp = var_current_ts
                where id = var_appid;
            else
                insert into loantrans values (var_loantransid, var_applicantid, var_appid, var_amount, 'accept', var_ts::timestamp, var_duration, var_current_ts);

                update loanapps
                set status    = 'accept',
                    timestamp = var_current_ts
                where id = var_appid;
            end if;
        end if;

    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;
    commit;
end
$$ language 'plpgsql';

create or replace procedure at4(var_end_ts text)
as
$$
declare
    var_applicantid     int;
    var_amount          real;
    var_loantransid     bigint;
    var_ts              timestamp;
    var_current_ts      timestamp;
    var_count           bigint;
    var_should_rollback boolean := false;
begin
    begin
        select applicantid, amount, id, timestamp
        into var_applicantid, var_amount, var_loantransid, var_ts
        from loantrans
        where status = 'accept'
        order by random()
        limit 1;
        var_current_ts = random_date(var_ts, var_end_ts::timestamp);

        select count(distinct sa2.accountid)
        into var_count
        from transfer t,
             savingaccount sa1,
             savingaccount sa2,
             loantrans lt
        where sa1.accountid = var_applicantid
          and sa1.accountid = t.targetid
          and t.sourceid = sa2.accountid
          and lt.applicantid = sa2.accountid
          and lt.status = 'accept';

        if var_count > 5 then
            var_should_rollback = true;
        else
            update savingaccount
            set balance = balance + var_amount;

            update loantrans set status = 'lent', timestamp = var_current_ts where id = var_loantransid;
        end if;
    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;

    if var_should_rollback then
        rollback;
    else
        commit;
    end if;
end
$$ language 'plpgsql';

create or replace procedure at5()
as
$$
declare
    l1 record;
    l2 record;
begin
    begin
        for l1 in
            select applicantid from loantrans where status = 'lent' order by random() limit 1
            loop
                for l2 in
                    select id from loantrans where applicantid = l1.applicantid and contract_timestamp::date + duration < cast('2024-01-01' as date) and status = 'lent'
                    loop
                        update loantrans
                        set delinquency=1
                        where id = l2.id;
                    end loop;
            end loop;
    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;
    commit;
end
$$ language 'plpgsql';

create or replace procedure at6()
as
$$
declare
    l         record;
    var_count bigint;
begin
    begin
        for l in
            select applicantid from loantrans where status = 'lent' and delinquency = 1 order by random() limit 1
            loop
                select count(id) into var_count from loantrans where contract_timestamp::date + duration + 30 < cast('2024-01-01' as date) and applicantid = l.applicantid;

                if var_count > 0 then
                    update savingaccount
                    set isblocked = 1
                    where accountid = l.applicantid;

                    update checkingaccount
                    set isblocked = 1
                    where accountid = l.applicantid;

                    insert into blockedaccounts values (l.applicantid);
                end if;
            end loop;
    exception
        when serialization_failure or deadlock_detected or no_data_found
            then
                rollback;
    end;
    commit;
end
$$ language 'plpgsql';
