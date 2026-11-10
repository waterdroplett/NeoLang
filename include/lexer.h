#ifndef NEOLANG_LEXER_H
#define NEOLANG_LEXER_H

#define MAX_LEXEME_VALUE 64

typedef enum 
{
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_HEX,
    TOKEN_STRING,

    TOKEN_VAR,

    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,

    TOKEN_EQUAL,
    TOKEN_DOUBLE_EQUAL,
    TOKEN_NOT_EQUAL,

    TOKEN_SEMICOLON,
    TOKEN_COLON,
    TOKEN_COMMA,

    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_LCURLY,
    TOKEN_RCURLY,

    TOKEN_UNKNOWN,
} TokenType;

typedef struct 
{
    char lexeme[MAX_LEXEME_VALUE];
    TokenType type;

    unsigned int column;
    unsigned int line;
} Token;

typedef struct 
{
    Token *tokens;
    unsigned int count;
} TokenList;

int lexer_lex(const char *filename, TokenList *tokens);

#endif