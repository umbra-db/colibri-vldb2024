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

CREATE OR REPLACE FUNCTION nurand(a INTEGER, lower INTEGER, upper INTEGER) RETURNS INTEGER
AS
$$
BEGIN
    RETURN ((random(a) | (random(upper - lower + 1) + lower)) % (upper - lower + 1)) + lower;
END
$$ LANGUAGE 'plpgsql' STRICT;

CREATE OR REPLACE FUNCTION subrand(subscriber_count INTEGER) RETURNS CHAR(15)
AS
$$
DECLARE
    s_id INTEGER;

BEGIN
    s_id := urand(1, subscriber_count);

    return CASE
               WHEN s_id < 10 THEN '00000000000000' || s_id::TEXT
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
END
$$ LANGUAGE 'plpgsql' STRICT;

CREATE OR REPLACE PROCEDURE get_subscriber_data(subscriber_count INTEGER)
AS
$$
DECLARE
    var_s_id         INTEGER;
    var_sub_nbr      CHAR(15);
    var_bit_1        SMALLINT;
    var_bit_2        SMALLINT;
    var_bit_3        SMALLINT;
    var_bit_4        SMALLINT;
    var_bit_5        SMALLINT;
    var_bit_6        SMALLINT;
    var_bit_7        SMALLINT;
    var_bit_8        SMALLINT;
    var_bit_9        SMALLINT;
    var_bit_10       SMALLINT;
    var_hex_1        SMALLINT;
    var_hex_2        SMALLINT;
    var_hex_3        SMALLINT;
    var_hex_4        SMALLINT;
    var_hex_5        SMALLINT;
    var_hex_6        SMALLINT;
    var_hex_7        SMALLINT;
    var_hex_8        SMALLINT;
    var_hex_9        SMALLINT;
    var_hex_10       SMALLINT;
    var_byte2_1      SMALLINT;
    var_byte2_2      SMALLINT;
    var_byte2_3      SMALLINT;
    var_byte2_4      SMALLINT;
    var_byte2_5      SMALLINT;
    var_byte2_6      SMALLINT;
    var_byte2_7      SMALLINT;
    var_byte2_8      SMALLINT;
    var_byte2_9      SMALLINT;
    var_byte2_10     SMALLINT;
    var_msc_location INTEGER;
    var_vlr_location INTEGER;

BEGIN
    BEGIN
        var_s_id := urand(1, subscriber_count);

        SELECT subscriber.sub_nbr,
               subscriber.bit_1,
               subscriber.bit_2,
               subscriber.bit_3,
               subscriber.bit_4,
               subscriber.bit_5,
               subscriber.bit_6,
               subscriber.bit_7,
               subscriber.bit_8,
               subscriber.bit_9,
               subscriber.bit_10,
               subscriber.hex_1,
               subscriber.hex_2,
               subscriber.hex_3,
               subscriber.hex_4,
               subscriber.hex_5,
               subscriber.hex_6,
               subscriber.hex_7,
               subscriber.hex_8,
               subscriber.hex_9,
               subscriber.hex_10,
               subscriber.byte2_1,
               subscriber.byte2_2,
               subscriber.byte2_3,
               subscriber.byte2_4,
               subscriber.byte2_5,
               subscriber.byte2_6,
               subscriber.byte2_7,
               subscriber.byte2_8,
               subscriber.byte2_9,
               subscriber.byte2_10,
               subscriber.msc_location,
               subscriber.vlr_location
        INTO var_sub_nbr,
            var_bit_1,
            var_bit_2,
            var_bit_3,
            var_bit_4,
            var_bit_5,
            var_bit_6,
            var_bit_7,
            var_bit_8,
            var_bit_9,
            var_bit_10,
            var_hex_1,
            var_hex_2,
            var_hex_3,
            var_hex_4,
            var_hex_5,
            var_hex_6,
            var_hex_7,
            var_hex_8,
            var_hex_9,
            var_hex_10,
            var_byte2_1,
            var_byte2_2,
            var_byte2_3,
            var_byte2_4,
            var_byte2_5,
            var_byte2_6,
            var_byte2_7,
            var_byte2_8,
            var_byte2_9,
            var_byte2_10,
            var_msc_location,
            var_vlr_location
        FROM subscriber
        WHERE subscriber.s_id = var_s_id;

    EXCEPTION
        WHEN serialization_failure OR deadlock_detected OR no_data_found
            THEN
                ROLLBACK;
    END;

    COMMIT;
END
$$ LANGUAGE 'plpgsql';


CREATE OR REPLACE PROCEDURE get_new_destination(subscriber_count INTEGER)
AS
$$
DECLARE
    var_s_id       INTEGER;
    var_sf_type    SMALLINT;
    var_start_time SMALLINT;
    var_end_time   SMALLINT;
    var_sf_it      RECORD;
    var_cf_it      RECORD;

BEGIN
    BEGIN
        var_s_id := urand(1, subscriber_count);
        var_sf_type := CAST(urand(1, 4) AS SMALLINT);
        var_start_time := CAST(8 * urand(0, 2) AS SMALLINT);
        var_end_time := CAST(urand(1, 24) AS SMALLINT);

        FOR var_sf_it IN
            SELECT special_facility.s_id AS it
            FROM special_facility
            WHERE special_facility.s_id = var_s_id
              AND special_facility.sf_type = var_sf_type
              AND special_facility.is_active = 1
            LOOP
                FOR var_cf_it IN
                    SELECT call_forwarding.numberx
                    FROM call_forwarding
                    WHERE call_forwarding.s_id = var_s_id
                      AND call_forwarding.sf_type = var_sf_type
                      AND call_forwarding.start_time <= var_start_time
                      AND call_forwarding.end_time > var_end_time
                    LOOP
                        NULL;
                    END LOOP;
            END LOOP;

    EXCEPTION
        WHEN serialization_failure OR deadlock_detected OR no_data_found
            THEN
                ROLLBACK;
    END;

    COMMIT;
END
$$ LANGUAGE 'plpgsql';



CREATE OR REPLACE PROCEDURE get_access_data(subscriber_count INTEGER)
AS
$$
DECLARE
    var_s_id    INTEGER;
    var_ai_type SMALLINT;
    var_ai_it   RECORD;
BEGIN
    BEGIN
        var_s_id = urand(1, subscriber_count);
        var_ai_type = CAST(urand(1, 4) AS SMALLINT);

        FOR var_ai_it IN
            SELECT access_info.data1, access_info.data2, access_info.data3, access_info.data4
            FROM access_info
            WHERE access_info.s_id = var_s_id
              AND access_info.ai_type = var_ai_type
            LOOP
                NULL;
            END LOOP;

    EXCEPTION
        WHEN serialization_failure OR deadlock_detected OR no_data_found
            THEN
                ROLLBACK;
    END;

    COMMIT;
END
$$ LANGUAGE 'plpgsql';



CREATE OR REPLACE PROCEDURE update_subscriber_data(subscriber_count INTEGER)
AS
$$
DECLARE
    var_s_id    INTEGER;
    var_sf_type SMALLINT;
    var_bit_1   SMALLINT;
    var_data_a  SMALLINT;
    var_success INTEGER;

BEGIN
    BEGIN
        var_s_id := urand(1, subscriber_count);
        var_sf_type = CAST(urand(1, 4) AS SMALLINT);
        var_bit_1 = CAST(urand(0, 1) AS SMALLINT);
        var_data_a = CAST(urand(0, 255) AS SMALLINT);

        UPDATE subscriber
        SET bit_1 = var_bit_1
        WHERE subscriber.s_id = var_s_id;

        UPDATE special_facility
        SET data_a = var_data_a
        WHERE special_facility.s_id = var_s_id
          AND special_facility.sf_type = var_sf_type
        RETURNING 1 INTO STRICT var_success;
    EXCEPTION
        WHEN serialization_failure OR deadlock_detected OR no_data_found
            THEN
                ROLLBACK;
    END;

    COMMIT;
END
$$ LANGUAGE 'plpgsql';



CREATE OR REPLACE PROCEDURE update_location(subscriber_count INTEGER)
AS
$$
DECLARE
    var_sub_nbr      CHAR(15);
    var_vlr_location INTEGER;

BEGIN
    BEGIN
        var_sub_nbr = subrand(subscriber_count);
        var_vlr_location = urand(1, 2147483647);

        UPDATE subscriber
        SET vlr_location = var_vlr_location
        WHERE subscriber.sub_nbr = var_sub_nbr;

    EXCEPTION
        WHEN serialization_failure OR deadlock_detected OR no_data_found
            THEN
                ROLLBACK;
    END;

    COMMIT;
END
$$ LANGUAGE 'plpgsql';



CREATE OR REPLACE PROCEDURE insert_call_forwarding(subscriber_count INTEGER)
AS
$$
DECLARE
    var_sub_nbr    CHAR(15);
    var_s_id       INTEGER;
    var_sf_type    SMALLINT;
    var_start_time SMALLINT;
    var_end_time   SMALLINT;
    var_numberx    CHAR(15);
    var_sf_it      RECORD;

BEGIN
    BEGIN
        var_sub_nbr = subrand(subscriber_count);
        var_sf_type = CAST(urand(1, 4) AS SMALLINT);
        var_start_time = CAST(8 * urand(0, 2) AS SMALLINT);
        var_end_time = CAST(urand(1, 24) AS SMALLINT);
        var_numberx = subrand(subscriber_count);

        SELECT s_id
        INTO var_s_id
        FROM subscriber
        WHERE subscriber.sub_nbr = var_sub_nbr;

        SELECT sf_type
        into var_sf_it
        FROM special_facility
        WHERE special_facility.s_id = var_s_id;

        INSERT INTO call_forwarding(s_id, sf_type, start_time, end_time, numberx)
        VALUES (var_s_id, var_sf_type, var_start_time, var_end_time, var_numberx);

    EXCEPTION
        WHEN serialization_failure OR deadlock_detected OR no_data_found OR unique_violation
            THEN
                ROLLBACK;
    END;

    COMMIT;
END
$$ LANGUAGE 'plpgsql';

CREATE OR REPLACE PROCEDURE delete_call_forwarding(subscriber_count INTEGER)
AS
$$
DECLARE
    var_sub_nbr    CHAR(15);
    var_s_id       INTEGER;
    var_sf_type    SMALLINT;
    var_start_time SMALLINT;

BEGIN
    BEGIN
        var_sub_nbr = subrand(subscriber_count);
        var_sf_type = CAST(urand(1, 4) AS SMALLINT);
        var_start_time = CAST(8 * urand(0, 2) AS SMALLINT);

        SELECT s_id
        INTO var_s_id
        FROM subscriber
        WHERE subscriber.sub_nbr = var_sub_nbr;

        DELETE
        FROM call_forwarding
        WHERE call_forwarding.s_id = var_s_id
          AND call_forwarding.sf_type = var_sf_type
          AND call_forwarding.start_time = var_start_time;


    EXCEPTION
        WHEN serialization_failure OR deadlock_detected OR no_data_found
            THEN
                ROLLBACK;
    END;

    COMMIT;
END
$$ LANGUAGE 'plpgsql';
