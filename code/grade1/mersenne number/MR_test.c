#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define Max 400
#define Utmost 200

void printnum(int a[], int max)
//输出高精度数a
{
    for (int i = max - 1; i >= 0; i--)
    {
        if (a[i] != 0) //跳过高位多余的0
        {
            for (int j = i; j >= 0; j--)
            {
                printf("%d", a[j]);
            }
            break;
        }
        else if (i == 0)
        {
            printf("0");
        }
    }
    printf("\n");
}

// void printnumtest()
// {
//     int a[Max] = {0, 7, 6, 1, 3, 0, 4, 2, 5, 0, 9, 8, 0, 0, 0};
//     printnum(a, Max);
// }

int real_len(int a[], int alen)
//返回高精度数a的实际位数
{
    for (int k = alen - 1; k >= 0; k--)
    {
        if (a[k] != 0)
        {
            return (k + 1);
        }
    }
    return 1; //全为零时，位数为1
}

// void real_lentest()
// {
//     int a[Max] = {0, 7, 6, 1, 3, 0, 4, 2, 5, 0, 9, 8, 0, 0, 0};
//     if (real_len(a, Max) == 12)
//     {
//         printf("ture");
//     }
// }

void prime_select(int a, int b, int prime[])
//筛选[a,b]区间的素数存放入数组prime中
{
    int k = 0;
    for (int i = a; i <= b; i++)
    {
        int flag;
        flag = 1;
        if (i == 0 || i == 1)
            continue;
        for (int j = 2; j * j <= i; j++) //素数判断
        {
            if (i % j == 0)
            {
                flag = 0;
                break;
            }
        }
        if (flag == 1)
        {
            prime[k] = i;
            k++;
        }
    }
}

// void prime_select_test()
// {
//     int prime[Max] = {0};
//     prime_select(5, 999, prime);
//     for (int i = 0; i < real_len(prime, Max); i++)
//     {
//         printf("%d", prime[i]);
//         printf("\n");
//     }
// }

void transfer(int a, int b[])
//整型a转为高精度b
{
    int k = 0;
    while (a != 0)
    {
        b[k] = a % 10;
        k++;
        a /= 10;
    }
}

// void transfertest()
// {
//     int a = 123456789;
//     int A[Max] = {0};
//     transfer(a, A);
//     printnum(A, Max);
// }

int compare(int a[], int alen, int b[], int blen)
//比较高精度数a与b的大小，a>b则返回1，alen与blen要求是实际长度
{
    if (alen < blen) //数量级不同时直接判断
        return -1;
    else if (alen > blen)
        return 1;
    else //从最高位逐位判断
    {
        for (int i = alen - 1; i >= 0; i--)
        {
            if (a[i] == b[i])
            {
                if (i == 0)
                    return 0;
                else
                    continue;
            }
            else if (a[i] > b[i])
                return 1;
            else if (a[i] < b[i])
                return -1;
        }
    }
}

// int comparetest()
// {
//     int a[Max] = {0, 7, 6, 1, 3, 0, 4, 2, 5, 0, 9, 8, 0, 0, 0};
//     int b[Max] = {0, 7, 6, 1, 3, 0, 4, 2, 5, 0, 9, 8, 0, 0, 0};
//     if (compare(a, 15, b, 15) == 0)
//     {
//         printf("ture");
//     }
// }

void multiply(int a[], int alen, int b[], int blen, int c[])
//高精度乘法，a*b=c， alen和blen必须是实际长度
{
    //逐位运算，没有处理进位
    for (int i = 0; i < blen; i++)
    {
        for (int j = 0; j < alen; j++)
        {
            c[i + j] += a[j] * b[i];
        }
    }
    //处理进位
    for (int i = 0; i < alen + blen; i++)
    {
        c[i + 1] += c[i] / 10;
        c[i] %= 10;
    }
}

// void multiplytest()
// {
//     int a[Max] = {0, 7, 6, 1, 3, 0, 4, 2, 5, 0, 9, 8, 0, 0, 0};
//     int b[Max] = {1, 2, 0, 3};
//     int c[Max] = {0};
//     multiply(a, 15, b, 4, c);
//     printnum(c, Max);
// }

void substract(int a[], int alen, int b[], int c[])
//高精度减低精度，a - b = c，alen必须是实际长度，且b数组至少要和a数组同等大小
{
    for (int i = 0; i < alen; i++)
    {
        c[i] += a[i] - b[i];
        if (c[i] < 0) //处理退位
        {
            c[i] += 10;
            c[i + 1]--;
        }
    }
}

// void substracttest()
// {
//     int a[Max] = {0, 7, 6, 1, 3, 0, 4, 2, 5, 0, 9, 8, 0, 0, 0};
//     int b[Max] = {1, 2, 0, 3};
//     int c[Max] = {0};
//     substract(a, 15, b, c);
//     printnum(c, Max);
// }

void division(int a[], int alen, int b[])
//将高精度数除以2,用于递归快速幂的迭代和二次探测，alen要求是实际长度
{
    int *A = (int *)calloc(Max, sizeof(int));
    for (int i = 0; i < alen; i++)
    {
        A[i] = a[i];
    }
    for (int i = alen - 1; i >= 0; i--)
    {
        if (A[i] % 2 == 0)
        {
            b[i] = A[i] / 2;
        }
        else
        {
            b[i] = A[i] / 2;
            A[i - 1] += A[i] % 2 * 10;
        }
    }
    free(A);
}

// void divisiontest()
// {
//     int a[Max] = {0, 7, 6, 1, 3, 0, 4, 2, 5, 0, 9, 8, 0, 0, 0};
//     int c[Max] = {0};
//     division(a, 12, c);
//     printnum(c, Max);
// }

void mod(int a[], int alen, int b[], int blen, int c[])
//高精度取模
{
    alen = real_len(a, alen);
    blen = real_len(b, blen);
    if (compare(a, alen, b, blen) == -1) //递归出口，得到余数
    {
        for (int i = 0; i < alen; i++)
        {
            c[i] = a[i];
        }
        return;
    }

    int t;
    int B[Max] = {0};
    int Blen = 0;
    t = alen - blen;
    for (int i = alen - 1; i >= 0; i--) //将除数扩大10^t倍
    {
        if (i >= t)
            B[i] = b[i - t];
        else
            B[i] = 0;
    }
    Blen = alen;
    if (compare(a, alen, B, Blen) == -1) //扩大过头了，后退一位
    {
        for (int i = 0; i <= Blen - 2; i++)
        {
            B[i] = B[i + 1];
        }
        B[Blen - 1] = 0;
        Blen--;
    }
    t = 0;

    int temp[Max] = {0};
    substract(a, alen, B, temp); //减法求余
    int templen;
    templen = real_len(temp, Max);
    while (compare(temp, templen, B, Blen) != -1)
    {
        int temp_[Max] = {0};
        memset(temp_, 0, Max * sizeof(int));
        for (int i = 0; i < templen; i++) //转存temp的值至temp_
        {
            temp_[i] = temp[i];
        }
        memset(temp, 0, Max * sizeof(int));
        substract(temp_, real_len(temp_, Max), B, temp);
        templen = real_len(temp, Max);
    }
    mod(temp, templen, b, blen, c); //递归继续对b求余
}

// void modtest()
// {
//     int a[Max] = {0, 7, 6, 1, 3, 0, 4, 2, 5, 0, 9, 8, 0, 0, 0};
//     int b[Max] = {1, 2, 0, 3};
//     int c[Max] = {0};
//     mod(a, 15, b, 4, c);
//     printnum(c, Max);
// }

void qpow(int a[], int alen, int b[], int blen, int c[])
//递归快速幂，a^b = c
{
    alen = real_len(a, alen);
    blen = real_len(b, blen);
    if (blen == 1 && b[0] == 1) //递归出口，a^1 = a;
    {
        for (int i = 0; i < alen; i++)
        {
            c[i] = a[i];
        }
        return;
    }
    int *x = (int *)calloc(Max, sizeof(int));
    x[0] = 2;
    int *temp = (int *)calloc(Max, sizeof(int));
    mod(b, blen, x, 1, temp);
    //如果b是奇数，拆分成b-1和b
    if (real_len(temp, Max) == 1 && temp[0] == 1)
    {
        int *B = (int *)calloc(Max, sizeof(int));
        int Blen;
        x[0] = 1;
        substract(b, blen, x, B);
        Blen = real_len(B, blen);
        memset(temp, 0, Max * sizeof(int));
        qpow(a, alen, B, Blen, temp);
        free(B);
        multiply(temp, real_len(temp, Max), a, alen, c);
        free(x);
        free(temp);
        return;
    }
    //如果b是偶数，拆分成b/2和b/2
    else
    {
        free(x);
        int *temp_ = (int *)calloc(Max, sizeof(int));
        division(b, blen, temp_);
        memset(temp, 0, Max * sizeof(int));
        qpow(a, alen, temp_, real_len(temp_, Max), temp);
        free(temp_);
        int templen = real_len(temp, Max);
        multiply(temp, templen, temp, templen, c);
        free(temp);
        return;
    }
}

// void qpowtest()
// {
//     int a[Max] = {7};
//     int b[Max] = {1, 2};
//     int c[Max] = {0};
//     qpow(a, Max, b, Max, c);
//     printnum(c, Max);
// }

void MODplus(int a[], int alen, int b[], int blen, int c[], int clen, int d[])
// a ^ b = d (mod c)        mod函数和qpow函数的结合体，用于迭代计算高次幂的取余
{
    alen = real_len(a, alen);
    blen = real_len(b, blen);
    clen = real_len(c, clen);
    if (blen == 1 && b[0] == 1)
    {
        for (int i = 0; i < alen; i++)
        {
            mod(a, alen, c, clen, d);
        }
        return;
    }
    int x[Max] = {2};
    int temp[Max] = {0};
    mod(b, blen, x, 1, temp);
    if (real_len(temp, Max) == 1 && temp[0] == 1)
    {
        int B[Max] = {0};
        int Blen;
        x[0] = 1;
        substract(b, blen, x, B);
        Blen = real_len(B, blen);
        memset(x, 0, Max * sizeof(int));
        memset(temp, 0, Max * sizeof(int));
        MODplus(a, alen, B, Blen, c, clen, x);
        multiply(x, real_len(x, Max), a, alen, temp);
        mod(temp, real_len(temp, Max), c, clen, d);
        return;
    }
    else
    {
        int B[Max] = {0};
        division(b, blen, B);
        int Blen = real_len(B, Max);
        memset(x, 0, Max * sizeof(int));
        memset(temp, 0, sizeof(temp));
        MODplus(a, alen, B, Blen, c, clen, x);
        int xlen = real_len(x, Max);
        multiply(x, xlen, x, xlen, temp);
        mod(temp, real_len(temp, Max), c, clen, d);
        return;
    }
}

// void MODplustest()
// {
//     int a[Max] = {7};
//     int b[Max] = {1, 2};
//     int d[Max] = {6, 6, 6};
//     int c[Max] = {0};
//     MODplus(a, Max, b, Max, d, Max, c);
//     printnum(c, Max);
// }

int MR_judge(int x[], int xlen)
//米勒罗宾算法判断x（x ≠ 2且x ≠ 1）是否是素数
{
    int con[Max] = {0};
    con[0] = 1;
    int Temp[Max] = {0};
    int remain[Max] = {0};
    int A[2];
    substract(x, xlen, con, Temp);
    int a[] = {2, 5, 11, 13, 17, 19};
    //费马小定理初筛
    for (int i = 0; i < 6; i++) //测试次数
    {
        A[0] = a[i] % 10;
        A[1] = a[i] / 10;
        MODplus(A, Max, Temp, Max, x, xlen, remain);
        if (!(real_len(remain, Max) == 1 && remain[0] == 1))
        {
            return 0;
        }
        memset(remain, 0, Max * sizeof(int));
        memset(A, 0, 2 * sizeof(int));
        //二次探测再筛
        memset(con, 0, Max * sizeof(int)); //数组重复利用
        memset(remain, 0, Max * sizeof(int));
        division(Temp, real_len(Temp, Max), con);
        int Con_[Max] = {0};
        A[0] = 2;
        int *temp = (int *)calloc(Max, sizeof(int));
        while (1)
        {
            memset(temp, 0, Max * sizeof(int));
            mod(con, real_len(con, Max), A, 1, temp);
            if (real_len(temp, Max) == 1 && temp[0] == 1) // con变为奇数，不能再探索了
            {
                break;
            }
            MODplus(A, Max, con, Max, x, xlen, remain);
            if (!(real_len(remain, Max) == 1 && remain[0] == 1))
            {
                if (compare(remain, real_len(remain, Max), Temp, real_len(Temp, Max)) == 0)
                    continue;
                return 0;
            }
            memset(Con_, 0, Max * sizeof(int));
            for (int i = 0; i < Max; i++)
            {
                Con_[i] = con[i];
            }
            division(Con_, real_len(Con_, Max), con);
        }
    }
    return 1;
}

void MR_judgetest()
{
    int x[Max] = {1, 3};
    if (MR_judge(x, 2) == 1)
    {
        printf("ture");
    }
}

int main()
{
    // printnumtest();
    // prime_select_test();
    // real_lentest();
    // transfertest();
    // comparetest();
    // multiplytest();
    // substracttest();
    // divisiontest();
    // modtest();
    // qpowtest();
    // MODplustest();
    MR_judgetest();
    system("pause");
}