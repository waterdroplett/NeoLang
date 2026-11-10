#include "lexer.h"
#include "neostd.h"
#include "colors.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int add_token(TokenList *tokens, char *lexeme, TokenType type, unsigned int line, int column)
{
    {
        void *temp = realloc(tokens->tokens,(tokens->count + 1) * sizeof(Token));
        if (temp != NULL)
        {
            tokens->tokens = temp;
        }
        else
        {
            return 1;
        }
    }
    tokens->tokens[tokens->count].column = column;
    tokens->tokens[tokens->count].line = line;
    tokens->tokens[tokens->count].type = type;
    strcpy(tokens->tokens[tokens->count].lexeme,lexeme);
    tokens->count++;
    return 0;
}

int realloc_tokens(TokenList *tokens, size_t amount)
{
    void *temp = realloc(tokens->tokens,(tokens->count + amount) * sizeof(Token));
    if (temp != NULL)
    {
        tokens->tokens = temp;
    }
    else
    {
        return 1;
    }
    return 0;
}

int lexer_lex(const char *filename, TokenList *tokens)
{
    FILE *file = fopen(filename,"r");
    if (!file)
    {
        printf(C_RED"neolang: failed to open '%s'\n"C_RESET, filename);
        return 1;
    }

    tokens->count = 0;

    char buf[512];

    unsigned int line = 0;
    int i = 0;
    while (fgets(buf,sizeof(buf),file))
    {
        line++;
        i = 0;
        while (buf[i] != '\n' && buf[i] != '\0')
        {
            if (iswhitespace(buf[i]))
            {
                i++;
            }
            else if (buf[i] == '/' && buf[i+1] == '/')
            {
                break;
            }
            else if (isnum(buf[i]))
            {
                int success = realloc_tokens(tokens,1);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                int start = i;
                tokens->tokens[tokens->count].column = i;
                tokens->tokens[tokens->count].line = line;
                if ((buf[i+1] == 'x' || buf[i+1] == 'X') && isxnum(buf[i+2]))
                {
                    i += 2;
                    while (isxnum(buf[i]))
                    {
                        i++;
                    }
                    tokens->tokens[tokens->count].type = TOKEN_HEX;
                }
                else
                {
                    while (isnum(buf[i]))
                    {
                        i++;
                    }
                    tokens->tokens[tokens->count].type = TOKEN_NUMBER;
                }
                int n = i - start;
                strncpy(tokens->tokens[tokens->count].lexeme,buf + start,n);
                tokens->tokens[tokens->count].lexeme[n] = '\0';
                tokens->count++;
            }
            else if (isalphalet(buf[i]))
            {
                int success = realloc_tokens(tokens,1);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                int start = i;
                tokens->tokens[tokens->count].column = i;
                tokens->tokens[tokens->count].line = line;
                while (isalphalet(buf[i]) || isnum(buf[i]) || buf[i] == '_')
                {
                    i++;
                }
                int n = i - start;
                strncpy(tokens->tokens[tokens->count].lexeme,buf + start,n);
                tokens->tokens[tokens->count].lexeme[n] = '\0';
                if (!strcmp(tokens->tokens[tokens->count].lexeme,"var"))
                {
                    tokens->tokens[tokens->count].type = TOKEN_VAR;
                }
                else
                {
                    tokens->tokens[tokens->count].type = TOKEN_IDENTIFIER;
                }
                tokens->count++;
            }
            else if (buf[i] == '+')
            {
                int success = add_token(tokens,"+",TOKEN_PLUS,line,i);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                i++;
            }
            else if (buf[i] == '-')
            {
                int success = add_token(tokens,"-",TOKEN_MINUS,line,i);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                i++;
            }
            else if (buf[i] == '*')
            {
                int success = add_token(tokens,"*",TOKEN_STAR,line,i);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                i++;
            }
            else if (buf[i] == '/')
            {
                int success = add_token(tokens,"/",TOKEN_SLASH,line,i);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                i++;
            }
            else if (buf[i] == ';')
            {
                int success = add_token(tokens,";",TOKEN_SEMICOLON,line,i);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                i++;
            }
            else if (buf[i] == ':')
            {
                int success = add_token(tokens,":",TOKEN_COLON,line,i);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                i++;
            }
            else if (buf[i] == ',')
            {
                int success = add_token(tokens,",",TOKEN_COMMA,line,i);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                i++;
            }
            else if (buf[i] == '=' && buf[i+1] != '=')
            {
                int success = add_token(tokens,"=",TOKEN_EQUAL,line,i);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                i++;
            }
            else if (buf[i] == '=' && buf[i+1] == '=')
            {
                int success = add_token(tokens,"==",TOKEN_DOUBLE_EQUAL,line,i);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                i += 2;
            }
            else if (buf[i] == '~' && buf[i+1] == '=')
            {
                int success = add_token(tokens,"~=",TOKEN_NOT_EQUAL,line,i);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                i += 2;
            }
            else if (buf[i] == '(')
            {
                int success = add_token(tokens,"(",TOKEN_LPAREN,line,i);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                i++;
            }
            else if (buf[i] == ')')
            {
                int success = add_token(tokens,")",TOKEN_RPAREN,line,i);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                i++;
            }
            else if (buf[i] == '[')
            {
                int success = add_token(tokens,"[",TOKEN_LBRACKET,line,i);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                i++;
            }
            else if (buf[i] == ']')
            {
                int success = add_token(tokens,"]",TOKEN_RBRACKET,line,i);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                i++;
            }
            else if (buf[i] == '{')
            {
                int success = add_token(tokens,"{",TOKEN_LCURLY,line,i);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                i++;
            }
            else if (buf[i] == '}')
            {
                int success = add_token(tokens,"}",TOKEN_RCURLY,line,i);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                i++;
            }
            else if (buf[i] == '"')
            {
                int success = realloc_tokens(tokens,1);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                i++;
                int start = i;
                tokens->tokens[tokens->count].column = i;
                tokens->tokens[tokens->count].line = line;
                tokens->tokens[tokens->count].type = TOKEN_STRING;
                while (buf[i] != '"' && buf[i] != '\n' && buf[i] != '\0')
                {
                    i++;
                    if (buf[i] == '\n' || buf[i] == '\0')
                    {
                        printf(C_RED"neolang: unclosed string on line %u\n"C_RESET, line);
                        return 1;
                    }
                }
                int n = i - start;
                strncpy(tokens->tokens[tokens->count].lexeme,buf + start,n);
                tokens->tokens[tokens->count].lexeme[n] = '\0';
                tokens->count++;
                i++;
            }
            else 
            {
                char short_buf[2];
                short_buf[0] = buf[i];
                short_buf[1] = '\0';
                int success = add_token(tokens,short_buf,TOKEN_UNKNOWN,line,i);
                if (success != 0)
                {
                    printf(C_RED"neolang: failed to add token\n"C_RESET);
                    return 1;
                }
                printf(C_RED"neolang: unknown token at column %d, line %u\n"C_RESET, i, line);
                i++;
            }
        }
        
    }

    fclose(file);
    return 0;
}