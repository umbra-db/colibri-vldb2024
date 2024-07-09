COPY subscriber FROM '/data/subscriber.csv' DELIMITER ',';
COPY access_info FROM '/data/access_info.csv' DELIMITER ',';
COPY special_facility FROM '/data/special_facility.csv' DELIMITER ',';
COPY call_forwarding FROM '/data/call_forwarding.csv' DELIMITER ',';

SELECT google_columnar_engine_add('subscriber');
SELECT google_columnar_engine_add('access_info');
SELECT google_columnar_engine_add('special_facility');
SELECT google_columnar_engine_add('call_forwarding');
