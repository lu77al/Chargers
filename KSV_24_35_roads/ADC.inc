; ��������� �������� ����� ������� ������� StartADC
;  1 - ������������ ADCDummy ������ ��������� ������
;  2 - ����������� ADCSamplesQnt �������� ���������
;  3 - ����� ����������� � ������ (��������� SaveADC)
;  3 - ������������������� �����
;  4 - ���� �� ��� ������ ���������� ������ � 1
;  5 - ��������������� ���� afADCComplete � ��������� ���������������
; � ��������� SaveADC ����� �������������� ��������������
; �������������� ���������� (r23 �������� ����� ������, r17:r16 - �����

.equ	Ref256  = 0b11000000
.equ	RefAVCC = 0b01000000
.equ	RefExt  = 0b00000000

.equ	ADCSamplesQnt	= 8
.equ	ADCDummy	= 27
.equ	ADCChQnt	= 3

; Tf=11.76��(85��), ��������� � ������������ 61�� (T=16��), ��� ������� 103

; CH#0 - ADC0 (PC0) -  Ivh (2.5V-0A; 0V-62.5A)
.equ	adcIch	= 0
.equ	ADMUX0	= Ref256 + 0
; CH#1 - ADC1 (PC1) -  Uab (2.56V <- 80V)
.equ	adcUab 	= 1
.equ	ADMUX1	= Ref256 + 1
; CH#1 - ADC4 (PC4) -  Uin (2.56V <- 150V)
.equ	adcUin	= 2
.equ	ADMUX2	= Ref256 + 4

.macro	GetADC
	lds	r16,$100+ADCBuf+@0*2
	lds	r17,$100+ADCBuf+@0*2+1
.endm

.macro	GetADC_
	lds	@1,$100+ADCBuf+@2*2
	lds	@0,$100+ADCBuf+@2*2+1
.endm

ADCChannels:
.db		ADMUX0, ADMUX1, ADMUX2, ADMUX2

InitADC:
	outi	ADCSR,0b10000110	; Enable ADC, CK/64
	outi	SFIOR,0
	stdi	yADCCurCh,ADCChQnt-1
	stdi	yADCSamCnt,ADCSamplesQnt+ADCDummy-1
	ldi_w	ZH,ZL,2*ADCChannels+(ADCChQnt-1)
	lpm	r16,Z
	out	ADMUX,r16
	sbi	ADCSRA,ADSC
	clt
	bld	AFlags,afADCCompl
	ldi	XL,ADCKoef
	ldi_w	r17,r16,$8000
iadc_1:	st	X+,r16
	st	X+,r17
	cpi	XL,ADCKoef+3*2
	brlo	iadc_1
	ret

.macro	tskADC
	sbrc	AFlags,afADCCompl
	rjmp	RetADC
	sbis	ADCSR,6
	rjmp	CheckADCProc
.endm

; Start ADC ->	ClrFlag	AFlags,afADCCompl

CheckADCProc:
	in	r24,ADCL	 ; ��������� ��������� ���������
	in	r25,ADCH
	ldd	r23,Y+yADCCurCh	 ; ������ ��������������� ������
	ldd	r22,Y+yADCSamCnt ; ������� ��������� ������
	subi	r22,1
	brcc	adc_1	; ���� ��� ��������� ������ ��������� ������� �����
	ldi	r22,ADCSamplesQnt+ADCDummy-1
	ldd	r16,Y+yADCCurCh	 ; ������ ��������������� ������
	subi	r16,1
	brcc	adc_2	; ���� ���������� ��� ������
	ldi	r16,ADCChQnt-1 ;  ������ � ����������
	set
	bld	AFlags,afADCCompl ; ���������� ���� ���������� ���������
	bld	AFlags,afStartReg ; 	     � ���� ������ ����������
adc_2:	std	Y+yADCCurCh,r16
	ldi_w	ZH,ZL,2*ADCChannels
	add	ZL,r16
	adc	ZH,ZeroReg
	lpm	r16,Z
	out	ADMUX,r16
adc_1:	sbi	ADCSRA,ADSC	; ����� ���������
	std	Y+yADCSamCnt,r22
	ldd	r16,Y+yADCSumL	; ��������� ���������
	ldd	r17,Y+yADCSumH
	add	r16,r24
	adc	r17,r25
	cpi	r22,ADCSamplesQnt+ADCDummy-1 ; ���� ����� ������������
	brlo	adc_3
	std	Y+yADCSumL,ZeroReg ; �������� �����
	std	Y+yADCSumH,ZeroReg
	rjmp	SaveADC		; ��������� �������� � ����� �����������
adc_3:	cpi	r22,ADCSamplesQnt-1 ; ���� �������������� �� ������
	brsh	adc_4
	std	Y+yADCSumL,r16 ; ��������� ������������� �����
	std	Y+yADCSumH,r17
adc_4:	rjmp	RetADC

SaveADC:
	ldi	XL,ADCKoef
	add	XL,r23
	add	XL,r23
	ld	r18,X+
	ld	r19,X+
	subi	XL,ADCKoef-ADCBuf+2
	lsl16	r17,r16
	lsl16	r17,r16
	lsl16	r17,r16
	rcall	K16x16
	lsl16	r17,r16
	brcc	sadc_1
	ldi_w	r17,r16,$FFFF
sadc_1:	st	X+,r16
	st	X+,r17
	rjmp	RetADC


