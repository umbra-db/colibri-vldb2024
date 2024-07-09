copy part from '/data/part.csv' delimiter '!';
copy supplier from '/data/supplier.csv' delimiter '!';
copy customer from '/data/customer.csv' delimiter '!';
copy date from '/data/date.csv' delimiter '!';
copy lineorder from '/data/lineorder.csv' delimiter '!';
copy history from '/data/history.csv' delimiter '!';

SELECT google_columnar_engine_add('part');
SELECT google_columnar_engine_add('customer');
SELECT google_columnar_engine_add('supplier');
SELECT google_columnar_engine_add('date');
SELECT google_columnar_engine_add('lineorder');
SELECT google_columnar_engine_add('history');
