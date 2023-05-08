entry #0000 ; Entry point of the program

; Syscalls
$sys_write #0001
$sys_read  #0002	
$sys_exit  #0003
$sys_file  #0004
$sys_rand  #0005

; Constants
$bios_limit_addr #1000

	; Just some spacing
	illegal
	illegal
	illegal
	illegal
	illegal
	illegal
	illegal
	illegal

:print
	jump :print_implemented
	break
	break
	break
	break
	break

:strcmp
	jump :strcmp_implemented
	break
	break
	break
	break
	break

:findidx
	jump :findidx_implemented
	break
	break
	break
	break
	break
	
:convhex
	jump :convhex_implemented
	break
	break
	break
	break
	break

:memcpy
	jump :memcpy_implemented
	break
	break
	break
	break
	break
	
:readstr
	jump :readstr_implemented
	break
	break
	break
	break
	break

:strtrim
	jump :strtrim_implemented
	break
	break
	break
	break
	break

:memset
:return
	ret

:print_implemented

:print_write_next_char
	cmp %r2, $bios_limit_addr
	j.l :return
	ldb %r0, [%r2]
	cmp %r0, #0
	j.z :print_return

	cmp %r0, #00f0
	j.z :print_0xf0
	cmp %r0, #00f1
	j.z :print_0xf1
	sys $sys_write
	inc %r2
	jump :print_write_next_char

:print_return
	ret

:print_0xf0
	inc %r2
	ld %r0, [%r2]
	cmp %r0, $bios_limit_addr
	j.l :return

	ld %r0, [%r0]
	call :print_r0_as_two_hex_bytes 
	inc %r2
	inc %r2
	jump :print_write_next_char

:print_0xf1
	inc %r2
	ld %r0, [%r2]
	cmp %r0, $bios_limit_addr
	j.l :return

	ldb %r0, [%r0]
	call :print_r0_as_1_byte_hex_bytes
	inc %r2
	inc %r2
	jump :print_write_next_char


:strcmp_implemented

:strcmp_implemented_next_char
	cmp %r2, $bios_limit_addr
	j.l :return
	cmp %r3, $bios_limit_addr
	j.l :return

	ldb %r0, [%r2]
	ldb %r1, [%r3]

	cmp %r0, %r1
	j.n :strcmp_return
	cmp %r0, #0
	j.z :strcmp_return

	inc %r2
	inc %r3
	jump :strcmp_implemented_next_char
:strcmp_return
	ret

:print_r0_as_two_hex_bytes
	swap %r0
	call :print_r0_low_byte
	swap %r0
:print_r0_as_1_byte_hex_bytes
	call :print_r0_low_byte
	ret

:print_r0_low_byte
	push %r0
	rls %r0
	rls %r0
	rls %r0
	rls %r0
	and %r0, #000f
	call :print_half_byte_r0_as_hex
	pop %r0
	push %r0
	and %r0, #000f
	call :print_half_byte_r0_as_hex
	pop %r0
	ret

:print_half_byte_r0_as_hex
	add %r0, :hexTable
	ldb %r0, [%r0]
	sys $sys_write
	ret

:hexTable
	&0123456789ABCDEF&

:findidx_implemented

	mov %r1, #ffff
:findidx_next
	inc %r1
	cmp %r2, $bios_limit_addr
	j.l :return
	ldb %r0, [%r2]
	inc %r2
	cmp %r0, #00ff
	j.z :findidx_fail
	cmp %r0, %r3
	j.n :findidx_next
	mov %r0, %r1
	ret
:findidx_fail
	mov %r0, #ffff
	ret


:convhex_implemented_func

	mov %r1, #ffff
:convhex_implemented_func_next
	inc %r1
	ldb %r0, [%r2]
	inc %r2
	cmp %r0, #00ff
	j.z :convhex_implemented_func_A
	cmp %r0, %r3
	j.n :convhex_implemented_func_next
	mov %r0, %r1
	ret
:convhex_implemented_func_A
	mov %r0, #ffff
	ret


	break
	break
	break

:bios_signature 
	&Glitch Research Laboratory GLVM BIOS version 1.3 / Do not distribute! / FOOLS2023_{DumpingWasAnInsideJob}&

	ret
	break
	break

:convhex_implemented

	mov %r3, #0000
:convhex_implemented_next
	cmp %r2, $bios_limit_addr
	j.l :return
	ldb %r0, [%r2]
	inc %r2
	cmp %r0, #0000
	j.z :convhex_end 
	cmp %r0, '\n'
	j.z :convhex_end
	add %r3, %r3
	add %r3, %r3
	add %r3, %r3
	add %r3, %r3 
	push %r2
	push %r3
	mov %r2, :hexTable2
	mov %r3, %r0
	call :convhex_implemented_func
	pop %r3
	pop %r2
	cmp %r0, #ffff
	j.z :convhex_return
	mov %r1, :hexLookupTable 
	add %r1, %r0
	ldb %r1, [%r1]
	add %r3, %r1
	jump :convhex_implemented_next

:convhex_end
	mov %r0, %r3
:convhex_return
	ret


:hexTable2
	&0123456789abcdefABCDEF\xff&
:hexLookupTable
	&\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x0a\x0b\x0c\x0d\x0e\x0f\xff&

:memcpy_implemented

:memcpy_copy_next_byte
	cmp %r3, $bios_limit_addr
	j.l :return
	
	cmp %r2, $bios_limit_addr
	j.l :return

	ldb %r0, [%r3]
	stb %r0, [%r2]
	inc %r3
	inc %r2
	dec %r1
	cmp %r1, #0
	j.n :memcpy_copy_next_byte
	ret

:readstr_implemented

:readstr_read_next_character
	sys $sys_read
	cmp %r2, $bios_limit_addr
	j.l :return 

	stb %r0, [%r2]
	cmp %r0, '\n'
	j.z :readstr_write_end_of_string_and_return

	cmp %r3, #0
	j.z :readstr_maximum_characters_reached
	inc %r2
	dec %r3

:readstr_maximum_characters_reached
	jump :readstr_read_next_character

:readstr_write_end_of_string_and_return
	inc %r2
	mov %r0, #0
	stb %r0, [%r2]
	ret

:strtrim_implemented

:strtrim_find_0_or_newline
	ldb %r0, [%r2]
	cmp %r0, '\n'
	j.z :strtrim_write_0_and_return

	cmp %r0, #0
	j.z :strtrim_write_0_and_return

	inc %r2
	jump :strtrim_find_0_or_newline

:strtrim_write_0_and_return
	mov %r0, #0
	cmp %r2, $bios_limit_addr
	j.l :return
	stb %r0, [%r2]
	ret
