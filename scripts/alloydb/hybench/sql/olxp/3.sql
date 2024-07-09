select co.name, cu.custid, cu.name, sa.isblocked, ca.isblocked
from customer cu,
     company co,
     savingaccount sa,
     checkingaccount ca
where co.companyid = $1
  and cu.companyid = co.companyid
  and sa.accountid = cu.custid
  and ca.accountid = cu.custid;
