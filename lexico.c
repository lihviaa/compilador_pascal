/*
    INTEGRANTES DO GRUPO
    NOME: Livia Alabarse dos Santos         RA: 10403046
    NOME: Pedro Henrique Araujo Farias      RA: 10265432

    Compile o programa com o seguinte comando:
    gcc -g -Og -Wall compilador.c -o compilador
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    ERRO,
    IDENTIFICADOR,
    PALAVRA_RESERVADA,
    NUMERO,
    COMENTARIO,
    EOS
} TAtomo;

typedef struct {
    TAtomo atomo;
    int linha;
    int atributo_numero;
    char atributo_ID[16];
} TInfoAtomo;

char* msgAtomo[] = {
    "ERRO",
    "IDENTIFICADOR",
    "PALAVRA RESERVADA",
    "NUMERO",
    "COMENTARIO",
    "EOS"
};

char* palavras_reservadas[] = {
    "and", "begin", "boolean", "elif", "end",
    "false", "for", "if", "integer", "not", "of",
    "or", "program", "read", "set", "to", "true", "write"
};

char* buffer;
int conta_linha = 1; 

TInfoAtomo get_atomo();
TInfoAtomo get_numero();
TInfoAtomo get_comentarioA();
TInfoAtomo get_comentarioB();
TInfoAtomo get_identificador();
void file_to_buffer(char* file_name);

int main(int argc, char* argv[]) {
    file_to_buffer(argv[1]);
    TInfoAtomo info_atomo;
    
    do
    {
        info_atomo = get_atomo();

        if(info_atomo.atomo == IDENTIFICADOR || info_atomo.atomo == PALAVRA_RESERVADA)
            printf("%03d# %s | %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo], info_atomo.atributo_ID);

        else if(info_atomo.atomo == NUMERO)
            printf("%03d# %s | %d\n", info_atomo.linha, msgAtomo[info_atomo.atomo], info_atomo.atributo_numero);

        else if(info_atomo.atomo == COMENTARIO)
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);

        else
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);
    } while(info_atomo.atomo != EOS && info_atomo.atomo != ERRO);
    
    printf("Fim da analise lexica");

    return 0;
}

void file_to_buffer(char* file_name) {
    FILE *pfile = fopen(file_name, "r");
    if(!pfile) {
        fprintf(stderr, "Erro ao abrir arquivo.\n");
        return;
    }

    fseek(pfile, 0, SEEK_END);
    long file_size = ftell(pfile);
    rewind(pfile);

    buffer = (char*) malloc((file_size + 1) * sizeof(char));
    
    if(!buffer) {
        fprintf(stderr, "Erro ao carregar buffer.\n");
        fclose(pfile);
        return;
    }

    size_t read_size = fread(buffer, 1, file_size, pfile);
    buffer[read_size] = '\0';

    fclose(pfile);
}

TInfoAtomo get_atomo() {
    TInfoAtomo info_atomo;
    info_atomo.atomo = ERRO;

    while(*buffer == ' ' || *buffer == '\n' ||
    *buffer == '\t' || *buffer == '\r') {
        if(*buffer == '\n') conta_linha++;
        buffer++;
    }

    if(islower(*buffer)) info_atomo = get_identificador();
    
    else if(*buffer == '0') info_atomo = get_numero();

    else if(*buffer == '#') info_atomo = get_comentarioA();

    else if(*buffer == '{') info_atomo = get_comentarioB();
    
    else if(*buffer == 0) info_atomo.atomo = EOS;
    
    if(info_atomo.atomo != COMENTARIO) info_atomo.linha = conta_linha;

    return info_atomo;
}

// Identificador -> letra(letra|digito|_)*
TInfoAtomo get_identificador() {
    TInfoAtomo info_identificador;
    info_identificador.atomo = ERRO;
    char* iniID = buffer;
    buffer++; // Primeira letra do identificador lida

    q1:
    if(islower(*buffer) || isdigit(*buffer) || *buffer == '_') {
        buffer++;
        goto q1;
    }

    if((buffer - iniID) > 15) return info_identificador;
    
    strncpy(info_identificador.atributo_ID, iniID, buffer - iniID);
    info_identificador.atributo_ID[buffer - iniID] = 0; // finaliza a string
    info_identificador.atomo = IDENTIFICADOR;

    for(int i = 0; palavras_reservadas[i] != NULL; i++) {
        if(strcmp(info_identificador.atributo_ID, palavras_reservadas[i]) == 0) {
            info_identificador.atomo = PALAVRA_RESERVADA;
        }
    }

    return info_identificador;
}

// Numero -> 0b(0|1)+
TInfoAtomo get_numero() {
    TInfoAtomo info_numero;
    info_numero.atomo = ERRO;
    char* iniID = buffer;
    buffer++; // 0 ja foi lido

    if(*buffer == 'b') {
        buffer++;
        goto q2;
    }

    return info_numero;

    q2:
    if(*buffer == '0' || *buffer == '1') {
        buffer++;
        goto q3;
    }

    return info_numero;

    q3:
    if(*buffer == '0' || *buffer == '1') {
        buffer++;
        goto q3;
    }

    char binario[999];
    iniID += 2;
    strncpy(binario, iniID, buffer - iniID);
    binario[buffer - iniID] = 0; // finaliza a string
    
    info_numero.atributo_numero = atoi(binario);
    info_numero.atomo = NUMERO;

    return info_numero;
}

TInfoAtomo get_comentarioA() {
    TInfoAtomo info_comentario;
    info_comentario.linha = conta_linha;
    buffer++; // Consome o #

    q1:
    if(*buffer != '\n') {
        if(*buffer == 0) goto q2;
        buffer++;
        goto q1;
    }

    q2:

    info_comentario.atomo = COMENTARIO;
    return info_comentario;
}

TInfoAtomo get_comentarioB() {
    TInfoAtomo info_comentario;
    info_comentario.atomo = ERRO;
    info_comentario.linha = conta_linha;
    buffer++; // Consome {

    if(*buffer == '-') {
        buffer++;
        goto q2;
    }

    return info_comentario;

    q2:
    if(*buffer == '-' && *(buffer + 1) == '}') {
        buffer++; // Consome -
        buffer++; // Consome }
        goto q3;
    }

    if(*buffer == '\0') return info_comentario;

    if(*buffer == '\n') conta_linha++;
    buffer++;
    goto q2;

    q3:

    info_comentario.atomo = COMENTARIO;
    return info_comentario;
}