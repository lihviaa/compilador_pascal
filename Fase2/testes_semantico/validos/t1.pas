# Declaração e Atribuição Simples

program exemplo1;
integer x, y;
begin
  set x to 0b1010; # 10 em decimal
  set y to 0b10100;  # 20 em decimal
  write(x, y)
end.


{- SAÍDA ESPERADA: 
  INPP
  AMEM 2
  CRCT 10
  ARMZ 00
  CRCT 20
  ARMZ 01
  CRVL 00
  IMPR
  CRVL 01
  IMPR
  PARA

Tabela de simbolos
[00]:   x
[01]:   y

-}