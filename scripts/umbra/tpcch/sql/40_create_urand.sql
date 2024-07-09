create function urand(lower integer, upper integer) returns integer
as
$$
    return random(upper - lower + 1) + lower;
$$ language 'umbrascript' strict
                          volatile;
