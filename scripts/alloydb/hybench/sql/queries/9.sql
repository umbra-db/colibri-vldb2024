select c.name, sum(t.amount) as total_revenue
from company c,
     transfer t
where c.companyid = t.targetid
  and exists (select *
              from loanapps la
              where la.applicantid = c.companyid
                and timestamp between '2023-11-01' and (cast('2023-11-01' as date) + 60))
  and not exists (select *
                  from loantrans lt
                  where lt.applicantid = c.companyid
                    and timestamp between '2023-11-01' and (cast('2023-11-01' as date) + 60))
group by c.name
order by total_revenue desc;
