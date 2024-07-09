select custid, name, phone, sum(amount)
from (select cu.custid, cu.name, cu.phone, t.amount
      from customer cu
               join transfer t on cu.custid = t.targetid
      where custid = $1
      union all
      select cu.custid, cu.name, cu.phone, ch.amount
      from customer cu
               join checking ch on cu.custid = ch.targetid
      where cu.custid = $1) income
group by custid, name, phone;
