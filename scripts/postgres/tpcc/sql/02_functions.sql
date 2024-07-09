CREATE OR REPLACE FUNCTION random(upper INTEGER) RETURNS INTEGER
AS
$$
BEGIN
    RETURN trunc(random() * upper);
END
$$ LANGUAGE 'plpgsql' STRICT;

CREATE OR REPLACE FUNCTION urand(lower INTEGER, upper INTEGER) RETURNS INTEGER
AS
$$
BEGIN
    RETURN random(upper - lower + 1) + lower;
END
$$ LANGUAGE 'plpgsql' STRICT;

CREATE OR REPLACE FUNCTION urandexcept(lower INTEGER, upper INTEGER, v INTEGER) RETURNS INTEGER
AS
$$
DECLARE
    r INTEGER;

BEGIN
    IF upper <= lower THEN
        RETURN lower;
    ELSE
        r := random(upper - lower) + lower;
        RETURN CASE WHEN r >= v THEN r + 1 ELSE r END;
    END IF;
END
$$ LANGUAGE 'plpgsql' STRICT
                      STABLE;

CREATE OR REPLACE FUNCTION nurand(a INTEGER, lower INTEGER, upper INTEGER) RETURNS INTEGER
AS
$$
BEGIN
    RETURN (((random(a) | (random(upper - lower + 1) + lower)) + 42) % (upper - lower + 1)) + lower;
END
$$ LANGUAGE 'plpgsql' STRICT;

CREATE OR REPLACE FUNCTION namePart(id INTEGER) RETURNS VARCHAR(20)
AS
$$
BEGIN
    RETURN
        CASE id
            WHEN 0 THEN 'BAR'
            WHEN 1 THEN 'OUGHT'
            WHEN 2 THEN 'ABLE'
            WHEN 3 THEN 'PRI'
            WHEN 4 THEN 'PRES'
            WHEN 5 THEN 'ESE'
            WHEN 6 THEN 'ANTI'
            WHEN 7 THEN 'CALLY'
            WHEN 8 THEN 'ATION'
            ELSE 'EING'
            END;
END
$$ LANGUAGE 'plpgsql' STRICT
                      IMMUTABLE;

CREATE OR REPLACE FUNCTION genName(id INTEGER) RETURNS VARCHAR(20)
AS
$$
BEGIN
    RETURN namePart(mod(id / 100, 10)) || namePart(mod(id / 10, 10)) || namePart(mod(id, 10));
END
$$ LANGUAGE 'plpgsql' STRICT
                      IMMUTABLE;

CREATE OR REPLACE PROCEDURE delivery(var_w_id INTEGER)
AS
$$
DECLARE
    var_carrier_id    INTEGER;
    var_delivery_d    DATE;
    var_o_id          INTEGER;
    var_c_id          INTEGER;
    var_sum_ol_amount INTEGER;
BEGIN
    BEGIN
        var_carrier_id := urand(1, 10);
        var_delivery_d := now();

        FOR d_d_id IN 1..10
            LOOP
                SELECT no_o_id
                INTO var_o_id
                FROM new_orders
                WHERE no_w_id = var_w_id
                  AND no_d_id = d_d_id
                ORDER BY no_o_id ASC
                LIMIT 1;

                SELECT o_c_id
                INTO var_c_id
                FROM orders
                WHERE o_id = var_o_id
                  AND o_d_id = d_d_id
                  AND o_w_id = var_w_id;

                SELECT SUM(ol_amount)
                INTO var_sum_ol_amount
                FROM order_line
                WHERE ol_o_id = var_o_id
                  AND ol_d_id = d_d_id
                  AND ol_w_id = var_w_id;

                DELETE
                FROM new_orders
                WHERE no_o_id = var_o_id
                  AND no_d_id = d_d_id
                  AND no_w_id = var_w_id;

                UPDATE orders
                SET o_carrier_id = var_carrier_id
                WHERE o_id = var_o_id
                  AND o_d_id = d_d_id
                  AND o_w_id = var_w_id;

                UPDATE order_line
                SET ol_delivery_d = var_delivery_d
                WHERE ol_o_id = var_o_id
                  AND ol_d_id = d_d_id
                  AND ol_w_id = var_w_id;

                UPDATE customer
                SET c_balance      = c_balance + var_sum_ol_amount,
                    c_delivery_cnt = c_delivery_cnt + 1
                WHERE c_id = var_c_id
                  AND c_d_id = d_d_id
                  AND c_w_id = var_w_id;
            END LOOP;
    EXCEPTION
        WHEN serialization_failure OR deadlock_detected OR no_data_found
            THEN
                ROLLBACK;
    END;

    COMMIT;
END
$$ LANGUAGE 'plpgsql';


CREATE OR REPLACE PROCEDURE newOrder(var_w_id INTEGER, var_warehouse_count INTEGER, var_generate_remote_accesses INTEGER,
                                     var_generate_rollbacks INTEGER)
AS
$$
DECLARE
    var_d_id            INTEGER;
    var_c_id            INTEGER;
    var_o_ol_cnt        INTEGER;
    var_o_all_local     INTEGER DEFAULT 1;
    var_should_rollback BOOLEAN;
    var_ol_i_id         INTEGER;
    var_ol_supply_w_id  INTEGER;
    var_ol_quantity     INTEGER;
    var_w_tax           NUMERIC(6, 2);
    var_d_next_o_id     INTEGER;
    var_d_tax           NUMERIC(6, 2);
    var_c_discount      NUMERIC(6, 2);
    var_c_last          CHAR(16);
    var_c_credit        CHAR(2);
    var_position        RECORD;
    var_positions       order_position[];
    var_i_price         NUMERIC(6, 2);
    var_s_quantity      INTEGER;
    var_s_dist_info     CHAR(24);
    var_s_ytd           INTEGER;
    var_s_order_cnt     INTEGER;
    var_s_remote_cnt    INTEGER;
BEGIN
    BEGIN
        var_d_id = urand(1, 10);
        var_c_id = nurand(1023, 1, 3000);
        var_o_ol_cnt = urand(5, 15);
        var_should_rollback = (var_generate_rollbacks > 0) AND (urand(1, 100) <= 1);

        FOR var_ol_number IN 1..var_o_ol_cnt
            LOOP
                var_ol_i_id := 0;
                var_ol_supply_w_id := var_w_id;
                var_ol_quantity := urand(1, 10);

                IF (var_ol_number = var_o_ol_cnt) AND var_should_rollback THEN
                    var_ol_i_id := 100001;
                ELSE
                    var_ol_i_id := nurand(8191, 1, 100000);
                END IF;

                IF (var_generate_remote_accesses > 0) AND (urand(1, 100) <= 1) THEN
                    var_ol_supply_w_id := urandexcept(1, var_warehouse_count, var_w_id);
                    var_o_all_local := 0;
                END IF;

                var_positions := array_append(var_positions, ROW (var_ol_i_id, var_ol_number, var_ol_supply_w_id, var_ol_quantity)::order_position);
            END LOOP;

        SELECT w_tax
        INTO var_w_tax
        FROM warehouse
        WHERE warehouse.w_id = var_w_id;

        SELECT d_next_o_id, d_tax
        INTO var_d_next_o_id, var_d_tax
        FROM district
        WHERE district.d_id = var_d_id
          AND district.d_w_id = var_w_id;

        SELECT c_discount, c_last, c_credit
        INTO var_c_discount, var_c_last, var_c_credit
        FROM customer
        WHERE customer.c_id = var_c_id
          AND customer.c_d_id = var_d_id
          AND customer.c_w_id = var_w_id;

        UPDATE district
        SET d_next_o_id = var_d_next_o_id + 1
        WHERE district.d_id = var_d_id
          AND district.d_w_id = var_w_id;

        INSERT INTO orders (o_id, o_d_id, o_w_id, o_c_id, o_entry_d, o_ol_cnt, o_all_local)
        VALUES (var_d_next_o_id, var_d_id, var_w_id, var_c_id, NOW(), var_o_ol_cnt, var_o_all_local);

        INSERT INTO new_orders (no_o_id, no_d_id, no_w_id)
        VALUES (var_d_next_o_id, var_d_id, var_w_id);

        FOREACH var_position IN ARRAY var_positions
            LOOP
                SELECT i_price
                INTO var_i_price
                FROM item
                WHERE item.i_id = var_ol_i_id;

                SELECT s_quantity,
                       CASE var_d_id
                           WHEN 1 THEN s_dist_01
                           WHEN 2 THEN s_dist_02
                           WHEN 3 THEN s_dist_03
                           WHEN 4 THEN s_dist_04
                           WHEN 5 THEN s_dist_05
                           WHEN 6 THEN s_dist_06
                           WHEN 7 THEN s_dist_07
                           WHEN 8 THEN s_dist_08
                           WHEN 9 THEN s_dist_09
                           WHEN 10 THEN s_dist_10
                           END,
                       s_ytd,
                       s_order_cnt,
                       s_remote_cnt
                INTO var_s_quantity, var_s_dist_info, var_s_ytd, var_s_order_cnt, var_s_remote_cnt
                FROM stock
                WHERE stock.s_w_id = var_position.ol_supply_w_id
                  AND stock.s_i_id = var_position.ol_i_id;

                var_s_quantity := CASE
                                      WHEN var_s_quantity >= var_position.ol_quantity + 10
                                          THEN var_s_quantity - var_position.ol_quantity
                                      ELSE var_s_quantity + 91 - var_position.ol_quantity END;
                var_s_ytd = var_s_ytd + var_position.ol_quantity;
                var_s_remote_cnt = var_s_remote_cnt + CASE WHEN var_position.ol_supply_w_id <> var_w_id THEN 1 ELSE 0 END;
                var_s_order_cnt = var_s_order_cnt + 1;

                UPDATE stock
                SET s_quantity   = var_s_quantity,
                    s_remote_cnt = var_s_remote_cnt,
                    s_order_cnt  = var_s_order_cnt,
                    s_ytd        = var_s_ytd
                WHERE stock.s_w_id = var_position.ol_supply_w_id
                  AND stock.s_i_id = var_position.ol_i_id;

                INSERT INTO order_line (ol_o_id, ol_d_id, ol_w_id, ol_number, ol_i_id, ol_supply_w_id, ol_quantity, ol_amount, ol_dist_info)
                VALUES (var_d_next_o_id, var_d_id, var_w_id, var_position.ol_number, var_position.ol_i_id, var_position.ol_supply_w_id, var_position.ol_quantity, var_position.ol_quantity * var_i_price, var_s_dist_info);
            END LOOP;
    EXCEPTION
        WHEN serialization_failure OR deadlock_detected OR no_data_found
            THEN
                ROLLBACK;
    END;

    IF var_should_rollback THEN
        ROLLBACK;
    ELSE
        COMMIT;
    END IF;
END
$$ LANGUAGE 'plpgsql';


CREATE OR REPLACE PROCEDURE orderStatus(var_w_id INTEGER)
AS
$$
DECLARE
    var_c_id         INTEGER;
    var_d_id         INTEGER;
    var_customer     RECORD;
    var_c_first      CHAR(16);
    var_c_middle     CHAR(2);
    var_c_last       CHAR(16);
    var_c_balance    NUMERIC(12, 2);
    var_o_id         INTEGER;
    var_o_entry_d    DATE;
    var_o_carrier_id INTEGER;
    var_order_line   RECORD;
BEGIN
    BEGIN
        var_d_id := urand(1, 10);

        IF urand(1, 100) <= 60 THEN
            -- order status by name
            var_c_last := genName(nurand(255, 0, 999));

            -- as requested by TPC-C, find the middle customer
            FOR var_customer IN
                SELECT customer.c_id
                FROM customer
                WHERE customer.c_last = var_c_last
                  AND customer.c_d_id = var_d_id
                  AND customer.c_w_id = var_w_id
                ORDER BY c_first ASC
                LOOP
                    var_c_id = var_customer.c_id;
                END LOOP;
        ELSE
            -- order status by id
            var_c_id := nurand(1023, 1, 3000);
        END IF;

        SELECT c_first, c_middle, c_last, c_balance
        INTO var_c_first, var_c_middle, var_c_last, var_c_balance
        FROM customer
        WHERE customer.c_id = var_c_id
          AND customer.c_d_id = var_d_id
          AND customer.c_w_id = var_w_id;

        SELECT o_id,
               o_entry_d,
               o_carrier_id
        INTO var_o_id, var_o_entry_d, var_o_carrier_id
        FROM orders
        WHERE orders.o_c_id = var_c_id
          AND orders.o_d_id = var_d_id
          AND orders.o_w_id = var_w_id
        ORDER BY o_id ASC
        LIMIT 1;

        FOR var_order_line IN
            SELECT ol_i_id, ol_supply_w_id, ol_quantity, ol_amount, ol_delivery_d
            FROM order_line
            WHERE order_line.ol_o_id = var_o_id
              AND order_line.
                      ol_d_id = var_d_id
              AND order_line.ol_w_id = var_w_id
            LOOP
                -- do nothing, we just have to retrieve data
            END LOOP;

    EXCEPTION
        WHEN serialization_failure OR deadlock_detected OR no_data_found
            THEN
                ROLLBACK;
    END;

    COMMIT;
END
$$ LANGUAGE 'plpgsql';


CREATE OR REPLACE PROCEDURE payment(var_w_id INTEGER, var_warehouse_count INTEGER, var_generate_remote_accesses INTEGER)
AS
$$
DECLARE
    var_d_id          INTEGER;
    var_c_id          INTEGER;
    var_c_w_id        INTEGER;
    var_c_d_id        INTEGER;
    var_h_date        DATE;
    var_h_amount      DECIMAL(6, 2);
    var_customer      RECORD;
    var_w_name        CHAR(10);
    var_w_street_1    CHAR(20);
    var_w_street_2    CHAR(20);
    var_w_city        CHAR(20);
    var_w_state       CHAR(2);
    var_w_zip         CHAR(9);
    var_w_ytd         DECIMAL(12, 2);
    var_d_name        CHAR(10);
    var_d_street_1    CHAR(20);
    var_d_street_2    CHAR(20);
    var_d_city        CHAR(20);
    var_d_state       CHAR(2);
    var_d_zip         CHAR(9);
    var_d_ytd         DECIMAL(12, 2);
    var_c_first       CHAR(16);
    var_c_middle      CHAR(2);
    var_c_last        CHAR(16);
    var_c_street_1    CHAR(20);
    var_c_street_2    CHAR(20);
    var_c_city        CHAR(20);
    var_c_state       CHAR(2);
    var_c_zip         CHAR(9);
    var_c_phone       CHAR(16);
    var_c_since       DATE;
    var_c_credit      CHAR(2);
    var_c_credit_lim  DECIMAL(12, 2);
    var_c_discount    DECIMAL(4, 4);
    var_c_balance     DECIMAL(12, 2);
    var_c_ytd_payment DECIMAL(12, 2);
    var_c_payment_cnt INTEGER;
    var_c_data        VARCHAR(500);
    var_c_new_data    VARCHAR(500);

BEGIN
    BEGIN
        var_d_id = urand(1, 10);

        IF (var_generate_remote_accesses > 0) AND (urand(1, 100) > 85) THEN
            -- remote customer
            var_c_w_id = urandexcept(1, var_warehouse_count, var_w_id);
            var_c_d_id = urand(1, 10);
        ELSE
            -- local customer
            var_c_w_id = var_w_id;
            var_c_d_id = var_d_id;
        END IF;

        var_h_date = now();
        var_h_amount = 0.01 * urand(100, 500000);

        IF urand(1, 100) <= 60 THEN
            -- payment by name
            var_c_last = genName(nurand(255, 0, 999));

            -- as requested by TPC-C, find the middle customer
            FOR var_customer IN
                SELECT customer.c_id
                FROM customer
                WHERE customer.c_last = var_c_last
                  AND customer.c_d_id = var_d_id
                  AND customer.c_w_id = var_w_id
                ORDER BY c_first ASC
                LOOP
                    var_c_id = var_customer.c_id;
                END LOOP;
        ELSE
            -- payment by id
            var_c_id = nurand(1023, 1, 3000);
        END IF;

        SELECT w_name, w_street_1, w_street_2, w_city, w_state, w_zip, w_ytd
        INTO var_w_name, var_w_street_1, var_w_street_2, var_w_city, var_w_state, var_w_zip, var_w_ytd
        FROM warehouse
        WHERE warehouse.w_id = var_w_id;

        var_w_ytd = var_w_ytd + var_h_amount;

        UPDATE warehouse
        SET w_ytd = var_w_ytd
        WHERE warehouse.w_id = var_w_id;

        SELECT d_name, d_street_1, d_street_2, d_city, d_state, d_zip, d_ytd
        INTO var_d_name, var_d_street_1, var_d_street_2, var_d_city, var_d_state, var_d_zip, var_d_ytd
        FROM district
        WHERE district.d_id = var_d_id
          AND district.d_w_id = var_w_id;

        var_d_ytd = var_d_ytd + var_h_amount;

        UPDATE district
        SET d_ytd = var_d_ytd
        WHERE district.d_id = var_d_id
          AND district.d_w_id = var_w_id;

        SELECT c_first,
               c_middle,
               c_last,
               c_street_1,
               c_street_2,
               c_city,
               c_state,
               c_zip,
               c_phone,
               c_since,
               c_credit,
               c_credit_lim,
               c_discount,
               c_balance,
               c_ytd_payment,
               c_payment_cnt
        INTO var_c_first, var_c_middle, var_c_last, var_c_street_1, var_c_street_2, var_c_city, var_c_state, var_c_zip, var_c_phone, var_c_since, var_c_credit, var_c_credit_lim, var_c_discount, var_c_balance, var_c_ytd_payment, var_c_payment_cnt
        FROM customer
        WHERE customer.c_id = var_c_id
          AND customer.c_d_id = var_c_d_id
          AND customer.c_w_id = var_c_w_id;

        var_c_balance = var_c_balance - var_h_amount;
        var_c_ytd_payment = var_c_ytd_payment + var_h_amount;
        var_c_payment_cnt = var_c_payment_cnt + 1;

        if var_c_credit = 'BC' THEN
            SELECT c_data
            INTO var_c_data
            FROM customer
            WHERE customer.c_id = var_c_id
              AND customer.c_d_id = var_c_d_id
              AND customer.c_w_id = var_c_w_id;

            var_c_new_data = var_c_id || ' ' || var_c_d_id || ' ' || var_c_w_id || ' ' || var_d_id || ' ' || var_w_id || ' ' || TO_CHAR(var_h_amount, '9999.99') || ' ' || var_h_date || ' ' || var_w_name || ' ' || var_d_name;
            var_c_new_data = SUBSTRING(var_c_data || var_c_new_data FROM 1 FOR 500);

            UPDATE customer
            SET c_balance     = var_c_balance,
                c_ytd_payment = var_c_ytd_payment,
                c_payment_cnt = var_c_payment_cnt,
                c_data        = var_c_new_data
            WHERE customer.c_id = var_c_id
              AND customer.c_d_id = var_c_d_id
              AND customer.c_w_id = var_c_w_id;
        ELSE
            UPDATE customer
            SET c_balance     = var_c_balance,
                c_ytd_payment = var_c_ytd_payment,
                c_payment_cnt = var_c_payment_cnt
            WHERE customer.c_id = var_c_id
              AND customer.c_d_id = var_c_d_id
              AND customer.c_w_id = var_c_w_id;
        END IF;

        INSERT INTO history (h_c_id, h_c_d_id, h_c_w_id, h_d_id, h_w_id, h_date, h_amount, h_data)
        VALUES (var_c_id, var_c_d_id, var_c_w_id, var_d_id, var_w_id, var_h_date, var_h_amount, var_w_name || ' ' || var_d_name);
    EXCEPTION
        WHEN serialization_failure OR deadlock_detected OR no_data_found
            THEN
                ROLLBACK;
    END;

    COMMIT;
END
$$ LANGUAGE 'plpgsql';


CREATE OR REPLACE PROCEDURE stockLevel(var_w_id INTEGER)
AS
$$
DECLARE
    var_d_id        INTEGER;
    var_threshold   INTEGER;
    var_d_next_o_id INTEGER;
    var_order_line  RECORD;
    var_count       INTEGER;
BEGIN
    BEGIN
        var_d_id = urand(1, 10);
        var_threshold = urand(10, 20);

        SELECT d_next_o_id
        INTO var_d_next_o_id
        FROM district
        WHERE district.d_id = var_d_id
          AND district.d_w_id = var_w_id;

        -- FIXME: ol_i_id should be deduplicated but our group by is slow on small inputs
        FOR var_order_line IN
            SELECT ol_i_id
            FROM order_line
            WHERE order_line.ol_d_id = var_d_id
              AND order_line.ol_w_id = var_w_id
              AND order_line.ol_o_id >= var_d_next_o_id - 20
            LOOP
                SELECT CASE WHEN s_quantity < var_threshold THEN 1 ELSE 0 END
                INTO var_count
                FROM stock
                WHERE stock.s_w_id = var_w_id
                  AND stock.s_i_id = var_order_line.ol_i_id;
            END LOOP;
    EXCEPTION
        WHEN serialization_failure OR deadlock_detected OR no_data_found
            THEN
                ROLLBACK;
    END;

    COMMIT;
END
$$ LANGUAGE 'plpgsql';