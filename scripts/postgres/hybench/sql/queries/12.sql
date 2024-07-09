select co.companyid, count(case when lt.status = 'accept' then 1 else null end) * 100.0 / count(la.id) as rate
from company co,
     loanapps la,
     loantrans lt
where co.category = $1
  and co.companyid = la.applicantid
  and la.id = lt.appid
group by co.companyid
order by rate desc
limit 10;
