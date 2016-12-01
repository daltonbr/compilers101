var x, y : integer;
w : real;
begin
    x := 1;
    y := 2;
    w := x + y;
    write("hello mypas");

    if w > 4.0 then
        x := 2 * y + 4;
        w := w + 1.2;
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