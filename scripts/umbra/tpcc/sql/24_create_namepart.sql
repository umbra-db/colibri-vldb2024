create function namepart(id integer) returns varchar(20)
as
$$
    return
    CASE id
        WHEN 0 THEN 'BAR'
        WHEN 1 THEN 'OUGHT'
        WHEN 2 THEN 'ABLE'
        WHEN 3 THEN 'PRI'
        WHEN 4 THEN 'PRES'
        WHEN 5 THEN 'ESE'
        WHEN 6 THEN 'ANTI'
        WHEN 7 THEN 'CALLY'
        WHEN 8 THEN 'ATION'
        ELSE 'EING'
    END;
$$ language 'umbrascript' strict
                          immutable;
