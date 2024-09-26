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
    AND,
    BEGIN,
    BOOLEAN,
    ELIF,
    END,
    FALSE,
    FOR,
    IF,
    INTEGER,
    NOT,
    OF,
    OR,
    PROGRAM,
    READ,
    SET,
    TO,
    TRUE,
    WRITE,
    NUMERO,
    COMENTARIO,
    PONTO_E_VIRGULA,
    PONTO,
    VIRGULA,
    DOIS_PONTOS,
    ABRE_PARENTESES,
    FECHA_PARENTESES,
    ADICAO,
    SUBTRACAO,
    MULTIPLICACAO,
    DIVISAO,
    IGUAL,
    DIFERENTE,
    MENOR,
    MAIOR,
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
    "AND",
    "BEGIN",
    "BOOLEAN",
    "ELIF",
    "END",
    "FALSE",
    "FOR",
    "IF",
    "INTEGER",
    "NOT",
    "OF",
    "OR",
    "PROGRAM",
    "READ",
    "SET",
    "TO",
    "TRUE",
    "WRITE",
    "NUMERO",
    "COMENTARIO",
    "PONTO E VIRGULA",
    "PONTO",
    "VIRGULA",
    "DOIS PONTOS",
    "ABRE PARENTESES",
    "FECHA PARENTESES",
    "ADICAO",
    "SUBTRACAO",
    "MULTIPLICACAO",
    "DIVISAO",
    "IGUAL",
    "DIFERENTE",
    "MENOR",
    "MAIOR",
    "EOS"
};

// Vetor para identificação e atribuição correta em ENUM de palavras reservadas
char* palavras_reservadas[] = {
    "and", "begin", "boolean", "elif", "end",
    "false", "for", "if", "integer", "not", "of",
    "or", "program", "read", "set", "to", "true", "write"
};

char* buffer; // Buffer obtido a partir de leitura de arquivo
int conta_linha = 1; 

// Declaração de funções do analisador léxico
TInfoAtomo get_atomo();
TInfoAtomo get_ponto();
TInfoAtomo get_igual();
TInfoAtomo get_menor();
TInfoAtomo get_maior();
TInfoAtomo get_numero();
TInfoAtomo get_adicao();
TInfoAtomo get_virgula();
TInfoAtomo get_divisao();
TInfoAtomo get_subtracao();
TInfoAtomo get_diferente();
TInfoAtomo get_comentarioA();
TInfoAtomo get_comentarioB();
TInfoAtomo get_dois_pontos();
TInfoAtomo get_identificador();
TInfoAtomo get_multiplicacao();
TInfoAtomo get_ponto_e_virgula();
TInfoAtomo get_abre_parenteses();
TInfoAtomo get_fecha_parenteses();

// Declaração de função de leitura de arquivo para buffer
void file_to_buffer(char* file_name);

int main(int argc, char* argv[]) {
    file_to_buffer(argv[1]);
    TInfoAtomo info_atomo;
    
    do
    {
        info_atomo = get_atomo();

        if(info_atomo.atomo == IDENTIFICADOR)
            printf("%03d# %s | %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo], info_atomo.atributo_ID);

        else if(info_atomo.atomo == PONTO_E_VIRGULA)
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);
        
        else if(info_atomo.atomo == PONTO)
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);

        else if(info_atomo.atomo == VIRGULA)
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);

        else if(info_atomo.atomo == DOIS_PONTOS)
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);

        else if(info_atomo.atomo == ABRE_PARENTESES)
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);

        else if(info_atomo.atomo == FECHA_PARENTESES)
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);

        else if(info_atomo.atomo == ADICAO)
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);

        else if(info_atomo.atomo == SUBTRACAO)
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);

        else if(info_atomo.atomo == MULTIPLICACAO)
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);

        else if(info_atomo.atomo == DIVISAO)
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);

        else if(info_atomo.atomo == IGUAL)
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);

        else if(info_atomo.atomo == DIFERENTE)
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);

        else if(info_atomo.atomo == MENOR)
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);

        else if(info_atomo.atomo == MAIOR)
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);

        else if(info_atomo.atomo > 1 && info_atomo.atomo < 20)
            printf("%03d# %s\n", info_atomo.linha, msgAtomo[info_atomo.atomo]);

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

// Função de leitura do conteúdo de arquivo para uma string no buffer
// Estratégia: ir até o final do arquivo, calcular número de bytes, alocar 
// string de número de bytes + 1, copiar conteúdo, adicionar terminal nulo
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

    // Ignora espaços, quebra de linha, tabulação
    while(*buffer == ' ' || *buffer == '\n' ||
    *buffer == '\t' || *buffer == '\r') {
        if(*buffer == '\n') conta_linha++; // Se for quebra de linha, incrementa contador
        buffer++;
    }

    // Se identificar uma letra minúscula, átomo esperado é identificador
    if(islower(*buffer)) info_atomo = get_identificador();
    
    // Se identificar '0', átomo esperado é um número no formato 0b(0|1)+
    else if(*buffer == '0') info_atomo = get_numero();

    // Se identificar '#', átomo esperado é um comentário de uma linha
    else if(*buffer == '#') info_atomo = get_comentarioA();

    // Se identificar '{', átomo esperado é um comentário de múltiplas linhas
    else if(*buffer == '{') info_atomo = get_comentarioB();

    // Se identificar ';', átomo esperado é ponto e vírgula
    else if(*buffer == ';') info_atomo = get_ponto_e_virgula();

    // Se identificar '.', átomo esperado é ponto
    else if(*buffer == '.') info_atomo = get_ponto();
    
    // Se identificar ',', átomo esperado é vírgula
    else if(*buffer == ',') info_atomo = get_virgula();
    
    // Se identificar ':', átomo esperado é dois pontos
    else if(*buffer == ':') info_atomo = get_dois_pontos();
    
    // Se identificar '(', átomo esperado é abre parenteses
    else if(*buffer == '(') info_atomo = get_abre_parenteses();
    
    // Se identificar ')', átomo esperado é fecha parenteses
    else if(*buffer == ')') info_atomo = get_fecha_parenteses();
    
    // Se identificar '+', átomo esperado é adição
    else if(*buffer == '+') info_atomo = get_adicao();
    
    // Se identificar '-', átomo esperado é subtração
    else if(*buffer == '-') info_atomo = get_subtracao();
    
    // Se identificar '*', átomo esperado é multiplicação
    else if(*buffer == '*') info_atomo = get_multiplicacao();
    
    // Se identificar '/', átomo pode ser divisão ou diferente
    else if(*buffer == '/') {
        // Se próximo caractere do buffer for '=', teremos '/='
        if(*(buffer + 1) == '=') info_atomo = get_diferente();
        else info_atomo = get_divisao(); // Do contrário, teremos '/'
    }
    
    // Se identificar '=', átomo esperado é igual
    else if(*buffer == '=') info_atomo = get_igual();
    
    // Se identificar '<', átomo esperado é menor
    else if(*buffer == '<') info_atomo = get_menor();
    
    // Se identificar '>', átomo esperado é maior
    else if(*buffer == '>') info_atomo = get_maior();
    
    // Se identificar terminador nulo, átomo esperado é fim do buffer (end of string)
    else if(*buffer == 0) info_atomo.atomo = EOS;
    
    // Condicional necessário para manter corretude do contador de linha
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

    // Condicional para rejeitar identificadores com mais de 15 caracteres
    if((buffer - iniID) > 15) return info_identificador;
    
    // Recorta o identificador encontrado para o atributo de identificador
    strncpy(info_identificador.atributo_ID, iniID, buffer - iniID);
    info_identificador.atributo_ID[buffer - iniID] = 0; // Finaliza a string
    info_identificador.atomo = IDENTIFICADOR;

    // Verifica se é uma palavra reservada
    for(int i = 0; palavras_reservadas[i] != NULL; i++) {
        if(strcmp(info_identificador.atributo_ID, palavras_reservadas[i]) == 0) {
            // Palavra reservada correspondente: AND + índice no vetor de reservadas
            info_identificador.atomo = AND + i;
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
    // Caso 0 seja lido, mas b não, não está na definição léxica
    return info_numero;

    q2:
    // Obrigatoriamente, devem ser lidos 0 ou 1
    if(*buffer == '0' || *buffer == '1') {
        buffer++;
        goto q3;
    }

    return info_numero; // Retorna erro se 0b não for seguido de 0 ou 1

    q3: // Consome 0s e 1s 
    if(*buffer == '0' || *buffer == '1') {
        buffer++;
        goto q3;
    }

    // Caso, por exemplo, 0b101a ou 0b1_ ocorra, léxico não reconhece
    if(isalpha(*buffer) || *buffer == '_') return info_numero;

    char binario[999]; // Não se sabe tamanho do binário, então por via das dúvidas...
    iniID += 2; // Recorte descarta 0b e armazena apenas o binário propriamente dito
    strncpy(binario, iniID, buffer - iniID); // Recorta o binário
    binario[buffer - iniID] = 0; // Finaliza a string
    
    // Binário é armazenado como um INTEIRO, conforme definição em TInfoAtomo
    info_numero.atributo_numero = atoi(binario); // atoi converte string para int
    info_numero.atomo = NUMERO; 

    return info_numero;
}

// Comentário de linha única
TInfoAtomo get_comentarioA() {
    TInfoAtomo info_comentario;
    info_comentario.linha = conta_linha; // Guarda linha no início do comentário
    buffer++; // Consome o #

    q1:
    if(*buffer != '\n') { // Enquanto conteúdo do buffer não for quebra de linha
        if(*buffer == 0) goto q2; // Tratando comentário caso string termine
        buffer++;
        goto q1;
    }

    // Se conteúdo do buffer for quebra de linha ou fim da string, encerra
    // Uma vez que é comentário de uma linha, não precisamos nos preocupar com
    // incrementar o contador, já que obterAtomo() consumirá \n
    q2: 
    info_comentario.atomo = COMENTARIO;
    return info_comentario;
}

// Comentário de múltiplas linhas
TInfoAtomo get_comentarioB() {
    TInfoAtomo info_comentario;
    info_comentario.atomo = ERRO;
    info_comentario.linha = conta_linha; // Guarda linha de início do comentário
    buffer++; // Consome {

    if(*buffer == '-') { // Comentários se iniciam com {-
        buffer++;
        goto q2;
    }

    return info_comentario; // Se, por exemplo, {abc, retorna erro

    q2: // Verifica fim de comentário: - seguido, necessariamente, por }
    if(*buffer == '-' && *(buffer + 1) == '}') {
        buffer++; // Consome -
        buffer++; // Consome }
        goto q3; // Vai para reconhecimento do átomo de comentário
    }

    // Se buffer acabar antes de fechar comentário, retorna erro
    if(*buffer == '\0') return info_comentario; 

    // Incrementa contador de linha, uma vez que obterAtomo() nao vai acessar os \n
    // processados dentro do átomo
    if(*buffer == '\n') conta_linha++;
    buffer++;
    goto q2; // Enquanto comentário não for finalizado com -}...

    q3: // Reconhecimento do átomo como comentário
    info_comentario.atomo = COMENTARIO;
    return info_comentario;
}

TInfoAtomo get_ponto_e_virgula() {
    TInfoAtomo info_ponto_e_virgula;
    info_ponto_e_virgula.atomo = PONTO_E_VIRGULA;
    buffer++; // Consome ponto e vírgula
    return info_ponto_e_virgula;
}

TInfoAtomo get_ponto() {
    TInfoAtomo info_ponto;
    info_ponto.atomo = PONTO;
    buffer++; // Consome ponto
    return info_ponto;
}

TInfoAtomo get_virgula() {
    TInfoAtomo info_virgula;
    info_virgula.atomo = VIRGULA;
    buffer++; // Consome vírgula
    return info_virgula;
}

TInfoAtomo get_dois_pontos() {
    TInfoAtomo info_dois_pontos;
    info_dois_pontos.atomo = DOIS_PONTOS;
    buffer++; // Consome dois pontos
    return info_dois_pontos;
}

TInfoAtomo get_abre_parenteses() {
    TInfoAtomo info_abre_parenteses;
    info_abre_parenteses.atomo = ABRE_PARENTESES;
    buffer++; // Consome abre parenteses
    return info_abre_parenteses;
}

TInfoAtomo get_fecha_parenteses() {
    TInfoAtomo info_fecha_parenteses;
    info_fecha_parenteses.atomo = FECHA_PARENTESES;
    buffer++; // Consome fecha parenteses
    return info_fecha_parenteses;
}

TInfoAtomo get_adicao() {
    TInfoAtomo info_adicao;
    info_adicao.atomo = ADICAO;
    buffer++; // Consome adicao
    return info_adicao;
}

TInfoAtomo get_subtracao() {
    TInfoAtomo info_subtracao;
    info_subtracao.atomo = SUBTRACAO;
    buffer++; // Consome subtracao
    return info_subtracao;
}

TInfoAtomo get_multiplicacao() {
    TInfoAtomo info_multiplicacao;
    info_multiplicacao.atomo = MULTIPLICACAO;
    buffer++; // Consome multiplicacao
    return info_multiplicacao;
}

TInfoAtomo get_divisao() {
    TInfoAtomo info_divisao;
    info_divisao.atomo = DIVISAO;
    buffer++; // Consome divisao
    return info_divisao;
}

TInfoAtomo get_igual() {
    TInfoAtomo info_igual;
    info_igual.atomo = IGUAL;
    buffer++; // Consome igual
    return info_igual;
}

TInfoAtomo get_diferente() {
    TInfoAtomo info_diferente;
    info_diferente.atomo = DIFERENTE;
    buffer++; buffer++; // Consome '/' e '=';
    return info_diferente;
}

TInfoAtomo get_menor() {
    TInfoAtomo info_menor;
    info_menor.atomo = MENOR;
    buffer++; // Consome menor
    return info_menor;
}

TInfoAtomo get_maior() {
    TInfoAtomo info_maior;
    info_maior.atomo = MAIOR;
    buffer++; // Consome maior
    return info_maior;
}
