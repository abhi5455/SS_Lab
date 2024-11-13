#include <stdio.h>
#include <stdlib.h>

int n,r[25],size,initial,traversal;

void fcfs()
{
    int i,seek=0,current=initial;
    printf("Disk Traversal Order : ");
    for(i=0;i<n;i++)
    {
        seek+=abs(current-r[i]);
        current=r[i];
        printf("%d ",r[i]);
    }
    printf("\nSeek Time : %d\n",seek);
}

void scan()
{
    int i,j,temp,current=initial,seek=0,index;
    printf("Disk Traversal Order : ");
    if(traversal == 1)
    {
        for(i=0;i<n;i++)
        {
            for(j=0;j<n-i-1;j++)
            {
                if(r[j] > r[j+1])
                {
                    temp=r[j];
                    r[j]=r[j+1];
                    r[j+1]=temp;
                }
            }
        }
        for(i=0;i<n;i++)
        {
            if(current < r[i])
            {
                index=i;
                break;
            }
        }
        for(i=index;i<n;i++)
        {
            seek+=abs(r[i]-current);
            current=r[i];
            printf("%d ",r[i]);
        }
        seek+=abs(size-1-current);
        current=size-1;
        for(i=index-1;i>=0;i--)
        {
            seek+=abs(current-r[i]);
            current=r[i];
            printf("%d ",r[i]);
        }
        printf("\nSeek Time : %d\n",seek);
    }
    else if(traversal == 2)
    {
        for(i=0;i<n;i++)
        {
            for(j=0;j<n-i-1;j++)
            {
                if(r[j] < r[j+1])
                {
                    temp=r[j];
                    r[j]=r[j+1];
                    r[j+1]=temp;
                }
            }
        }
        for(i=0;i<n;i++)
        {
            if(current > r[i])
            {
                index=i;
                break;
            }
        }
        for(i=index;i<n;i++)
        {
            seek+=abs(current-r[i]);
            current=r[i];
            printf("%d ",r[i]);
        }
        seek+=abs(current-0);
        current=0;
        for(i=index-1;i>=0;i--)
        {
            seek+=abs(r[i]-current);
            current=r[i];
            printf("%d ",r[i]);
        }
        printf("\nSeek Time : %d\n",seek);
    }
}

void cscan()
{
    int i,j,temp,current=initial,seek=0,index;
    printf("Disk Traversal Order : ");
    if(traversal == 1)
    {
        for(i=0;i<n;i++)
        {
            for(j=0;j<n-i-1;j++)
            {
                if(r[j] > r[j+1])
                {
                    temp=r[j];
                    r[j]=r[j+1];
                    r[j+1]=temp;
                }
            }
        }
        for(i=0;i<n;i++)
        {
            if(current < r[i])
            {
                index=i;
                break;
            }
        }
        for(i=index;i<n;i++)
        {
            seek+=abs(r[i]-current);
            current=r[i];
            printf("%d ",r[i]);
        }
        seek+=abs(size-1-current);
        current=size-1;
        seek+=abs(current-0);
        current=0;
        for(i=0;i<index;i++)
        {
            seek+=abs(current-r[i]);
            current=r[i];
            printf("%d ",r[i]);
        }
        printf("\nSeek Time : %d\n",seek);
    }
    else if(traversal == 2)
    {
        for(i=0;i<n;i++)
        {
            for(j=0;j<n-i-1;j++)
            {
                if(r[j] < r[j+1])
                {
                    temp=r[j];
                    r[j]=r[j+1];
                    r[j+1]=temp;
                }
            }
        }
        for(i=0;i<n;i++)
        {
            if(current > r[i])
            {
                index=i;
                break;
            }
        }
        for(i=index;i<n;i++)
        {
            seek+=abs(current-r[i]);
            current=r[i];
            printf("%d ",r[i]);
        }
        seek+=abs(current-0);
        current=0;
        seek+=abs(size-1-current);
        current=size-1;
        for(i=0;i<index;i++)
        {
            seek+=abs(r[i]-current);
            current=r[i];
            printf("%d ",r[i]);
        }
        printf("\nSeek Time : %d\n",seek);
    }
}

int main()
{
    int i,value;
    char ch='y';
    while(  ch=='y' || ch=='Y')
    {
        printf("1 : FCFS\n2 : SCAN\n3 : C-SCAN\n4 : EXIT\n");
        printf("Enter your value : ");
        scanf(" %d",&value);
        if(value==4)
        {
            break;
        }
        printf("Enter the size of disk : ");
        scanf("%d",&size);
        printf("How many requests ? : ");
        scanf("%d",&n);
        printf("Enter the request sequence : ");
        for(i=0;i<n;i++)
        {
            scanf("%d",&r[i]);
        }
        printf("Enter the initial position of header : ");
        scanf("%d",&initial);
        if(value==2 || value==3)
        {
            printf("1 : Increasing order traversal\n2 : Decreasing order traversal\n");
            printf("Enter your choice  : ");
            scanf(" %d",&traversal);
        }
        switch(value)
        {
            case 1 : fcfs();
                break;
            case 2 : scan();
                break;
            case 3 : cscan();
                break;
        }
        printf("Do you want to continue? (y/n): ");
        scanf(" %c",&ch);
    }
}