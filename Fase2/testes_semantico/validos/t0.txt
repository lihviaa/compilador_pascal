{- 
programa le dois numeros inteiros e encontra o maior 
-} 
program exemplo0;
    integer num_1, num_2;
    integer maior;
begin
    read(num_1);
    read(num_2);
    if num_1 > num_2:  
        set maior to num_1
    elif
        set maior to num_2;
    
    write(maior) # imprime o maior valor
end.

{- SAÍDA ESPERADA:

    INPP
    AMEM 3
    LEIT
    ARMZ 00
    LEIT
    ARMZ 01
    CRVL 00
    CRVL 01
    CMMA
    DSVF L1
    CRVL 00
    ARMZ 02
    DSVS L2
L1: NADA
    CRVL 01
    ARMZ 02
L2: NADA
    CRVL 02
    IMPR
    PARA

Tabela de simbolos
[00]:   num_1
[01]:   num_2
[02]:   maior

-}