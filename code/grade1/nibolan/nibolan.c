#include <stdio.h>
#include <stdlib.h>
#include <string.h>
double exp()      //逆波兰表达式的求解
{
    char a[10];
    scanf("%s", a);
    switch (a[0])
    {
    case '+':
        return exp() + exp();
    case '-':
        return exp() - exp();
    case '*':
        return exp() * exp();
    case '/':
        return exp() / exp();
    default:
        return atof(a); // 字符串转浮点数
    }
}
void exp_()     //逆波兰表达式化为常规表达式
{
    char a[10];
    scanf("%s", a);
    switch (a[0])
    {
    case '+':
        printf("(");
        exp_();
        printf(")");
        printf("+");
        printf("(");
        exp_();
        printf(")");
        break;
    case '-':
        printf("(");
        exp_();
        printf(")");
        printf("-");
        printf("(");
        exp_();
        printf(")");
        break;
    case '*':
        printf("(");
        exp_();
        printf(")");
        printf("*");
        printf("(");
        exp_();
        printf(")");
        break;
    case '/':
        printf("(");
        exp_();
        printf(")");
        printf("/");
        printf("(");
        exp_();
        printf(")");
        break;
    default:
        printf("%s", a);
        return;
    }
}

int main()
{
    // double ans;
    // ans = exp();
    // printf("%lf", ans);

    // exp_();

    system("pause");
    return 0;
}