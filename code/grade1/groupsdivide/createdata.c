/*
数据格式：学号\t性别\t是否是大佬\n
学号：三位数字表示
性别：M(男)，F(女)
是否是大佬：1(是)，0(不是)
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 200
int main()
{
    FILE *fp;
    fp = fopen("information.txt", "w");

    srand((unsigned)time(NULL));
    for (int i = 0; i < N; i++)
    {
        fprintf(fp, "%03d\t", i + 1);
        if (rand() % 2 ==0)
            fprintf(fp, "M\t");
        else 
            fprintf(fp, "F\t");
        if (i % 10 == 0)
            fprintf(fp, "1\n");
        else 
            fprintf(fp, "0\n");
    }
    
    printf("Data is ready.\n");
    system("pause");
    return 0;
}