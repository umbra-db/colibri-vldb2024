COPY item FROM '/data/item.csv' DELIMITER ',';
COPY warehouse FROM '/data/warehouse.csv' DELIMITER ',';
COPY stock FROM '/data/stock.csv' DELIMITER ',';
COPY district FROM '/data/district.csv' DELIMITER ',';
COPY customer FROM '/data/customer.csv' DELIMITER ',';
COPY history FROM '/data/history.csv' DELIMITER ',';
COPY orders FROM '/data/orders.csv' DELIMITER ',' NULL '';
COPY order_line FROM '/data/order_line.csv' DELIMITER ',' NULL '';
COPY new_orders FROM '/data/new_orders.csv' DELIMITER ',';
