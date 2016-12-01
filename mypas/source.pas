var x, y : integer;
begin
    x := 1;
    y := 2;

    repeat
        y := y + 1;
    until y > 4;

    if x > 3 then
        x := 2 * y + 4;
    elif x > 5 then
        x := 10;
    else
        x := 20;
    y := x * y;
    endif;
    while x < 2 do
        x := x + 1;
    end;

end.