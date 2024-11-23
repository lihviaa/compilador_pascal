# Laço de Repetição 1

program exemplo2;
integer i, soma;
begin
  set soma to 0b1010; # 10 em decimal
  for i of 0b1 to 0b101: # 1 e 5 em decimal
    set soma to soma + i;
  write(soma)
end.

{- SAÍDA ESPERADA: 
    INPP
    AMEM 2
    CRCT 10
    ARMZ 01
    CRCT 1
    ARMZ 00
L1: NADA
    CRVL 00
    CRCT 5
    CMEG
    DSVF L2
    CRVL 01
    CRVL 00
    SOMA
    ARMZ 01
    CRVL 00
    CRCT 1
    SOMA
    ARMZ 00
    DSVS L1
L2: NADA
    CRVL 01
    IMPR
    PARA

Tabela de simbolos
[00]:   i
[01]:   soma

-}