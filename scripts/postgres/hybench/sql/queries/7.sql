select tr.sourceid as sourceid, tr.targetid as targetid, tr.type as type, sum(tr.amount) as amount
from company co,
     transfer tr,
     customer cu
where cu.custid = tr.sourceid
  and tr.targetid = co.companyid
  and co.companyid = $1
  and type != 'salary'
group by sourceid, targetid, type
union all
select ch.sourceid as sourceid, ch.targetid as targetid, ch.type as type, sum(ch.amount) as amount
from company co,
     checking ch,
     customer cu
where cu.custid = ch.sourceid
  and ch.targetid = co.companyid
  and co.companyid = $1
  and type != 'salary'
group by sourceid, targetid, type
union all
select tr.sourceid as sourceid, tr.targetid as targetid, tr.type as type, sum(tr.amount) as amount
from company co,
     transfer tr,
     customer cu
where cu.custid = tr.targetid
  and tr.sourceid = co.companyid
  and co.companyid = $1
  and type != 'salary'
group by sourceid, targetid, type
union all
select ch.sourceid as sourceid, ch.targetid as targetid, ch.type as type, sum(ch.amount) as amount
from company co,
     checking ch,
     customer cu
where cu.custid = ch.targetid
  and ch.sourceid = co.companyid
  and co.companyid = $1
  and type != 'salary'
group by sourceid, targetid, type
order by amount desc
    limit 10;
