#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colors.h"
#include "lexer.h"
#include "parser.h"

void print_ast_node(ASTNode *__restrict__ node, int indentation)
{
    char type_buf[MAX_LEXEME_VALUE];
    char str_buf[MAX_LEXEME_VALUE];
    switch (node->type)
    {
    case AST_INTEGER:
    {
        strcpy(type_buf,"Integer");
        snprintf(str_buf,sizeof(str_buf),"%d",node->data.integer);
        break;
    }
    case AST_VARIABLE:
    {
        strcpy(type_buf,"Variable");
        strcpy(str_buf,node->data.variable_name);
        break;
    }
    case AST_DECLARATION:
    {
        strcpy(type_buf,"Declaration");
        str_buf[0] = '\0';
        for (int i = 0; i < indentation; i++)
        {
            printf("\t");
        }

        printf("%s | %s\n", type_buf, str_buf);
        print_ast_node(node->data.declaration.name,indentation + 1);
        print_ast_node(node->data.declaration.value,indentation + 1);
        return;
    }
    case AST_HEX:
    {
        strcpy(type_buf,"Hex");
        snprintf(str_buf,sizeof(str_buf),"%lu",node->data.hex);
        break;
    }
    case AST_STRING:
    {
        strcpy(type_buf,"String");
        strcpy(str_buf,node->data.variable_name);
        break;
    }
    case AST_BINARY:
    {
        strcpy(type_buf,"Binary");
        switch (node->data.binary.oper)
        {
        case 0:
            strcpy(str_buf,"+");
            break;
        case 1:
            strcpy(str_buf,"-");
            break;
        case 2:
            strcpy(str_buf,"*");
            break;
        case 3:
            strcpy(str_buf,"/");
            break;
        
        default:
            break;
        }
        for (int i = 0; i < indentation; i++)
        {
            printf("\t");
        }

        printf("%s | %s\n", type_buf, str_buf);
        print_ast_node(node->data.binary.left, indentation + 1);
        print_ast_node(node->data.binary.right, indentation + 1);
        return;
    }
    
    default:
        break;
    }

    for (int i = 0; i < indentation; i++)
    {
        printf("\t");
    }

    printf("%s | %s\n", type_buf, str_buf);
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf(C_RED"neolang: fatal error: no file to run\n"C_RESET);
        return 1;
    }

    TokenList tokens;
    Parser parser;
    tokens.count = 0;
    tokens.tokens = NULL;

    int lex_success = lexer_lex(argv[1], &tokens);
    if (lex_success != 0)
    {
        return lex_success;
    }

    parser.tokens = &tokens;

    for (int i = 0; i < tokens.count; i++)
    {
        printf("Token #%d: %s | %d\n", i, tokens.tokens[i].lexeme, tokens.tokens[i].type);
    }

    int parse_success = parser_parse(&parser);
    if (parse_success != 0)
    {
        return parse_success;
    }

    for (int i = 0; i < parser.program->count; i++)
    {
        print_ast_node(parser.program->statements[i],0);
    }
    
    return 0;
}
