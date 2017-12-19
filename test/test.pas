program test(input, output);
	var a : integer;
	var b : array[1..5] of real;
	function f1(a : integer; x : real) : real ;
		var c : integer;
		function f2(p : integer) : integer ;
		var d : real;
		begin
			f2 := 5 + p
		end;
	begin
		f1 := f2(a) * x
	end;
	function f3(a : real) : real ;
	var b : integer;
	begin
		f3 := 10 * f1(a, 2.3);
		test := 4
	end;
begin
	out := f3(5.3)
end.

