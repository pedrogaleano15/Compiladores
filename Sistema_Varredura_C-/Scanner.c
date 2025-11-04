/*
 * scanner.c - Analisador Léxico para a linguagem C-
 *
 * Este programa implementa um scanner (analisador lexico) para uma
 * versao simplificada da linguagem C (C-).
 *
 * Ele le um arquivo-fonte (.c-) e imprime todos os tokens encontrados,
 * mostrando o tipo (palavra-chave, operador, simbolo, numero, etc.)
 * e o lexema (texto correspondente).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ===============================================================
   ENUMERACAO DE TIPOS DE TOKENS
   ---------------------------------------------------------------
   Cada valor enum representa um tipo de token reconhecido pelo
   analisador lexico.
   =============================================================== */
typedef enum {
    FIMARQUIVO,   // Fim do arquivo
    ERRO,         // Erro de leitura ou simbolo invalido
    // Palavras-chave reservadas (da linguagem C-, nao do C)
    IF, ELSE, INT, RETURN, VOID, WHILE,
    // Simbolos (parenteses, chaves, colchetes, etc.)
    PAREN_ESQ, PAREN_DIR, COL_ESQ, COL_DIR, CHAVE_ESQ, CHAVE_DIR,
    VIRGULA, PONTOVIRGULA,
    // Operadores
    MAIS, MENOS, VEZES, DIV,
    MENOR, MENOR_IGUAL, MAIOR, MAIOR_IGUAL, IGUAL, DIFERENTE, ATRIBUICAO,
    E_LOGICO, OU_LOGICO, // Operadores logicos
    // Outros
    IDENTIFICADOR,   // Identificador (ex: nome de variavel)
    NUMERO           // Numero (literal inteiro)
} TipoToken;

/* ===============================================================
   ESTRUTURA DE TOKEN
   ---------------------------------------------------------------
   Representa um token encontrado no codigo-fonte, com:
   - tipo: tipo do token (segundo o enum acima)
   - lexema: cadeia de caracteres correspondente
   =============================================================== */
typedef struct {
    TipoToken tipo;
    char lexema[256];  // Texto do token (max. 255 caracteres)
} Token;

/* ===============================================================
   VARIAVEIS GLOBAIS
   =============================================================== */
FILE *arquivoFonte;      // Ponteiro para o arquivo de entrada (.c-)
char linhaAtual[256];    // Buffer que armazena a linha atual lida
int posLinha = 0;        // Posicao atual dentro do buffer da linha
int numLinha = 0;        // Numero da linha (para debug/erros)
int bufferPronto = 0;    // Indica se a linha foi lida e esta pronta

/* ===============================================================
   FUNCOES AUXILIARES
   =============================================================== */

/* ---------------------------------------------------------------
   obterProximoChar()
   Le o proximo caractere do arquivo-fonte.
   Retorna EOF se chegar ao fim.
   --------------------------------------------------------------- */
char obterProximoChar() {
    // Se o buffer acabou ou ainda nao foi preparado, le nova linha
    if (!bufferPronto || linhaAtual[posLinha] == '\0') {
        if (fgets(linhaAtual, sizeof(linhaAtual), arquivoFonte)) {
            posLinha = 0;
            bufferPronto = 1;
            numLinha++; // Incrementa numero da linha
            return linhaAtual[posLinha++];
        } else {
            return EOF; // Fim do arquivo
        }
    }
    // Caso contrario, retorna o proximo caractere da linha
    return linhaAtual[posLinha++];
}

/* ---------------------------------------------------------------
   devolverChar()
   Volta um caractere no buffer (lookahead).
   Usado quando o scanner le um caractere a mais.
   --------------------------------------------------------------- */
void devolverChar() {
    if (posLinha > 0) {
        posLinha--;
    }
}

/* ---------------------------------------------------------------
   imprimirToken()
   Exibe na tela o tipo e o lexema do token reconhecido.
   --------------------------------------------------------------- */
void imprimirToken(Token token) {
    const char* tipoString;

    // Traduz o tipo de token para texto legivel
    switch (token.tipo) {
        // Palavras-chave
        case IF: case ELSE: case INT: case RETURN: case VOID: case WHILE:
            tipoString = "Palavra-Chave"; break;
        // Simbolos
        case PAREN_ESQ: case PAREN_DIR: case COL_ESQ: case COL_DIR:
        case CHAVE_ESQ: case CHAVE_DIR: case VIRGULA: case PONTOVIRGULA:
            tipoString = "Simbolo"; break;
        // Operadores
        case MAIS: case MENOS: case VEZES: case DIV:
        case MENOR: case MENOR_IGUAL: case MAIOR: case MAIOR_IGUAL:
        case IGUAL: case DIFERENTE: case ATRIBUICAO:
        case E_LOGICO: case OU_LOGICO:
            tipoString = "Operador"; break;
        // Identificadores e numeros
        case IDENTIFICADOR: tipoString = "ID"; break;
        case NUMERO:        tipoString = "NUM"; break;
        // Outros
        case FIMARQUIVO: tipoString = "FIMARQUIVO"; break;
        case ERRO:       tipoString = "ERRO"; break;
        default:         tipoString = "DESCONHECIDO";
    }

    // Imprime o token de forma formatada
    if (token.tipo == FIMARQUIVO)
        printf("[Tipo: %s]\n", tipoString);
    else
        printf("[Tipo: %s, Lexema: %s]\n", tipoString, token.lexema);
}

/* ---------------------------------------------------------------
   verificarPalavraChave()
   Verifica se uma string (identificador) e uma palavra-chave.
   --------------------------------------------------------------- */
TipoToken verificarPalavraChave(char* s) {
    if (strcmp(s, "if") == 0) return IF;
    if (strcmp(s, "else") == 0) return ELSE;
    if (strcmp(s, "int") == 0) return INT;
    if (strcmp(s, "return") == 0) return RETURN;
    if (strcmp(s, "void") == 0) return VOID;
    if (strcmp(s, "while") == 0) return WHILE;
    return IDENTIFICADOR; // Se nao for palavra-chave, e um identificador comum
}

/* ===============================================================
   FUNCAO PRINCIPAL DO SCANNER: obterToken()
   ---------------------------------------------------------------
   Le o proximo token do arquivo e retorna sua estrutura.
   =============================================================== */
Token obterToken() {
    Token tokenAtual;
    int posLexema = 0;  // Posicao dentro do lexema
    char c;

    // Ignora espacos, tabulacoes e quebras de linha
    while ((c = obterProximoChar()) == ' ' || c == '\t' || c == '\n');

    // Limpa o conteudo anterior do lexema
    memset(tokenAtual.lexema, 0, sizeof(tokenAtual.lexema));

    // Se chegou ao fim do arquivo
    if (c == EOF) {
        tokenAtual.tipo = FIMARQUIVO;
        return tokenAtual;
    }

    // Adiciona o primeiro caractere do token
    tokenAtual.lexema[posLexema++] = c;

    /* ===========================================================
       VERIFICACAO DO TIPO DE TOKEN PELO PRIMEIRO CARACTERE
       =========================================================== */
    switch (c) {
        /* --- Simbolos Simples --- */
        case '+': tokenAtual.tipo = MAIS; break;
        case '-': tokenAtual.tipo = MENOS; break;
        case '*': tokenAtual.tipo = VEZES; break;
        case '(': tokenAtual.tipo = PAREN_ESQ; break;
        case ')': tokenAtual.tipo = PAREN_DIR; break;
        case '[': tokenAtual.tipo = COL_ESQ; break;
        case ']': tokenAtual.tipo = COL_DIR; break;
        case '{': tokenAtual.tipo = CHAVE_ESQ; break;
        case '}': tokenAtual.tipo = CHAVE_DIR; break;
        case ',': tokenAtual.tipo = VIRGULA; break;
        case ';': tokenAtual.tipo = PONTOVIRGULA; break;

        /* --- Operadores compostos (duplos) --- */
        case '=':
            c = obterProximoChar();
            if (c == '=') {
                tokenAtual.lexema[posLexema++] = c;
                tokenAtual.tipo = IGUAL;  // ==
            } else {
                devolverChar();
                tokenAtual.tipo = ATRIBUICAO; // =
            }
            break;
        case '!':
            c = obterProximoChar();
            if (c == '=') {
                tokenAtual.lexema[posLexema++] = c;
                tokenAtual.tipo = DIFERENTE;  // !=
            } else {
                devolverChar();
                tokenAtual.tipo = ERRO; // '!' sozinho e invalido
            }
            break;
            
        /* --- Operadores Logicos --- */
        case '&':
            c = obterProximoChar();
            if (c == '&') {
                tokenAtual.lexema[posLexema++] = c;
                tokenAtual.tipo = E_LOGICO;  // &&
            } else {
                devolverChar();
                tokenAtual.tipo = ERRO; // '&' sozinho e invalido
            }
            break;
        case '|':
            c = obterProximoChar();
            if (c == '|') {
                tokenAtual.lexema[posLexema++] = c;
                tokenAtual.tipo = OU_LOGICO;  // ||
            } else {
                devolverChar();
                tokenAtual.tipo = ERRO; // '|' sozinho e invalido
            }
            break;

        case '<':
            c = obterProximoChar();
            if (c == '=') {
                tokenAtual.lexema[posLexema++] = c;
                tokenAtual.tipo = MENOR_IGUAL;  // <=
            } else {
                devolverChar();
                tokenAtual.tipo = MENOR;   // <
            }
            break;
        case '>':
            c = obterProximoChar();
            if (c == '=') {
                tokenAtual.lexema[posLexema++] = c;
                tokenAtual.tipo = MAIOR_IGUAL;  // >=
            } else {
                devolverChar();
                tokenAtual.tipo = MAIOR;   // >
            }
            break;

        /* --- Tratamento de Comentarios --- */
        case '/':
            c = obterProximoChar();
            if (c == '*') {
                // Dentro de um comentario de bloco
                char prev_c = '\0';
                while (1) {
                    c = obterProximoChar();
                    if (c == EOF) {
                        tokenAtual.tipo = ERRO;
                        strcpy(tokenAtual.lexema, "Comentario nao fechado");
                        return tokenAtual;
                    }
                    // Sai do loop ao encontrar '*/'
                    if (prev_c == '*' && c == '/') {
                        break;
                    }
                    prev_c = c;
                }
                // Apos o comentario, busca proximo token
                return obterToken();
            } else {
                // E apenas o operador de divisao '/'
                devolverChar();
                tokenAtual.tipo = DIV;
            }
            break;

        /* --- Numeros --- */
        default:
            if (isdigit(c)) {
                tokenAtual.tipo = NUMERO;
                // Continua lendo enquanto forem digitos
                while (isdigit(c = obterProximoChar())) {
                    tokenAtual.lexema[posLexema++] = c;
                }
                devolverChar(); // devolve o caractere nao-numerico
            }

            /* --- Identificadores e Palavras-Chave --- */
            else if (isalpha(c)) {
                tokenAtual.tipo = IDENTIFICADOR;
                // Pode conter letras e numeros
                while (isalpha(c = obterProximoChar()) || isdigit(c)) {
                    tokenAtual.lexema[posLexema++] = c;
                }
                devolverChar();
                // Verifica se e palavra-chave
                tokenAtual.tipo = verificarPalavraChave(tokenAtual.lexema);
            }

            /* --- Qualquer outro caractere e erro --- */
            else {
                tokenAtual.tipo = ERRO;
            }
            break;
    }

    return tokenAtual;
}

/* ===============================================================
   FUNCAO PRINCIPAL (main)
   ---------------------------------------------------------------
   Controla a execucao do programa:
   - Le o nome do arquivo passado como argumento
   - Chama o scanner ate o fim do arquivo
   =============================================================== */
int main(int argc, char *argv[]) {
    // Verifica se o usuario forneceu o arquivo como argumento
    if (argc != 2) {
        fprintf(stderr, "Modo de uso: %s <arquivo.c->\n", argv[0]);
        return 1;
    }

    // Tenta abrir o arquivo
    arquivoFonte = fopen(argv[1], "r");
    if (arquivoFonte == NULL) {
        fprintf(stderr, "Erro: Nao foi possivel abrir o arquivo '%s'\n", argv[1]);
        return 1;
    }

    printf("Iniciando varredura do arquivo: %s\n", argv[1]);
    printf("-----------------------------------------\n");

    // Loop principal do scanner
    Token tokenAtual;
    do {
        tokenAtual = obterToken();  // Obtem proximo token
        imprimirToken(tokenAtual);  // Mostra resultado
    } while (tokenAtual.tipo != FIMARQUIVO);

    printf("-----------------------------------------\n");
    printf("Varredura concluida.\n");

    // Fecha o arquivo antes de sair
    fclose(arquivoFonte);
    return 0;
}