/*
常规表达式计算
◦ 输入为四则运算表达式，仅由数字、+、-、*、/ 、(、)组成，
    没有空格，要求求其值。
◦ 解题思路：常规表达式也有一个递归的定义，
    因此对于表达式可以进行递归分析处理
*/
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include <ctype.h>
                                       double factor_value();
double term_value();
double expression_value();
char curr_char;
void main()
{
    curr_char = getchar();
    double result = expression_value();
    printf("The result is %f\n", result);
    system("pause");
}
// 求一个表达式的值
double expression_value()
{
    double result = term_value(); // 求第一项的值
    int more = 1;
    while (more)
    {
        char op = curr_char;
        if (op == '+' || op == '-')
        {
            curr_char = getchar(); // 取下一个字符
            int value = term_value();
            if (op == '+')
                result += value;
            else
                result -= value;
        }
        else
            more = 0;
    }
    return result;
}
//求一个项的值
double term_value()
{
    double result = factor_value(); //求第一个因子的值
    int more = 1;
    while (more)
    {
        char op = curr_char;
        if (op == '*' || op == '/')
        {
            curr_char = getchar();
            int value = factor_value();
            if (op == '*')
                result *= value;
            else
                result /= value;
        }
        else
            more = 0;
    }
    return result;
}
//求一个因子的值
double factor_value()
{
    double result = 0;
    char c = curr_char;
    if (c == '(')
    {
        curr_char = getchar();
        result = expression_value();
        curr_char = getchar();
    }
    else
    {
        char num[64];
        int i = 0;
        while (isdigit(c) || c == '.')
        {
            num[i++] = c;
            c = curr_char = getchar();
        }
        result = atof(num);
    }
    return result;
}
