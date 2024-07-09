CREATE PROCEDURE get_subscriber_data(subscriber_count INTEGER)
AS
$$
    let var_s_id : INTEGER;

    var_s_id = urand(1, subscriber_count);

    SELECT subscriber.sub_nbr,
           subscriber.bit_1, subscriber.bit_2, subscriber.bit_3, subscriber.bit_4, subscriber.bit_5, subscriber.bit_6, subscriber.bit_7, subscriber.bit_8, subscriber.bit_9, subscriber.bit_10,
           subscriber.hex_1, subscriber.hex_2, subscriber.hex_3, subscriber.hex_4, subscriber.hex_5, subscriber.hex_6, subscriber.hex_7, subscriber.hex_8, subscriber.hex_9, subscriber.hex_10,
           subscriber.byte2_1, subscriber.byte2_2, subscriber.byte2_3, subscriber.byte2_4, subscriber.byte2_5, subscriber.byte2_6, subscriber.byte2_7, subscriber.byte2_8, subscriber.byte2_9, subscriber.byte2_10,
           subscriber.msc_location, subscriber.vlr_location
    FROM subscriber
    WHERE subscriber.s_id = var_s_id;

    COMMIT;
$$ LANGUAGE 'umbrascript';
