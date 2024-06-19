#include "token.h"
//#include "token.c"
//#include "vect.h"

#include <assert.h>
#include <stdio.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv) {
  // TODO: Implement the tokenize demo.
  char buffer[255];
  read(0, buffer, 255);

  vect_t *listTokens = tokenizeHelper(buffer);

  for(int i = 0; i < vect_size(listTokens); i++) {
    const char *token = vect_get(listTokens, i);
    printf("%s\n", token);
  }
  
}
