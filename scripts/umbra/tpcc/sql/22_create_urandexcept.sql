create function urandexcept(lower integer, upper integer, v integer) returns integer
as
$$
    if upper <= lower {
        return lower;
    } else {
        let r = random(upper - lower) + lower;
        return case when r >= v then r + 1 else r end;
    }
$$ language 'umbrascript' strict
                          volatile;
