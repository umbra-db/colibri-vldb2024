(select co.name, t.amount, t.timestamp
 from company co,
      transfer t
 where t.sourceid = $1
   and t.type = 'invest'
   and t.targetid = co.companyid)
union
(select cu.name, t.amount, t.timestamp
 from customer cu,
      transfer t
 where t.sourceid = $1
   and t.type = 'invest'
   and t.targetid = cu.custid);
