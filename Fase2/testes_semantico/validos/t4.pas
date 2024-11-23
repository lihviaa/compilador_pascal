# Condicional

program exemplo4;
integer x;
begin
  read(x);
  if x > 0b0:
    write(x)
  elif
    write(0b0)
end.

{- SAÍDA ESPERADA: 

    INPP
    AMEM 1
    LEIT
    ARMZ 00
    CRVL 00
    CRCT 0
    CMMA
    DSVF L1
    CRVL 00
    IMPR
    DSVS L2
L1: NADA
    CRCT 0
    IMPR
L2: NADA
    PARA

Tabela de simbolos
[00]:   x

-}