CREATE TABLE special_facility
(
    s_id        INTEGER  NOT NULL,
    sf_type     SMALLINT NOT NULL,
    is_active   SMALLINT NOT NULL,
    error_cntrl SMALLINT,
    data_a      SMALLINT,
    data_b      CHAR(5),
    PRIMARY KEY (s_id, sf_type)
) WITH (%%relationConfig%%);
