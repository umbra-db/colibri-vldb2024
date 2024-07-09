CREATE TABLE access_info
(
    s_id    INTEGER  NOT NULL,
    ai_type SMALLINT NOT NULL,
    data1   SMALLINT,
    data2   SMALLINT,
    data3   CHAR(3),
    data4   CHAR(5),
    PRIMARY KEY (s_id, ai_type)
) WITH (%%relationConfig%%);
