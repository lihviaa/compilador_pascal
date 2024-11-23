# Laço de Repetição 2

program exemplo3;
integer s, n, a;
begin
  read(n);
  for s of 0b1 to n: # 1 em decimal
  set a to a + 0b11 * s # 3 em decimal 
end.

{- SAÍDA ESPERADA: 
    INPP
    AMEM 3
    LEIT
    ARMZ 01
    CRCT 1
    ARMZ 00
L1: NADA
    CRVL 00
    CRVL 01
    CMEG
    DSVF L2
    CRVL 02
    CRCT 3
    CRVL 00
    MULT
    SOMA
    ARMZ 02
    CRVL 00
    CRCT 1
    SOMA
    ARMZ 00
    DSVS L1
L2: NADA
    PARA

Tabela de simbolos
[00]:   s
[01]:   n
[02]:   a

-}
