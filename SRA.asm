;
; Shift right arithmetic text
; Divide by 2
;
	org	#80
Data	word	$-16
;
	org	#1000
Start
;	movlz	Data,R0	; R0 <- Address of data
;	ldr	R0,$0,R0	; R0 <- mem[R0]
;
	movlz	$16,R0	; R0 <- 16
	xor	$-1,R0	; One's complement the bits
	add	#1,R0	; Two's complement
;
	sra	R0	; R0/2 (-8)
	sra	R0	; R0/2 (-4)
	sra	R0	; R0/2 (-2)
	sra	R0	; R0/2 (-1)
	end	Start