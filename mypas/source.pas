var x, y : integer;
begin
    x := 1;
    y := 2;
    if x > 3 then
        x := 2 * y + 4;
    elif x > 5 then
        x := 10;
    else
        x := 20;
    y := x * y;
    endif;
    while x < 2 do
    begin
        x := x + 1;
    end;
end.