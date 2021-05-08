;
; Subtraction
;
	org	#1000
Start
	movlz	#1,R0
	sub	#0,R0	; R0 <- R0 - 0
;
	movlz	#1,R0
	sub	#2,R0	; R0 <- R0 - 2
;
	movlz	#1,R0
	sub	$-1,R0	; R0 <- R0 - (-1)
;
	movlz	#1,R0
	sub	#1,R0	; R0 <- R0 - 1
;
	end	Start

