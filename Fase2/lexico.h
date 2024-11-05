// declaração de tipos e funções


#ifndef __LEXICO_H
#define __LEXICO_H

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

// Declaracao de funcao de leitura de conteudo do arquivo para buffer
void file_to_buffer(char* file_name);

#endif