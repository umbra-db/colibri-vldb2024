with abnormal_accounts as
         (select distinct(applicantid) as id, amount
          from loantrans lt1
          where contract_timestamp is not null and cast(contract_timestamp as date) + duration < cast('2024-01-01' as date)
          order by amount desc
          limit 100)
select aa.id, lt2.amount, lt2.timestamp, lt2.status, la.timestamp
from loantrans lt2,
     loanapps la,
     abnormal_accounts aa
where aa.id = lt2.applicantid
  and la.id = lt2.appid;
