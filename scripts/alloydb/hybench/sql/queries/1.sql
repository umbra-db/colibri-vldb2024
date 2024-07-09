select sourceid,
       targetid,
       case when sourceid = $1 then 'outbound' when targetid = $1 then 'inbound' end as direction,
       sum(amount)                                                                   as total_amount
from transfer
where sourceid = $1
   or targetid = $1
group by sourceid, targetid
order by total_amount desc;
