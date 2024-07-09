create procedure stocklevel(var_w_id integer)
as
$$
    let var_d_id : INTEGER;
    let var_threshold : INTEGER;

    var_d_id = urand(1, 10);
    var_threshold = urand(10, 20);

    SELECT d_next_o_id AS var_d_next_o_id
    FROM district
    WHERE district.d_id = var_d_id
      AND district.d_w_id = var_w_id;

    SELECT ol_o_id AS var_ol_o_id FROM generate_series(var_d_next_o_id - 20, var_d_next_o_id - 1) g(ol_o_id) {
    -- FIXME: ol_i_id should be deduplicated but our group by is slow on small inputs
        SELECT ol_i_id AS var_ol_i_id
        FROM order_line
        WHERE order_line.ol_d_id = var_d_id
          AND order_line.ol_w_id = var_w_id
          AND order_line.ol_o_id = CAST (var_ol_o_id AS INTEGER) {

            SELECT CASE WHEN s_quantity < var_threshold THEN 1 ELSE 0 END
            FROM stock
            WHERE stock.s_w_id = var_w_id
              AND stock.s_i_id = var_ol_i_id;
        }
    }

    COMMIT;
$$ language 'umbrascript';
