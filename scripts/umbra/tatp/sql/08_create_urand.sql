CREATE FUNCTION urand(lower INTEGER, upper INTEGER) RETURNS INTEGER
AS
$$
    return random(upper - lower + 1) + lower;
$$ LANGUAGE 'umbrascript' STRICT;
