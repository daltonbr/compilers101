var x, y : integer;
    z : float;
    w : double;
begin
    x := 1;
    y := 2;
    z := 3.14;
    w := 3.14159;

    if x > 3 then
        x := 2 * y + 4;
        w := w + 1.2;
    elif x > 5 then
        x := 10;
    else
        x := 20;
        z := z + 1.1;
    y := x * y;
    endif;
    while x < 2 do
    begin
        x := x + 1;
    end;

end.