#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define BUF_SIZE 256

int main( int argc, char ** argv ) { //  0x00010540
  char buf[BUF_SIZE];
  if(argc < 2) {
    fprintf(stderr, "Usage: stack <some input>\n");
    exit(1);
  }
  strcpy(buf, argv[1]);
  return 0;
}

void hidden_function( void ) { // 0x000105b4
  fprintf(stderr, "This program has been exploited\n");
}
