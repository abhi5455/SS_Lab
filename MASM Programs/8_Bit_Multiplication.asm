ASSUME CS:CODE, DS:DATA
DATA SEGMENT
    M1 DB 10,13,"Enter the 1st Number: $"
    M2 DB 10,13,"Enter the 2nd Number: $"
    M3 DB 10,13,"Product is: $"
DATA ENDS
PRTMSG MACRO MESSAGE
    LEA DX,MESSAGE
    MOV AH,09H
    INT 21H
    ENDM
GETDCM MACRO
    MOV AH,01
    INT 21H
    SUB AL,30H
    ENDM
CODE SEGMENT
START:  MOV AX,DATA
        MOV DS,AX
        PRTMSG M1
        GETDCM
        MOV BL,AL
        PRTMSG M2
        GETDCM
        MOV AH,00H
        MUL BL
        AAM
        MOV BX,AX
        PRTMSG M3
        MOV DL,BH
        OR DL,30H
        MOV AH,02H
        INT 21H
        MOV DL,BL
        OR DL,30H
        MOV AH,02H
        INT 21H
        MOV AH,4CH
        INT 21H
CODE ENDS
END START
