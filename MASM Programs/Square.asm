ASSUME CS:CODE, DS:DATA
DATA SEGMENT
    M1 DB 10,13,"Enter the Number: $"
    M2 DB 10,13,"Square is: $"
    S DB 3 DUP(00H)
DATA ENDS

PRTMSG MACRO MESSAGE
    LEA DX,MESSAGE
    MOV AH,09H
    INT 21H
    ENDM

GETDCM MACRO
    MOV AH,01H
    INT 21H
    SUB AL,30H
    ENDM

PRTDCM MACRO
    MOV DL,[SI]
    ADD DL,30H
    MOV AH,02H
    INT 21H
    ENDM

CODE SEGMENT
START:  MOV AX,DATA
        MOV DS,AX
        PRTMSG M1
        GETDCM
        MOV AH,00H
        MOV BH,00H
        MOV BL,AL
        MUL BL
        AAM
        MOV BX,AX
        LEA SI,S
        MOV [SI],BH
        PRTMSG M2
        PRTDCM
        INC SI
        MOV [SI],BL
        PRTDCM
        MOV AH,4CH
        INT 21H
CODE ENDS
END START