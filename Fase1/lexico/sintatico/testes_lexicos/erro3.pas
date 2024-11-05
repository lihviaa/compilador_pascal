program certo2;
integer i, soma;
begin
  set soma to 0bx0; # Erro lexico em 0bx
  for i of 0b1 to 0b101:
    set soma to soma + i;
  write(soma)
end.