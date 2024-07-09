select ch.timestamp, ch.amount, co.companyid, co.name
from checking ch,
     company co
where ch.sourceid = $1
  and ch.targetid = co.companyid
union
select ch.timestamp, ch.amount, co.companyid, co.name
from checking ch,
     company co
where ch.targetid = $1
  and ch.sourceid = co.companyid
order by timestamp
limit 10;
