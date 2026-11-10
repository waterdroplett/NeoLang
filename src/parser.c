#include "parser.h"
#include "colors.h"
#include "neostd.h"
#include <stdlib.h>
#include <string.h>

static ASTNode buf_node;
static ASTNode buf_node_two;

static int parser_error(char *__restrict__ _msg, int code)
{
    printf(_msg);
    return code;
}

int is_at_end(Parser *parser)
{
    if (parser->current >= parser->tokens->count)
        return 1;

    return 0;
}

Token *previous(Parser *parser)
{
    return &parser->tokens->tokens[parser->current - 1];
}

Token *advance(Parser *parser)
{
    if (!is_at_end(parser))
        parser->current++;

    return previous(parser);
}

Token *peek(Parser *parser)
{
    return &parser->tokens->tokens[parser->current];
}

ASTProgram *ast_program_create(void)
{
    ASTProgram *node = malloc(sizeof(ASTProgram));

    node->statements = NULL;
    node->count = 0;

    return node;
}

int ast_node_add(ASTProgram *program, ASTNode *statement)
{
    ASTNode **temp = realloc(
        program->statements,
        (program->count + 1) * sizeof(ASTNode *)
    );

    if (temp == NULL)
        return 1;

    program->statements = temp;
    program->statements[program->count] = statement;
    program->count++;

    return 0;
}

static int parse_declaration(Parser *parser, Token *token)
{
    buf_node.type = AST_DECLARATION;
    if (peek(parser)->type != TOKEN_IDENTIFIER)
    {
        printf(C_RED"neolang: unexpected token after var on line %u\n"C_RESET,token->line);
        return 1;
    }
    token = advance(parser);
    buf_node.data.declaration.name = malloc(sizeof(ASTNode));
    if (buf_node.data.declaration.name == NULL)
    {
        return parser_error(C_RED"neolang: failed to create AST node\n"C_RESET,1);
    }
    buf_node.data.declaration.name->type = AST_VARIABLE;
    strcpy(buf_node.data.declaration.name->data.variable_name,token->lexeme);
    if (peek(parser)->type != TOKEN_EQUAL)
    {
        printf(C_RED"neolang: unexpected token after %s on line %u\n"C_RESET,token->lexeme,token->line);
        return 1;
    }
    token = advance(parser);
    if (peek(parser)->type != TOKEN_NUMBER && peek(parser)->type != TOKEN_HEX && peek(parser)->type != TOKEN_IDENTIFIER && 
        peek(parser)->type != TOKEN_STRING)
    {
        printf(C_RED"neolang: unexpected token after %s on line %u\n"C_RESET,token->lexeme,token->line);
        return 1;
    }
    token = advance(parser);
    if (token->type == TOKEN_NUMBER)
    {
        buf_node_two.type = AST_INTEGER;
        buf_node_two.data.integer = atoi(token->lexeme);
    }
    else if (token->type == TOKEN_HEX)
    {
        int failed;
        unsigned long result = strtolu(token->lexeme,&failed);
        if (failed != 0)
        {
            if (failed == 2)
            {
                printf(C_RED"neolang: hex value contains junk '%s'\n"C_RESET, token->lexeme);
            }
            if (failed == 3)
            {
                printf(C_RED"neolang: hex value too large '%s'\n"C_RESET, token->lexeme);
            }
            return 1;
        }
        buf_node_two.type = AST_HEX;
        buf_node_two.data.hex = result;
    }
    else if (token->type == TOKEN_IDENTIFIER)
    {
        buf_node_two.type = AST_VARIABLE;
        strcpy(buf_node_two.data.variable_name, token->lexeme);
    }
    else if (token->type == TOKEN_STRING)
    {
        buf_node_two.type = AST_STRING;
        strcpy(buf_node_two.data.variable_name, token->lexeme);
    }

    if (peek(parser)->type != TOKEN_SEMICOLON)
    {
        printf(C_RED"neolang: unexpected token after %s on line %u\n"C_RESET,token->lexeme,token->line);
        return 1;
    }

    buf_node.data.declaration.value = malloc(sizeof(ASTNode));
    if (buf_node.data.declaration.value == NULL)
    {
        return parser_error(C_RED"neolang: failed to create AST node\n"C_RESET,1);
    }

    advance(parser);

    *buf_node.data.declaration.value = buf_node_two;

    ASTNode *newNode = malloc(sizeof(ASTNode));

    if (newNode == NULL)
    {
        return parser_error(C_RED"neolang: failed to create AST node\n"C_RESET,1);
    }

    *newNode = buf_node;

    int success = ast_node_add(parser->program,newNode);
    if (success != 0) {return parser_error(C_RED"neolang: failed to create AST node\n"C_RESET,success);}
}

int parser_parse(Parser *parser)
{
    parser->program = malloc(sizeof(ASTProgram));
    if (parser->program == NULL)
    {
        return parser_error(C_RED"neolang: failed to intialize program node\n"C_RESET,1);
    }

    parser->program->count = 0;
    parser->program->statements = NULL;
    
    parser->current = 0;
    while (!is_at_end(parser))
    {
        Token *token = advance(parser);

        if (token->type == TOKEN_VAR)
        {
            parse_declaration(parser,token);
        }
    }

    return 0;
}