/*
Implementacao do mini analisador lexico
Compile com:
gcc -g -Og -Wall compilador.c -o compilador
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

// definicao de tipo
typedef enum{
    ERRO,
    IDENTIFICADOR,
    NUMERO,
    PALAVRA_RESERVADA,
    COMENTARIO,
    EOS
}TAtomo;

typedef struct{
    TAtomo atomo;
    int linha;
    float atributo_numero;
    char atributo_ID[16];
}TInfoAtomo;

char *msgAtomo[] = {
    "ERRO",
    "IDENTIFICADOR",
    "NUMERO",
    "PALAVRA_RESERVADA"
    "EOS"
};
// variavel global para o analisador lexico
// variavel bufer devera ser inicializada a partir de um arquivo texto
char *buffer = "     var_1aaaaaaaaaa\n"
               " \r var2aaaaaaaaaa\t \n"
               " vaa3  boolean elif"
               "{- isso é um teste-}\n"
               "#teste\n"
               "teste\n"
               "# teste\n"
               "{-teste-}\n"
               "passou\n";

int contaLinha = 1;
char *palavrasReservadas[] = {
    "and", "begin", "boolean", "elif", "end", "false", "for", "if", "integer", "not", "of", "or", "program", "read", "set", "to", "true", "write"
};

// declaracao da funcao
TInfoAtomo obter_atomo(); // irah integrar com a Analisador Sintatico
TInfoAtomo reconhece_id();
TInfoAtomo reconhece_num();

int main ()
{
    TInfoAtomo info_atomo;
    do{
        info_atomo = obter_atomo();
        if (info_atomo.atomo == COMENTARIO)
            continue;
        else if( info_atomo.atomo == IDENTIFICADOR)
            printf("#   %d %s | %s\n",info_atomo.linha,msgAtomo[info_atomo.atomo], info_atomo.atributo_ID);
        else if( info_atomo.atomo == NUMERO)
            printf("#   %d %s | %f\n",info_atomo.linha,msgAtomo[info_atomo.atomo], info_atomo.atributo_numero);
        else if( info_atomo.atomo == PALAVRA_RESERVADA)
            printf("#   %d %s | %s\n",info_atomo.linha,msgAtomo[info_atomo.atomo], info_atomo.atributo_ID);
        else
            printf("#   %d %s\n",info_atomo.linha,msgAtomo[info_atomo.atomo]);

    }while(info_atomo.atomo != EOS && info_atomo.atomo != ERRO);
    printf("fim da analise lexica\n");
    return 0;
}

TInfoAtomo obter_atomo(){
    TInfoAtomo info_atomo;

    // consome espaços em branco quebra de linhas tabulação e retorno de carro
    while(*buffer == ' ' || *buffer == '\n' || *buffer == '\t' ||*buffer == '\r'){
        if( *buffer =='\n')
            contaLinha++;
        buffer++;
    }
    // reconhece identificador
    if( islower(*buffer)){ // ser for letra mininuscula
        info_atomo = reconhece_id();
    }else if (strncmp(buffer, "0b", 2) == 0) { // Checa se começa com '0b' para número binário
        info_atomo = reconhece_num(); // reconhece numero
    }else if(strncmp(buffer, "{-", 2) == 0){
        buffer += 2;
        while (strncmp(buffer, "-}", 2) != 0 && *buffer != 0) {
            if (*buffer == '\n') {
                contaLinha++;
            }
            buffer++;
        }
        if (*buffer != 0) buffer += 2;  // Avança além do "-}"
        info_atomo.atomo = COMENTARIO;
    }else if(*buffer == '#'){
        while (*buffer != '\n' && *buffer !=0) {
            buffer++;
        }
        info_atomo.atomo = COMENTARIO;
    }else if(*buffer == 0){
        info_atomo.atomo = EOS;
    }else{
        info_atomo.atomo = ERRO;
    }
    info_atomo.linha = contaLinha;
    return info_atomo;

}

// IDENTIFICADOR -> LETRA_MINUSCULA ( LETRA_MINUSCULA | DIGITO | _ )*
TInfoAtomo reconhece_id() {
    TInfoAtomo info_atomo;
    info_atomo.atomo = ERRO;
    char *iniID = buffer;
    // ja temos uma letra minuscula
    buffer++;

q1:
    if (islower(*buffer) || isdigit(*buffer) || *buffer == '_') {
        buffer++;
        goto q1;
    }
    if (isupper(*buffer))
        return info_atomo;

    if ((buffer - iniID) > 15) { // Verifica se o tamanho máximo foi excedido
        return info_atomo;
    }

    // Copia o identificador para info_atomo.atributo_ID
    strncpy(info_atomo.atributo_ID, iniID, buffer - iniID);
    info_atomo.atributo_ID[buffer - iniID] = '\0'; // Finaliza a string
    info_atomo.atomo = IDENTIFICADOR;

    // Verifica se é uma palavra reservada
    for (int i = 0; palavrasReservadas[i] != NULL; i++) {
        if (strcmp(info_atomo.atributo_ID, palavrasReservadas[i]) == 0) {
            info_atomo.atomo = PALAVRA_RESERVADA; // Ajusta o átomo para palavra reservada
            return info_atomo;
        }
    }

    return info_atomo;
}



// NUM BINARIO
TInfoAtomo reconhece_num(){
    TInfoAtomo info_atomo;
    info_atomo.atomo = ERRO;

    char *iniNum = buffer + 2; // Pula '0b'
    buffer += 2;

    // Verifica se é binário
    while (*buffer == '0' || *buffer == '1') {
        buffer++;
    }

    strncpy(info_atomo.atributo_ID, iniNum, buffer - iniNum);
    info_atomo.atributo_ID[buffer - iniNum] = '\0'; // Finaliza a string
    info_atomo.atributo_numero = strtol(info_atomo.atributo_ID, NULL, 2);
    info_atomo.atomo = NUMERO;
    return info_atomo;
}
