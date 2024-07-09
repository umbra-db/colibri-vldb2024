CREATE FUNCTION nurand(a INTEGER, lower INTEGER, upper INTEGER) RETURNS INTEGER
AS
$$
    return ((random(a) | (random(upper - lower + 1) + lower)) % (upper - lower + 1)) + lower;
$$ LANGUAGE 'umbrascript' STRICT;
