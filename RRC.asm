;
; RRC test - count number of set bits
;
	org	#800
Data	word	#AAAA
;
	org	#1000
Start
; R0 <- address of R0
	movl	Data,R0
	movh	Data,R0
;
	ldr	R0,$0,R0
;
	movlz	#0,R1	; Count of set bits
;
Loop
	cmp	#0,R0
	cex	ne,#5,#0
;
	clrcc	c	; Clear carry bit - could be set by CMP
	rrc	R0	; R0.msb <- C; 
	addc	#0,R1	; R1 < R1 + C (incr by 0 or 1)
	clrcc	c	; C <- 0
	bra	Loop	; Repeat
;
Done
	bra	Done
;
	end	Start