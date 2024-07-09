select t.amount, t.targetid, co.name, cu.name
from loantrans lt
         left join transfer t on lt.applicantid = t.sourceid
         left join customer cu on t.targetid = cu.custid
         left join company co on t.targetid = co.companyid
where t.sourceid = $1
  and lt.delinquency = 1;
