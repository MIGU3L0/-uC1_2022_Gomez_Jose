;Libreria de retardos:
;info:	 Tarjeta: exclusivo para PIC18F57Q84
	;Version: MPLAB X IDE v6.00 
	;fecha:08/01/2023
	;F:8MHz
	;lenguaje: ASM
;Descripcion: Esta es una Libreria capaz de implementar los siguientes retardos:
;nota: us(microsegundos);   ms(milisegundos)
;retardos:	(etiquetas a utilizar respectivamente)
;10us	    --		d10us 
;25us	    --		d25us
;50us	    --		d50us
;100us	    --		d100us
;200us	    --		d200us
;250us	    --		d250us
;500us	    --		d500s
;1ms	    --		d1ms
;5ms	    --		d5ms
;10ms	    --		d10ms
;25ms	    --		d25ms
;50ms	    --		d50ms
;100ms	    --		d100ms
;200ms	    --		d200ms
;250ms	    --		d250ms

;Ejemplo de archivo.s:
	
;PROCESSOR 18F57Q84
;#include <xc.inc>
;#include "retardos.inc" ;-->libreria incluida   
;PSECT resetVect,class=CODE,reloc=2
;resetVect:
;    goto Main   
;PSECT CODE
;Main:
;    CALL (etiqueta del retardo a utilizar),1
;    GOTO Main
;END resetVect

PSECT udata_acs 
contador1: DS 1		    ;reserva el byte en access Ram
contador2: DS 1
    
PSECT code
    CALL config_osc,1
    CALL config_port,1
    NOP
    
config_osc:
    ;CONFIGURACION DEL OSCILADOR INTERNO A UNA FRECUENCIA DE 8MHZ
    BANKSEL OSCCON1
    MOVLW 0x60		    ;seleccionamos el bloque del Osc interno (HFINTOSC) con un DIV:1
    MOVWF OSCCON1,1
    MOVLW 0x03		    ;seleccionamos frecuencia de 8MHZ
    MOVWF OSCFRQ,1
    RETURN
    
config_port:		    ;PORT-LAT-ANSEL-TRIS, LED:RF3, BUTTON:RA3
    ;config led
    BANKSEL PORTF
    CLRF PORTF,1	    ;PORTF = 0
    BSF PORTF,3,1	    ;LATF<3> = 1, led off
    CLRF ANSELF,1	    ;ANSELF<7:0> = 0 - PortF digital
    BCF TRISF,3,1	    ;TRISF<3> = 0 - RF3 como salida
    RETURN
;T = 6 + 4K    
d10us:			    ;2Tcy -- Call
    MOVLW 1		    ;1Tcy -- k2
    MOVWF contador1,0	    ;1Tcy
loop10us:
    NOP			    ;k*Tcy
    DECFSZ contador1,1,0    ;(k-1)+ 3Tcy
    GOTO loop10us	    ;(k-1)*2Tcy
    RETURN		    ;2Tcy     
;T = 7 + 3k  
d25us:			    ;2Tcy -- Call
    MOVLW 6		    ;1Tcy -- k2
    MOVWF contador1,0	    ;1Tcy
    NOP			    ;1Tcy
loop25us:
    DECFSZ contador1,1,0    ;(k-1)+ 3Tcy
    GOTO loop25us	    ;(k-1)*2Tcy
    RETURN		    ;2Tcy 
;T = 6 + 4K    
d50us:			    ;2Tcy -- Call
    MOVLW 11		    ;1Tcy -- k2
    MOVWF contador1,0	    ;1Tcy
loop50us:
    NOP			    ;k*Tcy
    DECFSZ contador1,1,0    ;(k-1)+ 3Tcy
    GOTO loop50us	    ;(k-1)*2Tcy
    RETURN		    ;2Tcy
;T = 7 + 3k  
d100us:			    ;2Tcy -- Call
    MOVLW 31		    ;1Tcy -- k2
    MOVWF contador1,0	    ;1Tcy
    NOP			    ;1Tcy
loop100us:
    DECFSZ contador1,1,0    ;(k-1)+ 3Tcy
    GOTO loop100us	    ;(k-1)*2Tcy
    RETURN		    ;2Tcy
;T = 8 + 4K    
d200us:			    ;2Tcy -- Call
    MOVLW 48		    ;1Tcy -- k2
    MOVWF contador1,0	    ;1Tcy
    NOP			    ;1Tcy
    NOP			    ;1Tcy
loop200us:
    NOP			    ;k*Tcy
    DECFSZ contador1,1,0    ;(k-1)+ 3Tcy
    GOTO loop200us	    ;(k-1)*2Tcy
    RETURN		    ;2Tcy
;T = 6 + 4K    
d250us:			    ;2Tcy -- Call
    MOVLW 61		    ;1Tcy -- k2
    MOVWF contador1,0	    ;1Tcy
loop250us:
    NOP			    ;k*Tcy
    DECFSZ contador1,1,0    ;(k-1)+ 3Tcy
    GOTO loop250us	    ;(k-1)*2Tcy
    RETURN		    ;2Tcy
;T = 8 + 4K    
d500us:			    ;2Tcy -- Call
    MOVLW 123		    ;1Tcy -- k2
    MOVWF contador1,0	    ;1Tcy
    NOP			    ;1Tcy
    NOP			    ;1Tcy
loop500us:
    NOP			    ;k*Tcy
    DECFSZ contador1,1,0    ;(k-1)+ 3Tcy
    GOTO loop500us	    ;(k-1)*2Tcy
    RETURN		    ;2Tcy
;T = 6 + (4*k1*k2) + 5k2
;T = 6 + k2(4*k1 + 5)
d1ms:			    ;2Tcy -- Call
    MOVLW 1		    ;1Tcy -- k2
    MOVWF contador2,0	    ;1Tcy
Ext_loop_1ms:
    MOVLW 250		    ;1Tcy -- k1
    MOVWF contador1,0	    ;1Tcy
Int_loop_1ms:
    NOP			    ;k1*Tcy
    DECFSZ contador1,1,0    ;(k1-1)+ 3Tcy
    GOTO Int_loop_1ms	    ;(k1-1)*2Tcy
    DECFSZ contador2,1,0    ;(k2-1)+ 3Tcy
    GOTO Ext_loop_1ms	    ;(k2-1)*2Tcy
    RETURN		    ;2Tcy

d5ms:			    ;2Tcy -- Call
    MOVLW 5		    ;1Tcy -- k2
    MOVWF contador2,0	    ;1Tcy
Ext_loop_5ms:
    MOVLW 250		    ;1Tcy -- k1
    MOVWF contador1,0	    ;1Tcy
Int_loop_5ms:
    NOP			    ;k1*Tcy
    DECFSZ contador1,1,0    ;(k1-1)+ 3Tcy
    GOTO Int_loop_5ms	    ;(k1-1)*2Tcy
    DECFSZ contador2,1,0    ;(k2-1)+ 3Tcy
    GOTO Ext_loop_5ms	    ;(k2-1)*2Tcy
    RETURN		    ;2Tcy
    
d10ms:			    ;2Tcy -- Call
    MOVLW 10		    ;1Tcy -- k2
    MOVWF contador2,0	    ;1Tcy
Ext_loop_10ms:
    MOVLW 250		    ;1Tcy -- k1
    MOVWF contador1,0	    ;1Tcy
Int_loop_10ms:
    NOP			    ;k1*Tcy
    DECFSZ contador1,1,0    ;(k1-1)+ 3Tcy
    GOTO Int_loop_10ms	    ;(k1-1)*2Tcy
    DECFSZ contador2,1,0    ;(k2-1)+ 3Tcy
    GOTO Ext_loop_10ms	    ;(k2-1)*2Tcy
    RETURN		    ;2Tcy

d25ms:			    ;2Tcy -- Call
    MOVLW 25		    ;1Tcy -- k2
    MOVWF contador2,0	    ;1Tcy
Ext_loop_25ms:
    MOVLW 250		    ;1Tcy -- k1
    MOVWF contador1,0	    ;1Tcy
Int_loop_25ms:
    NOP			    ;k1*Tcy
    DECFSZ contador1,1,0    ;(k1-1)+ 3Tcy
    GOTO Int_loop_25ms	    ;(k1-1)*2Tcy
    DECFSZ contador2,1,0    ;(k2-1)+ 3Tcy
    GOTO Ext_loop_25ms	    ;(k2-1)*2Tcy
    RETURN

d50ms:			    ;2Tcy -- Call
    MOVLW 50		    ;1Tcy -- k2
    MOVWF contador2,0	    ;1Tcy
Ext_loop_50ms:
    MOVLW 250		    ;1Tcy -- k1
    MOVWF contador1,0	    ;1Tcy
Int_loop_50ms:
    NOP			    ;k1*Tcy
    DECFSZ contador1,1,0    ;(k1-1)+ 3Tcy
    GOTO Int_loop_50ms	    ;(k1-1)*2Tcy
    DECFSZ contador2,1,0    ;(k2-1)+ 3Tcy
    GOTO Ext_loop_50ms	    ;(k2-1)*2Tcy
    RETURN   

d100ms:			    ;2Tcy -- Call
    MOVLW 100		    ;1Tcy -- k2
    MOVWF contador2,0	    ;1Tcy
Ext_loop_100ms:
    MOVLW 250		    ;1Tcy -- k1
    MOVWF contador1,0	    ;1Tcy
Int_loop_100ms:
    NOP			    ;k1*Tcy
    DECFSZ contador1,1,0    ;(k1-1)+ 3Tcy
    GOTO Int_loop_100ms	    ;(k1-1)*2Tcy
    DECFSZ contador2,1,0    ;(k2-1)+ 3Tcy
    GOTO Ext_loop_100ms	    ;(k2-1)*2Tcy
    RETURN 
    
d200ms:			    ;2Tcy -- Call
    MOVLW 199		    ;1Tcy -- k2
    MOVWF contador2,0	    ;1Tcy
Ext_loop_200ms:
    MOVLW 250		    ;1Tcy -- k1
    MOVWF contador1,0	    ;1Tcy
Int_loop_200ms:
    NOP			    ;k1*Tcy
    DECFSZ contador1,1,0    ;(k1-1)+ 3Tcy
    GOTO Int_loop_200ms	    ;(k1-1)*2Tcy
    DECFSZ contador2,1,0    ;(k2-1)+ 3Tcy
    GOTO Ext_loop_200ms	    ;(k2-1)*2Tcy
    RETURN 
    
d250ms:			    ;2Tcy -- Call
    MOVLW 249		    ;1Tcy -- k2
    MOVWF contador2,0	    ;1Tcy
Ext_loop_250ms:
    MOVLW 250		    ;1Tcy -- k1
    MOVWF contador1,0	    ;1Tcy
Int_loop_250ms:
    NOP			    ;k1*Tcy
    DECFSZ contador1,1,0    ;(k1-1)+ 3Tcy
    GOTO Int_loop_250ms	    ;(k1-1)*2Tcy
    DECFSZ contador2,1,0    ;(k2-1)+ 3Tcy
    GOTO Ext_loop_250ms	    ;(k2-1)*2Tcy
    RETURN 