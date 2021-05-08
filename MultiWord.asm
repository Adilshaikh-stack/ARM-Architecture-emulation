; 
; 32-bit addition
;
	org	#80
LOword	word	#FFFF	; Low-order word
HOword	word	#0	; High-order word
;
	org	#1000
Start
	movlz	LOword,R0
	ldr	R0,$0,R1	; R1 <- LOword
	ldr	R0,$2,R2	; R2 <- HOword
;
	add	#2,R1	; LOword <- LOword + 2
; 
	addc	#0,R2	; HOword <- HOword + 0 + C (Include carry in HOword)
;
	str	R1,R0,$0
	str	R2,R0,$2
;
	end	Start
