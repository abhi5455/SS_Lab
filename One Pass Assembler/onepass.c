#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define SIZE 30
#define null -9999

void write_add_txt_rec();
FILE *f1,*f2,*f3,*f4;
int ns=0,n=0;

struct node{
    int address;
    struct node* link;
};

struct sym{
    char symbol[30];
    struct node* head;
}*s[30]={NULL};

int search_symtab(char label[]){
    for(int i=0 ; i<ns ; i++)
        if(s[i]!=NULL && strcasecmp(s[i]->symbol,label)==0)
            return i;
    return -1;
}

void generate_txt_rec(int i){
    struct node* ptr;

    ptr=s[i]->head->link;
    while(ptr!=NULL){
        struct node* temp=ptr->link;
        fprintf(f3,"T%06X02%4X\n",ptr->address,s[i]->head->address);
        free(ptr);
        ptr=temp;
    }
    s[i]->head->link=NULL;
}

void insert_symb(char label[],int loc_ctr){
    s[ns]=(struct sym*)malloc(sizeof(struct sym));
    strcpy(s[ns]->symbol,label);
    s[ns]->head=(struct node*)malloc(sizeof(struct node));
    s[ns]->head->address=loc_ctr;
    s[ns]->head->link=NULL;
    ns++;
}

void insert_list(int j,int loc_ctr){
    struct node* ptr=s[j]->head;
    struct node* new;
    new=(struct node*)malloc(sizeof(struct node));
    new->address=loc_ctr;
    new->link=NULL;
    while(ptr->link!=NULL)
        ptr=ptr->link;
    ptr->link=new;
}

char* getconstant(char str[]){
    int n=strlen(str),i;
    char* out=(char*)malloc(20);
    out[0]='\0';
    for(i=2 ; i<n ; i++){
        int p=str[i];
        char temp[10];

        sprintf(temp,"%X",p);
        strcat(out,temp);
    }
    return out;
}

void one_pass(){
    char label[20],opcode[20],operand[20],code[20],mnemonic[20],hex_str[5];;
    char pgm_name[30],text_record[100],mach_inst[30];
    int loc_ctr=0,start_addr,t_size=30,t_len=0,i;
    int op_found,length,prev_start_addr,mod_needed;
    pgm_name[0]='\0';
    fscanf(f1,"%s\t%s\t%s",label,opcode,operand);

    if(strcasecmp(opcode,"start")==0){
        loc_ctr=(int)strtol(operand,NULL,16);
        start_addr=loc_ctr;
        prev_start_addr=start_addr;
        strcpy(pgm_name,label);
        fscanf(f1,"%s\t%s\t%s",label,opcode,operand);
    }else
        start_addr=0;
    fprintf(f3,"H%-6s%06X000000\n",pgm_name,start_addr);
    sprintf(text_record,"T%06XIE",start_addr);

    while(strcasecmp(opcode,"end")!=0){
        mod_needed=0;
        if(strcasecmp(label,"**")!=0){
            i=search_symtab(label);
            if(i>-1){
                if(s[i]->head->address==null){
                    mod_needed=1;
                    s[i]->head->address=loc_ctr;
                }
            }else{
                insert_symb(label,loc_ctr);
            }
        }
        if(strcasecmp(opcode,"word")==0){
            int num=atoi(operand);
            loc_ctr+=3;
            sprintf(mach_inst,"%06X",num);
            t_size-=3;

        }else if(strcasecmp(opcode,"resw")==0){
            loc_ctr+=3*atoi(operand);
            t_size-=3*atoi(operand);
            mach_inst[0]='\0';
        }else if(strcasecmp(opcode,"resb")==0){
            loc_ctr+=atoi(operand);
            t_size-=atoi(operand);
            mach_inst[0]='\0';
        }else if(strcasecmp(opcode,"byte")==0){
            if(operand[0]=='C'){
                char *str=getconstant(operand);
                sprintf(mach_inst,"%s",str);
                free(str);
                loc_ctr+=(strlen(mach_inst)-3);
                t_size-=(strlen(mach_inst)-3);
            }
            else if(operand[0]=='X'){
                sprintf(mach_inst,"%s",&operand[2]);
                loc_ctr+=(strlen(mach_inst)-3)/2;
                t_size-=(strlen(mach_inst)-3)/2;
            }
        }
        else{

            fseek(f2,0,0);
            fscanf(f2,"%s %s",code,mnemonic);
            op_found=0;
            while(strcasecmp(code,"end")!=0){
                if(strcasecmp(code,opcode)==0){
                    op_found=1;
                    break;
                }
                fscanf(f2,"%s %s",code,mnemonic);
            }
            if(op_found){
                int j=search_symtab(operand);
                int symb;
                if(j>-1){
                    if(s[j]->head->address!=null){
                        symb=s[j]->head->address;
                        sprintf(mach_inst,"%2s%4X",mnemonic,symb);
                    }
                    else{
                        insert_list(j,loc_ctr+1);
                        sprintf(mach_inst,"%2s0000",mnemonic);
                    }
                }else{
                    insert_symb(operand,null);
                    insert_list(ns-1,loc_ctr+1);
                    sprintf(mach_inst,"%2s0000",mnemonic);
                }
                loc_ctr+=3;
                t_size-=3;
            }
        }
        if(t_size<0){
            sprintf(hex_str,"%02X",t_len);
            text_record[7]=hex_str[0];
            text_record[8]=hex_str[1];
            fprintf(f3,"%s\n",text_record);
            prev_start_addr+=t_len;
            sprintf(text_record,"T%06XIE",prev_start_addr);
            strcat(text_record,mach_inst);
            t_len+=strlen(mach_inst)/2;
            t_size=30-strlen(mach_inst)/2;
        }else{
            strcat(text_record,mach_inst);
            t_len+=strlen(mach_inst)/2;
        }
        if(mod_needed){
            sprintf(hex_str,"%02X",t_len);
            text_record[7]=hex_str[0];
            text_record[8]=hex_str[1];
            if(strlen(text_record)>9)
                fprintf(f3,"%s\n",text_record);

            generate_txt_rec(i);

            prev_start_addr+=t_len;
            sprintf(text_record,"T%06XIE",prev_start_addr);
            t_len=0;
            t_size=30;
        }
        fscanf(f1,"%s %s %s",label,opcode,operand);
    }
    sprintf(hex_str,"%02X",t_len);
    text_record[7]=hex_str[0];
    text_record[8]=hex_str[1];
    if(strlen(text_record)>9)
        fprintf(f3,"%s\n",text_record);

    fprintf(f3,"E%06X\n",start_addr);
    length=loc_ctr-start_addr;
    fseek(f3,13,0);
    fprintf(f3,"%06X",length);
}
void Free(){
    for(int i=0 ; i<ns ; i++)
        free(s[i]);
}

void write_symtab(){
    int i;
    for(i=0 ; i<ns ; i++){
        fprintf(f4,"%s\t%X\n",s[i]->symbol,s[i]->head->address);
    }
}

void main() {
    f1 = fopen("input.txt", "r");
    if (f1 == NULL) {
        perror("Error opening input.txt");
        exit(1);
    }

    f2 = fopen("optab.txt", "r");
    if (f2 == NULL) {
        perror("Error opening optab.txt");
        fclose(f1); // Ensure previous file is closed
        exit(1);
    }

    f3 = fopen("output1.txt", "w");
    if (f3 == NULL) {
        perror("Error opening output.txt");
        fclose(f1);
        fclose(f2);
        exit(1);
    }

    f4 = fopen("symtab1.txt", "w");
    if (f4 == NULL) {
        perror("Error opening symtab.txt");
        fclose(f1);
        fclose(f2);
        fclose(f3);
        exit(1);
    }

    one_pass();
    write_symtab();

    fflush(f3);
    fflush(f4);

    fclose(f1);
    fclose(f2);
    fclose(f3);
    fclose(f4);

    Free();
}

