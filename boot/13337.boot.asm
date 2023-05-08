; ****************************************************************************************
; *    Copyright © 2023 OdnetninI                                                        *
; *                                                                                      *
; *    Permission is hereby granted, free of charge, to any person obtaining a copy of   *
; *    this software and associated documentation files (the “Software”), to deal in     *
; *    the Software without restriction, including without limitation the rights to      *
; *    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies     *
; *    of the Software, and to permit persons to whom the Software is furnished to do    *
; *    so, subject to the following conditions:                                          *
; *                                                                                      *
; *    The above copyright notice and this permission notice shall be included in all    *
; *    copies or substantial portions of the Software.                                   *
; *                                                                                      *
; *    THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR        *
; *    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,          *
; *    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE       *
; *    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, *
; *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN   *
; *    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.        *
; ****************************************************************************************

entry #f000  ; Default entry point of the boot programs

; BIOS provided functions
$print    #0008
$strcmp   #0010
$findidx  #0018
$convhex  #0020
$memcpy   #0028
$readstr  #0030
$strtrim  #0038
$memset   #0040

; Syscalls
$sys_write #0001
$sys_read  #0002	
$sys_exit  #0003
$sys_file  #0004
$sys_rand  #0005

; Constants
$break_pointer #fff0 ; When executing the break instruction, it will jump here
$init_stack    #ff00 ; Init position of stack pointer
$break_buffer  #fb3e ; Break manager needs data on this position
$input_buffer  #e000 ; Buffer for user input

; Entry Point
	mov %sp, $init_stack
	mov %r0, $break_pointer
	mov %r1, #0098
	stb  %r1, [%r0]
	inc  %r0
	mov %r1, :break_mng
	st   %r1, [%r0]
	mov %r2, .welcome_text
	call $print
	break
	mov %r2, .dry_stack
	call $print
	sys $sys_exit
:infinite_loop
	jump :infinite_loop

:break_mng
	push %sp
	st  %r0, [($break_buffer + #00)]
	st  %r1, [($break_buffer + #02)]
	st  %r2, [($break_buffer + #04)]
	st  %r3, [($break_buffer + #06)]
	pop %r0
	st  %r0, [($break_buffer + #08)]
	mov %r0, %sp
	ld  %r0, [%r0]
	add %r0, #ffff
	st  %r0, [($break_buffer + #0a)]
	mov %r1, #000c
	mov %r2 #fb4a
	ld  %r3, [($break_buffer + #08)]
	call $memcpy
	mov %r2, .break
	call $print

:ask_for_cmd
	mov %r0, #0000
	stb %r0, [($input_buffer + #02)]
	mov %r2, .ready
	call $print
	mov %r2, $input_buffer
	mov %r3, #0008
	call $readstr
	cmp %r3, #0000
	j.z :ask_for_cmd_input_too_long
	mov %r2, $input_buffer
	call $strtrim
	ldb %r0, [($input_buffer + #02)]
	cmp %r0, #0
	j.n :ask_for_cmd_bad_cmd
	ld %r0, [($input_buffer + #00)]
	cmp %r0, 'h\n'
	j.z :help
	cmp %r0, 'h'
	j.z :help
	cmp %r0, 'r'
	j.z :read_cmd
	cmp %r0, 'r\n'
	j.z :read_cmd
	cmp %r0, 'p'
	j.z :print_cmd
	cmp %r0, 'p\n'
	j.z :print_cmd
	cmp %r0, 'x'
	j.z :exec_cmd
	cmp %r0, 'x\n'
	j.z :exec_cmd
	cmp %r0, 'c'
	j.z :continue_cmd
	cmp %r0, 'c\n'
	j.z :continue_cmd
	cmp %r0, 'UC'
	j.z :secret_cmd
	cmp %r0, 'ls'
	j.z :ls_cmd
	cmp %r0, 'rf'
	j.z :read_file_cmd
	cmp %r0, 'w'
	j.z :write_cmd
	cmp %r0, 'w\n'
	j.z :write_cmd
:ask_for_cmd_bad_cmd
	mov %r2, .bad_cmd
	call $print
	jump :ask_for_cmd
:ask_for_cmd_input_too_long
	mov %r2, .input_too_long
	call $print
	jump :ask_for_cmd
	
:help
	mov %r2, .help
	call $print
	jump :ask_for_cmd

:read_cmd
	mov %r2, .which_addr
	call $print
	mov %r2, $input_buffer
	mov %r3, #0008
	call $readstr
	mov %r2, $input_buffer
	call $convhex
	cmp %r0, #ffff
	j.z :read_cmd_invalid
	push %r0
	mov %r2, .how_many_lines
	call $print
	mov %r2, $input_buffer
	mov %r3, #0008
	call $readstr
	mov %r2, $input_buffer
	call $convhex
	cmp %r0, #ffff
	j.z :read_cmd_invalid
	cmp %r0, #0
	j.z :read_cmd_invalid
	mov %r3, %r0
	pop %r2
:read_cmd_copy_print_loop
	push %r2
	push %r3
	mov %r1, #8
	mov %r3, %r2
	mov %r2, #fb56
	call $memcpy
	pop %r3
	pop %r2
	st %r2, [#fb5e]
	push %r2
	push %r3
	mov %r2, .hex_format
	call $print
	pop %r3
	pop %r2
	add %r2, #8
	dec %r3
	cmp %r3, #0
	j.n :read_cmd_copy_print_loop
	jump :ask_for_cmd
:read_cmd_invalid
	mov %r2, .invalid_input
	call $print
	jump :ask_for_cmd

:write_cmd
	mov %r2, .which_addr
	call $print
	mov %r2, $input_buffer
	mov %r3, #8
	call $readstr
	mov %r2, $input_buffer
	call $convhex
	cmp %r0, #ffff
	j.z :write_cmd_invalid_input
	st %r0, [#fb5e]
	mov %r2, .enter_data
	call $print
:write_cmd_D
	mov %r3, ($input_buffer + #02)
	mov %r0,#0
	stb %r0, [%r3]
	dec %r3
	stb %r0, [%r3]
	dec %r3
:write_cmd_B
	sys $sys_read
	stb %r0, [%r3]
	mov %r2, %r3
	cmp %r0, '.'
	j.z :write_cmd_A
	cmp %r0, '\n'
	j.z :write_cmd_B
	push %r3
	call $convhex
	pop %r3
	cmp %r0, #ffff
	j.z :write_cmd_B
	inc %r3
:write_cmd_C
	sys $sys_read
	stb %r0, [%r3]
	mov %r2, %r3
	cmp %r0, '.'
	j.z :write_cmd_A
	cmp %r0, '\n'
	j.z :write_cmd_C
	push %r3
	call $convhex
	pop %r3
	cmp %r0 #ffff
	j.z :write_cmd_C
	dec %r3
	mov %r2, %r3
	call $convhex
	ld %r1, [#fb5e]
	stb %r0, [%r1]
	inc %r1
	st %r1, [#fb5e]
	jump :write_cmd_D
:write_cmd_A
	sys $sys_read
	cmp %r0, '\n'
	j.n :write_cmd_A
	mov %r2, .loaded
	call $print
	jump :ask_for_cmd
:write_cmd_invalid_input
	mov %r2, .invalid_input
	call $print
	jump :ask_for_cmd

:print_cmd
	mov %r2, .which_addr
	call $print
	mov %r2, $input_buffer
	mov %r3, #8
	call $readstr
	mov %r2, $input_buffer
	call $convhex
	mov %r2, %r0
	call $print
	jump :ask_for_cmd

:exec_cmd
	mov %r2, .which_addr
	call $print
	mov %r2, $input_buffer
	mov %r3, #8
	call $readstr
	mov %r2, $input_buffer
	call $convhex
	push %r0
	st %r0, [#fb5e]
	mov %r2, .exec_at
	call $print
	mov %r2, $input_buffer
	mov %r3, #8
	call $readstr
	ldbi %r0, [$input_buffer]
	cmp %r0, 'Y'
	j.z :exec_cmd_execute
	cmp %r0, 'y'
	j.z :exec_cmd_execute
	mov %r2, .cancelled
	call $print
	pop %r0
	jump :ask_for_cmd
:exec_cmd_execute
	pop %r3
	call %r3
	jump :ask_for_cmd
	
:continue_cmd
	mov %r2, .continuing
	call $print
	ld %r0, [($break_buffer + #00)]
	ld %r1, [($break_buffer + #02)]
	ld %r2, [($break_buffer + #04)]
	ld %r3, [($break_buffer + #06)]
	ret

:ls_cmd
	mov %r2, .ls_header
	call $print
	mov %r0, #0
	mov %r1, $input_buffer
	sys $sys_file
	mov %r3, $input_buffer
:ls_cmd_loop	
	ldb %r0, [%r3]
	cmp %r0, #0000
	j.z :ls_cmd_out
	push %r3
	stb %r0, [#fb56]
	inc %r3
	ld %r0, [%r3]
	push %r3
	st %r0, [#fb5e]
	mov %r2, .file_idx_size
	call $print
	pop %r3
	inc %r3
	inc %r3
	mov %r2, %r3
	call $print
	mov %r0, '\n'
	sys $sys_write
	pop %r3
:ls_cmd_out
	addi %r3, #10
	cmp %r3, #ef00
	j.n :ls_cmd_loop
	jump :ask_for_cmd

:secret_cmd
	mov %r2, .secret
	call $print
	jump :ask_for_cmd

:read_file_cmd
	mov %r2, .filename
	call $print
	mov %r0, #0
	mov %r1, $input_buffer
	sys $sys_file
	mov %r2, #efb0
	mov %r3, #10
	call $readstr
	cmp %r3, #0
	j.z :inputoo_long
	mov %r2, #efb0
	call $strtrim
	mov %r3, ($input_buffer + #03)
:read_file_cmd_B
	mov %r2, #efb0
	push %r3
	call $strcmp
	pop %r3
	cmp %r0, #0
	j.z :read_file_cmd_A
	addi %r3, #10
	cmp %r3, #ef03
	j.z :file_not_found
	jump :read_file_cmd_B
:read_file_cmd_A
	dec %r3
	dec %r3
	dec %r3
	ldb %r0, [%r3]
	inc %r3
	ld %r1, [%r3]
	push %r1
	mov %r1, $input_buffer
	sys $sys_file
	mov %r2, .which_addr
	call $print
	mov %r2, #efe0
	mov %r3, #10
	call $readstr
	mov %r2, #efe0
	call $convhex
	pop %r1
	cmp %r0, #ffff
	j.z :invalid_input
	mov %r2, %r0
	mov %r3, $input_buffer
	call $memcpy
	mov %r2, .loaded
	call $print
	jump :ask_for_cmd

:file_not_found
	mov %r2, .file_not_found
	call $print
	jump :ask_for_cmd

:inputoo_long
	mov %r2, .input_too_long
	call $print
	jump :ask_for_cmd
	
:invalid_input
	mov %r2, .invalid_input
	call $print
	jump :ask_for_cmd
	
.welcome_text "======================================================================
Welcome to Glitch Research Laboratory Network: Test Server 1 (GRLTS01)
======================================================================
GRLTS01 is intended for research purposes only, to aid our engineers
in migrating their software to the latest GLVM architecture.
This server will boot into a machine language monitor, providing
a simple environment for development and testing.

Note that this server is publicly accessible, and thus, should never
be used to store any confidential information. If storage of data is
necessary, consider utilizing encryption, or using our dedicated
storage servers GRLFS01.
Additionally, consider using GRLTS02 for any serious work - it's
an authenticated test server, which should be far more secure.
======================================================================
Running machine language monitor now.
======================================================================\n\n"

.input_too_long "! Input too long error.\n"
.bad_cmd "! Bad command error (h for help).\n"
.invalid_input "! Invalid input error.\n"
.which_addr "> Which address? "
.how_many_lines "> How many lines? "
.exec_at "> Really exec at \xF0\x5e\xFB? Type Y if so: "
.cancelled "! Cancelled action error.\n"
.continuing "Continuing.\n"
.loaded "Loaded.\n"
.dry_stack "! Dry stack. Halting machine.\n"
.filename "> Filename? "
.file_not_found "! File not found error.\n"
.enter_data "> Enter hex data. End with dot \".\" + newline:\n"
.secret "Wow, undocumented monitor command! FOOLS2023_{Secret\xf0\x00\xe0x\xf0\x01\xe0Command}\n"

.help "Available commands:
r   :: print memory as hex
p   :: print memory as text
w   :: write hex data to memory
x   :: execute memory
rf  :: load memory from file
ls  :: print file index
h   :: print this help message
c   :: exit monitor and continue
Please enter hex numbers when prompted.
If necessary for debugging, you can break into monitor
with instruction BRK (0x00) and continue with 'c'
Note: memory region E000-FFFF is used by monitor\n"	

.ls_header "BLK  SIZE  NAME\n=======================\n"
.file_idx_size "\xf1\x56\xfb   \xf0\x5e\xfb  "
.ready "Ready.\n> "
.break "*** BREAK INTO MONITOR AT $\xf0\x48\xfb ***
R0=$\xf0\x3e\xfb R1=$\xf0\x40\xfb R2=$\xf0\x42\xfb R3=$\xf0\x44\xfb
SP=$\xf0\x46\xfb [\xf1\x4a\xfb\xf1\x4b\xfb\xf1\x4c\xfb\xf1\x4d\xfb\xf1\x4e\xfb\xf1\x4f\xfb\xf1\x50\xfb\xf1\x51\xfb\xf1\x52\xfb\xf1\x53\xfb\xf1\x54\xfb\xf1\x55\xfb]\n"
.hex_format "\xf0\x5e\xfb | \xf1\x56\xfb \xf1\x57\xfb \xf1\x58\xfb \xf1\x59\xfb \xf1\x5a\xfb \xf1\x5b\xfb \xf1\x5c\xfb \xf1\x5d\xfb\n"