CREATE PROCEDURE update_location(subscriber_count INTEGER)
AS
$$
    let var_sub_nbr : TEXT;
    let var_vlr_location : INTEGER;

    var_sub_nbr = subrand(subscriber_count);
    var_vlr_location = urand(1, 2147483647);

    UPDATE subscriber
       SET vlr_location = var_vlr_location
     WHERE subscriber.sub_nbr = var_sub_nbr
    CATCH serialization_failure {
        raise notice 'serialization failure';
        return;
    }

    COMMIT;
$$ LANGUAGE 'umbrascript';
