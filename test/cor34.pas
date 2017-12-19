
program test (input, output);
  var a : integer;
  var b : real;
  var c : array [1..2] of integer;
  var g : real;
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
       var f: integer;

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
       e := e + 4;
       a:= (a mod 4.4) div 4.4;
       while ((a >= 4.4) and ((b <= e)
                       or (not (a = c[a])))) do
         begin
           a:= c + 1.0
         end;
       fun2 := 2.5
     end;

begin
  b:= fun2(c[4], c[5]);
     b:= fun2(c[4], c[5, 5]);
     b:= fun2(c[4], c[5], 5);
     b:= fun2(c[4], c);
     b:= fun2(c[4]);
     b:= fun2();
     b:= fun2;
     b:= fun3(c[4], c[5]);
     b:= fun4(c[4], c[5]);
     b:= fun5(c[4], c[5]);
   b:= fun2(c[4],2);
  if (a < 2) then a:= 1 else a := a + 2;
  if (b > 4.2) then a := c[a]
end.
