program example_procedures;

{ este é um exemplo de multi-reentrâncias de funções e procedimentos}

   {lexical level 0}
   var x: integer;

   procedure A (var reference: integer; value1, value2: double; var txt: text);

      {lexical level 1}
      var y: integer;

      procedure B;

         {lexical level 2}
          var z, x: integer;
          w: text;
	 begin
           A(z, 2, 3, w);
           B;
           z := x := 3
	 end; {end-B}
      
      begin
      	     A; B;
	     x := y := 1
      end; {end-A}

   begin
     A;
     x := 8
   end.

{ symbol table:

	0	x	0
	1	y	1
	2	z	2
	3	x	2
}
