program exemplo6;
integer a, b, p, q;
begin
    if a and b:
        set q to p + q
    elif
        if a or b: 
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
    CONJ
    DSVF L1
    CRVL 02
    CRVL 03
    SOMA
    ARMZ 03
    DSVS L2
L1: NADA
    CRVL 00
    CRVL 01
    DISJ
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