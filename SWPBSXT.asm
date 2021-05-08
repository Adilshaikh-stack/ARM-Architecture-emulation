;
; SWPB and SXT
;
	org	#80
Data	byte	'A'
	byte	'B'
;
	org	#1000
Start
	movlz	Data,R0
	ldr	R0,$0,R0
;
	swpb	R0
;
	movls	#0F,R1
	sxt	R1
;
	movlz	#80,R1
	sxt	R1
;
	end	Start

