CREATE PROCEDURE update_subscriber_data(subscriber_count INTEGER)
AS
$$
    let var_s_id : INTEGER;
    let var_sf_type : SMALLINT;
    let var_bit_1 : SMALLINT;
    let var_data_a : SMALLINT;
    let mut var_should_rollback : BOOLEAN = false;

    var_s_id = urand(1, subscriber_count);
    var_sf_type = CAST (urand(1, 4) AS SMALLINT);
    var_bit_1 = CAST (urand(0, 1) AS SMALLINT);
    var_data_a = CAST (urand(0, 255) AS SMALLINT);

    UPDATE subscriber
       SET bit_1 = var_bit_1
     WHERE subscriber.s_id = var_s_id
    CATCH serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    UPDATE special_facility
       SET data_a = var_data_a
     WHERE special_facility.s_id = var_s_id
       AND special_facility.sf_type = var_sf_type
    WHEN no_data_found {
        var_should_rollback = true;
    } CATCH serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    if var_should_rollback {
        ROLLBACK;
    } else {
        COMMIT;
    }
$$ LANGUAGE 'umbrascript';
