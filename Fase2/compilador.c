/*
    INTEGRANTES DO GRUPO
    NOME: Livia Alabarse dos Santos         RA: 10403046
    NOME: Pedro Henrique Araujo Farias      RA: 10265432

    Compile o programa com o seguinte comando:
    gcc -g -Og -Wall compilador.c lexico.c sintatico.c -o compilador
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "lexico.h"
#include "sintatico.h"

//variaveis globais declarados no sintatico
extern TAtomo lookahead;
extern TInfoAtomo info_atomo;

// Funcao principal =)
int main(int argc, char* argv[]) {
    file_to_buffer(argv[1]); // Lê o arquivo informado em linha de comando
    
    info_atomo = get_atomo();
    lookahead = info_atomo.atomo;

    programa();
    consome(EOS);

    printf("\n%d linhas foram lidas.\nPrograma sintaticamente correto.\n", info_atomo.linha);
    
    return 0;
}
