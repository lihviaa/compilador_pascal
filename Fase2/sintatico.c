// variaveis globais e implementação de função
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "lexico.h"
#include "sintatico.h"

// Array de strings para mensagens de acordo com o enderecamento a partir do atomo
char* msgAtomo[] = {
    "ERRO",
    "IDENTIFICADOR",
    "and",
    "begin",
    "boolean",
    "elif",
    "end",
    "false",
    "for",
    "if",
    "integer",
    "not",
    "of",
    "or",
    "program",
    "read",
    "set",
    "to",
    "true",
    "write",
    "NUMERO",
    "COMENTARIO",
    "ponto e virgula",
    "ponto",
    "virgula",
    "dois pontos",
    "abre parenteses",
    "fecha parenteses",
    "adicao",
    "subtracao",
    "multiplicacao",
    "divisao",
    "igual",
    "diferente",
    "menor",
    "menor ou igual",
    "maior",
    "maior ou igual",
    "EOS"
};

// Variaveis globais do analisador sintatico
TAtomo lookahead;
TInfoAtomo info_atomo;

// ********** Definicao de funcoes do analisador sintatico **********

// Funcao do analisador sintatico que solicita um atomo ao analisador lexico, integrando-os
void consome(TAtomo atomo) {
    if(lookahead == ERRO) { // Erro lexico e imediatamente reportado
        printf("\n%03d# Erro lexico identificado.\n", info_atomo.linha);
        exit(0);
    }

    while(lookahead == COMENTARIO) { // Ignora atomo de comentario
        // Antes de ignora-lo, reportamos o comentario
        printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);
        info_atomo = get_atomo();
        lookahead = info_atomo.atomo;
    }

    if(atomo == IDENTIFICADOR)
        printf("%03d# %s | %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo], info_atomo.atributo_ID);

    else if(atomo == NUMERO)
        printf("%03d# %s | %d\n", info_atomo.linha, msgAtomo[info_atomo.atomo], info_atomo.atributo_numero);

    else // Palavras reservadas, outros atomos sem conteudo em atributo_ID ou atributo_numero
        printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);

    if(lookahead == atomo) { // Obtem proximo lookahead
        info_atomo = get_atomo();
        lookahead = info_atomo.atomo;
    } else { // Caso atomo nao corresponda ao esperado na sintaxe, reporta erro sintatico
        printf("\n%03d# Erro sintatico: Esperado [%s], encontrado [%s]\n", info_atomo.linha, msgAtomo[atomo], msgAtomo[lookahead]);
        exit(0);
    }

    // Pra ser sincero, nao sabemos ao certo o motivo de precisar verificar se e
    // comentario no final tambem, mas so funciona corretamente assim. 
    while(lookahead == COMENTARIO) { 
        printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);
        info_atomo = get_atomo();
        lookahead = info_atomo.atomo;
    } // Ficou meio gambiarra, mas esta funcionando. 
}

// Funcoes do analisador sintatico para regras de producao da sintaxe

// <programa> ::= "program" "identificador" ";" <bloco> "."
void programa() {
    consome(PROGRAM);
    consome(IDENTIFICADOR);
    consome(PONTO_E_VIRGULA);
    bloco();
    consome(PONTO);
}

// <bloco> ::= <declaracao_de_variaveis> <comando_composto>
void bloco() {
    declaracao_de_variaveis();
    comando_composto();
}

// <declaracao_de_variaveis> ::= { <tipo> <lista_variavel> ";" }
void declaracao_de_variaveis() {
    while(lookahead == INTEGER || lookahead == BOOLEAN) {
        tipo();
        lista_variavel();
        consome(PONTO_E_VIRGULA);
    }
}

// <tipo> ::= "integer" | "boolean"
void tipo() {
    if(lookahead == INTEGER) consome(INTEGER);
    else consome(BOOLEAN);
}

// <lista_variavel> ::= "identificador" { "," "identificador" }
void lista_variavel() {
    consome(IDENTIFICADOR);

    while(lookahead == VIRGULA) {
        consome(VIRGULA);
        consome(IDENTIFICADOR);
    }
}

// <comando_composto> ::= "begin" <comando> { ";" <comando> } "end"
void comando_composto() {
    consome(BEGIN);
    comando();

    while(lookahead == PONTO_E_VIRGULA) {
        consome(PONTO_E_VIRGULA);
        comando();
    }

    consome(END);
}

/* <comando> ::= <comando_atribuicao> | <comando_condicional> |
                 <comando_repeticao>  | <comando_entrada>     |
                 <comando_saida>      | <comando_composto>   */
void comando() {
    switch(lookahead) {
    
    case SET:
        comando_atribuicao();
        break;
    
    case IF:
        comando_condicional();
        break;

    case FOR:
        comando_repeticao();
        break;
    
    case READ:
        comando_entrada();
        break;
    
    case WRITE:
        comando_saida();
        break;

    default:
        comando_composto();
        break;
    }
}

// <comando_atribuicao> ::= "set" "identificador" "to" <expressao>
void comando_atribuicao() {
    consome(SET);
    consome(IDENTIFICADOR);
    consome(TO);
    expressao();
}

// <comando_condicional> ::= "if" <expressao> ":" <comando> [ "elif" <comando> ]
void comando_condicional() {
    consome(IF);
    expressao();
    consome(DOIS_PONTOS);
    comando();
    
    if(lookahead == ELIF) {
        consome(ELIF);
        comando();
    }
}

// <comando_repeticao> ::= "for" "identificador" "of" <expressao> "to" <expressao> ":" <comando>
void comando_repeticao() {
    consome(FOR);
    consome(IDENTIFICADOR);
    consome(OF);
    expressao();
    consome(TO);
    expressao();
    consome(DOIS_PONTOS);
    comando();
}

// <comando_entrada> ::= "read" "(" <lista_variavel> ")"
void comando_entrada() {
    consome(READ);
    consome(ABRE_PARENTESES);
    lista_variavel();
    consome(FECHA_PARENTESES);
}

// <comando_saida> ::= "write" "(" <expressao> { "," <expressao> } ")"
void comando_saida() {
    consome(WRITE);
    consome(ABRE_PARENTESES);
    expressao();

    while(lookahead == VIRGULA) {
        consome(VIRGULA);
        expressao();
    }

    consome(FECHA_PARENTESES);
}

// <expressao> ::= <expressao_logica> { "or" <expressao_logica> }
void expressao() {
    expressao_logica();

    while(lookahead == OR) {
        consome(OR);
        expressao_logica();
    }
}

// <expressao_logica> ::= <expressao_relacional> { "and" <expressao_relacional> }
void expressao_logica() {
    expressao_relacional();

    while(lookahead == AND) {
        consome(AND);
        expressao_relacional();
    }
}

// <expressao_relacional> ::= <expressao_simples> [ <op_relacional> <expressao_simples> ]
void expressao_relacional() {
    expressao_simples();

    if(lookahead == MENOR || lookahead == MENOR_OU_IGUAL ||
       lookahead == MAIOR || lookahead == MAIOR_OU_IGUAL ||
       lookahead == IGUAL || lookahead == DIFERENTE) {
        op_relacional();
        expressao_simples();
       }
}

// <op_relacional> ::= "<" | "<=" | "=" | "/=" | ">" | ">="
void op_relacional() {
    switch(lookahead) {

    case MENOR:
        consome(MENOR);
        break;

    case MENOR_OU_IGUAL:
        consome(MENOR_OU_IGUAL);
        break;

    case IGUAL:
        consome(IGUAL);
        break;

    case DIFERENTE:
        consome(DIFERENTE);
        break;

    case MAIOR:
        consome(MAIOR);
        break;

    default:
        consome(MAIOR_OU_IGUAL);
        break; 
    }
}

// <expressao_simples> ::= <termo> { ("+" | "-") <termo> }
void expressao_simples() {
    termo();

    while(lookahead == ADICAO || lookahead == SUBTRACAO) {
        consome(lookahead);
        termo();
    }
}

// <termo> ::= <fator> { ("*" | "/") <fator> }
void termo() {
    fator();

    while(lookahead == MULTIPLICACAO || lookahead == DIVISAO) {
        consome(lookahead);
        fator();
    }
}

// <fator> ::= "(" <expressao> ")" | "not" <fator> |
//             "identificador" | "numero" | "false" | "true"
void fator() {
    switch(lookahead) {
    
    case ABRE_PARENTESES:
        consome(ABRE_PARENTESES);
        expressao();
        consome(FECHA_PARENTESES);
        break;
    
    case NOT:
        consome(NOT);
        fator();
    
    case NUMERO:
        consome(NUMERO);
        break;

    case IDENTIFICADOR:
        consome(IDENTIFICADOR);
        break;
    
    case FALSE:
        consome(FALSE);
        break;
        
    default:
        consome(TRUE);
        break;
    }
}

// ********** Fim da definicao de funcoes do analisador sintatico **********