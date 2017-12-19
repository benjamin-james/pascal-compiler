
program test (input, output);
  var a : integer;
  var b : real;
  var c : array [1..2] of integer;

  function fun1(x:integer; y:real; 
                  z:array [1..2] of integer; 
                  q: real) : integer;
    var d: integer;
    begin
      a:= 2;
      z[a] := 4;
      c[2] := 3;
      fun1 := c[1]
     end;

   function fun2(x: integer; y: integer) : real;
     var e: real;

     function fun3(n: integer; z: real) : integer;
       var e: integer;
       begin
         a:= e;
         e:= c[e];
         fun3 := 3
       end;

     begin
       a:= fun1(x, e, c, b);
       x:= fun3(c[1], e);
       e := e + 4.44;
       a:= (a mod y) div x;
       while ((a >= 4) and ((b <= e) 
                       or (not (a = c[a])))) do
         begin
           a:= c[a] + 1
         end;
       fun2 := 2.5 
     end;

begin
  b:= fun2(c[4], c[5]);
  b:= fun2(c[4],2);
  if (a < 2) then a:= 1 else a := a + 2;
  if (b > 4.2) then a := c[a]
end.
