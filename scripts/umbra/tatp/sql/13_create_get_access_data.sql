CREATE PROCEDURE get_access_data(subscriber_count INTEGER)
AS
$$
    let var_s_id : INTEGER;
    let var_ai_type : SMALLINT;

    var_s_id = urand(1, subscriber_count);
    var_ai_type = CAST (urand(1, 4) AS SMALLINT);

    SELECT access_info.data1, access_info.data2, access_info.data3, access_info.data4
      FROM access_info
     WHERE access_info.s_id = var_s_id
       AND access_info.ai_type = var_ai_type {

     -- do nothing, we just have to select data
   }

    COMMIT;
$$ LANGUAGE 'umbrascript';
