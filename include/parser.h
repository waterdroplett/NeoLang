#ifndef NEOLANG_PARSER_H
#define NEOLANG_PARSER_H

#include "lexer.h"
#include <stdio.h>

typedef struct ASTNode ASTNode;

typedef enum 
{
    AST_PROGRAM,
    AST_INTEGER,
    AST_HEX,
    AST_BINARY,
    AST_VARIABLE,
    AST_STRING,
    AST_DECLARATION,
} ASTNodeType;

typedef struct 
{
    ASTNode **statements;
    size_t count;
} ASTProgram;

struct ASTNode
{
    ASTNodeType type;

    union 
    {
        ASTProgram program;

        int integer;
        unsigned long hex;

        struct 
        {
            ASTNode *left;
            ASTNode *right;
            int oper;
        } binary;

        char variable_name[MAX_LEXEME_VALUE];

        struct 
        {
            ASTNode *name;
            ASTNode *value;
        } declaration;
    } data;
};

typedef struct 
{
    TokenList *tokens;
    size_t current;
    ASTProgram *program;
} Parser;

int is_at_end(Parser *parser);

Token *previous(Parser *parser);

Token *advance(Parser *parser);

Token *peek(Parser *parser);

ASTProgram *ast_program_create(void);
int node_create(ASTNode *parent, ASTNode child);
int parser_parse(Parser *parser);

#endif