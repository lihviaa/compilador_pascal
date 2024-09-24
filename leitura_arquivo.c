/*
    INTEGRANTES DO GRUPO
    NOME: Livia Alabarse dos Santos         RA: 10403046
    NOME: Pedro Henrique Araujo Farias      RA: 10265432

    Compile o programa com o seguinte comando:
    gcc -g -Og -Wall compilador.c -o compilador
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* buffer;

void file_to_buffer(char* file_name);

int main(int argc, char* argv[]) {
    file_to_buffer(argv[1]);

    while(*buffer != 0) {
        printf("%d ", *buffer);
        buffer++;
    }

    printf("%d ", *buffer);
    if(*buffer == 0) printf("nulo");

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
