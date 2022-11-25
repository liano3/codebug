#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma GCC optimize(3)
#define MAX 50000001

int n;                       //皇后个数
int board[MAX];              //模拟棋盘
int right_diag[2 * MAX - 1]; // 主对角线上的皇后个数
int left_diag[2 * MAX - 1];  // 反对角线上的皇后个数

int conflict_num() //计算冲突数
{
    int ans = 0;
    memset(right_diag, 0, sizeof(int) * (2 * n - 1));
    memset(left_diag, 0, sizeof(int) * (2 * n - 1));
    for (int i = 0; i < n; i++)
    {
        right_diag[i - board[i] + n - 1]++;
        left_diag[i + board[i]]++;
    }
    for (int i = 0; i < 2 * n - 1; i++)
    {
        ans += right_diag[i] > 1 ? right_diag[i] - 1 : 0;
        ans += left_diag[i] > 1 ? left_diag[i] - 1 : 0;
    }
    return ans;
}

int main()
{
    FILE *fp = fopen("solutions.txt", "r");
    fscanf(fp, "%d", &n);
    for (int i = 0; i < n; i++)
        fscanf(fp, "%d", &board[i]);
    int sum = conflict_num();
    if(sum == 0)
        printf("True\n");
    else 
        printf("False\n");
    fclose(fp);
    system("pause");
    return 0;
}