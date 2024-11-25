{-
programa calcula o fatorial de um numero lido
-}
program fatorial;
	integer fat,num,cont;
begin
	read(num);
	set fat to 0b1;
	for cont of 0b10 to num:
		set fat to fat * cont;
	write(fat) # imprime o fatorial calculado
end.

{- SAÍDA ESPERADA: 

	INPP
	AMEM 3
	LEIT
	ARMZ 1
	CRCT 1
	ARMZ 0
	CRCT 2
	ARMZ 2
L1:	NADA
	CRVL 2
	CRVL 1
	CMEG
	DVSF L2
	CRVL 0
	CRVL 2
	MULT
	ARMZ 0
	CRVL 2
	CRCT 1
	SOMA
	ARMZ 2
	DSVS L1
L2:	NADA
	CRVL 0
	IMPR
	PARA

Tabela de simbolos
[00]:   fat
[01]:   num
[02]:   cont

-}