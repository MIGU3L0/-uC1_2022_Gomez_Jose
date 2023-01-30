;Nombre del arvivo.s: Ejer_2
;Descripcion: Programa con interrupciones, se inicia la secuencia de leds al activar el boton
;	      de la placa del PIC18F57Q84, y se detiene al activar la interrupcion 1 asignado al pin RB4
;	      en caso de querer reiniciar la secuencia, cuenta cn una segunda interrupcion alojada en RF2.
;Autor: José Miguel Gómez Sánchez 
;Fecha: 30/01/2023
PROCESSOR 18F57Q84
#include "Bit_Config.inc"   /config statements should precede project file includes./
#include <xc.inc>
    
PSECT resetVect,class=CODE,reloc=2
resetVect:
    goto Main
    
PSECT ISRVectLowPriority,class=CODE,reloc=2
ISRVectLowPriority:
    BTFSS   PIR1,0,0	; ¿Se ha producido la INT0?
    GOTO    Exit_1
LEDS_ON:
    BCF	    PIR1,0,0	; limpiamos el flag de INT0
    GOTO    Reload
Exit_1:
    RETFIE

PSECT ISRVectHighPriority,class=CODE,reloc=2
ISRVectHighPriority:
    BTFSS   PIR10,0,0	; ¿Se ha producido la INT0?
    GOTO    Loop_1
LEDS_OFF:
    BCF	    PIR10,0,0	; limpiamos el flag de INT2
    GOTO    final
Exit:
    RETFIE

PSECT udata_acs
contador1:  DS 1	    
contador2:  DS 1
offset:	    DS 1
counter:    DS 1
counter1:   DS 1 
    
PSECT CODE    
Main:
    CALL    Config_OSC,1
    CALL    Config_Port,1
    CALL    Config_PPS,1
    CALL    Config_INT0_INT1_INT2,1
    GOTO    Toggle_LED
       
Toggle_LED:
   BTFSC   PIR10,0,0	; ¿Se ha producido la INT0?
   GOTO	   ISRVectHighPriority
   BTG	   LATF,3,0
   CALL    Delay_250ms,1
   CALL    Delay_250ms,1
   BTG	   LATF,3,0
   CALL    Delay_250ms,1
   CALL    Delay_250ms,1
   goto	   Toggle_LED

Loop:
    BSF	    LATF,3,0		;toggle off
    BANKSEL PCLATU
    MOVLW   low highword(Table)
    MOVWF   PCLATU,1
    MOVLW   high(Table)
    MOVWF   PCLATH,1
    RLNCF   offset,0,0
    CALL    Table
    MOVWF   LATC,0
    CALL    Delay_250ms,1
    DECFSZ  counter,1,0
    GOTO    Next_Seq
    
Verifica:
    DECFSZ  counter1,1,0
    GOTO    Reload2
    Goto    final
    
Next_Seq:
    INCF    offset,1,0
    GOTO    Loop
    
Reload:
    MOVLW   0x05	
    MOVWF   counter1,0	; carga del contador con el numero de offsets
    MOVLW   0x00	
    MOVWF   offset,0	; definimos el valor del offset inicial
    
Reload2:
    MOVLW   0x0A	
    MOVWF   counter,0	; carga del contador con el numero de offsets
    MOVLW   0x00	
    MOVWF   offset,0	; definimos el valor del offset inicial
    GOTO    Loop  
        
Config_OSC:
    ;Configuracion del Oscilador Interno a una frecuencia de 4MHz
    BANKSEL OSCCON1
    MOVLW   0x60    ;seleccionamos el bloque del osc interno(HFINTOSC) con DIV=1
    MOVWF   OSCCON1,1 
    MOVLW   0x02    ;seleccionamos una frecuencia de Clock = 4MHz
    MOVWF   OSCFRQ,1
    RETURN
   
Config_Port:	
    ;Config Led
    BANKSEL PORTF
    CLRF    PORTF,1	
    BSF	    LATF,3,1
    BSF	    LATF,2,1
    CLRF    ANSELF,1	
    BCF	    TRISF,3,1
    BCF	    TRISF,2,1
    
    ;Config User Button
    BANKSEL PORTA
    CLRF    PORTA,1	
    CLRF    ANSELA,1	
    BSF	    TRISA,3,1	
    BSF	    WPUA,3,1
    
    ;Config Ext Button
    BANKSEL PORTB
    CLRF    PORTB,1	
    CLRF    ANSELB,1	
    BSF	    TRISB,4,1	
    BSF	    WPUB,4,1
    
    ;Config Ext Button2
    BANKSEL PORTF
    CLRF    PORTF,1	
    CLRF    ANSELF,1	
    BSF	    TRISF,2,1	
    BSF	    WPUF,2,1
    
    ;Config PORTC
    BANKSEL PORTC
    CLRF    PORTC,1	
    CLRF    LATC,1	
    CLRF    ANSELC,1	
    CLRF    TRISC,1
    RETURN
    
Config_PPS:
    ;Config INT0
    BANKSEL INT0PPS
    MOVLW   0x03
    MOVWF   INT0PPS,1	; INT0 --> RA3
    
    ;Config INT1
    BANKSEL INT1PPS
    MOVLW   0x0C
    MOVWF   INT1PPS,1	; INT1 --> RB4
    
    ;Config INT2
    BANKSEL INT2PPS
    MOVLW   0x2A
    MOVWF   INT2PPS,1
    
    RETURN

Config_INT0_INT1_INT2:
    ;Configuracion de prioridades
    BSF	INTCON0,5,0 ; INTCON0<IPEN> = 1 -- Habilitamos las prioridades
    BANKSEL IPR1
    BCF	IPR1,0,1    ; IPR1<INT0IP> = 0 --  INT0 de baja prioridad
    BSF	IPR6,0,1    ; IPR6<INT1IP> = 0 --  INT1 de alta prioridad
    BSF	IPR10,0,1   ; IPR1<INT2IP> = 1 --  INT2 de alta prioridad
    
    ;Config INT0
    BCF	INTCON0,0,0 ; INTCON0<INT0EDG> = 0 -- INT0 por flanco de bajada
    BCF	PIR1,0,0    ; PIR1<INT0IF> = 0 -- limpiamos el flag de interrupcion
    BSF	PIE1,0,0    ; PIE1<INT0IE> = 1 -- habilitamos la interrupcion ext0
    
    ;Config INT1
    BCF	INTCON0,1,0 ; INTCON0<INT1EDG> = 0 -- INT1 por flanco de bajada
    BCF	PIR6,0,0    ; PIR6<INT1IF> = 0 -- limpiamos el flag de interrupcion
    BSF	PIE6,0,0    ; PIE6<INT1IE> = 1 -- habilitamos la interrupcion ext1
    
    ;Config INT2
    BCF	INTCON0,2,0  ; INTCON0<INT2EDG> = 0 -- INT2 por flanco de bajada
    BCF	PIR10,0,0    ; PIR10<INT2IF> = 0 -- limpiamos el flag de interrupcion
    BSF	PIE10,0,0    ; PIE10<INT2IE> = 1 -- habilitamos la interrupcion ext1
    
    ;Habilitacion de interrupciones
    BSF	INTCON0,7,0 ; INTCON0<GIE/GIEH> = 1 -- habilitamos las interrupciones de forma global y de alta prioridad
    BSF	INTCON0,6,0 ; INTCON0<GIEL> = 1 -- habilitamos las interrupciones de baja prioridad
    RETURN

Table:
    ADDWF   PCL,1,0
    RETLW   10000001B	; offset: 0
    RETLW   01000010B	; offset: 1
    RETLW   00100100B	; offset: 2
    RETLW   00011000B	; offset: 3
    RETLW   00000000B	; offset: 4 -> se apagan todos
    RETLW   00011000B	; offset: 5
    RETLW   00100100B	; offset: 6
    RETLW   01000010B	; offset: 7
    RETLW   10000001B	; offset: 8
    RETLW   00000000B	; offset: 9
        
Loop_1:
    BTFSS   PIR6,0,0	; ¿Se ha producido la INT0?
    GOTO    Exit	
    GOTO    Toggle_LED     
    
;Delay_250ms
Delay_250ms:			;  CALL --> 2Tcy
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
final:     
End resetVect