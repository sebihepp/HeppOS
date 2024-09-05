
.section ".data"
.align 8

.global stdfont_old
stdfont_old:
	
# 0 - NULL
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000	
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 1 - SOH
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111110
	.byte 0b10000001
	.byte 0b10100101
	.byte 0b10000001
	.byte 0b10000001
	.byte 0b10111101	
	
	.byte 0b10011001
	.byte 0b10000001
	.byte 0b10000001
	.byte 0b01111110

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 2 - STX
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111110
	.byte 0b11111111
	.byte 0b11011011
	.byte 0b11111111
	.byte 0b11111111
	.byte 0b11000011	
	
	.byte 0b11100111
	.byte 0b11111111
	.byte 0b11111111
	.byte 0b01111110

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 3 - ETX
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01101100
	.byte 0b11101110
	.byte 0b11111110
	.byte 0b11111110	
	
	.byte 0b11111110
	.byte 0b01111100
	.byte 0b00111000
	.byte 0b00010000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 4 - EOT
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00010000
	.byte 0b00111000
	.byte 0b01111100
	.byte 0b11111110	
	
	.byte 0b11111110
	.byte 0b01111100
	.byte 0b00111000
	.byte 0b00010000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 5 - ENQ
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00011000
	.byte 0b00111100
	.byte 0b00111100
	.byte 0b01011010
	.byte 0b11111111	
	
	.byte 0b11111111
	.byte 0b01011010
	.byte 0b00011000
	.byte 0b00111100

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 6 - ACK
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00010000
	.byte 0b00111000
	.byte 0b01111100
	.byte 0b11111110
	.byte 0b11111110	
	
	.byte 0b11101110
	.byte 0b01010100
	.byte 0b00010000
	.byte 0b00111000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 7 - BEL
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00011000
	.byte 0b00111100	
	
	.byte 0b00111100
	.byte 0b00011000
	.byte 0b00000000
	.byte 0b00000000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 8 - BS
	.byte 0b11111111
	.byte 0b11111111
	.byte 0b11111111
	.byte 0b11111111
	.byte 0b11111111
	.byte 0b11111111
	.byte 0b11100111
	.byte 0b11000011	
	
	.byte 0b11000011
	.byte 0b11100111
	.byte 0b11111111
	.byte 0b11111111

	.byte 0b11111111
	.byte 0b11111111
	.byte 0b11111111
	.byte 0b11111111
	
# 9 - HT
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00111100
	.byte 0b01100110
	.byte 0b01000010	
	
	.byte 0b01000010
	.byte 0b01100110
	.byte 0b00111100
	.byte 0b00000000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 10 - LF
	.byte 0b11111111
	.byte 0b11111111
	.byte 0b11111111
	.byte 0b11111111
	.byte 0b11111111
	.byte 0b11000011
	.byte 0b10011001
	.byte 0b10111101	
	
	.byte 0b10111101
	.byte 0b10011001
	.byte 0b11000011
	.byte 0b11111111

	.byte 0b11111111
	.byte 0b11111111
	.byte 0b11111111
	.byte 0b11111111
	
# 11 - VT
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00011110
	.byte 0b00001110
	.byte 0b00011010
	.byte 0b00110000
	.byte 0b01111000
	.byte 0b11001100	
	
	.byte 0b11001100
	.byte 0b11001100
	.byte 0b11001100
	.byte 0b01111000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 12 - FF
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00111100
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b00111100	
	
	.byte 0b00011000
	.byte 0b01111110
	.byte 0b00011000
	.byte 0b00011000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 13 - CR
	.byte 0b00000000
	.byte 0b00001000
	.byte 0b00001100
	.byte 0b00001010
	.byte 0b00001010
	.byte 0b00001010
	.byte 0b00001000
	.byte 0b00001000	

	.byte 0b00001000
	.byte 0b01111000
	.byte 0b11111000
	.byte 0b01110000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 14 - SO
	.byte 0b00000000
	.byte 0b00010000
	.byte 0b00011000
	.byte 0b00010100
	.byte 0b00010010
	.byte 0b00011010
	.byte 0b00010110
	.byte 0b00010010	
	
	.byte 0b00010010
	.byte 0b01110010
	.byte 0b11110010
	.byte 0b01100010

	.byte 0b00001110
	.byte 0b00011110
	.byte 0b00011100
	.byte 0b00000000
	
# 15 - SI
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00010000
	.byte 0b10010010
	.byte 0b01111100
	.byte 0b01101100
	.byte 0b11000110	
	
	.byte 0b01101100
	.byte 0b01111100
	.byte 0b10010010
	.byte 0b00010000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 16 - DLE
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b10000000
	.byte 0b11000000
	.byte 0b11100000
	.byte 0b11111000
	.byte 0b11111110	
	
	.byte 0b11111000
	.byte 0b11100000
	.byte 0b11000000
	.byte 0b10000000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 17 - DC1
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000010
	.byte 0b00000110
	.byte 0b00001110
	.byte 0b00111110
	.byte 0b11111110	
	
	.byte 0b00111110
	.byte 0b00001110
	.byte 0b00000110
	.byte 0b00000010

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 18 - DC2
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00010000
	.byte 0b00111000
	.byte 0b01111100
	.byte 0b11010110
	.byte 0b00010000
	.byte 0b00010000	
	
	.byte 0b11010110
	.byte 0b01111100
	.byte 0b00111000
	.byte 0b00010000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 19 - DC3
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110	
	
	.byte 0b01100110
	.byte 0b00000000
	.byte 0b01100110
	.byte 0b01100110

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 20 - DC4
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111111
	.byte 0b11011011
	.byte 0b11011011
	.byte 0b11011011
	.byte 0b11011011
	.byte 0b01111011
	
	.byte 0b00011011
	.byte 0b00011011
	.byte 0b00011011
	.byte 0b00011011

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 21 - NAK
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b01100000
	.byte 0b00111000
	.byte 0b01101100
	.byte 0b11000110
	.byte 0b11000110	
	
	.byte 0b01101100
	.byte 0b00111000
	.byte 0b00001100
	.byte 0b11000110

	.byte 0b01111100
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 22 - SYN
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000	
	
	.byte 0b11111110
	.byte 0b11111110
	.byte 0b11111110
	.byte 0b11111110

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 23 - ETB
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00010000
	.byte 0b00111000
	.byte 0b01111100
	.byte 0b11010110
	.byte 0b00010000
	.byte 0b00010000	
	
	.byte 0b11010110
	.byte 0b01111100
	.byte 0b00111000
	.byte 0b00010000

	.byte 0b11111110
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 24 - CAN
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00010000
	.byte 0b00111000
	.byte 0b01111100
	.byte 0b11010110
	.byte 0b00010000
	.byte 0b00010000	
	
	.byte 0b00010000
	.byte 0b00010000
	.byte 0b00010000
	.byte 0b00010000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 25 - EM
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00010000
	.byte 0b00010000
	.byte 0b00010000
	.byte 0b00010000
	.byte 0b00010000
	.byte 0b00010000	
	
	.byte 0b11010110
	.byte 0b01111100
	.byte 0b00111000
	.byte 0b00010000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 26 - SUB
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00010000
	.byte 0b00011000
	.byte 0b00001100
	.byte 0b11111110	
	
	.byte 0b00001100
	.byte 0b00011000
	.byte 0b00010000
	.byte 0b00000000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 27 - ESC
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00010000
	.byte 0b00110000
	.byte 0b01100000
	.byte 0b11111110	
	
	.byte 0b01100000
	.byte 0b00110000
	.byte 0b00010000
	.byte 0b00000000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 28 - FS
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11000000
	.byte 0b11000000	
	
	.byte 0b11000000
	.byte 0b11111110
	.byte 0b00000000
	.byte 0b00000000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 29 - GS
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00100100
	.byte 0b01000010
	.byte 0b11111111	
	
	.byte 0b01000010
	.byte 0b00100100
	.byte 0b00000000
	.byte 0b00000000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 30 - RS
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00010000
	.byte 0b00111000
	.byte 0b00111000
	.byte 0b01111100	
	
	.byte 0b01111100
	.byte 0b11111110
	.byte 0b11111110
	.byte 0b00000000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 31 - US
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11111110
	.byte 0b11111110
	.byte 0b01111100
	.byte 0b01111100	
	
	.byte 0b00111000
	.byte 0b00111000
	.byte 0b00010000
	.byte 0b00000000

	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	



# 32 - Space
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 33 - Exclamation Mark
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00011000
	.byte 0b00111100
	.byte 0b00111100
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00011000
	.byte 0b00011000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 34 - Double quotes
	.byte 0b00000000
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b00100010
	.byte 0b00100010
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 35 - Number sign
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b11111111
	.byte 0b01100110
	
	.byte 0b01100110
	.byte 0b11111111
	.byte 0b01100110
	.byte 0b01100110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 36 - Dollar 
	.byte 0b00000000
	.byte 0b00010000
	.byte 0b00010000
	.byte 0b01111100
	.byte 0b11010110
	.byte 0b11010110
	.byte 0b01110000
	.byte 0b00111000
	
	.byte 0b00011100
	.byte 0b11010110
	.byte 0b11010110
	.byte 0b01111100
	
	.byte 0b00010000
	.byte 0b00010000
	.byte 0b00000000
	.byte 0b00000000
	
# 37 - Percent sign
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01100000
	.byte 0b10010010
	.byte 0b10010110
	.byte 0b01101100
	.byte 0b00011000
	.byte 0b00110000
	
	.byte 0b01101100
	.byte 0b11010010
	.byte 0b10010010
	.byte 0b00001100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 38 - Ampersand
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00111000
	.byte 0b01101100
	.byte 0b01101100
	.byte 0b00111000
	.byte 0b00110000
	.byte 0b01110110
	
	.byte 0b11011100
	.byte 0b11001100
	.byte 0b11001100
	.byte 0b01110110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 39 - Apostroph
	.byte 0b00000000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00001000
	.byte 0b00010000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 40 - round bracket open
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00001100
	.byte 0b00011000
	.byte 0b00110000
	.byte 0b00110000
	.byte 0b00110000
	.byte 0b00110000
	
	.byte 0b00110000
	.byte 0b00110000
	.byte 0b00011000
	.byte 0b00001100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 41 - round bracket close
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00110000
	.byte 0b00011000
	.byte 0b00001100
	.byte 0b00001100
	.byte 0b00001100
	.byte 0b00001100
	
	.byte 0b00001100
	.byte 0b00001100
	.byte 0b00011000
	.byte 0b00110000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 42 - Star
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01100110
	.byte 0b00111100
	.byte 0b11111111
	
	.byte 0b00111100
	.byte 0b01100110
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 43 - Plus sign
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b01111110
	
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 44 - colon
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00110000
	.byte 0b00110000
	
	.byte 0b00010000
	.byte 0b00100000
	.byte 0b00000000
	.byte 0b00000000

# 45 - hyfen
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11111110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 46 - full stop
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00110000
	.byte 0b00110000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 47 - forward slash
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000010
	.byte 0b00000110
	.byte 0b00001100
	.byte 0b00011000
	.byte 0b00110000
	
	.byte 0b01100000
	.byte 0b11000000
	.byte 0b10000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 48 - Number zero
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11001110
	.byte 0b11011110
	.byte 0b11110110
	
	.byte 0b11100110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b01111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 49 - Number one
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00011000
	.byte 0b00111000
	.byte 0b01111000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b01111110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 50 - Number two
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b00000110
	.byte 0b00001100
	.byte 0b00011000
	.byte 0b00110000
	
	.byte 0b01100000
	.byte 0b11000000
	.byte 0b11000110
	.byte 0b11111110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 51 - Number three
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b00000110
	.byte 0b00000110
	.byte 0b00111100
	.byte 0b00000110
	
	.byte 0b00000110
	.byte 0b00000110
	.byte 0b11000110
	.byte 0b01111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 52 - Number four
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00001100
	.byte 0b00011100
	.byte 0b00111100
	.byte 0b01101100
	.byte 0b11001100
	.byte 0b11001100
	
	.byte 0b11111110
	.byte 0b00001100
	.byte 0b00001100
	.byte 0b00011110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 53 - Number five
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11111110
	.byte 0b11000000
	.byte 0b11000000
	.byte 0b11000000
	.byte 0b11111100
	.byte 0b00000110
	
	.byte 0b00000110
	.byte 0b00000110
	.byte 0b11000110
	.byte 0b01111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 54 - Number six
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b11000000
	.byte 0b11000000
	.byte 0b11111100
	.byte 0b11000110
	
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b01111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 55 - Number seven
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11111110
	.byte 0b11000110
	.byte 0b00000110
	.byte 0b00000110
	.byte 0b00001100
	.byte 0b00011000
	
	.byte 0b00110000
	.byte 0b00110000
	.byte 0b00110000
	.byte 0b00110000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 56 - Number eight
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b01111100
	.byte 0b11000110
	
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b01111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 57 - Number nine
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b01111110
	
	.byte 0b00000110
	.byte 0b00000110
	.byte 0b11000110
	.byte 0b01111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 58 - Double point
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 59 - semicolon
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00001000
	
	.byte 0b00010000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 60 - less than sign
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000110
	.byte 0b00001100
	.byte 0b00011000
	.byte 0b00110000
	.byte 0b01100000
	
	.byte 0b00110000
	.byte 0b00011000
	.byte 0b00001100
	.byte 0b00000110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 61 - equal sign
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111110
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b01111110
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 62 - greater than sign
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01100000
	.byte 0b00110000
	.byte 0b00011000
	.byte 0b00001100
	.byte 0b00000110
	
	.byte 0b00001100
	.byte 0b00011000
	.byte 0b00110000
	.byte 0b01100000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 63 - question mark
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b00000110
	.byte 0b00001100
	.byte 0b00011000
	
	.byte 0b00011000
	.byte 0b00000000
	.byte 0b00011000
	.byte 0b00011000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 64 - at (@)
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00111100
	.byte 0b01000010
	.byte 0b10011101
	.byte 0b10100101
	.byte 0b10100101
	
	.byte 0b10101101
	.byte 0b10110110
	.byte 0b01000000
	.byte 0b00111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 65 - Capital A
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00111000
	.byte 0b01101100
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11111110
	
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 66 - Capital B
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11111100
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01111100
	.byte 0b01100110
	
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b11111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 67 - Capital C
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000000
	.byte 0b11000000
	.byte 0b11000000
	
	.byte 0b11000000
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b01111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 68 - Capital D
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11111100
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b11111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 69 - Capital E
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11111110
	.byte 0b01100110
	.byte 0b01100010
	.byte 0b01101000
	.byte 0b01111000
	.byte 0b01111000
	
	.byte 0b01101000
	.byte 0b01100010
	.byte 0b01100110
	.byte 0b11111110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 70 - Capital F
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11111110
	.byte 0b01100110
	.byte 0b01100010
	.byte 0b01101000
	.byte 0b01111000
	.byte 0b01111000
	
	.byte 0b01101000
	.byte 0b01100000
	.byte 0b01100000
	.byte 0b11110000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 71 - Capital G
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000000
	.byte 0b11000000
	.byte 0b11001110
	
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b01111110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 72 - Capital H
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11111110
	.byte 0b11000110
	
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 73 - Capital I
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00111100
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 74 - Capital J
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00011110
	.byte 0b00001100
	.byte 0b00001100
	.byte 0b00001100
	.byte 0b00001100
	.byte 0b00001100
	
	.byte 0b00001100
	.byte 0b11001100
	.byte 0b11001100
	.byte 0b01111000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 75 - Capital K
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01101100
	.byte 0b01111000
	.byte 0b01111000
	
	.byte 0b01101100
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b11100110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 76 - Capital L
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11110000
	.byte 0b01100000
	.byte 0b01100000
	.byte 0b01100000
	.byte 0b01100000
	.byte 0b01100000
	
	.byte 0b01100000
	.byte 0b01100010
	.byte 0b01100110
	.byte 0b11111110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 77 - Capital M
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b10000010
	.byte 0b11000110
	.byte 0b11101110
	.byte 0b11111110
	.byte 0b11111110
	.byte 0b11010110
	
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 78 - Capital N
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b10000110
	.byte 0b11000110
	.byte 0b11100110
	.byte 0b11110110
	.byte 0b11111110
	.byte 0b11011110
	
	.byte 0b11001110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 79 - Capital O
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b01111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 80 - Capital P
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11111100
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b0111110
	
	.byte 0b01100000
	.byte 0b01100000
	.byte 0b01100000
	.byte 0b11110000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 81 - Capital Q
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	
	.byte 0b11000110
	.byte 0b11010110
	.byte 0b11011110
	.byte 0b01111100
	
	.byte 0b00000110
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 82 - Capital R
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11111100
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01111100
	
	.byte 0b01101100
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b11100110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 83 - Capital S
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b01100000
	.byte 0b00111000
	.byte 0b00001100
	
	.byte 0b00000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b01111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 84 - Capital T
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111110
	.byte 0b01111110
	.byte 0b01011010
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 85 - Capital U
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b01111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 86 - Capital V
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	
	.byte 0b11000110
	.byte 0b01101100
	.byte 0b00111000
	.byte 0b00010000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 87 - Capital W
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11010110
	
	.byte 0b11111110
	.byte 0b11101110
	.byte 0b11000110
	.byte 0b10000010
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 88 - Capital X
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b01101100
	.byte 0b01111100
	.byte 0b00111000
	.byte 0b00111000
	
	.byte 0b01111100
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b11000110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 89 - Capital Y
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b00111100
	.byte 0b00011000
	
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 90 - Capital Z
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11111110
	.byte 0b11000110
	.byte 0b10000110
	.byte 0b00001100
	.byte 0b00011000
	.byte 0b00110000
	
	.byte 0b01100000
	.byte 0b11000010
	.byte 0b11000110
	.byte 0b11111110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 91 - square bracket open
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00111100
	.byte 0b00110000
	.byte 0b00110000
	.byte 0b00110000
	.byte 0b00110000
	.byte 0b00110000
	
	.byte 0b00110000
	.byte 0b00110000
	.byte 0b00110000
	.byte 0b00111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	
# 92 - back slash
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b10000000
	.byte 0b11000000
	.byte 0b01100000
	.byte 0b00110000
	.byte 0b00011000
	
	.byte 0b00001100
	.byte 0b00000110
	.byte 0b00000010
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 93 - square bracken close
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00111100
	.byte 0b00001100
	.byte 0b00001100
	.byte 0b00001100
	.byte 0b00001100
	.byte 0b00001100
	
	.byte 0b00001100
	.byte 0b00001100
	.byte 0b00001100
	.byte 0b00111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 94 - Circumflex
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00010000
	.byte 0b00111000
	.byte 0b01101100
	.byte 0b11000110
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 95 - Underscore
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b11111111
	.byte 0b00000000
	.byte 0b00000000
	
# 96 - Backtick
	.byte 0b00000000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00010000
	.byte 0b00001000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 97 - Lowercase a
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00111100
	.byte 0b00001100
	.byte 0b01111100
	
	.byte 0b11001100
	.byte 0b11001100
	.byte 0b11001100
	.byte 0b01110110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 98 - Lowercase b
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11100000
	.byte 0b01100000
	.byte 0b01100000
	.byte 0b01111100
	.byte 0b01100110
	.byte 0b01100110
	
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 99 - Lowercase c
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b11000000
	
	.byte 0b11000000
	.byte 0b11000000
	.byte 0b11000110
	.byte 0b01111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 100 - Lowercase d
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00011100
	.byte 0b00001100
	.byte 0b00001100
	.byte 0b01111100
	.byte 0b11001100
	
	.byte 0b11001100
	.byte 0b11001100
	.byte 0b11001100
	.byte 0b01110110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 101 - Lowercase e
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b11000110
	
	.byte 0b11111110
	.byte 0b11000000
	.byte 0b11000110
	.byte 0b01111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 102 - Lowercase f
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00011100
	.byte 0b00110110
	.byte 0b00110000
	.byte 0b01111100
	.byte 0b00110000
	.byte 0b00110000
	
	.byte 0b00110000
	.byte 0b00110000
	.byte 0b00110000
	.byte 0b01111000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 103 - Lowercase g
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01110110
	.byte 0b11001100
	.byte 0b11001100
	
	.byte 0b11001100
	.byte 0b11001100
	.byte 0b11001100
	.byte 0b01111100
	
	.byte 0b00001100
	.byte 0b11001100
	.byte 0b01111000
	.byte 0b00000000
	
# 104 - Lowercase h
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11100000
	.byte 0b01100000
	.byte 0b01100000
	.byte 0b01101100
	.byte 0b01110110
	.byte 0b01100110
	
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b11100110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 105 - Lowercase i
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00000000
	.byte 0b00111000
	.byte 0b00011000
	.byte 0b00011000
	
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 106 - Lowercase j
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00001100
	.byte 0b00001100
	.byte 0b00000000
	.byte 0b00011100
	.byte 0b00001100
	.byte 0b00001100
	
	.byte 0b00001100
	.byte 0b00001100
	.byte 0b00001100
	.byte 0b00001100
	
	.byte 0b11001100
	.byte 0b11001100
	.byte 0b01111000
	.byte 0b00000000
	
# 107 - Lowercase k
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11100000
	.byte 0b01100000
	.byte 0b01100000
	.byte 0b01100110
	.byte 0b01101100
	.byte 0b01111000
	
	.byte 0b01111000
	.byte 0b01101100
	.byte 0b01100110
	.byte 0b11100110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 108 - Lowercase l
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00111000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 109 - Lowercase m
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11101100
	.byte 0b11111110
	.byte 0b11010110
	
	.byte 0b11010110
	.byte 0b11010110
	.byte 0b11010110
	.byte 0b11000110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 110 - Lowercase n
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11011100
	.byte 0b01100110
	.byte 0b01100110
	
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 111 - Lowercase o
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b11000110
	
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b01111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 112 - Lowercase p
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11011100
	.byte 0b01100110
	.byte 0b01100110
	
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01100110
	.byte 0b01111100
	
	.byte 0b01100000
	.byte 0b01100000
	.byte 0b11110000
	.byte 0b00000000
	
# 113 - Lowercase q
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11001100
	.byte 0b11001100
	
	.byte 0b11001100
	.byte 0b11001100
	.byte 0b11001100
	.byte 0b01111100
	
	.byte 0b00001100
	.byte 0b00001100
	.byte 0b00011110
	.byte 0b00000000
	
# 114 - Lowercase r
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11011110
	.byte 0b01110110
	.byte 0b01100000
	
	.byte 0b01100000
	.byte 0b01100000
	.byte 0b01100000
	.byte 0b11110000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 115 - Lowercase s
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b01100000
	
	.byte 0b00111000
	.byte 0b00001100
	.byte 0b11000110
	.byte 0b01111100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000

# 116 - Lowercase t
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00010000
	.byte 0b00110000
	.byte 0b00110000
	.byte 0b11111100
	.byte 0b00110000
	.byte 0b00110000
	
	.byte 0b00110000
	.byte 0b00110000
	.byte 0b00110100
	.byte 0b00011000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 117 - Lowercase u
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11001100
	.byte 0b11001100
	.byte 0b11001100
	
	.byte 0b11001100
	.byte 0b11001100
	.byte 0b11001100
	.byte 0b01110110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 118 - Lowercase v
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	
	.byte 0b11000110
	.byte 0b01101100
	.byte 0b00111000
	.byte 0b00010000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 119 - Lowercase w
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11000110
	.byte 0b11010110
	.byte 0b11010110
	
	.byte 0b11010110
	.byte 0b11010110
	.byte 0b11111110
	.byte 0b01101100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 120 - Lowercase x
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b01101100
	
	.byte 0b00111000
	.byte 0b01101100
	.byte 0b11000110
	.byte 0b11000110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 121 - Lowercase y
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b01111110
	
	.byte 0b00000110
	.byte 0b00001100
	.byte 0b11111000
	.byte 0b00000000
	
# 122 - Lowercase z
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b11111110
	.byte 0b10001100
	.byte 0b00011000
	
	.byte 0b00110000
	.byte 0b01100000
	.byte 0b11000010
	.byte 0b11111110
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 123 - strange bracket open 
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00001110
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b01110000
	.byte 0b00011000
	
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 124 - vertical line
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00011100
	.byte 0b00011100
	.byte 0b00011100
	.byte 0b00011100
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00011100
	.byte 0b00011100
	.byte 0b00011100
	.byte 0b00011100
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 125 - strange bracket close 
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01110000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00001110
	.byte 0b00011000
	
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b00011000
	.byte 0b01110000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 126 - tilde 
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01110110
	.byte 0b11011100
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 127 - house 
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00010000
	.byte 0b00111000
	.byte 0b01101100
	.byte 0b11000110
	
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11111110
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
# 128 - Je  
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b01111100
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b11000000
	.byte 0b11000000
	.byte 0b11000000
	
	.byte 0b11000000
	.byte 0b11000110
	.byte 0b11000110
	.byte 0b01111100
	
	.byte 0b00010000
	.byte 0b00001000
	.byte 0b01110000
	.byte 0b00000000
	
# 129 - ü  
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	.byte 0b00000000
	
	
.skip (255-129)*16, 0
 
 