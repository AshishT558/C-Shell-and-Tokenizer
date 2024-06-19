#include <assert.h>
#include <stdio.h>


#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include "token.h"


//The maximum size of a single line shall be at least 255 characters.
#define MAX_SIZE 256
//previously entered command in the shell
static char *previousCommand;

//goes through an array of strings and check if there is a > or < inside
bool needsRedirect(char **args) {
  bool hasRedirect = false;
  int i = 0;
  while(args[i] != NULL) {
    if (strcmp(args[i], ">") == 0 || strcmp(args[i], "<") == 0) {
      hasRedirect = true;
    }
    i++;
  }
  return hasRedirect;
}

//finds which type of redirect is needed and splits command into sides
void find_redirect_args(char **args) {
  
  //string array of the command that will be redirected
  char *sendArgs[MAX_SIZE];
  int i = 0;
  while(args[i] != NULL) {
    if (strcmp(args[i], ">") == 0) {
      int j = 0;
      while(j < i && sendArgs[j] != NULL) {
        sendArgs[j] = args[j];
        j++;
      }
      sendArgs[j] = NULL;
      redirect_single(sendArgs, args[i+1], "input right");

    }
    if (strcmp(args[i], "<") == 0) {
      int j = 0;
      while(j < i) {
        sendArgs[j] = args[j];
        j++;
      }
      sendArgs[j] = NULL;
      redirect_single(sendArgs, args[i+1], "input left");

    }
    i++;
  }
}


//takes the different sides and redirects as indicated by the mode
void redirect_single(char **sendArgs, char *baseArg, char *mode) {
    int fileDescriptor;

    // Create a child process
    //pid_t pid = fork();

   if (fork() == 0) { // In the child process
    //when you have a ">"
    if (strcmp(mode, "input right") == 0) {
      fileDescriptor = open(baseArg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (fileDescriptor == -1) {
        perror("Error opening file for output");
          exit(1);
      }

      // Close standard output (file descriptor 1)
      if (close(1) == -1) {
        perror("Error closing stdout");
        exit(1);
      }

      // Replace standard output with the opened file descriptor
      if (dup(fileDescriptor) == -1) {
        perror("Error duplicating file descriptor");
        exit(1);
      }

      close(fileDescriptor);
    }
    //when you have a "<" 
    else if (strcmp(mode, "input left") == 0) {
      fileDescriptor = open(baseArg, O_RDONLY);
      if (fileDescriptor == -1) {
        perror("Error opening file for input");
        exit(1);
      }

    
      // Close standard input (file descriptor 0)
      if (close(0) == -1) {
        perror("Error closing stdin");
        exit(1);
      }

      // Replace standard input with the opened file descriptor
      if (dup(fileDescriptor) == -1) {
        perror("Error duplicating file descriptor");
        exit(1);
      }

      close(fileDescriptor);
    }

    // Execute the command
    execvp(sendArgs[0], sendArgs);
    perror("Error - execvp failed");
    exit(1);
  } 
  else { // In the parent process
    wait(NULL);
  }
}

//able to execute one given command
void execute_single(char **args) {
  // ignore enter command with empty args
  if (args[0] == NULL) {
    return;
  }
  //case of exit
  else if (strcmp(args[0], "exit") == 0) {
    printf("Bye bye.\n");
    exit(0);
  }
  //case of help
  else if (strcmp(args[0], "help") == 0) {
    printf("Built-in Commands:\n");
    printf("cd : Changes the current working directory of the shell to the path specified as the argument.\n");
    printf("source : Execute a script. Takes a filename as an argument and processes each line of the file as a command.\n");
    printf("prev : Prints the previous command line and executes it again, without becoming the new command line.\n");
    printf("help : Explains all the built-in commands available in this shell.\n");
    printf("exit : exit the shell. Can also be executed with Ctrl + D.\n");
  }
  //case of cd
  else if (strcmp(args[0], "cd") == 0) {
    chdir(args[1]);
  }

  //case of source
  else if (strcmp(args[0], "source") == 0) {
    char *fileName = args[1];
    FILE* file = fopen(fileName, "r");
    if(file == NULL) {
      printf("%s : file not found \n", args[1]);
    }
    //look at each line in the file and send it to parser
    char inputLine[MAX_SIZE];
    while(fgets(inputLine, MAX_SIZE, file) != NULL) {
      read_input(inputLine);
    }
  }

  //case of prev
  else if (strcmp(args[0], "prev") == 0) {
    if(previousCommand != NULL) {
      read_input(previousCommand);
    }
  }

  //when it is a custom command and not built into exec
  if (fork() == 0) {
    //run custom command
    if(execvp(args[0], args) == -1) {
      printf("%s: command not found \n", args[0]);
      exit(1);
    }   
  } 
  else {
    wait(NULL);
    }
}

void emptyArr(char** array) {
  int i = 0;
  while(array[i] != NULL) {
    array[i] = NULL;
    i++;
  }
}
//parses the given string into multiple commands using tokenizer and runs them
void read_input(char *input) {
  //get tokens
  vect_t *tokens = tokenizeHelper(input);
  //place to hold each separate command
  char **args;
  //to mark where in the arg array you are
  int argindex = 0;
  //go through list of each token set
  for(int i = 0; i < vect_size(tokens); i++) {
    char *token = vect_get_copy(tokens, i);
    //if sequencing, split and run first command that is stored
    if (strcmp(token,";") == 0) { 
      args[argindex] = NULL;
      //check if first command needs a redirect
      if (needsRedirect(args)) {
        find_redirect_args(args);
      }
      //otherwise execute normally
      else {
        execute_single(args); 
      }
      emptyArr(args);
      argindex = 0;
      continue;
    }
    else {
      args[argindex] = token;
      argindex++;
    }
    
  }
  args[argindex] = NULL;
  //check if last command needs a redirect
  if (needsRedirect(args)) {
    find_redirect_args(args);
  }
  //otherwise execute normally
  else {
    execute_single(args); 
  }
}


int main(int argc, char **argv) {
  //initialize previous command to NULL
  previousCommand = NULL;
  // After starting, the shell should print a welcome message: Welcome to mini-shell.
  printf("Welcome to mini-shell\n");

  // runs until the user terminates the program.
  while(1) {

    char input[MAX_SIZE]; // Initializes an input buffer of 256 bytes.
    
    printf("shell $ ");
    fflush(stdout);
    
    
    // Reads what is entered into stdin into the input buffer.
    char *prompt = fgets(input, MAX_SIZE, stdin);

    //int len = strlen(input);
    // if (len > 0 && input[len-1] == '\n') {
    //   input[len-1] = '\0';
    // }
    
    char *exit = "exit";
    // if the user presses ctrl + D or exit
    if(prompt == NULL || (strcmp(prompt, exit) == 0)) { 
      printf("Bye bye.\n");
      return 0;
    }
    //send string to parser
    read_input(input);
    previousCommand = input;

  }
  return 0;
}