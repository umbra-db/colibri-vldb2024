copy part from '/data/part.tbl' delimiter '|';
copy supplier from '/data/supplier.tbl' delimiter '|';
copy partsupp from '/data/partsupp.tbl' delimiter '|';
copy customer from '/data/customer.tbl' delimiter '|';
copy orders from '/data/orders.tbl' delimiter '|';
copy lineitem from '/data/lineitem.tbl' delimiter '|';
copy nation from '/data/nation.tbl' delimiter '|';
copy region from '/data/region.tbl' delimiter '|';

select google_columnar_engine_add('part');
select google_columnar_engine_add('supplier');
select google_columnar_engine_add('partsupp');
select google_columnar_engine_add('customer');
select google_columnar_engine_add('orders');
select google_columnar_engine_add('lineitem');
select google_columnar_engine_add('nation');
select google_columnar_engine_add('region');
