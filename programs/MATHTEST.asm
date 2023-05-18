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

; Syscalls
$math_start    #66
$math_gen_next #64
$math_check    #65

; Constants
$load_addr       #2000
$sqrt_function   #3009

$math_test_next  #0001
$math_test_error #0002

:program_start
	push %pc
	pop %r3
	cmp %r3, $load_addr
	j.z :load_addr_check_passed
	mov %r2, %r3
	add %r2, (.load_check - $load_addr)
	jump $print

:load_addr_check_passed
	mov %r2, .explain 
	call $print
	mov %r2, .buffer
	mov %r3, #07
	call $readstr
	ld %r0, [.buffer]
	cmp %r0, 'Y'
	j.z :perform_math_check
	cmp %r0, 'y'
	j.z :perform_math_check
	cmp %r0, 'Y\n'
	j.z :perform_math_check
	cmp %r0, 'y\n'
	j.z :perform_math_check
	mov %r2, .aborted
	call $print
	ret

:perform_math_check
	sys $math_start

:math_check_next
	sys $math_gen_next
	call $sqrt_function
	sys $math_check
	cmp %r0, $math_test_error
	j.z :math_check_failure
	cmp %r0, $math_test_next
	j.z :math_check_next
	ret

:math_check_failure
	mov %r2, .unsuccess
	call $print
	ret

.explain "Glitch Research Laboratory Math Coprocessor
Testing Software: Function SQRT

This program will test the SQRT function of the math module.
This function, executable with CALL 0x3009, should compute the
integer part of sqrt(R0) and return it in R0 (preserving R1-R3).

Math module results will be compared with the coprocessor.
Note - if math module is not loaded, this test might crash the machine!
Report any bugs to administrator: ax.arwen@glitchlabsresearch.internal

Continue with running the test (Y/N): "

.aborted "Aborted.\n"
.unsuccess "Test was unsuccessful.\n"
.load_check "Please load this program at address $2000.\n"
.buffer "________"