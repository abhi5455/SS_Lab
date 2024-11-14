#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char t1[20],t2[20],t3[20],t4[10],address[10],label[10],opcode[10],operand[10],length[10],size[10],a[10],ad[10],st_addr[10];
int s=-1,o=-1,i,j;

FILE *fp1,*fp2,*fp3,*fp4,*fp5,*fp6;

struct SYNTAB
{
    char label[10];
    char addr[10];
} ST[30];

struct OPTAB
{
    char opcode[10];
    char hexcode[10];
} OT[30];

void read_optab()
{
    while(1)
    {
        o++;
        fscanf(fp3,"%s %s",OT[o].opcode,OT[o].hexcode);
        if (getc(fp3) == EOF)
        {
            break;
        }
    }
}

void read_symtab()
{
    while (1)
    {
        s++;
        fscanf(fp2, "%s %s", ST[s].label, ST[s].addr);
        if (getc(fp2) == EOF)
        {
            break;
        }
    }
}

void read_line()
{
    strcpy(t1,"");
    strcpy(t2,"");
    strcpy(t3,"");
    strcpy(t4,"");

    fscanf(fp1,"%s",t1);
    if (getc(fp1)!='\n')
    {
        fscanf(fp1,"%s",t2);
        if (getc(fp1)!='\n')
        {
            fscanf(fp1, "%s", t3);
            if (getc(fp1) != '\n')
            {
                fscanf(fp1, "%s", t4);
            }
        }
    }

    if (strcmp(t1,"")!=0 && strcmp(t2,"")!=0 && strcmp(t3,"")!=0 && strcmp(t4,"") != 0)
    {
        strcpy(address, t1);
        strcpy(label, t2);
        strcpy(opcode, t3);
        strcpy(operand, t4);
    }
    else if (strcmp(t1,"")!=0 && strcmp(t2,"")!=0 && strcmp(t3,"")!=0 && strcmp(t4,"")==0)
    {
        strcpy(address, t1);
        strcpy(label, "");
        strcpy(opcode, t2);
        strcpy(operand, t3);
    }
    else if (strcmp(t1,"")!=0 && strcmp(t2,"")!=0 && strcmp(t3,"")==0 && strcmp(t4,"")==0)
    {
        if (strcmp(t1,"END")==0)
        {
            strcpy(address,"");
            strcpy(label,"");
            strcpy(opcode,t1);
            strcpy(operand,t2);
        }
        else
        {
            strcpy(address,t1);
            strcpy(label,"");
            strcpy(opcode,t2);
            strcpy(operand, "");
        }
    }
}

int main()
{
    fp1=fopen("intermediate.txt", "r");
    fp2=fopen("symtab.txt", "r");
    fp3=fopen("optab.txt", "r");
    fp4=fopen("length.txt", "r");
    fp5=fopen("output.txt", "w");
    fp6=fopen("object_program.txt", "w");

    if (fp1==NULL||fp2==NULL||fp3==NULL||fp4==NULL||fp5==NULL||fp6==NULL)
    {
        printf("Error opening files\n");
        return 1;
    }

    fscanf(fp4,"%s\t%s\t",length,size);

    read_optab();
    read_symtab();

    fscanf(fp1,"%s\t%s\t%s\t",label,opcode,operand);
    strcpy(st_addr,operand);

    if (strcmp(opcode,"START")==0)
    {
        fprintf(fp5,"%s\t%s\t%s\t%s\n",address,label,opcode,operand); // Print header line in assembly listing
        fprintf(fp6,"H^%s^00%s^0000%s\n",label,operand,length);
        fprintf(fp6,"T^00%s^%s",operand,size);
        read_line();
    }

    while (strcmp(opcode,"END")!=0)
    {
        if (strcmp(opcode,"BYTE")==0)
        {
            // Handle BYTE directive
            if (operand[0]=='C')
            {
                fprintf(fp5,"%s\t%s\t%s\t%s\t",address,label,opcode,operand); // Print assembly
                for (i=2;operand[i]!='\'';i++)
                {
                    sprintf(ad,"%x",operand[i]);  // Convert character to hex
                    fprintf(fp5,"%s",ad);         // Print object code
                    fprintf(fp6,"^%s",ad);        // Write object code to object file
                }
                fprintf(fp5, "\n");
            }
            else if(operand[0] == 'X')
            {
                strncpy(ad,operand + 2,strlen(operand) - 3);  // Extract hex value
                fprintf(fp5,"%s\t%s\t%s\t%s\t%s\n",address,label,opcode,operand,ad);
                fprintf(fp6,"^%s",ad);
            }
        }
        else if(strcmp(opcode, "WORD")==0)
        {
            // Handle WORD directive
            sprintf(a, "%06x", atoi(operand));  // Convert to 6-digit hex
            fprintf(fp5, "%s\t%s\t%s\t%s\t%s\n", address, label, opcode, operand, a);
            fprintf(fp6, "^%s", a);
        }
        else if (strcmp(opcode, "RESW") == 0 || strcmp(opcode, "RESB") == 0)
        {
            // For RESW and RESB, print the line but no object code is generated
            fprintf(fp5, "%s\t%s\t%s\t%s\n", address, label, opcode, operand);
        }
        else
        {
            // Look for opcode in OPTAB
            i = 0;
            while (i <= o && strcmp(opcode, OT[i].opcode) != 0) {
                i++;
            }

            if (i > o) {
                printf("Error: Opcode %s not found in OPTAB\n", opcode);
            }
            else {
                // Look for operand in SYMTAB
                j = 0;
                while (j <= s && strcmp(operand, ST[j].label) != 0) {
                    j++;
                }

                if (j > s) {
                    printf("Error: Operand %s not found in SYMTAB\n", operand);
                }
                else {
                    fprintf(fp5, "%s\t%s\t%s\t%s\t%s%s\n", address, label, opcode, operand, OT[i].hexcode, ST[j].addr);
                    fprintf(fp6, "^%s%s", OT[i].hexcode, ST[j].addr);
                }
            }
        }
        read_line();  // Move to the next line in the intermediate file.
    }

    fprintf(fp5,"%s\t%s\t%s\t%s\t%s\n","*",label,opcode,operand,"*");
    fprintf(fp6,"\nE^00%s\n",st_addr);  // Write end record to object code

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    fclose(fp5);
    fclose(fp6);

    return 0;
}