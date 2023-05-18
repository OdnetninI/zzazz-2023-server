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


entry #2000 ; Entry point of the program

; BIOS provided functions
$print    #0008
$strcmp   #0010
$findidx  #0018
$convhex  #0020
$memcpy   #0028
$readstr  #0030
$strtrim  #0038
$memset   #0040

; Constants
$load_addr       #2000
$cypher_mul_constant #41a7
$cypher_xor_constant #5555

:program_start
	push %pc
	pop %r3 
	mov %r2, %r3
	add %r2, (:welcome - $load_addr)
	push %r3
	call $print
	pop %r3
	cmp %r3, $load_addr
	j.z :main
	mov %r2, %r3
	add %r2, (:load_check - $load_addr)
	jump $print

:main
	mov %r2, :password
	mov %r1, #0012
	mov %r0, #0000
	call $memset ; HaHa, This does not do anything

	mov %r2, :enter_password
	call $print
	mov %r2, :password
	mov %r3, #000f
	call $readstr
	mov %r2, :password
	call $strtrim
	mov %r2, :password

	mov %r0, #9fc0

:repeat
	ld %r1, [%r2]
	inc %r2
	cmp %r1, #0000
	lls %r0
	xor %r0, %r1
	lls %r0
	add %r0, %r1
	j.z :after
	jump :repeat

:after
	st %r0, [:cypher_current_value]
	mov %r3, :encrypted_document
	mov %r2, :decrypted_document
	mov %r1, #0200
	call $memcpy
	mov %r2, :decrypted_document
	mov %r3, #0100

:cypher_decrypt_next
	call :next_cypher_num
	mov %r1, %r0
	ld %r0, [%r2]
	xor %r0, %r1
	st %r0, [%r2]
	inc %r2
	inc %r2
	dec %r3
	cmp %r3, #0000
	j.n :cypher_decrypt_next
	mov %r2, :here_is
	call $print
	mov %r2, :decrypted_document
	call $print
	mov %r2, (:here_is + #20)
	call $print
	ret

:next_cypher_num
	ld %r0, [:cypher_current_value]
	mul %r0, $cypher_mul_constant
	xor %r0, $cypher_xor_constant
	st %r0, [:cypher_current_value]
	ret
	
:welcome &GLITCH RESEARCH LABORATORY SELF-CONTAINED ENCRYPTION TOOL\n\x00&
:load_check &PLEASE LOAD ME AT ADDRESS $2000... (EXITING)\n\x00&
:enter_password &PLEASE ENTER ENCRYPTION PASSWORD: \x00&

:here_is &HERE IS THE ENCRYPTED DOCUMENT: \n--\n\x00&

:password &________________\x00\x00&

:cypher_current_value &\x5f\x5f&

:buffer &\x33\x33\x33\x33\x33\x33\x33\x33&

:encrypted_document &\xfb\x72\x3a\x22\x21\x4b\x6a\xc2\x28\xe6\xe4\xc1\xa7\x25\xb6\xe0\x98\xb2\x0a\xe0\x9b\xab\xc1\x84\xb8\x2d\x8f\xd8\x7e\x07\x3a\xcb\xc9\xf0\xf5\x38\x51\x43\xed\x52\xf8\x44\xe4\xc2\x50\x22\x27\x2a\x65\x44\x81\x79\xc2\x6c\xb4\xaf\xd9\x9d\x7c\x74\xb3\x58\xea\xcd\x69\xe2\xa2\xdf\xe7\xca\xc4\x65\xe0\xe8\xf3\xbe\xee\x97\x68\xad\x61\x0e\x27\x78\x32\x86\x6d\x5c\x02\xc2\x6e\xd1\xdf\x93\x14\x4d\xcb\x1a\x90\xf4\x20\x95\xc0\xd3\x1d\xe8\x73\x2b\xdb\xaf\xae\x74\x7a\x1d\x50\x4a\xa6\x9d\x56\xcc\xb5\x5d\x07\xd1\x04\xa7\x16\x5c\x1d\x40\x4e\x65\x51\xf2\x68\x0a\x4a\xb3\x56\x56\xc1\x51\x1f\x0f\xc2\x7d\x4b\x29\x5b\x58\x1d\x2f\xff\x66\xc0\x3c\xbb\xbb\x34\x93\x30\xe6\x34\xb7\x8d\xd4\x4a\xa3\xd7\x88\x00\x72\xba\x13\x0c\x4d\x97\x46\xe0\xbb\x07\xa6\xef\xfa\x12\x0a\xfe\x56\xe9\x14\x2b\xda\x64\x3c\xfe\xc5\x3c\xa3\x50\x02\x20\xb7\x20\x05\x3d\xdb\xf1\x8a\x81\x41\x4c\x24\x52\x38\x8f\xb9\x7e\x2e\x6f\x76\x1c\x3f\x97\x87\x16\xd5\xd3\xc9\x70\x92\xd7\xd2\x47\x59\xe6\x87\x12\x2c\xe0\xa5\x6c\x17\x9c\xbe\xee\xe4\xd4\x50\xea\x94\x57\x91\x4f\xb1\x57\xaa\x46\xe9\x86\x76\xd3\xf8\xb9\xa5\x8c\x9c\x57\x56\x0a\x72\x48\xc8\x14\xb6\x8b\xf2\x48\x3c\x4d\xb5\x3a\x48\x07\xa6\xf2\x8e\xa3\x0b\x39\x02\x75\x9b\xc8\xdc\x6f\x0a\x75\x00\x52\x6d\xbf\x77\x89\x2f\xce\xb0\x27\xef\x42\x1e\x36\xe1\x37\xc6\xf7\x77\x34\x1d\x6a\xd3\xa7\x48\x6a\xc1\x7d\xe7\x76\x8c\x62\x8f\x62\x8c\x5c\xe4\xf1\xf3\xfd\x5f\xb6\x9d\xb4\xd7\xa8\xa0\xcd\xde\x8b\x72\x73\xe4\xfa\xe7\x33\x8f\x11\x1e\xaf\xd3\x40\xa7\x86\x26\xf3\x77\x4c\x6e\x2d\x2d\xa4\x03\xc2\x74\x2e\xb3\xdd\x85\x29\x41\x90\xc3\xc7\x3a\x96\xb9\x8e\x35\xcf\x37\xc0\xc8\xff\xd5\x8b\x9f\xa1\xd9\x0f\x8e\x8e\xc6\x11\xc1\xc0\xc9\x9e\x5b\x9c\xa1\x2d\xc6\x4b\xf5\x34\x65\xb4\xcc\xac\x88\xbf\x49\x43\x77\xac\x8f\x61\x71\x8e\x59\x7f\x78\xd9\x73\x41\x90\x73\x8b\x8e\x93\x3f\x6e\xdd\x2e\x37\xe8\x72\x43\xa1\xf3\xa9\xfe\x7f\x98\xf3\xa5\x98\x43\xe5\x60\xb9\x5d\xbd\xba\x3f\xae\x71\xf6\xed\xc1\xfe\x8f\x2d\x15\xd9\xc6\xf7\x57\x83\x57\x7e\x10\xb6\xc3\x76\x29\xb1\x10\x12\x1e\xea\xa1\x15\x81\x9c\x22\x5f\x8a\xf3\xff\x1c\x37\x4e\x57\x1b\x26\x64\xb7\xca\x7c\xc8\x57\xd9\x57&

:decrypted_document &\x00&