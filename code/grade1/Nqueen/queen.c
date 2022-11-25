#include <stdio.h>
#include <stdlib.h>
#pragma GCC optimize(3)
int a[300], sum = 0;
int ans[300];

//判断当前放置方法是否合法
int check(int x)
{
    for (int i = 1; i <= x - 1; i++)
    {
        for (int j = i + 1; j <= x; j++)
        {
            if ((a[i] == a[j]) ||
                (a[i] - i == a[j] - j) ||
                (a[i] + i == a[j] + j))
            {
                return 0;
            }
        }
    }
    return 1;
}

//放置第 x 行的皇后
void setqueen(int x, int n, FILE *fp)
{
    //退出递归条件:放置完成
    if(sum == 1)
        return;
    if (x == n)
    {
        for (int i = 1; i <= n; i++)
        {
            a[x] = i;
            if (check(x))
            {
                // 储存当前解
                for (int j = 1; j <= n; j++)
                {
                    ans[j] = 0;
                    ans[j] = a[j];
                }
                for (int i = 1; i <= n; i++)
                {
                    fprintf(fp, "%d ", ans[i]);
                }
                fprintf(fp, "\n");
                sum++;
            }
        }
    }
    else
    {
        for (int i = 1; i <= n; i++)
        {
            a[x] = i;
            if (check(x))
            {
                //递归放置第 x+1 行的皇后
                setqueen(x + 1, n, fp);
            }
        }
    }
    return;
}

int main()
{
    int n;
    printf("Please enter the number of Queens: ");
    scanf("%d", &n);
    //放置皇后
    FILE *fp = fopen("details.txt", "w");
    fprintf(fp, "%d\n", n);
    setqueen(1, n, fp);
    fclose(fp);
    //输出结果
    printf("\nThe number of solutions: %d\n\n", sum);
    printf("More information in details.txt\n");
    system("pause");
    return 0;
}
