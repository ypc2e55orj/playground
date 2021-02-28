Data		segment
HexTable	db	'0123456789ABCDEF'
Row		db	' '
SegmentAddress	db	'0000'
		db	':'
OffsetAddress	db	'0000'
		db	' | '
BinaryAsHex	db	'00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F'
		db	' '
BinaryAsASCII	db	'................'
		db	1Bh, '[0m', 0Dh, 0Ah
RowEnd		db	' '
RowLength	dw	offset RowEnd - offset Row
RowDescripton	db	1Bh, '[2J'
		db	'Sgmnt:Offst|', 0Dh, 0Ah
		db	'  Address  | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F      ASCII      '
		db	0Dh, 0Ah
		db	'-----------+------------------------------------------------ ----------------'
		db	1Bh, '[0m', 0Dh, 0Ah, '$'
EscSqHighlight	db	1Bh, '[1m', '$'
PressAnyKeyMsg	db	1Bh, '[33m'
		db	'Press any key(press Esc to exit)...'
		db	1Bh, '[0m', 0Dh, 0Ah, '$'
InvalidParamMsg	db	1Bh, '[1m', 1Bh, '[31m'
		db	'The parameter is invalid. Exit...'
		db	1Bh, '[0m', '$'
CancelledMsg	db	1Bh, '[1m', 1Bh, '[31m'
		db	0Dh, 0Ah, 'Cancelled(Esc was pressed) by user. Exit...'
		db	1Bh, '[0m', '$'
CompleteMsg	db	1Bh, '[1m', 1Bh, '[32m'
		db	0Dh, 0Ah, 'Complete outputting 0000h ~ 1000h. Exit...'
		db	1Bh, '[0m', '$'
Data		ends

Stack		segment	stack
			dw	100h	dup(?)
Stack		ends

Code		segment
		assume	cs:Code, ds:Data, ss:Stack

;--------------------------------------------------------------------
;	StdoutString(ah=09h)
;	@param	dx	address
;--------------------------------------------------------------------
StdoutString		proc	near
			push	ax

			mov	ah, 09h
			int	21h

			pop	ax
			ret
StdoutString		endp

;--------------------------------------------------------------------
;	Set hexadecimal as ASCII to specified address
;	@param	di	address
;	@param	bx	hexadecimal
;--------------------------------------------------------------------
SetASCIIHexAddress		proc	near
			push	ax

			mov	al, bh
			call	BinaryToASCIIHex
			mov	word ptr [di + 0], ax

			mov	al, bl
			call	BinaryToASCIIHex
			mov	word ptr [di + 2], ax

			pop	ax
			ret
SetASCIIHexAddress		endp

;--------------------------------------------------------------------
;	Get parameter from PSP as hexadecimal
;	@return es	hexadecimal
;	@return zf	1(success) or 0(error)
;--------------------------------------------------------------------
PSPParamWordHex		proc	near
			push	ax
			push	cx
			push	si
			push	es

			mov	ah, 62h
			int	21h
			mov	es, bx

			mov	ch, 00h
			mov	cl, es:[80h]

			cmp	cx, 2
			jl	PSPParamWordHexError
			cmp	cx, 5
			jg	PSPParamWordHexError

			mov	ah, 0
			mov	si, 0
			mov	bx, 0

PSPParamWordHexLoop:
			mov	al, es:[80h + 1 + si]

			cmp	al, ' '
			je	PSPParamWordHexLoopContinue

PSPParamWordHexNumber:
			cmp	al, '0'
			jl	PSPParamWordHexError
			cmp	al, '9'
			jg	PSPParamWordHexUppercase

			sub	al, 30h
			jmp	PSPParamWordHexLoopNext

PSPParamWordHexUppercase:
			cmp	al, 'A'
			jl	PSPParamWordHexError
			cmp	al, 'F'
			jg	PSPParamWordHexLowercase

			sub	al, 37h
			jmp	PSPParamWordHexLoopNext

PSPParamWordHexLowercase:
			cmp	al, 'a'
			jl	PSPParamWordHexError
			cmp	al, 'f'
			jg	PSPParamWordHexError

			sub	al, 57h

PSPParamWordHexLoopNext:
			push	cx

			mov	cl, 4
			shl	bx, cl
			add	bx, ax

			pop	cx

PSPParamWordHexLoopContinue:
			inc	si
			loop	PSPParamWordHexLoop

PSPParamWordHexSuccess:
			lahf
			or	ah, 01000000b
			sahf
			jmp	PSPParamWordHexRet

PSPParamWordHexError:
			lahf
			and	ah, 10111111b
			sahf

PSPParamWordHexRet:
			pop	es
			pop	si
			pop	cx
			pop	ax
			ret
PSPParamWordHex		endp

;--------------------------------------------------------------------
;	Binary to ASCII code
;	@param	al	binary data
;	@return	ax	hexadecimal number as ASCII code
;--------------------------------------------------------------------
BinaryToASCIIHex		proc	near
			push	bx
			push	cx
			push	di

			mov	ah, 00h

; lower 4 bit
			push	ax

			and	al, 0Fh
			mov	di, ax
			mov	bh, byte ptr [HexTable + di]

; next 4 bit
			pop	ax

			mov	cl, 4
			shr	al, cl
			mov	di, ax
			mov	al, byte ptr [HexTable + di]
			mov	ah, bh

			pop	di
			pop	cx
			pop	bx
			ret
BinaryToASCIIHex		endp

;--------------------------------------------------------------------
;	Set hexadecimal to BinaryAsASCII address + di
;	@param	di	indirect addressing
;	@param	al	hexadecimal
;--------------------------------------------------------------------
SetHexAddress		proc	near
			cmp	al, 60h
			je	SetHexAddressReplace2Eh
			cmp	al, 20h
			jl	SetHexAddressReplace2Eh
			cmp	al, 7Eh
			jg	SetHexAddressReplace2Eh

			mov	byte ptr [BinaryAsASCII + di], al

			ret

SetHexAddressReplace2Eh:
			mov	byte ptr [BinaryAsASCII + di], '.'

			ret
SetHexAddress		endp

;--------------------------------------------------------------------
;	SetRow
;	@param	bx	current row
;	@param	es	specified segment address
;--------------------------------------------------------------------
SetRow		proc	near
			push	ax
			push	si

			mov	di, 0
SetRowLoop:
			mov	al, byte ptr es:[bx + di]
			call	SetHexAddress

; set binary as hex
			mov	si, di
			shl	si, 1
			add	si, di
			call	BinaryToASCIIHex
			mov	word ptr [BinaryAsHex + si], ax

			inc	di
			cmp	di, 000Fh
			jle	SetRowLoop

			pop	si
			pop	ax
			ret
SetRow		endp

;--------------------------------------------------------------------
;	IsEscPressed
;	@return	zf	Esc is pressed(1) or Other Key is Pressed(0)
;--------------------------------------------------------------------
IsEscPressed		proc	near
			push	ax

			mov	ah, 08h
			int	21h

			cmp	al, 1Bh
			je	IsEscPressedEsc

; other key is pressed
			lahf
			and	ah, 10111111b
			sahf

			jmp	IsEscPressedRet

IsEscPressedEsc:
			lahf
			or	ah, 01000000b
			sahf

IsEscPressedRet:
			pop	ax
			ret
IsEscPressed		endp

;--------------------------------------------------------------------
;	StdoutRow
;--------------------------------------------------------------------
StdoutRow		proc	near
; show contents
			push	ax
			push	bx
			push	cx

			mov	ah, 40h
			mov	bx, 1
			mov	cx, word ptr [RowLength]
			mov	dx, offset Row
			int	21h

			pop	cx
			pop	bx
			pop	ax
			ret
StdoutRow		endp

;--------------------------------------------------------------------
;	Exit
;	@param	al	exit code
;--------------------------------------------------------------------
Exit		proc	near
			mov	ah, 4Ch
			int 21h
Exit		endp

;--------------------------------------------------------------------
;	Main
;--------------------------------------------------------------------
Main		proc	near
			mov	ax,Data
			mov	ds,ax

; set segment address to bx register
			call	PSPParamWordHex
			jnz	MainInvalidParamError
			mov	es, bx

			mov	dx, offset RowDescripton
			call	StdoutString

			mov	di, offset SegmentAddress
			call	SetASCIIHexAddress

; set offset address
			; current row
			mov	bx, 0
			; row counter
			mov	ax, 0
RowLoop:
			mov	di, offset OffsetAddress
			call	SetASCIIHexAddress

			call	SetRow
			call	StdoutRow

			inc	ax
			cmp	ax, 20
			jne	RowLoopNext

			mov	ax, 0

			mov	dx, offset PressAnyKeyMsg
			call	StdoutString

			call	IsEscPressed
			jz	MainCancelledByUser

			mov	dx, offset RowDescripton
			call	StdoutString

RowLoopNext:
			push	ax
			push	bx
			mov	bx, 2
			mov	dx, 0
			div	bx
			pop	bx
			pop	ax

			cmp	dx, 0
			je	RowLoopNextNonHighlight

			mov	dx, offset EscSqHighlight
			call StdoutString

RowLoopNextNonHighlight:
			add	bx, 0010h
			cmp	bx, 1000h
			jle	RowLoop

; Exit(complete outputting)
			mov	dx, offset CompleteMsg
			call	StdoutString

			mov	al, 00h
			call	Exit

MainInvalidParamError:
			mov	dx, offset InvalidParamMsg
			call	StdoutString

			mov	al, 02h
			call	Exit

MainCancelledByUser:
			mov	dx, offset CancelledMsg
			call	StdoutString

			mov	al, 01h
			call	Exit
Main		endp

Code		ends
			end	Main
