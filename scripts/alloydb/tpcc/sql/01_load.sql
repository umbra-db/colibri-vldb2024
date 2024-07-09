COPY item FROM '/data/item.csv' DELIMITER ',';
COPY warehouse FROM '/data/warehouse.csv' DELIMITER ',';
COPY stock FROM '/data/stock.csv' DELIMITER ',';
COPY district FROM '/data/district.csv' DELIMITER ',';
COPY customer FROM '/data/customer.csv' DELIMITER ',';
COPY history FROM '/data/history.csv' DELIMITER ',';
COPY orders FROM '/data/orders.csv' DELIMITER ',' NULL '';
COPY order_line FROM '/data/order_line.csv' DELIMITER ',' NULL '';
COPY new_orders FROM '/data/new_orders.csv' DELIMITER ',';

SELECT google_columnar_engine_add('item');
SELECT google_columnar_engine_add('warehouse');
SELECT google_columnar_engine_add('stock');
SELECT google_columnar_engine_add('district');
SELECT google_columnar_engine_add('customer');
SELECT google_columnar_engine_add('history');
SELECT google_columnar_engine_add('orders');
SELECT google_columnar_engine_add('order_line');
SELECT google_columnar_engine_add('new_orders');
