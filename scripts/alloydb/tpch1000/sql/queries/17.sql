with avg_lineitem as (select l_partkey as a_partkey, 0.2 * avg(l_quantity) as a_avg
                      from lineitem
                      group by l_partkey)

select sum(l_extendedprice) / 7.0 as avg_yearly
from lineitem,
     part,
     avg_lineitem
where p_partkey = l_partkey
  and p_brand = 'Brand#23'
  and p_container = 'MED BOX'
  and l_partkey = a_partkey
  and l_quantity < a_avg;
