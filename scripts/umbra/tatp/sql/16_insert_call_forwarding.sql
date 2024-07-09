CREATE PROCEDURE insert_call_forwarding(subscriber_count INTEGER)
AS
$$
    let var_sub_nbr : TEXT;
    let var_sf_type : SMALLINT;
    let var_start_time : SMALLINT;
    let var_end_time : SMALLINT;
    let var_numberx : TEXT;

    var_sub_nbr = subrand(subscriber_count);
    var_sf_type = CAST (urand(1, 4) AS SMALLINT);
    var_start_time = CAST (8 * urand(0, 2) AS SMALLINT);
    var_end_time = CAST (urand(1, 24) AS SMALLINT);
    var_numberx = subrand(subscriber_count);

    SELECT s_id as var_s_id
      FROM subscriber
     WHERE subscriber.sub_nbr = var_sub_nbr;

    SELECT sf_type AS existing_sf_type
      FROM special_facility
     WHERE special_facility.s_id = var_s_id {}

    INSERT INTO call_forwarding(s_id, sf_type, start_time, end_time, numberx)
    VALUES (var_s_id, var_sf_type, var_start_time, var_end_time, var_numberx)
    CATCH unique_violation {
        raise notice 'unique violation';
        return;
    } CATCH serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    COMMIT;
$$ LANGUAGE 'umbrascript';
