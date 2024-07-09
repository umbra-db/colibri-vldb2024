select co.companyid,
       cu.custid,
       sum(t.amount)   as invest_amount,
       count(t.amount) as count,
       avg(sa.balance) as saving_balance,
       avg(ca.balance) as checking_balance,
       avg(la.amount)  as avg_loan_amount
from company co,
     transfer t,
     customer cu,
     savingaccount sa,
     loanapps la,
     checkingaccount ca
where t.targetid = co.companyid
  and t.sourceid = cu.custid
  and cu.custid = sa.userid
  and cu.custid = ca.userid
  and la.applicantid = cu.custid
  and t.type = 'invest'
  and sa.balance > 1000
  and ca.balance > 1000
  and co.category in ('software_IT', 'internet_service', 'telecommunication', 'technology_service', 'computer_communication_manufacturing')
group by co.companyid, cu.custid
order by invest_amount desc, count desc, saving_balance desc, checking_balance desc, avg_loan_amount desc;
