
.section ".data"
.align 8

.global stdfont
stdfont:
	
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
	
	
.skip (255-68)*16, 0
