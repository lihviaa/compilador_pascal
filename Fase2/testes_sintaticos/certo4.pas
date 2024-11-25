program certo4;
integer a, b;
boolean ambos_maiores;
begin
  read(a, b);
  set ambos_maiores to (a > 0b1) and (b > 0b1);
  write(ambos_maiores)
end.