# Condicionais Aninhadas

program exemplo6;
integer a, b, p, q;
begin
    if a /= b:
        set q to p + q
    elif
        if (a < (0b10*b)): # 2 em decimal
            set p to 0b1 # 1 em decimal
        elif
            set q to 0b0; # 0 em decimal
    write(p);
    write(q)
end.

{- SAÍDA ESPERADA: 

    INPP
    AMEM 4
    CRVL 00
    CRVL 01
    CMDG
    DSVF L1
    CRVL 02
    CRVL 03
    SOMA
    ARMZ 03
    DSVS L2
L1: NADA
    CRVL 00
    CRCT 2
    CRVL 01
    MULT
    CMME
    DSVF L3
    CRCT 1
    ARMZ 02
    DSVS L4
L3: NADA
    CRCT 0
    ARMZ 03
L4: NADA
L2: NADA
    CRVL 02
    IMPR
    CRVL 03
    IMPR
    PARA

Tabela de simbolos
[00]:   a
[01]:   b
[02]:   p
[03]:   q

-}