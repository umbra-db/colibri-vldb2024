create function genname(id integer) returns varchar(20)
as
$$
   return namePart(mod(id/100,10)) || namePart(mod(id/10,10)) || namePart(mod(id,10));
$$ language 'umbrascript' strict
                          immutable;
