program erro4;
integer a, b;
begin
  set a to 0b10;
  set b to 0b01;
  set c to a + b;  # 'c' não foi declarado
  write(c)
end.
