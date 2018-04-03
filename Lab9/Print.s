; Print.s
; Student names: change this to your names or look very silly
; Last modification date: 3/28/2017
; Runs on LM4F120 or TM4C123
; EE319K lab 7 device driver for any LCD
;
; As part of Lab 7, students need to implement these LCD_OutDec and LCD_OutFix
; This driver assumes two low-level LCD functions
; ST7735_OutChar   outputs a single 8-bit ASCII character
; ST7735_OutString outputs a null-terminated string 

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

  

;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11

;division algorithm: n = qd + r

;bindings
divisor EQU -8
	
LCD_OutDec

	PUSH {R0,LR}
	
	;allocation
	MOV R12,SP
	SUB SP,#8
	MOV R3,#10
	STR R3,[R12,#divisor]
	
	;check parameter < 10
	CMP R0,#10
	BLO exit	
	;modulo param
	LDR R1,[R12,#divisor]
	UDIV R2,R0,R1 ;get floor quotient
	MUL R3,R2,R1 ;quotient*divisor
	SUB R3,R0,R3 ;R3 holds R0%10; quotient*divisor - dividend
	
	PUSH {R3,R12} ;push result on stack	
	;update param
	UDIV R0,R1	
	;recursive call	
	BL LCD_OutDec
	POP {R0,R12}
exit
	ADD R0,#0x30
	BL ST7735_OutChar
	ADD SP,#8		;deallocate var from stack
	POP {R0,LR}
    BX  LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000 "
;       R0=3,    then output "0.003 "
;       R0=89,   then output "0.089 "
;       R0=123,  then output "0.123 "
;       R0=9999, then output "9.999 "
;       R0>9999, then output "*.*** "
; Invariables: This function must not permanently modify registers R4 to R11

tempVar EQU -8
	
LCD_OutFix

	PUSH {R0,R11,R12,LR}
	
	;allocation
	MOV R11,SP
	SUB SP,#8
	MOV R3,#10
	STR R3,[R11,#tempVar]
	
	;check less than 9999
	LDR R1,=9999
	CMP R0,R1
	BHI printStar
	B next
printStar
	;LDR R0,=fail
	;BL ST7735_OutString
	MOV	R0, #"*"
	BL ST7735_OutChar
	MOV	R0, #"."
	BL ST7735_OutChar
	MOV	R0, #"*"
	BL ST7735_OutChar
	MOV	R0, #"*"
	BL ST7735_OutChar
	MOV	R0, #"*"
	BL ST7735_OutChar
	B reallyLeave
	
	;check parameter < 10
next	
	MOV R12,#4
	;modulo param
next2
	CMP R12,#0
	BEQ	leave
	LDR R1,[R11,#tempVar]
	UDIV R2,R0,R1 ;get floor quotient
	MUL R3,R2,R1 ;quotient*divisor
	SUB R3,R0,R3 ;R3 holds R0%10; quotient*divisor - dividend
	
	PUSH {R3,R11} ;push result on stack
	SUB R12,#1
	UDIV R0,R1
	B next2
	
leave
	;print out results (remember decimal point)
	POP {R0,R11}
	ADD R0,#0x30
	BL ST7735_OutChar
	MOV R0,#"."
	BL ST7735_OutChar
	POP {R0,R11}
	ADD R0,#0x30
	BL ST7735_OutChar
	POP {R0,R11}
	ADD R0,#0x30
	BL ST7735_OutChar
	POP {R0,R11}
	ADD R0,#0x30
	BL ST7735_OutChar

reallyLeave	
	ADD SP,#8		;deallocate var from stack
	POP {R0,R11,R12,LR}
    BX  LR
	
	 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
