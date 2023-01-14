PROCESSOR 18F57Q84
#include "bit_config.inc"     /*config statements should precede project file includes.*/
#include "delays.inc" 
#include <xc.inc>

PSECT resetVect,class=CODE,reloc=2
resetVect:
    goto main  
        
PSECT CODE
main:
    CALL config_osc,1	    ;va a la subrutina config_osc
    CALL config_port,1	    ;va a la subrutina config_port
    MOVLW 00000000B	    ;Carga 0 a W
    MOVWF LATC		    ;Escribe 0 en C
    MOVWF TRISC		    ;asigna los puertos C como salida
    MOVWF TRISE		    ;asigna los puertos E como salida
    
loop:
    BTFSC PORTA,3,0	    ;PortA<3> = 0? - Button press?
    goto loop		    ;regresa a loop si el boton no esta presionado
    goto corrimiento	    ;salta a corrimiento si el boton esta presionado
loop1:
    BTFSC PORTA,3,0	    ;PortA<3> = 0? - Button press?
    goto corrimiento	    ;salta corrimiento si el boton esta presionado
    MOVLW 00000000B	    ;escribe 0 en w si el boton esta presionado
    MOVWF PORTC		    ;mueve w->c y lee el puerto C
    CALL Delay_250ms	    ;4 retardos de 250ms = 1 seg
    CALL Delay_250ms
    CALL Delay_250ms
    CALL Delay_250ms
    goto main		    ;salto a main
    
corrimiento:
    MOVLW 00000001B	    ;escribe el valor a W
    MOVWF PORTC		    ;mueve w->c y lee el puerto C
    MOVLW 00000010B	    ;escribe el valor a W
    MOVWF PORTE,1	    ;mueve w->E y lee el puerto E
    CALL Delay_250ms	    ;retardo de 250ms para impares
   
    
    MOVLW 00000010B	    ;escribe el valor a W
    MOVWF PORTC		    ;mueve w->c y lee el puerto C
    MOVLW 00000001B	    ;escribe el valor a W
    MOVWF PORTE,1	    ;mueve w->E y lee el puerto E
    CALL Delay_250ms	    ;retardos de 5000ms para impares
    CALL Delay_250ms

    MOVLW 00000100B
    MOVWF PORTC
    MOVLW 00000010B
    MOVWF PORTE,1
    CALL Delay_250ms
    
    MOVLW 00001000B
    MOVWF PORTC
    MOVLW 00000001B
    MOVWF PORTE,1
    CALL Delay_250ms
    CALL Delay_250ms

    MOVLW 00010000B
    MOVWF PORTC
    MOVLW 00000010B
    MOVWF PORTE,1
    CALL Delay_250ms
    
    MOVLW 00100000B
    MOVWF PORTC
    MOVLW 00000001B
    MOVWF PORTE,1
    CALL Delay_250ms
    CALL Delay_250ms
    
    MOVLW 01000000B
    MOVWF PORTC
    MOVLW 00000010B
    MOVWF PORTE,1
    CALL Delay_250ms
    
    MOVLW 10000000B
    MOVWF PORTC
    MOVLW 00000001B
    MOVWF PORTE,1
    CALL Delay_250ms
    CALL Delay_250ms
    goto loop1		    ; salta a loop1

END resetVect