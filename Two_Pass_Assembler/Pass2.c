#include <stdio.h>
#include <string.h>
#define SIZE 50
#define tsize 10

int main(){
    FILE *fp1, *fp2, *fp3, *fp4, *fp5, *fp6;
    char locctr[SIZE], label[SIZE], opcode[SIZE], operand[SIZE];

    fp1 = fopen("./Two_Pass_Assembler/intermediate.txt", "r");
    fp2 = fopen("./Two_Pass_Assembler/optab.txt", "r");
    fp3 = fopen("./Two_Pass_Assembler/symtab.txt", "r");
    fp4 = fopen("./Two_Pass_Assembler/length.txt", "r");
    fp5 = fopen("./Two_Pass_Assembler/output.txt", "w");
    fp6 = fopen("./Two_Pass_Assembler/object_pgm.txt", "w");
    if (fp1 == NULL || fp2 == NULL || fp3 == NULL || fp4 == NULL || fp5 == NULL || fp6 == NULL) {
        printf("ERROR OPENING FILE");
        return 0;
    }

    fscanf(fp1,"%s\t%s\t%s\t%s",locctr,label,opcode,operand);
    if(strcmp(opcode,"START")){

    }
    return 0;
}