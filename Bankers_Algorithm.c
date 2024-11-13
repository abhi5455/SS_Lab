#include<stdio.h>
#define SIZE 50

void printMatrix(int Matrix[SIZE][SIZE], int n, int m){
    int i, j;
    for(i=0;i<n;i++){
        printf("   ");
        for(j=0;j<m;j++){
            printf("%d ",Matrix[i][j]);
        }
        printf("\n");
    }
}

int isArrayLess(int A[], int B[], int m){
    int i;
    for(i=0;i<m;i++){
        if(A[i]>B[i]){
            return 0;
        }
    }
    return 1;
}

int main(){
    int i, j, k, z, n, m, isSafe=1, Avail[SIZE], Work[SIZE], Finish[SIZE], safeSeq[SIZE], Max[SIZE][SIZE], Alloc[SIZE][SIZE], Need[SIZE][SIZE];
    printf("Enter the No of Processes: ");
    scanf(" %d",&n);
    printf("Enter the No of type of Resources: ");
    scanf(" %d",&m);

    printf("Enter Available Resources: ");
    for(i=0;i<m;i++){
        scanf("%d",&Avail[i]);
        Work[i]=Avail[i];
    }

    printf("Enter Maximum Resources required: \n");
    for(i=0;i<n;i++){
        printf("   p%d: ",i+1);
        for(j=0;j<m;j++){
            scanf(" %d",&Max[i][j]);
        }
    }

    printf("Enter Resources allocated: \n");
    for(i=0;i<n;i++){
        printf("   p%d: ",i+1);
        for(j=0;j<m;j++){
            scanf(" %d",&Alloc[i][j]);
        }
    }

    for(i=0;i<n;i++){
        for(j=0;j<m;j++){
            Need[i][j]=Max[i][j]-Alloc[i][j];
        }
        Finish[i]=0;
    }

    /*printf("\nAvailable Resources: ");
    for(i=0;i<m;i++) {
        printf("%d ", Avail[i]);
    }

    printf("\nMax Matrix:\n");
    printMatrix(Max, n, m);

    printf("\nAllocation Matrix:\n");
    printMatrix(Alloc, n, m);*/

    printf("\nNeed Matrix:\n");
    printMatrix(Need, n, m);

    z=0;
    for(k=0;k<n;k++){
        for(i=0;i<n;i++){
            if(!Finish[i] && isArrayLess(Need[i], Work, m)){
                Finish[i]=1;
                safeSeq[z++]=i+1;
                for(j=0;j<m;j++){
                    Work[j]+=Alloc[i][j];
                }
            }
        }
    }

    for(i=0;i<n;i++){
        if(!Finish[i]){
            isSafe=0;
        }
    }

    if(!isSafe)
        printf("\nThe processes are not safe, will cause DeadLock.\n");
    else{
        printf("\nThe processes are safe.\nSafe Sequence: ");
        printf("P%d ",safeSeq[0]);
        for(i=1;i<n;i++)
            printf("-> P%d ",safeSeq[i]);
        printf("\n");
    }

    return 0;
}