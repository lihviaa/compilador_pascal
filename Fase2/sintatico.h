#include "lexico.h"
#ifndef __SINTATICO_H
#define __SINTATICO_H

// Declaração de tipos e funções

// Declaracao de funcoes do analisador sintatico
void tipo();
void bloco();
void fator();
void termo();
void comando();
void programa();
void expressao();
void comando_saida();
void op_relacional();
void lista_variavel();
void comando_entrada();
void comando_composto();
void expressao_logica();
void expressao_simples();
void comando_repeticao();
void comando_atribuicao();
void comando_condicional();
void consome(TAtomo atomo); // Comunicacao com o analisador lexico
void expressao_relacional();
void declaracao_de_variaveis();

#endif