{ testmypas.pas }

program testmypas;

var x,y: integer;

var z: real;

var A: array [3,4] of array [10] of integer;

procedure test(var x,y: integer)

   var a, b: real;
   
   function Y(var x: real; y: integer): integer
   
		var tatu: integer;
		
		begin
		
			read (tatu);
			
			if tatu = 1 then 
			   x := 2 * x + y
			else 
			   x := 0;
			Y := tatu * x * y;
		end {end-Y};
	
  begin
    
    x:= y:= Y(0, z)
   
   end {end-test};
    
begin

	test(0,0, 1+2)
	
end.
