//Tokenize function to be used in tokenizer.c and shell.c
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "token.h"
//#include "vect.h"
//#include "vect.c"

/**
 * Is the given character a string?
 */
int is_valid_string(char ch) {
  // checking whether the given char is a valid char
  return ch != ' ' 
  && ch != '"'
  && ch != '('
  && ch != ')'
  && ch != '>'
  && ch != '<'
  && ch != ';'
  && ch != '|'
  && ch != '\n'
  && ch != '\t';
}

/**
 * Read the next integer as a string from the input into the output.
 */
int read_string(const char *input, char *output) {
  int i = 0;
  // while we have input and the character is a string,
  while (input[i] != '\0' && is_valid_string(input[i])) {
    output[i] = input[i]; // copy character to output buffer
    ++i;
  }
  output[i] = '\0'; // add the terminating byte

  return i; // return the length of the string
}

/**
 * Read the entire sentence if a sentence is detected.
 */
int read_sentence(const char *input, char *output) {
  int i = 0;
  // while we have input and the character is a string,
  while (input[i] != '\0' && input[i] != '"') {
    output[i] = input[i]; // copy character to output buffer
    ++i;
  }
  output[i] = '\0'; // add the terminating byte

  return i; // return the length of the string
}

vect_t *tokenizeHelper(char *str) {
  //char expr[] = "12+4 - 20543 /         12"; // example expression string
  char buf[256]; // temp buffer

  vect_t *listTokens = vect_new(); //list of Tokens, starts empty

  bool inQuotes = false; //whether a certain part of the string is in quotes

  int i = 0; // current position in string
  while (str[i] != '\0') { // while the end of string is not reached

  // if Quotation is detected, set inQuotes to true and go to next char.
  if(str[i] == '"' && !inQuotes) {
    inQuotes = true;

    ++i; // move to the next index.
    continue;
  }
  
  // if inQuotes is true, read the sentence until a terminating quote is reached.
  if(inQuotes) {
    inQuotes = false;
    i+= read_sentence(&str[i], buf);
    vect_add(listTokens, buf);
    continue;
  }
   
    // If the current char is a valid String
    if (is_valid_string(str[i])) {

      // Reads an entire String and adds it to buffer.
      i += read_string(&str[i], buf); 

      vect_add(listTokens, buf);
      continue; // skip the rest of this iteration
    }

    // if not valid string : 
    switch (str[i]) {
      case '(':
        vect_add(listTokens, (char[2]) { (char) str[i], '\0' } );
        break;
      case ')':
        vect_add(listTokens, (char[2]) { (char) str[i], '\0' } );
        break;
      case '<':
        vect_add(listTokens, (char[2]) { (char) str[i], '\0' } );
        break;
      case '>':
        vect_add(listTokens, (char[2]) { (char) str[i], '\0' } );
        break;
      case ';':
        vect_add(listTokens, (char[2]) { (char) str[i], '\0' } );
        break;
      case '|':
        vect_add(listTokens, (char[2]) { (char) str[i], '\0' } );
        break;

    }

    ++i; // advance to the next character
  }

  return listTokens;

}