var x, y : integer;
begin
    x := 1;
    y := 2;
    if x > 3 then
        x := 2 * y + 4;
    else
        x:= 10;
    y := x * y;
    endif;
    while x < 2 do
        x := x + 1;
end;
end.