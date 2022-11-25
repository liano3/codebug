#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double num[4];                             //存放原始数字或中间结果
double origin[4], second[4], third[4];     //分别记录原数组，第一步得到的数组，第二步得到的数组，用于回溯
char sign[4];                              //记录选择的运算符
double figure[6];                          //记录选择的数字
int mark[6] = {0}, count = 0, record = -1; //辅助记录表达式
double out = 0;                            //中间运算结果
int run = 0;                               //运算次数
int flag = 0;                              //控制函数结束
int SUM = 0;

void print_solution() //分析表达式数据，输出表达式
{
    int debug;
    printf("Expression:\n");
    if (mark[3] == 0 && mark[2] == 0)
    //修正bug的补丁,如果第3,4个数没有用到之前的运算结果，则第5,6个一定全用到
    {
        if (mark[5] == 1)
            debug = 5; //用于判断第一次中间结果与第二次中间结果的前后
        else if (mark[4] == 1)
            debug = 4;
        mark[4] = 1;
        mark[5] = 1;
    }
    //下面就表达式形式按三种情况讨论
    if (mark[5] == 1 && mark[4] == 0)
    {
        printf("%d%c", (int)figure[4], sign[2]);
        if (mark[3] == 1)
        {
            printf("(%d%c(%d%c%d))", (int)figure[2], sign[1], (int)figure[0], sign[0], (int)figure[1]);
        }
        else if (mark[2] == 1)
        {
            printf("((%d%c%d)%c%d)", (int)figure[0], sign[0], (int)figure[1], sign[1], (int)figure[3]);
        }
    }
    else if (mark[5] == 0 && mark[4] == 1)
    {
        if (mark[3] == 1)
        {
            printf("(%d%c(%d%c%d))", (int)figure[2], sign[1], (int)figure[0], sign[0], (int)figure[1]);
        }
        else if (mark[2] == 1)
        {
            printf("((%d%c%d)%c%d)", (int)figure[0], sign[0], (int)figure[1], sign[1], (int)figure[3]);
        }
        printf("%c%d", sign[2], (int)figure[5]);
    }
    else
    {
        if (debug == 5)
        {
            printf("(%d%c%d)%c(%d%c%d)", (int)figure[0], sign[0], (int)figure[1], sign[2], (int)figure[2], sign[1], (int)figure[3]);
        }
        else if (debug == 4)
        {
            printf("(%d%c%d)%c(%d%c%d)", (int)figure[2], sign[1], (int)figure[3], sign[2], (int)figure[0], sign[0], (int)figure[1]);
        }
    }
    printf("\n");
}

void calcu() //判断是否能够计算出24，并记录表达式数据
{
    if (flag == 1) //得到一个解即可退出
        return;
    if (run == 3) //递归出口
    {
        if (out - 24 < 1e-6 && out - 24 > -(1e-6))
        {
            // printf("The result is:\ntrue\n");
            flag++;
            SUM++;
            // system("pause");
            // print_solution();
        }
        return;
    }

    if (run == 1) //记录原状态
    {
        for (int i = 0; i < 4; i++)
        {
            second[i] = num[i];
        }
    }
    if (run == 2) //记录原状态
    {
        for (int i = 0; i < 4; i++)
        {
            third[i] = num[i];
        }
    }

    double x = 0, y = 0;
    double z = out, record_ = record; //记录原状态
    int error = 0;                    //考虑分母不能为0

    for (int i = 0; i < 4; i++) //穷举所有两个不同数的组合
    {
        if (num[i] == 10000) //标记用过的
            continue;
        for (int j = 0; j < 4; j++)
        {
            if (i == j || num[j] == 10000)
                continue;
            x = num[i];
            y = num[j];
            if (i == record)
                mark[count] = 1; //标记计算中用到了中间结果
            figure[count++] = x;
            if (j == record)
                mark[count] = 1;
            figure[count++] = y;
            for (int k = 0; k < 4; k++) //穷举四则运算
            {
                switch (k)
                {
                case 0:
                    out = x + y;
                    sign[run] = '+';
                    break;
                case 1:
                    out = x - y;
                    sign[run] = '-';
                    break;
                case 2:
                    out = x * y;
                    sign[run] = '*';
                    break;
                case 3:
                    if (y != 0)
                    {
                        out = x / y;
                        sign[run] = '/';
                    }
                    else
                        error++;
                }
                if (error == 0)
                {
                    num[j] = 10000;
                    run++;
                    num[i] = out; //将中间结果放入数组，参与后续运算
                    record = i;
                    calcu();
                    if (flag == 1)
                        return;
                    //回溯
                    record = record_;
                    out = z;
                    run--;
                }
            }
            mark[count - 1] = 0;
            mark[count - 2] = 0;
            count -= 2;
            if (run == 0) //回到原状态
            {
                for (int m = 0; m < 4; m++)
                {
                    num[m] = origin[m];
                }
            }
            if (run == 1) //回到原状态
            {
                for (int m = 0; m < 4; m++)
                {
                    num[m] = second[m];
                }
            }
            if (run == 2) //回到原状态
            {
                for (int m = 0; m < 4; m++)
                {
                    num[m] = third[m];
                }
            }
        }
    }
}

int main()
{
    for (int i = 1; i <= 20; i++)
    {
        for (int j = 1; j <= 20; j++)
        {
            for (int k = 1; k <= 20; k++)
            {
                for (int w = 1; w <= 20; w++)
                {
                    num[0] = i;
                    num[1] = j;
                    num[2] = k;
                    num[3] = w;
                    for (int i = 0; i < 4; i++)
                    {
                        origin[i] = num[i];
                    }
                    calcu();
                    memset(mark, 0, sizeof(int) * 6);
                    record = -1;
                    count = 0;
                    run = 0;
                    flag = 0;
                }
            }
        }
    }
    printf("SUM = %d\n", SUM);
    system("pause");
    return 0;
}