# Operações

program exemplo5;
integer a, b, c;
begin
  set a to 0b1010;  # 10 em decimal
  set b to 0b0101;  # 5 em decimal
  set c to (a + b) * 0b11110;  # 30 em decimal
  write(c)
end.

{- SAÍDA ESPERADA: 

    INPP
    AMEM 3
    CRCT 10
    ARMZ 00
    CRCT 5
    ARMZ 01
    CRVL 00
    CRVL 01
    SOMA
    CRCT 30
    MULT
    ARMZ 02
    CRVL 02
    IMPR
    PARA

Tabela de simbolos
[00]:   a
[01]:   b
[02]:   c

-}