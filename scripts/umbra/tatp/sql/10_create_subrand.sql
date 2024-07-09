CREATE FUNCTION subrand(subscriber_count INTEGER) RETURNS TEXT
AS
$$
    let s_id : INTEGER;

    s_id = urand(1, subscriber_count);

    return CASE WHEN s_id < 10 THEN '00000000000000' || s_id::TEXT
                          WHEN s_id < 100 THEN '0000000000000' || s_id::TEXT
                          WHEN s_id < 1000 THEN '000000000000' || s_id::TEXT
                          WHEN s_id < 10000 THEN '00000000000' || s_id::TEXT
                          WHEN s_id < 100000 THEN '0000000000' || s_id::TEXT
                          WHEN s_id < 1000000 THEN '000000000' || s_id::TEXT
                          WHEN s_id < 10000000 THEN '00000000' || s_id::TEXT
                          WHEN s_id < 100000000 THEN '0000000' || s_id::TEXT
                          WHEN s_id < 1000000000 THEN '000000' || s_id::TEXT
                          WHEN s_id < 10000000000 THEN '00000' || s_id::TEXT
                          WHEN s_id < 100000000000 THEN '0000' || s_id::TEXT
                          WHEN s_id < 1000000000000 THEN '000' || s_id::TEXT
                          WHEN s_id < 10000000000000 THEN '00' || s_id::TEXT
                          WHEN s_id < 100000000000000 THEN '0' || s_id::TEXT
                          ELSE s_id::TEXT
                    END;
$$ LANGUAGE 'umbrascript' STRICT;
