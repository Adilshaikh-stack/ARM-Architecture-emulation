;
; Register initialization tests
; Load/Store relative test
;
	org	#1000
Data1	bss	#2
;
	org	#2000
Start
	movl	Data1,R0
	movh	Data1,R0	; R0 = #1000
; 1000
	movlz	#F0,R1		; R1 = #00F0
	str.b	R1,R0,#0	; Write LSB to 1000
; 1001
	movls	#0F,R2		; R2 = #FF0F
	str.b	R2,R0,#1	; Write LSB to 1001
;
; 1000
	ldr.w	R0,#0,R3	; Read 1000
				; R3 = #0FF0
;
Done
	bra	Done
;
	end	Start

