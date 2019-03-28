# Shellcode

Uses a stack overflow to insert code on the stack that opens a shell

Potential improvements to be made;
- Currently puts the shellcode after the return, meaning the buffer itself is not actively used. Could alter the shellcode to not use the end as the string terminating null byte and place everything in the buffer itself.
