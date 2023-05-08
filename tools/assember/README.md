# GLVM assembler
This tool compiles a program written in the assembly language I developed for the GLVM architecture.

```
Usage:
	./az source.asm output.bin
```

## Syntax
First of all, comments are preceded by a `;`
```
; This is a comment
```

All numbers are written in hexadecimal, and they need the symbol '#'
```
#2a85 ; This is a literal number
```

To declare and use constants, the `$` symbol is used:
```
$print #0008 ; Now print is 0x0008
call $print  ; Here print will be replaced for 0x0008
```

All programs need an entry point defined (The code will start here)
```
entry #f000 ; The code will start at 0xf000
```

To declare an use a string, the `.` symbol. The location in memory will be defined by the assembler:
```
mov %r2, .which_addr  ; .which_addr will be replaced by the location of the string
.which_addr "> Which address? " 
```

Strings can contain special symbols:
```
.enter_data "> Enter hex data. End with dot \".\" + newline:\n"
.exec_at "> Really exec at \xF0\x5e\xFB? Type Y if so: "
```

Additionally char literals, up to two characters can be used in place with the `'` symbol:
```
cmp %r0, 'UC'
cmp %r0, 'r'
cmp %r0, 'h\n'
```

There are also labels. Labels addresses will be defined while assembling because it depends on the amount of code written. They use the `:` prefix.
```
:ask_for_cmd
	...
	cmp %r0, 'h'
	j.z :help
	...
	jump :ask_for_cmd
	
:help
	...
	jump :ask_for_cmd
```

Simple expressions are suported by surrounding them in `(` `)`:
```
mov %r3, ($input_buffer + #02)
ldb %r0, [($input_buffer + #04)]
```

Lastly, due to some programs requiring it, implace bytes, surounded by `&`, allows to store data at that specific it is written.
```
:hexTable
	&0123456789abcdefABCDEF\xff&
:hexLookupTable
	&\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x0a\x0b\x0c\x0d\x0e\x0f\xff&
```

## Available instructions
| Instruction | Arg1 | Arg2 | Description |
|--|--|--|--|
| break | | | Calls the break handler |
| mov | Register | Register/Value | Copies Arg2 into Arg1 |
| inc | Register |  | Increases Arg1 by 1 |
| dec | Register |  | Decreases Arg1 by 1 |
| jump | Value | | Jumps to Arg1 |
| call | Value | | Jumps to Arg1 saving the PC to the stack |
| ret |  |  | Returns the execution to the last value of the stack |
| push | Register | | Save the value Arg1 into the stack |
| pop | Register | | Obtains the value from the stack into Arg1 |
| add | Register | Register/Value | Adds Arg1 with Arg2 and stores the result into Arg1 |
| and | Register | Register/Value | Logical AND Arg1 with Arg2 and stores the result into Arg1 |
| or | Register | Register/Value | Logical OR Arg1 with Arg2 and stores the result into Arg1 |
| xor | Register | Register/Value | Logical XOR Arg1 with Arg2 and stores the result into Arg1 |
| lls | Register | | Logical Left Shift Arg1 by 1 |
| rls | Register | | Logical Right Shift Arg1 by 1 |
| swap | Register | | Exchanges High and Low byte of Arg1 |
| cmp | Register | Register/Value | Performs a comparison of Arg1 with Arg2 storing the results in the flags|
| j.l | Value | | Jumps to Arg1 if in the last comparison Arg1 was smaller than Arg2 |
| j.g | Value | | Jumps to Arg1 if in the last comparison Arg1 was bigger than Arg2 |
| j.n | Value | | Jumps to Arg1 if in the last comparison Arg1 was not equals to Arg2 |
| j.z | Value | | Jumps to Arg1 if in the last comparison Arg1 was equals to Arg2 |
| call.l | Value | | Jumps to Arg1 saving the PC to the stack  if in the last comparison, Arg1 was smaller than Arg2 |
| call.g | Value | | Jumps to Arg1 saving the PC to the stack  if in the last comparison Arg1 was bigger than Arg2 |
| call.n | Value | | Jumps to Arg1 saving the PC to the stack  if in the last comparison, Arg1 was not equal to Arg2 |
| call.z | Value | | Jumps to Arg1 saving the PC to the stack  if in the last comparison, Arg1 was equal to Arg2 |
| sys | Value | | Perform the syscall determined by Arg1 |
| ld | Register | [Register/Value] | Loads the address pointed by Arg2 into Arg1 |
| ldb | Register | [Register/Value] | Loads 1 byte of the address pointed by Arg2 into Arg1 |
| st | Register | [Register/Value] | Stores Arg1 into the address pointed by Arg2 |
| stb | Register | [Register/Value] | Stores 1 byte of Arg1 into the address pointed by Arg2 |
| zzazz | | | The secret MIX command. |

