UserPanel:
	ldd	r17,Y+yButState	;  244 ��
	ldd	r18,Y+yButPress
	ldd	r19,Y+yButRep
	ldi	XL,ButBuf
	ld	r20,X+	; ButCnt
	ld	r21,X+	; ButCntStart
	ld	r23,X+	; BakButCnt
	ld	r22,X	; BakButState
	in	r16,PINB
	ori	r16,$FF-(1<<btLeft)-(1<<btRight)
	com	r16	; ��������� �����
	st	X,r16   ; ������ ��������
	cp	r16,r22
	breq	but_1
	ldi	r23,5
but_1:	dec	XL
	subi	r23,1
	brcs	but_2
	st	X,r23   ; BakButCnt
	mov	r16,r17
but_2:	mov	r23,r16
	com	r23
	and	r23,r17	; Rel flags
	ldd	r14,Y+yButRel
	or	r14,r23
	std	Y+yButRel,r14
	com	r17
	and	r17,r16	; Press flags
;	andi	r23,0b1010
;	or	r23,r17	 ; ���� ��� ���������� (��� ���������)
	breq	but_4
	ldi	r23,3
	std	Y+ySoundCnt,r23
	ldi	r21,172
	ldi	r20,150
but_4:	or	r18,r17


	ldi	XL,IndBuf	
	ldd	r16,Y+yButRep
	sbrs	r16,btLeft
	rjmp	p_1
	ldd	r16,Y+yButState
	sbrs	r16,btRight
	rjmp	p_5
	ld	r16,X+
	ld	r17,X+
	ld	r18,X+
	st	-X,r16
	st	-X,r18
	ldi	r16,$80
	eor	r17,r16
	st	-X,r17
	rjmp	p_4
p_5:	ld	r16,X
	andi	r16,$7F
	inc	r16
	cpi	r16,33
	brlo	p_3
	ldi	r16,0
p_3:	st	X,r16
	rjmp	p_4
p_1:	sbrs	r16,btRight
	rjmp	p_2
	ld	r16,X+
	ld	r17,X+
	ld	r18,X+
	st	-X,r17
	st	-X,r16
	st	-X,r18
	rjmp	p_4
p_2:
p_4:

	rcall	FormUPBuf



Blink1:	ldd	XL,Y+yBPoint1
	cpi	XL,BlinkBuf1-1
	breq	bi1_4
	ld	r16,X
bi1_3:	subi	r16,$10
	brcs	bi1_1
	sbi	LED1
bi1_2:	st	X,r16
bi1_4:	ret
bi1_1:	cbi	LED1
	andi	r16,$0F
	subi	r16,1
	brcc	bi1_2
	inc	XL
	std	Y+yBPoint1,XL
	ld	r16,X
	cpi	r16,0
	brne	bi1_3
	ldi	XL,BlinkBuf1-1
	std	Y+yBPoint1,XL
	ret



	ldd	XL,Y+yBPoint1
	cpi	XL,BlinkBuf1-1
	brne	iu_9
	ldi	XL,BlinkBuf1
	std	Y+yBPoint1,XL
	ldd	r20,Y+yUab
	ldi	r21,0
	subi_w	r21,r20,-400
	ldi	r16,$15
iu_2:	subi_w	r21,r20,100
	brcs	iu_1
	st	X+,r16
	rjmp	iu_2
iu_1:	subi	r20,-100
	ldi	r16,$09
	st	X+,r16
iu_4:	subi	r20,10
	brcs	iu_3
	ldi	r16,$15
	st	X+,r16
	rjmp	iu_4
iu_3:	subi	r20,-10
	cpi	r16,$09
	brne	iu_5
	ldi	r16,$55
	st	X+,r16
iu_5:	ldi	r16,$09
	st	X+,r16
	cpi	r20,0
	brne	iu_6
	ldi	r16,$55
	st	X+,r16
	rjmp	iu_7
iu_6:	ldi	r16,$15
iu_8:	st	X+,r16
	loop	r20,iu_8
	brcs	iu_7
iu_7:	ldi	r16,$0F
	st	X+,r16
	ldi	r16,$0A
	st	X+,r16
	ldi	r16,$00
	st	X+,r16
iu_9:	rcall	Blink1



	sbrs	BFlags,bfNoGrid
	rjmp	a1_2
	set
;	bld	

a1_2:


	ret	
a1_1:	cpi	r25,ssInv
	brne	a2_1
	ret	
a2_1:	cpi	r25,ssMainReley
	brne	a3_1
	ret	
a3_1:	cpi	r25,ssAddReley
	brne	a4_1
	ret	
a4_1:	cpi	r25,ssBothReleys
	brne	a5_1
	ret	
a5_1:	cpi	r25,ssInvAddReley
	brne	a6_1
	ret	
a6_1:	cpi	r25,ssFail
	brne	a7_1
	ret	
a7_1:



	ldi	ZH,1
	in	r24,ADCL
	in	r25,ADCH
	in	r23,ADMUX
	andi	r23,7
	cpi	r23,chUgrd
	brne	a_1
	ldi	ZL,GRDBuf
	ldd	r16,Y+yUzeroL	; "0"
	ldd	r17,Y+yUzeroH
	sub	r24,r16		; U rel "0"
	sbc	r25,r17
	asr16	r25,r24
	asr16	r25,r24		; reduce to 8 bit
	sbrc	r25,7
	neg	r24		; absolute
	ldd	r18,Z+siMaxAbs
	cp	r18,r24		; check max 
	brsh	a_2
	std	Z+siMaxAbs,r18
a_2:	ldd	r16,Z+siSideCnt		; side counter
	bst	r25,7
	bld	r17,0
	eor	r17,r16
	andi	r17,1
	breq	a_3
	subi	r16,2
	brcc	a_3
	std	Z+siMaxAbs,ZeroReg
	com
	sbrs	r16,0
	rjmp	a_4
	std	Z+siMaxNeg,r18
	rjmp	a_3
a_4:	std	Z+siMaxAbs,r18
	ldd	r17,Z+siLastChS
	ldd	r18,Y+yMainCntL
	std	Z+siLastChS,r18
	sub	r18,r17
	std	Z+siPeriod,r18






a_3:	andi	r16,1
	ori	r16,2*2
	std	Z+siSideCnt,r16


	com	r24
	com	r25
	subi_w	r25:r24,-1

	sbrc	r16,0
	rjmp	a_3
	andi	r16,1
	ori	r16,2*2
	rjmp	a_4
a_3:	subi	r16,2
	brcc	a_4
	com	r16
	andi	r16,1
	ori	r16,2*2
a_4:	

a_1:

	ld	r16,X+	; SideCnt
	ld	r17,X+	; AmplVar

	in	SSREG,SREG
	push	XL
	push	r16
	in	XL,ADMUX
	inc	XL
	andi	XL,0b11
	mov	r16,XL
	ori	r16,0b11000000
	out	ADMUX,r16
	sbi	ADCSRA,ADSC
	lsl	XL
	subi	XL,-DADCBuf
	in	r16,ADCL
	st	X+,r16
	in	r16,ADCH
	st	X,r16
	pop	r16
	pop	XL
	out	SREG,SSREG
	reti







MainAlgo: ; 7.5
	ldd	r25,Y+yUnBrakeCnt
	ldd	r24,Y+yBrakeDelCnt
	ldd	r23,Y+yReleyDelCnt
	ldd	r22,Y+yMainCntM
	sbrc	r22,0
	rjmp	mac_1
	ldd	r16,Y+yMinFrCnt
	subi	r16,1
	brcc	mac_2
	subi	r25,1
	brcc	mac_3
	ldi	r25,0
mac_3:	std	Y+yUnBrakeCnt,r25
	ldi	r16,228
mac_2:	std	Y+yMinFrCnt,r16
mac_1:
	andi	r22,0b111
	brne	mac_4
	subi	r24,1
	brcc	mac_5
	ldi	r24,0
mac_5:	std	Y+yBrakeDelCnt,r24
	subi	r23,1
	brcc	mac_6
	ldi	r23,0
mac_6:	std	Y+yReleyDelCnt,r23
mac_4:
	ldd	r22,Y+yButState
	lds	r20,CurNL
	lds	r21,CurNH
	ldd	r18,Y+yCurVoltL
	ldd	r19,Y+yCurVoltH

; **** ��������� �������� ****
	ldd	r16,Y+yFlugStage ; ������������ ��� ������ �������
	cpi	r16,$FF
	brne	ma_1
	cpi	r24,0  ; �� ������������ ���� �� ���������� ��������
	brne	ma_1

; ����������� �� ��������
	ldd	r14,Y+yBrakeSpeedL
	ldd	r15,Y+yBrakeSpeedH
	ldi_w	r17,r16,351	; ����� ���� ��������� � ����
	cp	r14,r16
	cpc	r15,r17
	breq	ma_2
	cp	r20,r14		; ����� ���� �� ��������� �������
	cpc	r21,r15
	brlo	ma_2
ma_4:	set
	bld	BFlags,bfAutoBrake
	stdi	yFlugStage,7	; �����������
	std	Y+yMinFrCnt,ZeroReg ; ������� �������� ��������
	ldd	r16,Y+yUnBrakeDelay
	std	Y+yUnBrakeCnt,r16
	stdi	yMinFrCnt,228
ma_6:	stdi	yBrakeDelCnt,171
	ret
ma_2:
; ����������� �� ����������
	ldd	r14,Y+yBrakeVoltL
	ldd	r15,Y+yBrakeVoltH
	ldi_w	r17,r16,755	; ����� ���� ��������� � ����
	cp	r14,r16
	cpc	r15,r17
	breq	ma_3
	cp	r18,r14		; ����� ���� �� ��������� �������
	cpc	r19,r15
	brsh	ma_4 ; �����������
ma_3:
; �������������� �� �������
	sbrs	BFlags,bfAutoBrake
	rjmp	ma_5
	ldd	r16,Y+yUnBrakeDelay ; ����� ���� ��������� � ����
	cpi	r16,61
	breq	ma_5
	sbrc	AFlags,afBrakeState ; ����� ���� ��������������
	sbrc	r22,btFluger ; ����� ���� ������ �������
	rjmp	ma_5
	cpi	r25,0 ; ����� ���� �� ������ �����
	brne	ma_5
	std	Y+yFlugStage,ZeroReg ; ��������������
	ret
ma_5:
ma_1:
; **** ��������� ���� ****
	cpi	r23,0  ; �� ������������ ���� �� ���������� ��������
	brne	ma_7

; ��������� ����� �� ��������
	sbrs	AFlags,afReleyState
	rjmp	ma_12
	ldd	r14,Y+yOffSpeedL
	ldd	r15,Y+yOffSpeedH
	ldi_w	r17,r16,351	; ����� ���� ��������� � ����
	cp	r14,r16
	cpc	r15,r17
	breq	ma_8
	cp	r20,r14		; ����� ���� �� ��������� �������
	cpc	r21,r15
	brlo	ma_8
	clt
	bld	AFlags,afReleyState
	set
	bld	BFlags,bfSpeedOff
	stdi	yReleyDelCnt,171
	ret
ma_8:
	sbrs	r22,btPhases	; �� �������� ����� ��� ��������� ��������
	rjmp	ma_12
	clt
	bld	AFlags,afReleyState
	ret
ma_12:

; ���������� ����� �� ��������
	sbrs	BFlags,bfSpeedOff
	rjmp	ma_9
	ldd	r14,Y+yOnSpeedL
	ldd	r15,Y+yOnSpeedH
	ldi_w	r17,r16,49	; ����� ���� ��������� � ����
	cp	r14,r16
	cpc	r15,r17
	breq	ma_9
	cp	r20,r14		; ����� ���� ������� �������
	cpc	r21,r15
	brsh	ma_9
	clt
	bld	BFlags,bfSpeedOff
	sbrc	BFlags,bfVoltageOff
	rjmp	ma_9		; ����� ���� ��������� �� ����������
ma_11:	set
	bld	AFlags,afReleyState
	stdi	yReleyDelCnt,7
	ret
ma_9:

; ���������� ����� �� ����������
	sbrs	BFlags,bfVoltageOff
	rjmp	ma_10
	ldd	r14,Y+yOnVoltL
	ldd	r15,Y+yOnVoltH
	ldi_w	r17,r16,45	; ����� ���� ��������� � ����
	cp	r14,r16
	cpc	r15,r17
	breq	ma_10
	cp	r18,r14		; ����� ���� ���������� �������
	cpc	r19,r15
	brsh	ma_10
	clt
	bld	BFlags,bfVoltageOff
	sbrc	BFlags,bfSpeedOff
	rjmp	ma_10		; ����� ���� ��������� �� ��������
	rjmp	ma_11
ma_10:
ma_7:
	ret

CalcSpeed:
;	ldd	r16,Y+yTestN
;	ldi	r17,0
;	lsl16	r17,r16
;	rjmp	csp_d

	ldi	XL,PHTimeBuf
	ldi_w	r17,r16,0
csp_2:	ld	r18,X+
;	ldi	r18,$34
	cpi	r18,0
	breq	csp_1
	add	r16,r18
	adc	r17,ZeroReg
	cpi	XL,PHTimeBuf+20
	brlo	csp_2
csp_3:	sts	RPeriodL,r16
	sts	RPeriodH,r17
	movw	r19:r18,r17:r16
;	lsr16	r19,r18
;	lsr16	r19,r18
	ldi_w	r17,r16,58593
	rcall	Div
csp_d:	sts	CurNL,r16
	sts	CurNH,r17
	ret	
csp_1:	sts	RPeriodL,ZeroReg
	sts	RPeriodH,ZeroReg
	sts	CurNL,ZeroReg
	sts	CurNH,ZeroReg
	ret

; ����� �������  0.5c
; ������ ������� 0.4�
; ������� ���������� 0.2�
; ������ ������� 0.2�
; ������� ���������� 0.2�
; ������ ������� 20�
;  �������/������� "�����" ����� 2 ��� ����� ���������� ������������

;
;PWM,Time,PWM,Time,
;.db	
AlgoTab:
.db	$20,42, $00,24, $02,12, $00,12, $02,12, $00,122, $00,$FF ; 0
.db	$20,42, $00,24, $02,12, $00,146, $00,$FF	         ; 7

FlugPWR:
	ldd	r16,Y+yFlugStage
	cpi	r16,$FF
	brne	fp_1
	stdi	yFPWMSize,$20
	std	Y+yFlugDelay,ZeroReg
	ret
fp_1:	cpi	r16,60
	brlo	fp_3
	ldd	r18,Y+yFlugDelay
	ldd	r16,Y+yACCnt
	lds	r17,BakACCnt
	sts	BakACCnt,r16
	cp	r17,r16
	breq	fp_5
	ldi	r18,6
fp_5:	ldd	r16,Y+yMainCntL
	andi	r16,0b11100000
	brne	fp_6
	subi	r18,1
	brcc	fp_6
	stdi	yFlugStage,$FF
	ldd	r16,Y+yACCnt
	cpi	r16,1
	brne	fp_7
	set
	bld	AFlags,afBrakeState
	rjmp	fp_8
fp_7:	cpi	r16,2
	brne	fp_9
	clt
	bld	AFlags,afBrakeState
	set
	rjmp	fp_8
fp_9:	clt
fp_8:	bld	AFlags,afBrakeOk
	ret
fp_6:	std	Y+yFlugDelay,r18
	ret
fp_3:	ldd	r17,Y+yFlugDelay
	subi	r17,1
	brcs	fp_4
	std	Y+yFlugDelay,r17
	ret
fp_4:	cpi	r16,0
	brne	fp_10
	clt
	rjmp	fp_12
fp_10:	cpi	r16,7
	brne	fp_11
	set
fp_12:	bld	BFlags,bfBraking
	bld	AFlags,afBrakeState
fp_11:	mov	r17,r16
	lsl	r17
	inc	r16
	std	Y+yFlugStage,r16
	ldi_w	ZH,ZL,2*AlgoTab
	add	ZL,r17
	adc	ZH,ZeroReg
	lpm	r16,Z+
	std	Y+yFPWMSize,r16
	lpm	r16,Z+
	cpi	r16,$FF
	breq	fp_2
	std	Y+yFlugDelay,r16
	ret
fp_2:	stdi	yFlugStage,70
	std	Y+yACCnt,ZeroReg
	stdi	yFlugDelay,137
	sts	BakACCnt,ZeroReg	
	ret
