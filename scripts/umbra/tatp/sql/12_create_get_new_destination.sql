CREATE PROCEDURE get_new_destination(subscriber_count INTEGER)
AS
$$
    let var_s_id : INTEGER;
    let var_sf_type : SMALLINT;
    let var_start_time : SMALLINT;
    let var_end_time : SMALLINT;

    var_s_id = urand(1, subscriber_count);
    var_sf_type = CAST (urand(1, 4) AS SMALLINT);
    var_start_time = CAST (8 * urand(0, 2) AS SMALLINT);
    var_end_time = CAST (urand(1, 24) AS SMALLINT);

    SELECT special_facility.s_id AS it
      FROM special_facility
     WHERE special_facility.s_id = var_s_id
       AND special_facility.sf_type = var_sf_type
       AND special_facility.is_active = 1 {

       SELECT call_forwarding.numberx
         FROM call_forwarding
        WHERE call_forwarding.s_id = var_s_id
          AND call_forwarding.sf_type = var_sf_type
          AND call_forwarding.start_time <= var_start_time
          AND call_forwarding.end_time > var_end_time {

         -- do nothing, we just have to select data
       }
    }

    COMMIT;
$$ LANGUAGE 'umbrascript';
