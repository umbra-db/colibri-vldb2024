create procedure orderstatus(var_w_id integer)
as
$$
    let mut var_c_id : INTEGER = 0;
    let var_d_id : INTEGER;

    var_d_id = urand(1,10);

    if urand(1,100) <= 60 {
        -- order status by name
        let var_c_last = genName(nurand(255, 0, 999));

        -- as requested by TPC-C, find the middle customer
        SELECT customer.c_id AS var_it
        FROM customer
        WHERE customer.c_last = var_c_last
          AND customer.c_d_id = var_d_id
          AND customer.c_w_id = var_w_id
        ORDER BY c_first ASC {
            var_c_id = var_it;
        } when no_data_found {
            raise error 'no customer found';
        }
    } else {
        -- order status by id
        var_c_id = nurand(1023, 1, 3000);
    }

    SELECT c_first AS var_c_first,
           c_middle AS var_c_middle,
           c_last AS var_c_last,
           c_balance AS var_c_balance
    FROM customer
    WHERE customer.c_id = var_c_id
      AND customer.c_d_id = var_d_id
      AND customer.c_w_id = var_w_id;

    SELECT o_id AS var_o_id,
           o_entry_d AS var_o_entry_d,
           o_carrier_id AS var_o_carrier_id
    FROM orders
    WHERE orders.o_c_id = var_c_id
      AND orders.o_d_id = var_d_id
      AND orders.o_w_id = var_w_id
    ORDER BY o_id ASC
    LIMIT 1;

    SELECT ol_i_id AS var_o_i_id,
           ol_supply_w_id AS var_ol_supply_w_id,
           ol_quantity AS var_ol_quantity,
           ol_amount AS var_ol_amount,
           ol_delivery_d AS var_ol_delivery_d
    FROM order_line
    WHERE order_line.ol_o_id = var_o_id
      AND order_line.ol_d_id = var_d_id
      AND order_line.ol_w_id = var_w_id {
      -- do nothing, we just have to retrieve data
    }

    COMMIT;
$$ language 'umbrascript';
