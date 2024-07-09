alter table lineorder
    add constraint fk_lo_custkey foreign key (lo_custkey) references customer (c_custkey) match full;
alter table lineorder
    add constraint fk_lo_partkey foreign key (lo_partkey) references part (p_partkey) match full;
alter table lineorder
    add constraint fk_lo_suppkey foreign key (lo_suppkey) references supplier (s_suppkey) match full;
alter table lineorder
    add constraint fk_lo_orderdate foreign key (lo_orderdate) references date (d_datekey) match full;
alter table lineorder
    add constraint fk_lo_commitdate foreign key (lo_commitdate) references date (d_datekey) match full;

create index idx_p_category on part using btree (p_category);
create index idx_p_brand1 on part using btree (p_brand1);
create index idx_p_mfgr on part using btree (p_mfgr);
create index idx_s_region on supplier using btree (s_region);
create index idx_c_supplier on supplier using btree (s_nation);
create index idx_c_city on supplier using btree (s_city);
create index idx_c_region on customer using btree (c_region);
create index idx_c_nation on customer using btree (c_nation);
create index idx_c_customer on customer using btree (c_city);
create index idx_d_year on date using btree (d_year);
create index idx_d_yearmonthnum on date using btree (d_yearmonthnum);
create index idx_d_weeknuminyear on date using btree (d_weeknuminyear);
create index idx_lo_quantity on lineorder using btree (lo_quantity);
create index idx_lo_discount on lineorder using btree (lo_discount);
create index idx_lo_orderdate on lineorder using btree (lo_orderdate);
create index idx_lo_partkey on lineorder using btree (lo_partkey);
create index idx_lo_suppkey on lineorder using btree (lo_suppkey);

create index idx_lo_custkey on lineorder using btree (lo_custkey); -- countorders
create index idx_c_name on customer using btree (c_name); -- neworder, countorders
create index idx_s_name on supplier using btree (s_name); -- neworder
create index idx_d_date on date using btree (d_date); -- neworder

analyze lineorder;
analyze customer;
analyze supplier;
analyze part;
analyze history;
analyze date;
