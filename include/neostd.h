#ifndef NEOLANG_NEOSTD_H
#define NEOLANG_NEOSTD_H

int iswhitespace(char c);
int isnum(char c);
int isxnum(char c);
int isalphalet(char c);

unsigned long strtolu(char *__restrict__ _str, int *__restrict__ _failed);

#endif