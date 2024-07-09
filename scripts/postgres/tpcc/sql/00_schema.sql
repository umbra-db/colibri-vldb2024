DROP TABLE IF EXISTS item;
DROP TABLE IF EXISTS warehouse;
DROP TABLE IF EXISTS stock;
DROP TABLE IF EXISTS district;
DROP TABLE IF EXISTS customer;
DROP TABLE IF EXISTS history;
DROP TABLE IF EXISTS orders;
DROP TABLE IF EXISTS order_line;
DROP TABLE IF EXISTS new_orders;
DROP TYPE IF EXISTS order_position;

CREATE TABLE item
(
    i_id    INTEGER NOT NULL,
    i_im_id INTEGER,
    i_name  CHAR(24),
    i_price DECIMAL(5, 2),
    i_data  CHAR(50),
    PRIMARY KEY (i_id)
);

CREATE TABLE warehouse
(
    w_id       INTEGER NOT NULL,
    w_name     CHAR(10),
    w_street_1 CHAR(20),
    w_street_2 CHAR(20),
    w_city     CHAR(20),
    w_state    CHAR(2),
    w_zip      CHAR(9),
    w_tax      DECIMAL(4, 4),
    w_ytd      DECIMAL(12, 2),
    PRIMARY KEY (w_id)
);

CREATE TABLE stock
(
    s_i_id       INTEGER NOT NULL,
    s_w_id       INTEGER NOT NULL,
    s_quantity   NUMERIC(4),
    s_dist_01    CHAR(24),
    s_dist_02    CHAR(24),
    s_dist_03    CHAR(24),
    s_dist_04    CHAR(24),
    s_dist_05    CHAR(24),
    s_dist_06    CHAR(24),
    s_dist_07    CHAR(24),
    s_dist_08    CHAR(24),
    s_dist_09    CHAR(24),
    s_dist_10    CHAR(24),
    s_ytd        INTEGER,
    s_order_cnt  INTEGER,
    s_remote_cnt INTEGER,
    s_data       CHAR(50),
    PRIMARY KEY (s_w_id, s_i_id)
);

CREATE TABLE district
(
    d_id        INTEGER NOT NULL,
    d_w_id      INTEGER NOT NULL,
    d_name      CHAR(10),
    d_street_1  CHAR(20),
    d_street_2  CHAR(20),
    d_city      CHAR(20),
    d_state     CHAR(2),
    d_zip       CHAR(9),
    d_tax       DECIMAL(4, 4),
    d_ytd       DECIMAL(12, 2),
    d_next_o_id INTEGER,
    PRIMARY KEY (d_w_id, d_id)
);

CREATE TABLE customer
(
    c_id           INTEGER NOT NULL,
    c_d_id         INTEGER NOT NULL,
    c_w_id         INTEGER NOT NULL,
    c_first        CHAR(16),
    c_middle       CHAR(2),
    c_last         CHAR(16),
    c_street_1     CHAR(20),
    c_street_2     CHAR(20),
    c_city         CHAR(20),
    c_state        CHAR(2),
    c_zip          CHAR(9),
    c_phone        CHAR(16),
    c_since        DATE,
    c_credit       CHAR(2),
    c_credit_lim   DECIMAL(12, 2),
    c_discount     DECIMAL(4, 4),
    c_balance      DECIMAL(12, 2),
    c_ytd_payment  DECIMAL(12, 2),
    c_payment_cnt  INTEGER,
    c_delivery_cnt INTEGER,
    c_data         VARCHAR(500),
    PRIMARY KEY (c_w_id, c_d_id, c_id)
);

CREATE TABLE history
(
    h_c_id   INTEGER,
    h_c_d_id INTEGER,
    h_c_w_id INTEGER,
    h_d_id   INTEGER,
    h_w_id   INTEGER,
    h_date   DATE,
    h_amount DECIMAL(6, 2),
    h_data   CHAR(24)
);

CREATE TABLE orders
(
    o_id         INTEGER NOT NULL,
    o_c_id       INTEGER NOT NULL,
    o_d_id       INTEGER NOT NULL,
    o_w_id       INTEGER NOT NULL,
    o_entry_d    DATE,
    o_carrier_id INTEGER,
    o_ol_cnt     INTEGER,
    o_all_local  INTEGER,
    PRIMARY KEY (o_w_id, o_d_id, o_id)
);

CREATE TABLE order_line
(
    ol_o_id        INTEGER NOT NULL,
    ol_d_id        INTEGER NOT NULL,
    ol_w_id        INTEGER NOT NULL,
    ol_number      INTEGER NOT NULL,
    ol_i_id        INTEGER,
    ol_supply_w_id INTEGER,
    ol_delivery_d  DATE,
    ol_quantity    INTEGER,
    ol_amount      DECIMAL(6, 2),
    ol_dist_info   CHAR(24),
    PRIMARY KEY (ol_w_id, ol_d_id, ol_o_id, ol_number)
);

CREATE TABLE new_orders
(
    no_o_id INTEGER NOT NULL,
    no_d_id INTEGER NOT NULL,
    no_w_id INTEGER NOT NULL,
    PRIMARY KEY (no_w_id, no_d_id, no_o_id)
);

CREATE TYPE order_position AS
(
    ol_i_id        INTEGER,
    ol_number      INTEGER,
    ol_supply_w_id INTEGER,
    ol_quantity    INTEGER
);

-- postgres does _not_ like indexes on varchar columns
CREATE INDEX ON customer (c_w_id, c_d_id, c_last, c_first, c_id);
CREATE INDEX ON orders (o_w_id, o_d_id, o_c_id, o_id);
