/*
    INTEGRANTES DO GRUPO
    NOME: Livia Alabarse dos Santos         RA: 10403046
    NOME: Pedro Henrique Araujo Farias      RA: 10265432

    Compile o programa com o seguinte comando:
    gcc -g -Og -Wall semantico.c -o semantico
*/

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE_ARRAY 100
#define MAX_SIZE_STRING 16

// Definicao dos atomos reconhecidos pela gramatica
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
    MENOR_OU_IGUAL,
    MAIOR,
    MAIOR_OU_IGUAL,
    EOS
} TAtomo;

// Definicao de estrutura das informacoes de um atomo
typedef struct { 
    TAtomo atomo;
    int linha;
    int atributo_numero; // caso atomo seja NUMERO, armazena o numero 
    char atributo_ID[16]; // caso atomo seja IDENTIFICADOR, armazena identificador
} TInfoAtomo;

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

// Vetor para identificacao e atribuição correta em ENUM de palavras reservadas
char* palavras_reservadas[] = {
    "and", "begin", "boolean", "elif", "end",
    "false", "for", "if", "integer", "not", "of",
    "or", "program", "read", "set", "to", "true", "write"
};

// Variaveis globais para analisador lexico
char* buffer; // Buffer obtido a partir de leitura de arquivo
int conta_linha = 1; // Contador de linhas do programa

// Declaracao de funcoes do analisador lexico
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
TInfoAtomo get_menor_ou_igual();
TInfoAtomo get_maior_ou_igual();
TInfoAtomo get_ponto_e_virgula();
TInfoAtomo get_abre_parenteses();
TInfoAtomo get_fecha_parenteses();

// Variaveis globais do analisador sintatico
TAtomo lookahead;
TInfoAtomo info_atomo;

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

// Variaveis globais do analisador semantico
int num_simbolos = 0;
int flag_declaracao_variaveis = 0;
char tabela_simbolos[MAX_SIZE_ARRAY][MAX_SIZE_STRING];

// Declaracao de funcoes do analisador semantico
void exibir_tabela_simbolos();
int buscar_tabela_simbolos(char* simbolo);
void adicionar_tabela_simbolos(char* simbolo);
void excessao_nao_declarada(char* nao_declarada);

// Variaveis globais para geracao de codigo MEPA
int num_variaveis_declaradas = 0;
int rotulo_atual = 1;

// Declaracao de funcoes para geracao de codigo MEPA
int proximo_rotulo();

// Declaracao de funcao de leitura de conteudo do arquivo para buffer
void file_to_buffer(char* file_name);

// Funcao principal =)
int main(int argc, char* argv[]) {
    file_to_buffer(argv[1]); // Lê o arquivo informado em linha de comando
    
    info_atomo = get_atomo();
    lookahead = info_atomo.atomo;

    programa();
    consome(EOS);

    printf("\nTabela de simbolos\n");
    exibir_tabela_simbolos();

    return 0;
}

// Funcao de leitura do conteudo de arquivo para uma string em buffer
// Estrategia: ir ate o final do arquivo, calcular numero de bytes, alocar 
// string de numero de bytes + 1, copiar conteudo, adicionar terminador nulo
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

// ********** Definicao de funcoes do analisador lexico **********

// Funcao do analisador lexico que obtem um atomo a partir do conteudo no buffer
TInfoAtomo get_atomo() {
    TInfoAtomo info_atomo;
    info_atomo.atomo = ERRO;

    // Ignora espaços, quebra de linha, tabulacao, retorno de carro
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
    
    // Se identificar '<', átomo pode ser menor ou menor ou igual
    else if(*buffer == '<') {
        // Se próximo caractere do buffer for '=', teremos '<='
        if(*(buffer + 1) == '=') info_atomo = get_menor_ou_igual();
        else info_atomo = get_menor(); // Do contrário, teremos '<'
    }
    
    // Se identificar '>', átomo pode ser maior ou maior ou igual
    else if(*buffer == '>') {
        // Se próximo caractere do buffer for '=', teremos '>='
        if(*(buffer + 1) == '=') info_atomo = get_maior_ou_igual();
        else info_atomo = get_maior(); // Do contrário, teremos '>'
    }
    
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

    q1: // Consome caracteres correspondentes a definicao de identificador
    if(islower(*buffer) || isdigit(*buffer) || *buffer == '_') {
        buffer++;
        goto q1;
    }

    // Letras maiusculas nao sao reconhecidas 
    if(isupper(*buffer)) return info_identificador;

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
    iniID += 2; // Recorte descarta 0b 
    strncpy(binario, iniID, buffer - iniID); // Recorta o binário
    binario[buffer - iniID] = 0; // Finaliza a string
    
    // Numero é armazenado como um INTEIRO, conforme definição em TInfoAtomo
    info_numero.atributo_numero = strtol(binario, NULL, 2); // strtol para conversão direta em DECIMAL
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

// A partir daqui temos algumas funcoes simples do analisador lexico, que consomem
// o atomo indicado no nome da funcao... bem autoexplicativo.

TInfoAtomo get_ponto_e_virgula() {
    TInfoAtomo info_ponto_e_virgula;
    info_ponto_e_virgula.atomo = PONTO_E_VIRGULA;
    buffer++; // Consome ';'
    return info_ponto_e_virgula;
}

TInfoAtomo get_ponto() {
    TInfoAtomo info_ponto;
    info_ponto.atomo = PONTO;
    buffer++; // Consome '.'
    return info_ponto;
}

TInfoAtomo get_virgula() {
    TInfoAtomo info_virgula;
    info_virgula.atomo = VIRGULA;
    buffer++; // Consome ','
    return info_virgula;
}

TInfoAtomo get_dois_pontos() {
    TInfoAtomo info_dois_pontos;
    info_dois_pontos.atomo = DOIS_PONTOS;
    buffer++; // Consome ':'
    return info_dois_pontos;
}

TInfoAtomo get_abre_parenteses() {
    TInfoAtomo info_abre_parenteses;
    info_abre_parenteses.atomo = ABRE_PARENTESES;
    buffer++; // Consome '('
    return info_abre_parenteses;
}

TInfoAtomo get_fecha_parenteses() {
    TInfoAtomo info_fecha_parenteses;
    info_fecha_parenteses.atomo = FECHA_PARENTESES;
    buffer++; // Consome ')'
    return info_fecha_parenteses;
}

TInfoAtomo get_adicao() {
    TInfoAtomo info_adicao;
    info_adicao.atomo = ADICAO;
    buffer++; // Consome '+'
    return info_adicao;
}

TInfoAtomo get_subtracao() {
    TInfoAtomo info_subtracao;
    info_subtracao.atomo = SUBTRACAO;
    buffer++; // Consome '-'
    return info_subtracao;
}

TInfoAtomo get_multiplicacao() {
    TInfoAtomo info_multiplicacao;
    info_multiplicacao.atomo = MULTIPLICACAO;
    buffer++; // Consome '*'
    return info_multiplicacao;
}

TInfoAtomo get_divisao() {
    TInfoAtomo info_divisao;
    info_divisao.atomo = DIVISAO;
    buffer++; // Consome '/'
    return info_divisao;
}

TInfoAtomo get_igual() {
    TInfoAtomo info_igual;
    info_igual.atomo = IGUAL;
    buffer++; // Consome '='
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
    buffer++; // Consome '<'
    return info_menor;
}

TInfoAtomo get_menor_ou_igual() {
    TInfoAtomo info_menor_ou_igual;
    info_menor_ou_igual.atomo = MENOR_OU_IGUAL;
    buffer++; buffer++; // Consome '<' e '='
    return info_menor_ou_igual;
}

TInfoAtomo get_maior() {
    TInfoAtomo info_maior;
    info_maior.atomo = MAIOR;
    buffer++; // Consome '>'
    return info_maior;
}

TInfoAtomo get_maior_ou_igual() {
    TInfoAtomo info_maior_ou_igual;
    info_maior_ou_igual.atomo = MAIOR_OU_IGUAL;
    buffer++; buffer++;// Consome '>' e '='
    return info_maior_ou_igual;
}

// ********** Fim da definicao de funcoes do analisador lexico **********

// ********** Definicao de funcoes do analisador sintatico **********

// Funcao do analisador sintatico que solicita um atomo ao analisador lexico, integrando-os
void consome(TAtomo atomo) {
    if(lookahead == ERRO) { // Erro lexico e imediatamente reportado
        printf("\n%03d# Erro lexico identificado.\n", info_atomo.linha);
        exit(0);
    }

    while(lookahead == COMENTARIO) { // Ignora atomo de comentario
        info_atomo = get_atomo();
        lookahead = info_atomo.atomo;
    }

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
    printf("\tINPP\n");
    bloco();
    consome(PONTO);
    printf("\tPARA\n");
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
        flag_declaracao_variaveis = 1;
        lista_variavel();
        flag_declaracao_variaveis = 0;
        consome(PONTO_E_VIRGULA);
    }

    printf("\tAMEM %d\n", num_variaveis_declaradas);
}

// <tipo> ::= "integer" | "boolean"
void tipo() {
    if(lookahead == INTEGER) consome(INTEGER);
    else consome(BOOLEAN);
}

// <lista_variavel> ::= "identificador" { "," "identificador" }
void lista_variavel() {
    char simbolo_id[16];
    strcpy(simbolo_id, info_atomo.atributo_ID);
    consome(IDENTIFICADOR);
    
    if(flag_declaracao_variaveis) {
        adicionar_tabela_simbolos(simbolo_id);
        num_variaveis_declaradas++;
    } else {
        int endereco = buscar_tabela_simbolos(simbolo_id);
        if(endereco == -1) excessao_nao_declarada(simbolo_id);
        printf("\tLEIT\n");
        printf("\tARMZ %d\n", endereco);
    }

    while(lookahead == VIRGULA) {
        consome(VIRGULA);
        
        strcpy(simbolo_id, info_atomo.atributo_ID);
        consome(IDENTIFICADOR);
    
        if(flag_declaracao_variaveis) {
            adicionar_tabela_simbolos(simbolo_id);
            num_variaveis_declaradas++;
        } else {
            int endereco = buscar_tabela_simbolos(simbolo_id);
            if(endereco == -1) excessao_nao_declarada(simbolo_id);
            printf("\tLEIT\n");
            printf("\tARMZ %d\n", endereco);
        }
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
    char simbolo_id[16];
    strcpy(simbolo_id, info_atomo.atributo_ID);
    consome(IDENTIFICADOR);
    consome(TO);
    expressao();
    int endereco = buscar_tabela_simbolos(simbolo_id);
    printf("\tARMZ %d\n", endereco);
}

// <comando_condicional> ::= "if" <expressao> ":" <comando> [ "elif" <comando> ]
void comando_condicional() {
    int L1 = proximo_rotulo();
    int L2 = proximo_rotulo();
    consome(IF);
    expressao();
    consome(DOIS_PONTOS);
    printf("\tDSVF L%d\n", L1);
    comando();
    printf("\tDSVS L%d\n", L2);
    printf("L%d: \tNADA\n", L1);
    if(lookahead == ELIF) {
        consome(ELIF);
        comando();
    }
    printf("L%d: \tNADA\n", L2);
}

// <comando_repeticao> ::= "for" "identificador" "of" <expressao> "to" <expressao> ":" <comando>
void comando_repeticao() {
    int L1 = proximo_rotulo();
    int L2 = proximo_rotulo();
    consome(FOR);
    int endereco = buscar_tabela_simbolos(info_atomo.atributo_ID);
    consome(IDENTIFICADOR);
    consome(OF);
    expressao();
    printf("\tARMZ %d\n", endereco);
    printf("L%d: \tNADA\n", L1);
    consome(TO);
    printf("\tCRVL %d\n", endereco);
    expressao();
    printf("\tCMEG\n");
    printf("\tDSVF L%d\n", L2);
    consome(DOIS_PONTOS);
    comando();
    printf("\tCRVL %d\n", endereco);
    printf("\tCRCT 1\n");
    printf("\tSOMA\n");
    printf("\tARMZ %d\n", endereco);
    printf("\tDSVS L%d\n", L1);
    printf("L%d: \tNADA\n", L2);
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
    printf("\tIMPR\n");

    while(lookahead == VIRGULA) {
        consome(VIRGULA);
        expressao();
        printf("\tIMPR\n");
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
        TAtomo operador = lookahead;
        op_relacional();
        expressao_simples();
        if(operador == MENOR) printf("\tCMME\n");
        else if(operador == MENOR_OU_IGUAL) printf("\tCMEG\n");
        else if(operador == MAIOR) printf("\tCMMA\n");
        else if(operador == MAIOR_OU_IGUAL) printf("\tCMAG\n");
        else if(operador == IGUAL) printf("\tCMIG\n");
        else printf("\tCMDG\n"); 
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
        TAtomo operador = lookahead;
        consome(lookahead);
        termo();
        if(operador == ADICAO) printf("\tSOMA\n");
        else printf("\tSUBT\n");
    }
}

// <termo> ::= <fator> { ("*" | "/") <fator> }
void termo() {
    fator();

    while(lookahead == MULTIPLICACAO || lookahead == DIVISAO) {
        TAtomo operador = lookahead;
        consome(lookahead);
        fator();
        if(operador == MULTIPLICACAO) printf("\tMULT\n");
        else printf("\tDIVI\n");
    }
}

// Regra de producao alterada para analise semantica e geracao de codigo MEPA
// <fator> ::= "identificador" | "numero" | "(" <expressao> ")"
void fator() {
    switch(lookahead) {
    
    case NUMERO:
        int numero_id = info_atomo.atributo_numero;
        consome(NUMERO);
        printf("\tCRCT %d\n", numero_id);
        break;

    case IDENTIFICADOR:
        char simbolo_id[16];
        strcpy(simbolo_id, info_atomo.atributo_ID);
        consome(IDENTIFICADOR);
        int endereco = buscar_tabela_simbolos(simbolo_id);
        if(endereco == -1) excessao_nao_declarada(simbolo_id);
        printf("\tCRVL %d\n", endereco);
        break;
        
    default:
        consome(ABRE_PARENTESES);
        expressao();
        consome(FECHA_PARENTESES);
        break;
    }
}

// ********** Fim da definicao de funcoes do analisador sintatico **********

// ********** Definicao de funcoes do analisador semantico **********

int buscar_tabela_simbolos(char* simbolo) {
    for(int i = 0; i < num_simbolos; i++) {
        if(strcmp(tabela_simbolos[i], simbolo) == 0) return i;
    }

    return -1;
}

void adicionar_tabela_simbolos(char* simbolo) {
    if(buscar_tabela_simbolos(simbolo) == -1) {
        strcpy(tabela_simbolos[num_simbolos], simbolo);
        num_simbolos++;
    } else {
        printf("\n%03d# Erro semantico: Variavel %s ja declarada\n", info_atomo.linha, simbolo);
        exit(0);
    }
}

void exibir_tabela_simbolos() {
    for(int i = 0; i < num_simbolos; i++) {
        printf("[%02d]:\t%s\n", i, tabela_simbolos[i]);
    }
}

void excessao_nao_declarada(char* nao_declarada) {
    printf("\n%03d# Erro semantico: Variavel %s nao declarada\n", info_atomo.linha, nao_declarada);
    exit(0);
}

// ********** Fim da definicao de funcoes do analisador semantico **********

// ********** Definicao de funcoes para geracao de codigo MEPA **********

int proximo_rotulo() {
    int rotulo = rotulo_atual;
    rotulo_atual++;
    return rotulo;
}

// ********** Fim da definicao de funcoes do analisador semantico **********