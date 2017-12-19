program example(input, output);
var x: integer; var y: integer;
function gcd(a:integer; b: integer): integer;
begin
	if b = 0 then gcd := a
	else gcd := gcd(b, a mod b)
end;

begin
	out := read(x, y);
	out := write(gcd(x, y));
	out := 0
end.
