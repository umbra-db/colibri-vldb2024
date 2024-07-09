CREATE TABLE subscriber
(
    s_id         INTEGER  NOT NULL PRIMARY KEY,
    sub_nbr      CHAR(15) NOT NULL UNIQUE,
    bit_1        SMALLINT,
    bit_2        SMALLINT,
    bit_3        SMALLINT,
    bit_4        SMALLINT,
    bit_5        SMALLINT,
    bit_6        SMALLINT,
    bit_7        SMALLINT,
    bit_8        SMALLINT,
    bit_9        SMALLINT,
    bit_10       SMALLINT,
    hex_1        SMALLINT,
    hex_2        SMALLINT,
    hex_3        SMALLINT,
    hex_4        SMALLINT,
    hex_5        SMALLINT,
    hex_6        SMALLINT,
    hex_7        SMALLINT,
    hex_8        SMALLINT,
    hex_9        SMALLINT,
    hex_10       SMALLINT,
    byte2_1      SMALLINT,
    byte2_2      SMALLINT,
    byte2_3      SMALLINT,
    byte2_4      SMALLINT,
    byte2_5      SMALLINT,
    byte2_6      SMALLINT,
    byte2_7      SMALLINT,
    byte2_8      SMALLINT,
    byte2_9      SMALLINT,
    byte2_10     SMALLINT,
    msc_location INTEGER,
    vlr_location INTEGER
) WITH (%%relationConfig%%);