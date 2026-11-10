#include "neostd.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int iswhitespace(char c)
{
    if (c == ' ' || c == '\t' || c == '\r')
    {
        return 1;
    }

    return 0;
}
int isnum(char c)
{
    if (c >= '0' && c <= '9')
    {
        return 1;
    }
    
    return 0;
}
int isxnum(char c)
{
    if (c >= '0' && c <= '9')
    {
        return 1;
    }
    if (c >= 'a' && c <= 'f')
    {
        return 1;
    }
    if (c >= 'A' && c <= 'F')
    {
        return 1;
    }

    return 0;
}
int isalphalet(char c)
{
    if (c >= 'a' && c <= 'z')
    {
        return 1;
    }
    if (c >= 'A' && c <= 'Z')
    {
        return 1;
    }

    return 0;
}

unsigned long strtolu(char *__restrict__ _str, int *__restrict__ _failed)
{
    unsigned long result = 0;
    int i = 0;

    if (_str[0] != '0' || (_str[1] != 'x' && _str[1] != 'X'))
    {
        if (_failed != NULL)
        {
            *_failed = 1;
        }
        return 0;
    }

    i = 2;

    while (_str[i] != '\0')
    {
        int val = 0;

        if (_str[i] >= '0' && _str[i] <= '9')
        {
            val = _str[i] - '0';
        }
        else if (_str[i] >= 'a' && _str[i] <= 'f')
        {
            val = _str[i] - 'a' + 10;
        }
        else if (_str[i] >= 'A' && _str[i] <= 'F')
        {
            val = _str[i] - 'A' + 10;
        }
        else
        {
            if (_failed != NULL)
            {
                *_failed = 2;
            }
            break;
        }

        result = (result << 4) | val;
        i++;
    }
    
    return result;
}