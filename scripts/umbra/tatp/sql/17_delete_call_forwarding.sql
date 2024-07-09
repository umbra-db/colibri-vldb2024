CREATE PROCEDURE delete_call_forwarding(subscriber_count INTEGER)
AS
$$
    let var_sub_nbr : TEXT;
    let var_sf_type : SMALLINT;
    let var_start_time : SMALLINT;

    var_sub_nbr = subrand(subscriber_count);
    var_sf_type = CAST (urand(1, 4) AS SMALLINT);
    var_start_time = CAST (8 * urand(0, 2) AS SMALLINT);

    SELECT s_id as var_s_id
    FROM subscriber
    WHERE subscriber.sub_nbr = var_sub_nbr;

    DELETE
    FROM call_forwarding
    WHERE call_forwarding.s_id = var_s_id
      AND call_forwarding.sf_type = var_sf_type
      AND call_forwarding.start_time = var_start_time
    CATCH serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    COMMIT;
$$ LANGUAGE 'umbrascript';
