with avg_lineitem as (select l_partkey             as a_partkey,
                             l_suppkey             as a_suppkey,
                             0.5 * sum(l_quantity) as a_quantity
                      from lineitem
                      where l_shipdate >= date '1994-01-01'
                        and l_shipdate < date '1994-01-01' + interval '1' year
                      group by l_partkey, l_suppkey)

select s_name,
       s_address
from supplier,
     nation
where s_suppkey in (select ps_suppkey
                    from partsupp,
                         avg_lineitem
                    where ps_partkey in (select p_partkey
                                         from part
                                         where p_name like 'forest%')
                      and a_partkey = ps_partkey
                      and a_suppkey = ps_suppkey
                      and ps_availqty > a_quantity)
  and s_nationkey = n_nationkey
  and n_name = 'CANADA'
order by s_name;
