with blocked_account as (select c.name as name, s.userid as id
                         from customer c,
                              savingaccount s
                         where c.custid = s.userid
                           and (c.isblocked = 1 or s.isblocked = 1)
                         limit 100)
select name, t.sourceid, t.targetid, sum(t.amount) as amount
from blocked_account b,
     transfer t
where b.id = t.sourceid
group by name, t.sourceid, t.targetid
union all
select name, t.sourceid, t.targetid, sum(t.amount) as amount
from blocked_account b,
     transfer t
where b.id = t.targetid
group by name, t.sourceid, t.targetid
union all
select name, c.sourceid, c.targetid, sum(c.amount) as amount
from blocked_account b,
     checking c
where b.id = c.sourceid
group by name, c.sourceid, c.targetid
union all
select name, c.sourceid, c.targetid, sum(c.amount) as amount
from blocked_account b,
     checking c
where b.id = c.targetid
group by name, c.sourceid, c.targetid
order by amount desc;
