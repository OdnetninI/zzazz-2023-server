entry f000
	
$print    0008
$strcmp   0010
$findidx  0018
$convhex  0020
$memcpy   0028
$readstr  0030
$strtrim  0038
$memset   0040

$sys_write 0001
$sys_read  0002	
$sys_exit  0003
$sys_file  0004
$sys_rand  0005
	
	movi sp, ff00
	movi r0, fff0
	movi r1, 0098
	stb  r1, [r0]
	inc  r0
	movt r1, :break_mng
	st   r1, [r0]
	movl r2, .welcome_text
	call_v $print
	break
	movl r2, .dry_stack
	call_v $print
	sys_v $sys_exit
	jump f01e

:break_mng
	push sp
	sti  r0, [fb3e]
	sti  r1, [fb40]
	sti  r2, [fb42]
	sti  r3, [fb44]
	pop  r0
	sti  r0, [fb46]
	mov  r0, sp
	ld   r0, [r0]
	addi r0, ffff
	sti  r0, [fb48]
	movi r1, 000c
	movi r2 fb4a
	ldi  r3, [fb46]
	call_v $memcpy
	movl r2, .break
	call_v $print

:ask_for_cmd
	movi r0, 0000
	stbi r0, [e002]
	movl r2, .ready
	call_v $print
	movi r2, e000
	movi r3, 0008
	call_v $readstr
	cmpi r3, 0000
	jz   f0dc
	movi r2, e000
	call_v $strtrim
	ldbi r0, [e002]
	cmpi r0, 0
	jnz f0d3
	ldi r0, [e000]
	cmpi r0, 0a68
	jz_t :help
	cmpi r0, 0068
	jz_t :help
	cmpi r0, 0072
	jz_t :read_cmd
	cmpi r0, 0a72
	jz_t :read_cmd
	cmpi r0, 0070
	jz_t :print_cmd
	cmpi r0, 0a70
	jz_t :print_cmd
	cmpi r0, 0078
	jz_t :exec_cmd
	cmpi r0, 0a78
	jz_t :exec_cmd
	cmpi r0, 0063
	jz_t :continue_cmd
	cmpi r0, 0a63
	jz_t :continue_cmd
	cmpi r0, 4355
	jz_t :secret_cmd
	cmpi r0, 736c
	jz_t :ls_cmd
	cmpi r0, 6672
	jz_t :read_file_cmd
	cmpi r0, 0077
	jz_t :write_cmd
	cmpi r0, 0a77
	jz_t :write_cmd
	movl r2, .bad_cmd
	call_v $print
	jump_t :ask_for_cmd
	movl r2, .input_too_long
	call_v $print
	jump_t :ask_for_cmd
	
:help
	movl r2, .help
	call_v $print
	jump_t :ask_for_cmd

:read_cmd
	movl r2, .which_addr
	call_v $print
	movi r2, e000
	movi r3, 0008
	call_v $readstr
	movi r2, e000
	call_v $convhex
	cmpi r0, ffff
	jz_t :read_cmd_invalid
	push r0
	movl r2, .how_many_lines
	call_v $print
	movi r2, e000
	movi r3, 0008
	call_v $readstr
	movi r2, e000
	call_v $convhex
	cmpi r0, ffff
	jz_t :read_cmd_invalid
	cmpi r0, 0
	jz_t :read_cmd_invalid
	mov r3, r0
	pop r2
:read_cmd_copy_print_loop
	push r2
	push r3
	movi r1, 8
	mov r3, r2
	movi r2, fb56
	call_v $memcpy
	pop r3
	pop r2
	sti r2, [fb5e]
	push r2
	push r3
	movl r2, .hex_format
	call_v $print
	pop r3
	pop r2
	addi r2, 8
	dec r3
	cmpi r3, 0
	jnz_t :read_cmd_copy_print_loop
	jump_t :ask_for_cmd
:read_cmd_invalid
	movl r2, .invalid_input
	call_v $print
	jump_t :ask_for_cmd

:write_cmd
	movl r2, .which_addr
	call_v $print
	movi r2, e000
	movi r3, 8
	call_v $readstr
	movi r2, e000
	call_v $convhex
	cmpi r0, ffff
	jz_t :write_cmd_invalid_input
	sti r0, [fb5e]
	movl r2, .enter_data
	call_v $print
:write_cmd_D
	movi r3, e002
	movi r0, 0
	stb r0, [r3]
	dec r3
	stb r0, [r3]
	dec r3
:write_cmd_B
	sys_v $sys_read
	stb r0, [r3]
	mov r2, r3
	cmpi r0, 002e
	jz_t :write_cmd_A
	cmpi r0, 0a
	jz_t :write_cmd_B
	push r3
	call_v $convhex
	pop r3
	cmpi r0, ffff
	jz_t :write_cmd_B
	inc r3
:write_cmd_C
	sys_v $sys_read
	stb r0, [r3]
	mov r2, r3
	cmpi r0, 002e
	jz_t :write_cmd_A
	cmpi r0, 0a
	jz_t :write_cmd_C
	push r3
	call_v $convhex
	pop r3
	cmpi r0 ffff
	jz_t :write_cmd_C
	dec r3
	mov r2, r3
	call_v $convhex
	ldi r1, [fb5e]
	stb r0, [r1]
	inc r1
	sti r1, [fb5e]
	jump_t :write_cmd_D
:write_cmd_A
	sys_v $sys_read
	cmpi r0, 0a
	jnz_t :write_cmd_A
	movl r2, .loaded
	call_v $print
	jump_t :ask_for_cmd
:write_cmd_invalid_input
	movl r2, .invalid_input
	call_v $print
	jump_t :ask_for_cmd

:print_cmd
	movl r2, .which_addr
	call_v $print
	movi r2, e000
	movi r3, 8
	call_v $readstr
	movi r2, e000
	call_v $convhex
	mov r2, r0
	call_v $print
	jump_t :ask_for_cmd

:exec_cmd
	movl r2, .which_addr
	call_v $print
	movi r2, e000
	movi r3, 8
	call_v $readstr
	movi r2, e000
	call_v $convhex
	push r0
	sti r0, [fb5e]
	movl r2, .exec_at
	call_v $print
	movi r2, e000
	movi r3, 8
	call_v $readstr
	ldbi r0, [e000]
	cmpi r0, 59
	jz_t :exec_cmd_execute
	cmpi r0, 79
	jz_t :exec_cmd_execute
	movl r2, .cancelled
	call_v $print
	pop r0
	jump_t :ask_for_cmd
:exec_cmd_execute
	pop r3
	callr r3
	jump_t :ask_for_cmd
	
:continue_cmd
	movl r2, .continuing
	call_v $print
	ldi r0, [fb3e]
	ldi r1, [fb40]
	ldi r2, [fb42]
	ldi r3, [fb44]
	ret

:ls_cmd
	movl r2, .ls_header
	call_v $print
	movi r0, 0
	movi r1, e000
	sys_v $sys_file
	movi r3, e000
:ls_cmd_loop	
	ldb r0, [r3]
	cmpi r0, 0000
	jz_t :ls_cmd_out
	push r3
	stbi r0, [fb56]
	inc r3
	ld r0, [r3]
	push r3
	sti r0, [fb5e]
	movl r2, .file_idx_size
	call_v $print
	pop r3
	inc r3
	inc r3
	mov r2, r3
	call_v $print
	movi r0, 000a
	sys_v $sys_write
	pop r3
:ls_cmd_out
	addi r3, 10
	cmpi r3, ef00
	jnz_t :ls_cmd_loop
	jump_t :ask_for_cmd

:secret_cmd
	movl r2, .secret
	call_v $print
	jump_t :ask_for_cmd

:read_file_cmd
	movl r2, .filename
	call_v $print
	movi r0, 0
	movi r1, e000
	sys_v $sys_file
	movi r2, efb0
	movi r3, 10
	call_v $readstr
	cmpi r3, 0
	jz_t :input_too_long
	movi r2, efb0
	call_v $strtrim
	movi r3, e003
:read_file_cmd_B
	movi r2, efb0
	push r3
	call_v $strcmp
	pop r3
	cmpi r0, 0
	jz_t :read_file_cmd_A
	addi r3, 10
	cmpi r3, ef03
	jz_t :file_not_found
	jump_t :read_file_cmd_B
:read_file_cmd_A
	dec r3
	dec r3
	dec r3
	ldb r0, [r3]
	inc r3
	ld r1, [r3]
	push r1
	movi r1, e000
	sys_v $sys_file
	movl r2, .which_addr
	call_v $print
	movi r2, efe0
	movi r3, 10
	call_v $readstr
	movi r2, efe0
	call_v $convhex
	pop r1
	cmpi r0, ffff
	jz_t :invalid_input
	mov r2, r0
	movi r3, e000
	call_v $memcpy
	movl r2, .loaded
	call_v $print
	jump_t :ask_for_cmd

:file_not_found
	movl r2, .file_not_found
	call_v $print
	jump_t :ask_for_cmd

:input_too_long
	movl r2, .input_too_long
	call_v $print
	jump_t :ask_for_cmd
	
:invalid_input
	movl r2, .invalid_input
	call_v $print
	jump_t :ask_for_cmd
	
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