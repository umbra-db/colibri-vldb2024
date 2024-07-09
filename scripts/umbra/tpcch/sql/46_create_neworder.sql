create procedure neworder(var_w_id integer, var_warehouse_count integer, var_generate_remote_accesses integer,
                          var_generate_rollbacks integer)
as
$$
    let var_d_id : INTEGER;
    let var_c_id : INTEGER;
    let var_o_ol_cnt : INTEGER;
    let mut var_o_all_local : INTEGER = 1;
    let var_should_rollback : BOOL;

    var_d_id = urand(1, 10);
    var_c_id = nurand(1023, 1, 3000);
    var_o_ol_cnt = urand(5, 15);
    var_should_rollback = (var_generate_rollbacks > 0) AND (urand(1,100) <= 1);

    SELECT ol_number AS var_ol_number FROM generate_series(1, var_o_ol_cnt) g(ol_number) {
        let mut var_ol_i_id : INTEGER = 0;
        let mut var_ol_supply_w_id : INTEGER = var_w_id;
        let var_ol_quantity : INTEGER = urand(1, 10);

        if (var_ol_number = var_o_ol_cnt) AND var_should_rollback {
            var_ol_i_id = 100001;
        } else {
            var_ol_i_id = nurand(8191, 1, 100000);
        }

        if (var_generate_remote_accesses > 0) AND (urand(1, 100) <= 1) {
            var_ol_supply_w_id = urandexcept(1, var_warehouse_count, var_w_id);
            var_o_all_local = 0;
        }

        INSERT INTO positions (ol_i_id, ol_number, ol_supply_w_id, ol_quantity)
        VALUES (var_ol_i_id, var_ol_number, var_ol_supply_w_id, var_ol_quantity);
    }

    SELECT w_tax AS var_w_tax
    FROM warehouse
    WHERE warehouse.w_id = var_w_id;

    SELECT d_next_o_id AS var_d_next_o_id,
           d_tax AS var_d_tax
    FROM district
    WHERE district.d_id = var_d_id
      AND district.d_w_id = var_w_id;

    SELECT c_discount AS var_c_discount,
           c_last AS var_c_last,
           c_credit AS var_c_credit
    FROM customer
    WHERE customer.c_id = var_c_id
      AND customer.c_d_id = var_d_id
      AND customer.c_w_id = var_w_id;

    let var_new_d_next_o_id = var_d_next_o_id + 1;

    UPDATE district
    SET d_next_o_id = var_new_d_next_o_id
    WHERE district.d_id = var_d_id
      AND district.d_w_id = var_w_id
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    INSERT INTO orders (o_id, o_d_id, o_w_id, o_c_id, o_entry_d, o_ol_cnt, o_all_local)
    VALUES (var_d_next_o_id, var_d_id, var_w_id, var_c_id, NOW(), var_o_ol_cnt, var_o_all_local)
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    INSERT INTO new_orders (no_o_id, no_d_id, no_w_id)
    VALUES (var_d_next_o_id, var_d_id, var_w_id)
    catch serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    SELECT ol_i_id AS var_ol_i_id,
           ol_number AS var_ol_number,
           ol_supply_w_id AS var_ol_supply_w_id,
           ol_quantity AS var_ol_quantity
    FROM positions {
        SELECT i_price AS var_i_price
          FROM item
         WHERE item.i_id = var_ol_i_id
        when no_data_found {
            continue;
        }

        SELECT s_quantity AS var_s_quantity,
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
               END AS var_s_dist,
               s_ytd AS var_s_ytd,
               s_order_cnt AS var_s_order_cnt,
               s_remote_cnt AS var_s_remote_cnt
        FROM stock
        WHERE stock.s_w_id = var_ol_supply_w_id
          AND stock.s_i_id = var_ol_i_id;

        let var_s_new_quantity = CASE WHEN var_s_quantity >= var_ol_quantity + 10 then var_s_quantity - var_ol_quantity else var_s_quantity + 91 - var_ol_quantity END;
        let var_s_new_remote_cnt = var_s_remote_cnt + CASE WHEN var_ol_supply_w_id <> var_w_id THEN 1 ELSE 0 END;
        let var_s_new_order_cnt = var_s_order_cnt + 1;
        let var_s_new_ytd = var_s_ytd + var_ol_quantity;

        UPDATE stock
        SET s_quantity = var_s_new_quantity,
            s_remote_cnt = var_s_new_remote_cnt,
            s_order_cnt = var_s_new_order_cnt,
            s_ytd = var_s_new_ytd
        WHERE stock.s_w_id = var_ol_supply_w_id
          AND stock.s_i_id = var_ol_i_id
        catch serialization_failure {
            raise notice 'serialization failure';
            return;
        }

        INSERT INTO order_line (ol_o_id, ol_d_id, ol_w_id, ol_number, ol_i_id, ol_supply_w_id, ol_quantity, ol_amount, ol_dist_info)
        VALUES (var_d_next_o_id, var_d_id, var_w_id, var_ol_number, var_ol_i_id, var_ol_supply_w_id, var_ol_quantity, var_ol_quantity * var_i_price, var_s_dist)
        catch serialization_failure {
            raise notice 'serialization failure';
            return;
        }
    }

    if var_should_rollback {
        ROLLBACK;
    } else {
        COMMIT;
    }
$$ language 'umbrascript';
