select sourceid, targetid, province, amount, rank() over (partition by province order by amount desc) as ranking
from transfer tr,
     customer cu
where tr.targetid = cu.custid
  and tr.sourceid = $1;
