//header file for token.c
#ifndef TOKEN_H
#define TOKEN_H

#include "vect.h"
//check if a given string is valid
int is_valid_string(char ch);
//read from input to output
int read_string(const char *input, char *output);
//function to take a string and return its tokens in a list
vect_t *tokenizeHelper(char *str);

#endif