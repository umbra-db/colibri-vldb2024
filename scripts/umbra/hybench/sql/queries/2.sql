select cu.custid, la.timestamp, la.amount, lt.timestamp, lt.amount, lt.status
from customer cu,
     loanapps la,
     loantrans lt
where cu.custid = la.applicantid
  and la.id = lt.appid
  and cu.custid = $1;
