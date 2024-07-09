create function nurand(a integer, lower integer, upper integer) returns integer
as
$$
    return (((random(a) | (random(upper - lower + 1) + lower)) + 42) % (upper - lower + 1)) + lower;
$$ language 'umbrascript' strict
                          volatile;
