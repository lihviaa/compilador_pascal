program certo2;
integer i, soma;
begin
  set soma to 0b1011_; # Erro lexico em 0b1011_
  for i of 0b1 to 0b101:
    set soma to soma + i;
  write(soma)
end.