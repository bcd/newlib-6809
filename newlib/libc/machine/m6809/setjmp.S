;;;
;;; Copyright 2006 by Brian Dominy <brian@oddchange.com>
;;;
;;; setjmp/longjmp for m6809
;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; The jmp_buf is structured as follows:
;
; Offset       Value
; 0            A
; 1            B
; 2-3          X
; 4-5          Y
; 6-7          U
; 8-9          S
; 10-11        PC
; 12           DP
; 13           CC
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;; TODO:
;;; - Reduce size of jmp_buf a little more
;;; - Optimize
;;; - Support nonstandard compiler options
;;; - Support other state (i.e. HW registers for various platforms)

SAVE_A = 0
SAVE_B = 1
SAVE_X = 2
SAVE_Y = 4
SAVE_U = 6
SAVE_S = 8
SAVE_PC = 10
SAVE_DP = 12
SAVE_CC = 13

SETJMP_S = 4

.module sjlj
.area .text

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; int setjmp(jmp_buf env)
;
; A pointer to env is passed in register X.
; The return value is placed in X.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.globl _setjmp 
_setjmp:
	pshs y,u              ; Save registers gcc requires
	std SAVE_A,x          ; Save the A/B registers
	sty SAVE_Y,x          ; Save the Y register
	stu SAVE_U,x          ; Save the U register
	leay SETJMP_S+2,s
	sty SAVE_S,x          ; Save the S register
	ldy SETJMP_S,s
	sty SAVE_PC,x         ; Save the PC register

	tfr dp,a
	tfr cc,b
	std SAVE_DP,x         ; Save the DP/CC registers

	ldx #0                ; Return zero
	puls y,u,pc           ; Restore nonvolatile registers


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; void longjmp(jmp_buf env, int val)
;
; A pointer to env is passed in register X.
; val is placed on the stack.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.globl _longjmp 
_longjmp:
	;-----------------------------------------------
	; TODO: this sequence assumes that the first
	; function argument is passed directly in X.
	; If all args go onto the stack, this is wrong.
	;-----------------------------------------------
                         ; X = env
	ldd 2,s               ; D = return value

	;---------------------------------------------------------
	; TODO : storing the return value and then reloading it
	; at the end of the function is wasteful.  Just reference
	; it from the stack directly at the end, and eliminate
	; the ldd/std here.
	;---------------------------------------------------------
	std SAVE_A,x          ; Store return value into jmp_buf

	ldu SAVE_U,x          ; Restore the U register
	lds SAVE_S,x          ; Restore the S register
	lda SAVE_DP,x
	tfr a,dp              ; Restore the DP register
	lda SAVE_CC,x
	ldy SAVE_PC,x
	pshs y,a              ; Push restored CC/PC onto stack
	ldy SAVE_Y,x          ; Restore the Y register
	ldx SAVE_A,x          ; Restore the X register (return val)
	puls cc,pc            ; Restore CC and PC registers (returning)

