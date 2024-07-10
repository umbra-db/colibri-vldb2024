select i_name,
       substr(i_data, 1, 3)                            as brand,
       i_price,
       count(distinct (mod((s_w_id * s_i_id), 10000))) as supplier_cnt
from stock,
     item
where i_id = s_i_id
  and i_data not like 'zz%'
  and (mod((s_w_id * s_i_id), 10000) not in
       (select su_suppkey
        from supplier
        where su_comment like '%bad%'))
group by i_name, substr(i_data, 1, 3), i_price
order by supplier_cnt desc
