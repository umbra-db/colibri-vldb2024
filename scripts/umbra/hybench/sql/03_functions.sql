create function random_date(lower timestamp, upper timestamp) returns timestamp
as
$$
    return lower + ((upper - lower) / 10000 * random(10000));
$$ language 'umbrascript' strict
                          volatile;


create procedure tp1(var_custid int)
as
$$
    select *
    from customer
    where custid = var_custid {
        -- do nothing, we just have to select data
    }

    commit;
$$ language 'umbrascript';

create procedure tp2(var_companyid int)
as
$$
    select *
    from company
    where companyid = var_companyid {
        -- do nothing, we just have to select data
    }

    commit;
$$ language 'umbrascript';

create procedure tp3(var_accountid int)
as
$$
    select *
    from savingaccount
    where accountid = var_accountid {
        -- do nothing, we just have to select data
    }

    commit;
$$ language 'umbrascript';

create procedure tp4(var_accountid int)
as
$$
    select *
    from checkingaccount
    where accountid = var_accountid {
        -- do nothing, we just have to select data
    }

    commit;
$$ language 'umbrascript';

create procedure tp5(var_accountid int)
as
$$
    select *
    from transfer
    where sourceid = var_accountid or targetid = var_accountid
    order by timestamp desc
    limit 1 {
        -- do nothing, we just have to select data
    }

    commit;
$$ language 'umbrascript';

create procedure tp6(var_accountid int)
as
$$
    select *
    from checking
    where sourceid = var_accountid or targetid = var_accountid
    order by timestamp desc
    limit 1 {
        -- do nothing, we just have to select data
    }

    commit;
$$ language 'umbrascript';

create procedure tp7(var_applicantid int)
as
$$
    select *
    from loanapps
    where applicantid = var_applicantid
    order by timestamp desc
    limit 1 {
        -- do nothing, we just have to select data
    }

    commit;
$$ language 'umbrascript';

create procedure tp8(var_applicantid int)
as
$$
    select *
    from loantrans
    where applicantid = var_applicantid
    order by timestamp desc
    limit 1 {
        -- do nothing, we just have to select data
    }

    commit;
$$ language 'umbrascript';

create procedure tp9(var_transferid bigint, var_sourceid int, var_targetid int, var_amount_bi bigint, var_type text, var_ts text)
as
$$
    let var_amount = var_amount_bi::real / 100;

    select balance as var_balance from savingaccount where accountid = var_sourceid;

    if var_balance < var_amount {
        rollback;
        return;
    }

    update savingAccount set balance = balance - var_amount where accountID = var_sourceid
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    update savingAccount set balance = balance + var_amount where accountID = var_targetid
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    insert into transfer values (var_transferid, var_sourceid, var_targetid, var_amount::real / 100, var_type, var_ts::timestamp);

    commit;
$$ language 'umbrascript';

create procedure tp10(var_transferid bigint, var_sourceid int, var_salary_bi bigint, var_type text, var_ts text)
as
$$
    let var_salary = var_salary_bi::real / 100;

    select custid as var_custid, row_number() over () as rn from customer where companyid = var_sourceid {
        update savingaccount set balance = balance - var_salary where accountID = var_sourceid
        catch serialization_failure {
            raise notice 'serialization failure';
            return;
        }

        update savingaccount set balance = balance + var_salary where accountID = var_custid
        catch serialization_failure {
            raise notice 'serialization failure';
            return;
        }

        insert into transfer values (var_transferid + rn, var_sourceid, var_custid, var_salary, var_type, var_ts::timestamp);
    }

    commit;
$$ language 'umbrascript';

create procedure tp11(var_checkingid bigint, var_sourceid int, var_targetid int, var_amount_bi bigint, var_type text, var_ts text)
as
$$
    let var_amount = var_amount_bi::real / 100;

    select balance as var_balance from checkingaccount where accountid = var_sourceid;

    if var_balance < var_amount {
        rollback;
        return;
    }

    update checkingaccount set balance = balance - var_amount where accountID = var_sourceid
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }


    update checkingaccount set balance = balance + var_amount where accountID = var_targetid
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    insert into checking values (var_checkingid, var_sourceid, var_targetid, var_amount, var_type, var_ts::timestamp);

    commit;
$$ language 'umbrascript';

create procedure tp12(var_appid bigint, var_applicantid int, var_amount_bi bigint, var_duration int, var_status text, var_ts text, var_customercount bigint)
as
$$
    let var_amount = var_amount_bi::real / 100;
    let mut var_balance: real;
    if var_applicantid <= var_customercount {
        select loan_balance as tmp_balance from customer where custid = var_applicantid;
        var_balance = tmp_balance;

        if var_balance < var_amount {
            rollback;
            return;
        }

        update customer set loan_balance = loan_balance - var_amount where custid = var_applicantid
        catch serialization_failure {
            raise notice 'serialization failure';
            return;
        }


    } else {
        select loan_balance as tmp_balance from company where companyid = var_applicantid;
        var_balance = tmp_balance;

        if var_balance < var_amount {
            rollback;
            return;
        }

        update company set loan_balance = loan_balance - var_amount where companyid = var_applicantid
        catch serialization_failure {
            raise notice 'serialization failure';
            return;
        }
    }

    insert into loanapps values (var_appid, var_applicantid, var_amount, var_duration, var_status, var_ts::timestamp);

    commit;
$$ language 'umbrascript';

create procedure tp13(var_transid bigint, var_status text, var_end_ts text, var_customercount bigint)
as
$$
    select id as var_appid, applicantid as var_applicantid, amount as var_amount, duration as var_duration, timestamp as var_ts
    from loanapps
    where status='under_review'
    order by random() limit 1;

    let var_contract_ts = random_date(var_ts, var_end_ts::timestamp);
    if var_status = 'accept' {
        insert into loantrans values (var_transid, var_applicantid, var_appid, var_amount, var_status, var_ts, var_duration, var_contract_ts, 0);
    } else {
        if var_applicantid <= var_customercount {
            update customer set loan_balance = loan_balance + var_amount where custid = var_applicantid
            catch serialization_failure {
                raise notice 'serialization failure';
                return;
            }
        } else {
            update company set loan_balance = loan_balance + var_amount where companyid = var_applicantid
            catch serialization_failure {
                raise notice 'serialization failure';
                return;
            }
        }
    }

    update loanapps set status = var_status where id = var_appid
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }


    commit;
$$ language 'umbrascript';

create procedure tp14(var_end_ts text)
as
$$
    select id as var_transid, applicantid as var_applicantid, amount as var_amount, contract_timestamp as var_contract_ts from loantrans where status='accept' order by random() limit 1;
    let var_ts = random_date(var_contract_ts, var_end_ts::timestamp);

    update savingaccount set balance = balance + var_amount where accountid = var_applicantid
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    update loantrans set status = 'lent', timestamp = var_ts where id = var_transid
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    commit;
$$ language 'umbrascript';

create procedure tp15(var_current_ts text)
as
$$
    select id as var_transid, duration as var_duration, timestamp as var_ts
    from loantrans where status='lent' order by timestamp limit 1 {
        if var_ts + var_duration * interval '1' day < var_current_ts::timestamp {
            update loantrans set delinquency = 1, timestamp = var_current_ts where id = var_transid
            catch serialization_failure {
                raise notice 'serialization failure';
                return;
            }
        }
    }

    commit;
$$ language 'umbrascript';

create procedure tp16(var_end_ts text)
as
$$
    select id as var_transid, applicantid as var_applicantid, amount as var_amount, timestamp as var_ts from loantrans where status='lent' order by random() limit 1 {
    select balance as var_balance from savingaccount where accountid = var_applicantid;
        if var_balance < var_amount {
            rollback;
            return;
        }

        let var_current_ts = random_date(var_ts, var_end_ts::timestamp);
        update savingaccount set balance = balance - var_amount where accountid = var_applicantid
        catch serialization_failure {
            raise notice 'serialization failure';
            return;
        }

        update loantrans set status = 'repaid', timestamp = var_current_ts where id = var_transid
        catch serialization_failure {
            raise notice 'serialization failure';
            return;
        }
    }

    commit;
$$ language 'umbrascript';

create procedure tp17(var_accountid int, var_amount_bi bigint)
as
$$
    let var_amount = var_amount_bi::real / 100;

    select balance as var_balance1, isblocked as var_isblocked1
    from savingaccount where accountid = var_accountid;

    if var_balance1 < var_amount or var_isblocked1 = 1 {
        rollback;
        return;
    }

    select balance as var_balance2, isblocked as var_isblocked2
    from checkingaccount where accountid = var_accountid;

    if var_isblocked2 = 1 {
        rollback;
        return;
    }

    update savingaccount set balance = balance - var_amount where accountid = var_accountid
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    update checkingaccount set balance = balance + var_amount where accountid = var_accountid
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    commit;
$$ language 'umbrascript';

create procedure tp18(var_accountid int, var_amount_bi bigint)
as
$$
    let var_amount = var_amount_bi::real / 100;

    select balance as var_balance1, isblocked as var_isblocked1
    from checkingaccount where accountid = var_accountid;

    if var_balance1 < var_amount or var_isblocked1 = 1 {
        rollback;
        return;
    }

    select balance as var_balance2, isblocked as var_isblocked2
    from savingaccount where accountid = var_accountid;

    if var_isblocked2 = 1 {
        rollback;
        return;
    }

    update checkingaccount set balance = balance - var_amount where accountid = var_accountid
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    update savingaccount set balance = balance + var_amount where accountid = var_accountid
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    commit;
$$ language 'umbrascript';

create procedure at1(var_transferid bigint, var_sourceid int, var_targetid int, var_amount_bi bigint, var_type text, var_ts text)
as
$$
    let var_amount = var_amount_bi::real / 100;

    select isblocked as var_isblocked, balance as var_balance
    from savingaccount where accountID = var_sourceid;

    if var_balance < var_amount or var_isblocked = 1 {
        rollback;
        return;
    }

    select count(*) as var_count from transfer t, savingAccount s
    where t.targetid = var_targetid and s.isblocked = 1 and t.sourceid = s.accountid;

    if var_count > 0 {
        rollback;
        return;
    }

    update savingaccount set balance = balance - var_amount where accountID = var_sourceid
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    update savingaccount set balance = balance + var_amount where accountID = var_targetid
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    insert into transfer values(var_transferid, var_sourceid, var_targetid, var_amount, var_type, var_ts::timestamp);

    commit;
$$ language 'umbrascript';

create procedure at2(var_checkingid bigint, var_sourceid int, var_targetid int, var_amount_bi bigint, var_type text, var_ts text)
as
$$
    let var_amount = var_amount_bi::real / 100;

    select isblocked as var_isblocked, balance as var_balance
    from checkingaccount where accountID = var_sourceid;

    if var_balance < var_amount or var_isblocked = 1 {
        rollback;
        return;
    }

    select count(*) as var_count from checking ch, checkingaccount ca
    where ch.sourceid = var_sourceid and ca.isblocked = 1 and ch.targetid = ca.accountid;

    if var_count > 0 {
        rollback;
        return;
    }

    update checkingaccount set balance = balance - var_amount where accountID = var_sourceid
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    update checkingaccount set balance = balance + var_amount where accountID = var_targetid
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    insert into checking values(var_checkingid, var_sourceid, var_targetid, var_amount, var_type, var_ts::timestamp);

    commit;
$$ language 'umbrascript';

create procedure at3(var_loantransid bigint, var_end_ts text)
as
$$
    select applicantid as var_applicantid, id as var_appid, timestamp as var_ts, duration as var_duration
    from loanapps where status = 'under_review' order by random() limit 1;
    let var_current_ts = random_date(var_ts, var_end_ts::timestamp);

    select isblocked var_isblocked from savingaccount WHERE accountID = var_applicantid;
    if var_isblocked = 1 {
        update loanapps set status = 'reject', timestamp = var_current_ts where id = var_appid
        catch serialization_failure {
            raise notice 'serialization failure';
            return;
        }
    } else {
        select (inbound.t_amount-outbound.t_amount) as var_amount from
            (select sa.accountid as sid, sum(t.amount) as t_amount from savingaccount sa, transfer t
            where sa.accountid = var_applicantid and sa.accountid = t.sourceid group by sid) as outbound,
            (select sa.accountid as sid, sum(t.amount) as t_amount from savingaccount sa, transfer t
            where sa.accountid = var_applicantid and sa.accountid=t.targetid group by sid) as inbound;

        if var_amount < 0 {
           update loanapps set status = 'reject', timestamp = var_current_ts where id = var_appid
           catch serialization_failure {
               raise notice 'serialization failure';
               return;
           }
        } else {
            insert into loantrans values(var_loantransid, var_applicantid, var_appid, var_amount, 'accept', var_ts::timestamp, var_duration, var_current_ts);

            update loanapps set status = 'accept', timestamp = var_current_ts where id = var_appid
            catch serialization_failure {
                raise notice 'serialization failure';
                return;
            }
        }
    }

    commit;
$$ language 'umbrascript';

create procedure at4(var_end_ts text)
as
$$
    select applicantid as var_applicantid, amount as var_amount, id as var_loantransid, timestamp as var_ts
    from loantrans where status = 'accept' order by random() limit 1;
    let var_current_ts = random_date(var_ts, var_end_ts::timestamp);

    select count(distinct sa2.accountid) as var_count from transfer t, savingAccount sa1, savingAccount sa2, loantrans lt
    where sa1.accountid = var_applicantid and sa1.accountid = t.targetid and t.sourceid = sa2.accountid and lt.applicantid = sa2.accountid and lt.status = 'accept';

    if var_count > 5 {
        rollback;
        return;
    }

    update savingaccount set balance = balance + var_amount where accountid = var_applicantid
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    update loantrans set status = 'lent', timestamp = var_current_ts where id = var_loantransid
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    commit;
$$ language 'umbrascript';

create procedure at5()
as
$$
    select applicantid as var_applicantid from loantrans where status='lent' order by random() limit 1 {
        select id as var_loantransid from loantrans where applicantid = var_applicantid and contract_timestamp::date + duration < cast('2024-01-01' as date) and status='lent' {
            update loantrans set delinquency=1 where id = var_loantransid
            catch serialization_failure {
                raise notice 'serialization failure';
                return;
            }
        }
    }

    commit;
$$ language 'umbrascript';

create procedure at6()
as
$$
    select applicantid as var_applicantid from loantrans where status = 'lent' and delinquency=1 order by random() limit 1 {
        select count(id) as var_count from loantrans where contract_timestamp::date + duration + 30 < cast('2024-01-01' as date) and applicantid = var_applicantid;

        if var_count > 0 {
            update savingaccount set isblocked = 1 where accountid = var_applicantid
            catch serialization_failure {
                raise notice 'serialization failure';
                return;
            }

            update checkingaccount set isblocked = 1 where accountid = var_applicantid
            catch serialization_failure {
                raise notice 'serialization failure';
                return;
            }

            insert into blockedaccounts values (var_applicantid);
        }
    }

    commit;
$$ language 'umbrascript';
