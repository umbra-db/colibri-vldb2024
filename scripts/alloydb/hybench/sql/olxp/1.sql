select t.timestamp, t.amount, c.custid, c.name
from transfer t,
     customer c
where t.sourceid = $1
  and t.targetid = c.custid
union
select t.timestamp, t.amount, c.custid, c.name
from transfer t,
     customer c
where t.targetid = $1
  and t.sourceid = c.custid
order by timestamp
limit 10;
