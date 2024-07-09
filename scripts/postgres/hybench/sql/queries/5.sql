select cu.custid, sum(tr.amount) as total_amount
from company co,
     transfer tr,
     customer cu
where cu.custid = tr.targetid
  and cu.companyid = co.companyid
  and tr.sourceid = co.companyid
  and co.companyid = $1
  and tr.type = 'salary'
group by cu.custid
order by total_amount desc;
