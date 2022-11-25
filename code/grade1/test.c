#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 高精度数加法
void add(char *a, char *b, char *c)
{
    int alen = strlen(a);
    int blen = strlen(b);
    if (alen >= blen)
    {
        for (int i = alen - 1; i >= alen - blen; i--)
        {
            c[i + 1] = a[i] - '0' + b[i - (alen - blen)];
        }
        for (int i = alen - blen - 1; i >= 0; i--)
        {
            c[i + 1] = a[i];
        }
        for (int i = alen; i >= 1; i--)
        {
            if (c[i] > '9')
            {
                if (c[i - 1] == 0)
                    c[i - 1] += (c[i] - '0') / 10 + '0';
                else
                    c[i - 1] += (c[i] - '0') / 10;
                c[i] = (c[i] - '0') % 10 + '0';
            }
        }
    }
    else
    {
        for (int i = blen - 1; i >= blen - alen; i--)
        {
            c[i + 1] = b[i] - '0' + a[i - (blen - alen)];
        }
        for (int i = blen - alen - 1; i >= 0; i--)
        {
            c[i + 1] = b[i];
        }
        for (int i = blen; i >= 1; i--)
        {
            if (c[i] > '9')
            {
                if (c[i - 1] == 0)
                    c[i - 1] += (c[i] - '0') / 10 + '0';
                else
                    c[i - 1] += (c[i] - '0') / 10;
                c[i] = (c[i] - '0') % 10 + '0';
            }
        }
        if (c[0] == 0)
            c[0] = '0';
    }
}

// 高精度减法
void substract(char *a, char *b, char *c)
{
    int alen = strlen(a);
    int blen = strlen(b);
    for (int i = alen - 1; i >= alen - blen; i--)
    {
        c[i] = a[i] - b[i - alen + blen] + '0';
    }
    for (int i = alen - blen - 1; i >= 0; i--)
    {
        c[i] = a[i];
    }
    for (int i = alen - 1; i > 0; i--)
    {
        if (c[i] < '0')
        {
            c[i] += 10;
            c[i - 1]--;
        }
    }
}

// 八皇后
int check(int x, int y, int *ans, int n)
{
    for (int i = 1; i <= n; i++)
    {
        if (i == x || ans[i] == 0)
            continue;
        if (y == ans[i] || y + x == ans[i] + i || y - x == ans[i] - i)
            return 0;
    }
    return 1;
}
void setqueen(int x, int *ans, int n)
{
    if (x == n + 1)
    {
        for (int i = 1; i <= n; i++)
        {
            printf("%d ", ans[i]);
        }
        printf("\n");
        return;
    }
    for (int i = 1; i <= n; i++)
    {
        if (check(x, i, ans, n))
        {
            ans[x] = i;
            setqueen(x + 1, ans, n);
            ans[x] = 0;
        }
    }
}

// 24点
void calcu(double *num, int run, double out, int *ans)
{
    if (run == 3 && out - 24 < 1e-6 && out - 24 > -(1e-6))
    {
        *ans = 1;
        return;
    }
    double x = 0, y = 0;
    for (int i = 0; i < 4; i++)
    {
        if (num[i] == 10000)
            continue;
        for (int j = 0; j < 4; j++)
        {
            if (i == j || num[j] == 10000)
                continue;
            x = num[i];
            y = num[j];
            int error = 0;
            for (int k = 0; k < 4; k++)
            {
                switch (k)
                {
                case 0:
                    out = x + y;
                    break;
                case 1:
                    out = x - y;
                    break;
                case 2:
                    out = x * y;
                    break;
                case 3:
                    if (y != 0)
                        out = x / y;
                    else
                        error++;
                }
                if (error == 0)
                {
                    num[j] = 10000;
                    num[i] = out;
                    calcu(num, run + 1, out, ans);
                    if (*ans == 1)
                        return;
                    num[i] = x;
                    num[j] = y;
                }
            }
        }
    }
}

// 旋转矩阵
int *spiralOrder(int **matrix, int matrixSize, int *matrixColSize, int *returnSize)
{
    int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
    int *ans = (int *)calloc(matrixSize * matrixColSize[0], sizeof(int));
    int mark[matrixSize][matrixColSize[0]];
    memset(mark, 0, sizeof(mark));
    int x = 0, y = 0, d = 1;
    for (int i = 0; i < matrixSize * matrixColSize[0]; i++)
    {
        ans[i] = matrix[x][y];
        mark[x][y] = 1;
        int a = x + dx[d], b = y + dy[d];
        if (a > matrixSize - 1 || b > matrixColSize[0] - 1 || a < 0 || b < 0 || mark[a][b])
        {
            d = (d + 1) % 4;
            a = x + dx[d];
            b = y + dy[d];
        }
        x = a;
        y = b;
    }
    *returnSize = matrixSize * matrixColSize[0];
    return ans;
}

int main()
{
    system("pause");
}