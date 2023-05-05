# GLVM disassembler
This tool disassembles a program for the GLVM architecture into the assembly language I developed.

```
Usage:
	./dump program.bin output.asm
```

# Warning
The disassembler is not able to locate strings. Therefore, those bytes are interpreted as instructions.

Excluding that, the output of the program can be reassembled with the GLVM assembler.