copy customer from '/data/customer.csv' delimiter ',' null 'null';
copy company from '/data/company.csv' delimiter ',' null 'null';
copy savingaccount from '/data/savingaccount.csv' delimiter ',' null 'null';
copy checkingaccount from '/data/checkingaccount.csv' delimiter ',' null 'null';
copy transfer from '/data/transfer.csv' delimiter ',' null 'null';
copy checking from '/data/checking.csv' delimiter ',' null 'null';
copy loanapps from '/data/loanapps.csv' delimiter ',' null 'null';
copy loantrans from '/data/loantrans.csv' delimiter ',' null 'null';

SELECT google_columnar_engine_add('customer');
SELECT google_columnar_engine_add('company');
SELECT google_columnar_engine_add('savingaccount');
SELECT google_columnar_engine_add('checkingaccount');
SELECT google_columnar_engine_add('transfer');
SELECT google_columnar_engine_add('checking');
SELECT google_columnar_engine_add('loanapps');
SELECT google_columnar_engine_add('loantrans');
