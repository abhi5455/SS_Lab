#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define SIZE 25

int main() {
    char label[SIZE], opcode[SIZE], operand[SIZE], symbol[SIZE], address[SIZE], err_desc[100], opvalue[SIZE], mnemonic[SIZE];
    int start, locctr, error = 0, op_found = 0, length, size;

    FILE *fp1, *fp2, *fp3, *fp4, *fp5, *fp6;
    fp1 = fopen("./Two_Pass_Assembler/input.txt", "r");
    fp2 = fopen("./Two_Pass_Assembler/optab.txt", "r");
    fp3 = fopen("./Two_Pass_Assembler/symtab.txt", "w");
    fp4 = fopen("./Two_Pass_Assembler/intermediate.txt", "w");
    fp5 = fopen("./Two_Pass_Assembler/length.txt", "w");
    fp6 = fopen("./Two_Pass_Assembler/symtab.txt", "r");
    if (fp1 == NULL || fp2 == NULL || fp3 == NULL || fp4 == NULL || fp5 == NULL || fp6 == NULL) {
        printf("ERROR OPENING FILE");
        return 0;
    }

    fscanf(fp1, "%s\t%s\t%s", label, opcode, operand);
    if (strcmp(opcode, "START") == 0) {
        sscanf(operand, "%X", &start);
        locctr = start;
    }
    else {
        locctr = 0;
    }
    fprintf(fp4, "*\t%s\t%s\t%s\n", label, opcode, operand);

    fscanf(fp1, "%s\t%s\t%s", label, opcode, operand);
    while (strcmp(opcode, "END") != 0 && error == 0) {
        fprintf(fp4, "%4X", locctr);
        if (strcmp(label, "-") != 0) {
            if (strcmp(label, "*") != 0) {
                while (fscanf(fp6, "%s\t%s\t", symbol, address) != EOF) {
                    if (strcmp(label, symbol) == 0) {
                        error = 1;
                        strcat(err_desc, "ERROR: DUPLICATE SYMBOL ");
                        strcat(err_desc, label);
                        break;
                    }
                }
                rewind(fp6);
                if (!error) {
                    fprintf(fp3, "%s\t%X\n", label, locctr);
                }
            }
            if (strcmp(opcode, "WORD") == 0) {
                locctr += 3;
            } else if (strcmp(opcode, "RESW") == 0) {
                locctr += 3 * atoi(operand);
            } else if (strcmp(opcode, "RESB") == 0) {
                locctr += atoi(operand);
            } else if (strcmp(opcode, "BYTE") == 0) {
                locctr += strlen(operand) - 3;
            } else {
                while (fscanf(fp2, "%s\t%s", opvalue, mnemonic) != EOF) {
                    if (strcmp(opcode, opvalue) == 0) {
                        op_found = 1;
                        break;
                    }
                }
                rewind(fp2);
                if (op_found) {
                    locctr += 3;
                } else {
                    error = 1;
                    strcat(err_desc, "ERROR: DUPLICATE SYMBOL ");
                    strcat(err_desc, label);
                }
            }
        }
        fprintf(fp4, "\t%s\t%s\t%s\n", label, opcode, operand);
        fscanf(fp1, "%s\t%s\t%s", label, opcode, operand);
    }
    if (error) {
        printf("%s\n", err_desc);
    } else {
        length = locctr - start;
        size = locctr - start;
        fprintf(fp4, "*\t%s\t*\n", opcode);
        printf("ASSEMBLED SUCCESSFULLY\n");
        printf("Length of the program : %d\n", length);
        fprintf(fp5, "%d\t%X", length, size);
    }

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    fclose(fp5);
    fclose(fp6);
}