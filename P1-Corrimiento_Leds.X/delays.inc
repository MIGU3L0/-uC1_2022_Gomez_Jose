PSECT udata_acs 
contador1: DS 1		    ;reserva el byte en access Ram
contador2: DS 1		    ;reserva el byte en access Ram
    
PSECT code
config_osc:
;CONFIGURACION DEL OSCILADOR INTERNO A UNA FRECUENCIA DE 8MHZ
    BANKSEL OSCCON1
    MOVLW 0x60		    ;seleccionamos el bloque del Osc internoh(HFINTOSC) con un DIV:1
    MOVWF OSCCON1
    MOVLW 0x03		    ;seleccionamos frecuencia de 8MHZ
    MOVWF OSCFRQ
    RETURN
config_port:		    ;PORT-LAT-ANSEL-TRIS, LED:RF3, BUTTON:RA3
    ;config led
    BANKSEL PORTC
    CLRF PORTC,1	;PORTF = 0
    BSF PORTC,3,1	;LATF<3> = 1, led off
    CLRF ANSELC,1	;ANSELF<7:0> = 0 - PortF digital
    BCF TRISC,3,1	;TRISF<3> = 0 - RF3 como salida
    ;config button
    BANKSEL PORTA
    CLRF PORTA,1	;portA<7:0> = 0
    CLRF ANSELA,1	;PortA digital
    BSF TRISA,3,1	;RA3 como entrada
    BSF WPUA,3,1	;Actiamos la resistencia Pull- Up del pin RA3
    RETURN
    
Delay_250ms:		    ;2Tcy -- Call
    MOVLW 249		    ;1Tcy -- k2
    MOVWF contador1,0	    ;1Tcy
Ext_loop:
    MOVLW 250		    ;1Tcy -- k1
    MOVWF contador1,0	    ;1Tcy
Int_loop:
    NOP			    ;k*Tcy
    DECFSZ contador1,1,0    ;(k1-1)+ 3Tcy
    GOTO Int_loop	    ;(k1-1)*2Tcy
    DECFSZ contador2,1,0    ;(k2-1)+ 3Tcy
    GOTO Ext_loop	    ;(k2-1)*2Tcy
    RETURN		    ;2Tcy     


