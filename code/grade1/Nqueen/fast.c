#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#pragma GCC optimize(3)
#define LL long long

int num;
FILE* fp;

void printbyoneBit(LL *a, int len) //输出函数，用于输出结果
{
    for (int i = 0; i < len; i++)
    {
        //将二进制数a[i]中的1的序号输出，即皇后所在列序号
        fprintf(fp, "%d ", (int)(log(a[i]) / log(2)) + 1);
    }
    fprintf(fp, "\n");
}

void settlequeen(int n, LL col, LL slash, LL backslash, LL *cols, int k)
{
    if(num == 1)
        return;
    if (k >= n)
    {
        //递归出口,n个皇后全部放置完毕
        num++;
        printbyoneBit(cols, n);
        return;
    }
    //找到该行可放的位置， currentState = 0 代表全部位置都不能放
    LL currentState = (~(col | slash | backslash)) & (((LL)1 << n) - 1);
    while (currentState > 0)
    {
        //从可以放的位置中选择最后一个放置
        LL currentCol = currentState & -currentState;
        cols[k] = currentCol;
        //更新占用状态并放置下一行
        settlequeen(n, col | currentCol, (slash | currentCol) << 1, (backslash | currentCol) >> 1, cols, k + 1);
        //回溯
        cols[k] = 0;
        currentState &= currentState - 1;
    }
}

int main()
{
    clock_t start, end;
    num = 0;
    fp = fopen("solutions.txt", "w");
    int N;
    printf("Please enter the number of queens : ");
    scanf("%d", &N);
    start = clock();
    fprintf(fp, "%d\n", N);
    LL *results = (LL *)calloc(N, sizeof(LL));
    settlequeen(N, 0, 0, 0, results, 0);
    end = clock();
    printf("\nRun time : %lf (s)\n", (double)(end - start) / CLK_TCK);
    free(results);
    fclose(fp);
    system("pause");
    return 0;
}