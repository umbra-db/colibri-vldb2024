CREATE TABLE call_forwarding
(
    s_id       INTEGER  NOT NULL,
    sf_type    SMALLINT NOT NULL,
    start_time SMALLINT NOT NULL,
    end_time   SMALLINT,
    numberx    VARCHAR(15),
    PRIMARY KEY (s_id, sf_type, start_time)
) WITH (%%relationConfig%%);
