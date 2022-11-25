// code Format GB2312
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define n 210   //最大精度，即小数部分最大位数。
#define Max 200 //整数部分最大位数
#define MX 20   //多项式最项数

typedef struct
{
    int neg;           //符号位，0为正，1为负
    int inte[Max + 1]; //整数部分，第一位存实际长度
    int deci[n + 1];   //小数部分，第一位存实际长度
    int base;          //进制类型
} num;

//结构体初始化
num *InitNum()
{
    //分配空间
    num *tmp = (num *)malloc(sizeof(num));
    if (!tmp)
    {
        printf("分配空间失败！");
        system("pause");
        exit(0);
    }
    //初始化
    tmp->base = 0;
    tmp->neg = 0;
    memset(tmp->inte, 0, sizeof(int) * (Max + 1));
    memset(tmp->deci, 0, sizeof(int) * (n + 1));
    //返回结构体指针
    return tmp;
}

//打印函数
void PrintNum(num *data)
{
    printf("%d进制：", data->base);
    //特判0,输出0即可
    if (data->inte[0] == 0 && data->deci[0] == 0)
    {
        printf("0\n");
        return;
    }
    //打印负号
    if (data->neg == 1)
        printf("-");
    //纯小数
    if (data->inte[0] == 0)
        printf("0");
    //打印整数部分
    for (int i = 1; i <= data->inte[0]; i++)
    {
        //超出9用小写字母表述
        if (data->inte[i] >= 10)
            printf("%c", data->inte[i] - 10 + 'a');
        else
            printf("%d", data->inte[i]);
    }
    //打印小数部分，同整数
    //去除尾部多余的0
    int temp = data->deci[0];
    while(data->deci[temp] == 0 && temp > 0)
        temp--;
    data->deci[0] = temp;
    //打印小数点
    if (data->deci[0] > 0)
        printf(".");
    //打印
    //多余的精度直接舍弃
    int end = data->deci[0];
    if (end > 200)
        end = 200;
    for (int i = 1; i <= end; i++)
    {
        if (data->deci[i] >= 10)
            printf("%c", data->deci[i] - 10 + 'a');
        else
            printf("%d", data->deci[i]);
    }
    printf("\n");
}

//读入二进制或十进制数据，转为num类型存储
num *ReadStr(int N, char *data)
{
    num *ans = InitNum(); //存放转换结果
    int pos;              //指向data
    int i;                //指向ans
    if (N != 2 && N != 10)
    {
        printf("只能读入二进制或十进制数哦！");
        system("pause");
        exit(0);
    }
    //进制
    ans->base = N;
    //判断负数
    if (data[0] == '-')
    {
        ans->neg = 1;
        pos = 1; //指针后移一位
    }
    //判断正数
    else if (data[0] == '+')
    {
        ans->neg = 0;
        pos = 1;
    }
    //考虑正号的缺省和纯小数的整数部分的缺省
    else if ((data[0] < N + '0' && data[0] >= '0') || data[0] == '.')
    {
        ans->neg = 0;
        pos = 0;
    }
    //否则为非法输入
    else
    {
        printf("请输入正确的数据！");
        system("pause");
        exit(0);
    }
    //往后一个个读入
    //整数部分
    i = 0;
    int flg = 0;
    while (pos < strlen(data) && data[pos] != '.')
    {
        //用于多项式解析，碰到x或+-号停止读入
        if (data[pos] == 'x' || data[pos] == '+' || data[pos] == '-')
        {
            //记录长度
            ans->inte[0] = i;
            ans->deci[0] = 0;
            return ans;
        }
        //判断输入合法性
        if (data[pos] >= N + '0' || data[pos] < '0')
        {
            printf("请输入正确的数据");
            system("pause");
            exit(0);
        }
        //去除头部多余的0
        if (data[pos] == '0' && flg == 0)
        {
            pos++;
            continue;
        }
        flg = 1; //用于判断0是否可去
        ans->inte[++i] = data[pos++] - '0';
    }
    //记录长度
    ans->inte[0] = i;
    pos++; //小数点占位，指针后移

    //小数部分
    i = 0; // 回到起点，读入小数部分
    while (pos < strlen(data))
    {
        //用于解析多项式
        if (data[pos] == 'x' || data[pos] == '+' || data[pos] == '-')
        {
            //去除尾部多余的0
            while (ans->deci[i] == 0 && i >= 0)
                i--;
            ans->deci[0] = i;
            return ans;
        }
        //判断合法性
        if (data[pos] >= N + '0' || data[pos] < '0')
        {
            printf("请输入正确的数据！");
            system("pause");
            exit(0);
        }
        ans->deci[++i] = data[pos++] - '0';
    }
    //去除尾部多余的0
    while (ans->deci[i] == 0 && i > 0)
        i--;
    ans->deci[0] = i;
    // PrintNum(ans);
    return ans;
}

//进制转换函数，实现任意进制互转，将data转为N进制
num *BaseConversion(num *data, int N)
{
    // temp存储转化结果
    num *temp = InitNum();
    int base = data->base;
    temp->base = N;
    temp->neg = data->neg; //符号位不变

    //整数部分
    //模拟手工进制转化
    //大循环是inte[]/N，取余，商再/N，取余，直到商为0；
    //因为数字很大，不能直接除，所以一位一位的除，商拼接去首0就是上面的商，余数*data->base+下一位数作为下一个被除数，继续/N，取最后一次的余数为上面的余数

    int div[data->inte[0] + 1]; //存储大循环中每一次的被除数
    //第一次的被除数就是inte[]本身
    for (int i = 0; i <= data->inte[0]; i++)
        div[i] = data->inte[i];
    //存储大循环中的商
    int ans[data->inte[0] + 1];
    memset(ans, 0, sizeof(ans));
    //存储大循环中的余数
    int res[Max];
    memset(res, 0, sizeof(res));
    //大循环
    while (div[0] >= 1)
    {
        int y = 0; //记录小循环中的余数，会不断更新
        int i = 1; //循环参数
        ans[0] = div[0];
        //小循环
        while (i <= div[0])
        {
            y = y * base + div[i]; //当前被除数
            ans[i++] = y / N;      //记录商
            y %= N;                //更新余数
        }
        //大循环中的一位余数就是当前最新的y
        res[++res[0]] = y;
        i = 1; //重复利用，去除下次被除数(即上次的商)的多余的0
        while (i <= ans[0] && ans[i] == 0)
            i++;
        //清空被除数，重新用上次的商赋值
        memset(div, 0, sizeof(div));
        for (int j = i; j <= ans[0]; j++)
            div[++div[0]] = ans[j];
        memset(ans, 0, sizeof(ans));
    }
    //逆序取余数即可得到转化后的整数部分
    temp->inte[0] = res[0];
    for (int i = 1; i <= temp->inte[0]; i++)
        temp->inte[i] = res[res[0]--];

    // 小数部分
    //模拟手工进制转化，逻辑上和整数部分类似
    //大循环是*N取整，剩下的继续*N取整，直到0，把取出的整数正序排列即可得到转化结果
    //小循环是一位一位的*N，/data->base进位，%data->base留下，拼接起来进入大循环的计算

    int multy[data->deci[0] + 1]; //存储大循环中每一次的被乘数(暂且这样称呼)
    //第一次的被乘数就是deci[]本身，需要逆序存储
    multy[0] = data->deci[0];
    int pos = 0;
    for (int i = data->deci[0]; i >= 1; i--)
        multy[++pos] = data->deci[i];
    //存储大循环中的乘积结果
    int ans_[data->deci[0] + 1];
    memset(ans_, 0, sizeof(ans_));
    //存储大循环中的取整结果
    int res_[n + 1];
    memset(res_, 0, sizeof(res_));
    //大循环
    while (multy[0] >= 1)
    {
        int y = 0; //记录小循环中的进位量，会不断更新
        int i = 1; //用来拼接小循环中每次的乘积结果
        //小循环
        while (i <= multy[0])
        {
            y += (multy[i] * N);  //当前乘积结果
            ans_[i++] = y % base; //记录当前保留的数
            y /= base;            //更新进位量
        }
        ans_[0] = i - 1;
        //取整，即取最后一次的进位量
        res_[0]++;
        //达到精度后退出大循环
        if (res_[0] > n)
        {
            res_[0]--;
            break;
        }
        res_[res_[0]] = y;
        //清空被乘数，重新用上次的乘积结果赋值
        int count = 0;
        memset(multy, 0, sizeof(multy));
        for (int j = 1; j <= ans_[0]; j++)
        {
            //检测被乘数是否为0
            if (ans_[j] == 0)
                count++;
            multy[++multy[0]] = ans_[j];
        }
        //被乘数为 0，退出大循环
        if (count == ans_[0])
            multy[0] = 0;
        count = 0;
        memset(ans_, 0, sizeof(ans_));
    }
    //正序取余数即可得到转化后的小数部分
    for (int i = 0; i <= res_[0]; i++)
        temp->deci[i] = res_[i];
    return temp;
}

//高精度比较器，返回|x|>|Y|
int compare(num *x, num *y)
{
    //位数不同直接比较
    if (x->inte[0] > y->inte[0])
        return 1;
    //位数相同向后比较
    else if (x->inte[0] == y->inte[0])
    {
        //比较整数部分
        for (int i = 1; i <= x->inte[0]; i++)
        {
            //出现不同就结束了
            if (x->inte[i] > y->inte[i])
                return 1;
            else if (x->inte[i] < y->inte[i])
                return 0;
        }
        //比较小数部分
        int low = (x->deci[0] > y->deci[0] ? x->deci[0] : y->deci[0]);
        for (int i = 1; i <= low; i++)
        {
            if (x->deci[i] > y->deci[i])
                return 1;
            else if (x->deci[i] < y->deci[i])
                return 0;
        }
        return 1;
    }
    else
        return 0;
}

//高精度加法器，严格规定x，y均为正数且x>y
num *adding_box(num *x, num *y)
{
    //存储结果
    num *ans = InitNum();
    int base = x->base;
    //进制不变
    ans->base = base;
    //符号为+
    ans->neg = 0;

    //小数部分

    //确定最后一位小数
    int low = (x->deci[0] >= y->deci[0] ? x->deci[0] : y->deci[0]);
    int pos = low; //指向当前操作位的指针
    int temp = 0;  //暂存当前进位和运算结果
    for (int i = low; i >= 1; i--)
    {
        //逐位相加，考虑进位
        temp += (x->deci[i] + y->deci[i]);
        ans->deci[pos--] = temp % base; //当前位保留的数
        temp /= base;                   //进位值
    }
    ans->deci[0] = low; //和的小数部分长度

    //整数部分

    //计算整数部分长度之差，小数点对齐，逐位相加，和小数部分类似
    int dis = x->inte[0] - y->inte[0];
    pos = x->inte[0];
    for (int i = x->inte[0]; i - dis >= 1; i--)
    {
        temp += (x->inte[i] + y->inte[i - dis]);
        ans->inte[pos--] = temp % base;
        temp /= base;
    }
    for (int i = dis; i >= 1; i--)
    {
        temp += x->inte[i];
        ans->inte[pos--] = temp % base;
        temp /= base;
    }
    ans->inte[0] = x->inte[0];
    //如果最后一位有进位，需要整数部分整体右移
    if (temp > 0)
    {
        for (int i = ans->inte[0]; i >= 1; i--)
            ans->inte[i + 1] = ans->inte[i];
        ans->inte[1] = temp;           //加上最后的进位
        ans->inte[0] = x->inte[0] + 1; //更新长度
    }
    else
        ans->inte[0] = x->inte[0];
    return ans;
}

//高精度减法器，严格规定x，y均为正数且x>y
num *sub_box(num *x, num *y)
{
    //和加法器类似
    num *ans = InitNum();
    int base = x->base;
    ans->base = base;
    ans->neg = 0;
    int low = (x->deci[0] > y->deci[0] ? x->deci[0] : y->deci[0]);
    int pos = low;
    int temp = 0; //存储当前借位值
    int a;        //存储当前位数值
    for (int i = low; i >= 1; i--)
    {
        a = x->deci[i] - temp;
        temp = 0;
        //借位
        if (a < y->deci[i])
        {
            temp = 1;
            a += base;
        }
        ans->deci[pos--] = a - y->deci[i];
    }
    //去除多余的0
    while (ans->deci[low] == 0 && low > 0)
        low--;
    ans->deci[0] = low;
    int dis = x->inte[0] - y->inte[0];
    pos = x->inte[0];
    for (int i = x->inte[0]; i - dis >= 1; i--)
    {
        a = x->inte[i] - temp;
        temp = 0;
        if (a < y->inte[i - dis])
        {
            temp = 1;
            a += base;
        }
        ans->inte[pos--] = a - y->inte[i - dis];
    }
    for (int i = dis; i >= 1; i--)
    {
        a = x->inte[i] - temp;
        temp = 0;
        if (a < 0)
        {
            temp = 1;
            a += base;
        }
        ans->inte[pos--] = a;
    }
    //去除多余的0
    int count = 0;
    while (ans->inte[++pos] == 0 && pos <= x->inte[0])
        count++;
    for (int i = pos; i <= x->inte[0]; i++)
        ans->inte[i - pos + 1] = ans->inte[i];
    ans->inte[0] = x->inte[0] - count;
    return ans;
}

//高精度加法函数，调用加法器和减法器，根据两个运算数的不同情况选择调用方式
num *Add(num *x, num *y)
{
    // x>0,y>0
    if (x->neg == 0 && y->neg == 0)
    {
        if (compare(x, y))
            return adding_box(x, y);
        else
            return adding_box(y, x);
    }
    // x<0,y<0
    else if (x->neg == 1 && y->neg == 1)
    {
        num *ans;
        if (compare(x, y))
            ans = adding_box(x, y);
        else
            ans = adding_box(y, x);
        ans->neg = 1;
        return ans;
    }
    // x>0,y<0
    else if (x->neg == 0 && y->neg == 1)
    {
        if (compare(x, y))
            return sub_box(x, y);
        else
        {
            num *ans = sub_box(y, x);
            ans->neg = 1;
            return ans;
        }
    }
    // x<0,y>0
    else
    {
        if (compare(x, y))
        {
            num *ans = sub_box(x, y);
            ans->neg = 1;
            return ans;
        }
        else
            return sub_box(y, x);
    }
}

//高精度减法函数，调用加法函数即可
num *Sub(num *x, num *y)
{
    // x - y = x + (-y)
    num *ans;
    if (y->neg == 0)
    {
        y->neg = 1;
        ans = Add(x, y);
        y->neg = 0;
        return ans;
    }
    else
    {
        y->neg = 0;
        ans = Add(x, y);
        y->neg = 1;
        return ans;
    }
}

//大整数乘法器，模拟手算，通过z[]返回，返回有效位起点(0或1)，第一个有效位位存储长度
int multy_box(int base, int x[], int y[], int z[])
{
    //按位计算即可
    for (int i = y[0]; i >= 1; i--)
    {
        for (int j = x[0]; j >= 1; j--)
        {
            z[i + j] += x[j] * y[i];
            z[i + j - 1] += z[i + j] / base;
            z[i + j] %= base;
        }
    }
    //返回有效位置与有效长度
    if (z[1] == 0)
    {
        z[1] = x[0] + y[0] - 1;
        return 1;
    }
    else
    {
        z[0] = x[0] + y[0];
        return 0;
    }
}

//高精度乘法函数，调用大整数乘法器，在合适位置点上小数点即可
num *Mul(num *x, num *y)
{
    //将x转为大整数a
    int xlen = x->inte[0] + x->deci[0];
    int a[xlen + 1];
    a[0] = xlen;
    int pos = 0;
    for (int i = 1; i <= x->inte[0]; i++)
        a[++pos] = x->inte[i];
    for (int i = 1; i <= x->deci[0]; i++)
        a[++pos] = x->deci[i];
    //将y转为大整数b
    int ylen = y->inte[0] + y->deci[0];
    int b[ylen + 1];
    b[0] = ylen;
    pos = 0;
    for (int i = 1; i <= y->inte[0]; i++)
        b[++pos] = y->inte[i];
    for (int i = 1; i <= y->deci[0]; i++)
        b[++pos] = y->deci[i];
    //存储结果的z
    int z[xlen + ylen + 1];
    memset(z, 0, sizeof(z));
    //进入大整数乘法器，获取z的有效区间
    int start = multy_box(x->base, a, b, z);
    int end = z[start] + start;
    //小数点位置
    int dot = x->deci[0] + y->deci[0];
    //把大整数z转为需要的结果ans
    num *ans = InitNum(ans);
    ans->base = x->base;
    ans->neg = (x->neg + y->neg) % 2; //判断符号
    pos = 0;
    // printf("%d\n", z[start]);
    int temp = start + 1;
    while (z[temp] == 0) //去除高位0
        temp++;
    for (int i = temp; i <= end - dot; i++) {
        // printf("%d %d\n", i, z[i]);
        ans->inte[++pos] = z[i];
    }
    ans->inte[0] = pos;
    //指针归零，读入小数部分
    pos = 0;
    //有效位数较少，需要补0
    if (end - start < dot)
    {
        //z中有效位数和补的0加一起足够小数位数为止
        while (pos + end - start < dot)
        {
            if (pos >= n)
                break;
            ans->deci[++pos] = 0;
        }
        //再读入z中的全部数据
        for (int i = start + 1; i <= end; i++)
        {
            ans->deci[++pos] = z[i];
            if (pos >= n)
                break;
        }
    }
    //否则直接读入
    else
    {
        for (int i = end - dot + 1; i <= end; i++)
        {
            ans->deci[++pos] = z[i];
            // printf("%d ", z[i]);
            //因为大整数乘法器得到的结果精度是两个乘数精度之和
            //为了防止把ans撑死，达到目标精度就可以退出了
            if (pos >= n)
                break;
        }
    }
    ans->deci[0] = pos;
    return ans;
}

//多项式的结构体
typedef struct
{
    num *a[MX];    //系数，按位存储
    int power[MX]; //指数
    int sum;       //项数
} var;

//初始化
var *InitVar()
{
    var *tmp = (var *)malloc(sizeof(var));
    for (int i = 0; i < MX; i++)
    {
        tmp->a[i] = NULL;
        tmp->power[i] = 0;
    }
    return tmp;
}

//销毁
var *DestroyVar(var *tmp)
{
    for (int i = 0; i < MX; i++)
    {
        if (tmp->a[i] != NULL)
            free(tmp->a[i]);
    }
    free(tmp);
}

//打印var类型变量，便于调试
void PrintVar(var *cal)
{
    for (int i = 0; i < cal->sum; i++)
    {
        PrintNum(cal->a[i]);
        printf("%d\n", cal->power[i]);
    }
}

//除法函数，低精除以低精，返回高精
num *divide(int x, int y)
{
    num *ans = InitNum();
    ans->base = 10;
    //确定符号
    ans->neg = (x * y > 0 ? 0 : 1);
    //负数转为正数，方便运算
    if (x < 0)
        x = -x;
    if (y < 0)
        y = -y;
    // printf("x=%d y=%d\n", x, y);
    //先得到整数部分
    int z = x / y;
    //存储z的每一位数，放在整数部分里
    int temp[10];
    memset(temp, 0, sizeof(temp));
    int i = 0;
    while (z)
    {
        temp[++i] = z % 10;
        z /= 10;
    }
    temp[0] = i;
    ans->inte[0] = i;
    for (int j = 1; j <= i; j++)
        ans->inte[j] = temp[temp[0]--];
    //如果除尽了，没有小数部分
    if (x % y == 0)
    {
        ans->deci[0] = 0;
        return ans;
    }
    //否则余数*10继续除，直到除尽或者达到精度
    else
    {
        int pos = 0;
        int res = x % y;
        for (int i = 0; i < n; i++)
        {
            res *= 10;
            ans->deci[++pos] = res / y;
            res %= y;
            if (res == 0)
                break;
        }
        ans->deci[0] = pos;
        return ans;
    }
}

//解析表达式，首先标准化格式，然后提取系数与指数，将多项式信息以Var类型存储
var *trans(char expr[], int type)
{
    //首先把系数缺省的1和指数缺省的1和0补全，方便统一操作
    char line[MX * 10];
    int pos = 0;
    for (int i = 0; i < strlen(expr); i++)
    {
        //系数为1的缺省
        if (i == 0 && expr[i] == 'x')
            line[pos++] = '1';
        if ((expr[i] == '+' || expr[i] == '-') && expr[i + 1] == 'x')
        {
            line[pos++] = expr[i];
            line[pos++] = '1';
            i++;
        }
        //指数为1的缺省
        if (expr[i] == 'x' && expr[i + 1] != '^')
        {
            line[pos++] = 'x';
            line[pos++] = '^';
            line[pos++] = '1';
            i++;
        }
        //指数为0的缺省
        if (expr[i] == '+' || expr[i] == '-' || (expr[i] >= '0' && expr[i] <= '9' && i == 0))
        {
            int j = i + 1;
            int flg = 0;
            while (expr[j] != '+' && expr[j] != '-' && j < strlen(expr))
            {
                //检测是否缺省
                if (expr[j] == 'x')
                {
                    flg = 1;
                    break;
                }
                j++;
            }
            if (flg == 1)
            {
                line[pos++] = expr[i];
                continue;
            }
            else
            {
                //缺省了，补齐
                for (int k = i; k <= j - 1; k++)
                    line[pos++] = expr[k];
                line[pos++] = 'x';
                line[pos++] = '^';
                line[pos++] = '0';
                i = j - 1;
                continue;
            }
        }
        line[pos++] = expr[i];
    }
    line[pos] = '\0';
    // printf("%s\n", expr);
    // printf("%s\n", line);

    //分析出系数并以高精度形式存储，并提取指数
    if (type == 1)
    {
        var *cal = InitVar();
        int ptr;
        for (int i = 0; i < strlen(line);)
        {
            for (ptr = 0; ptr < MX; ptr++)
            {
                if (i >= strlen(line))
                    break;
                int part1 = atoi(line + i);
                // printf("part1=%d\n", part1);
                i++;
                int skip = 0;
                while (line[i] != '/' && i < strlen(line))
                {
                    if (line[i] == 'x')
                    {
                        cal->a[ptr] = divide(part1, 1);
                        cal->power[ptr] = atoi(line + i + 2);
                        while (line[i] != '+' && line[i] != '-' && i < strlen(line))
                            i++;
                        skip = 1;
                        break;
                    }
                    i++;
                }
                if (skip == 1)
                    continue;
                // printf("%c\n", line[i]);
                int part2 = atoi(line + i + 1);
                // printf("part2=%d\n", part2);
                cal->a[ptr] = divide(part1, part2);
                while (line[i] != 'x')
                    i++;
                cal->power[ptr] = atoi(line + i + 2);
                while (line[i] != '+' && line[i] != '-' && i < strlen(line))
                    i++;
            }
        }
        cal->sum = ptr;
        // PrintVar(cal);
        return cal;
    }
    else if (type == 2)
    {
        var *cal = InitVar();
        int ptr;
        int x_pos[MX], s_pos[MX];
        int pos1 = 0, pos2 = 0;
        memset(x_pos, 0, sizeof(x_pos));
        memset(s_pos, 0, sizeof(s_pos));
        s_pos[pos2++] = 0; //第一个系数读取位置
        // printf("%s\n", line);
        for (int i = 1; i < strlen(line); i++)
        {
            if (line[i] == 'x')
                x_pos[pos1++] = i;
            if (line[i] == '+' || line[i] == '-')
                s_pos[pos2++] = i;
        }
        for (ptr = 0; ptr < MX; ptr++)
        {
            if (x_pos[ptr] == 0)
                break;
            cal->a[ptr] = ReadStr(10, line + s_pos[ptr]);
            cal->power[ptr] = atoi(line + x_pos[ptr] + 2);
        }
        cal->sum = ptr;
        // PrintVar(cal);
        return cal;
    }
}

//乘方函数，方便运算，调用乘法函数即可
num *Pow(num *x, int k)
{
    num *ans = InitNum();
    //特判指数为0
    if (k == 0)
    {
        ans->inte[0] = 1;
        ans->inte[1] = 1;
        ans->base = 10;
        return ans;
    }
    num temp = *x;
    num *used;
    for (int i = 0; i < k - 1; i++)
    {
        //防止内存泄漏
        used = Mul(&temp, x);
        temp = *used;
        free(used);
    }
    *ans = temp;
    return ans;
}

//计算多项式的值，调用上面的函数即可
num *Calcu(char expr[], num *x, int type)
{
    //先解析表达式
    var *cal = trans(expr, type);
    //调用加法和乘法运算
    // PrintVar(cal);
    num *ans = InitNum();
    num *used;
    num temp[cal->sum];
    for (int i = 0; i < cal->sum; i++)
    {
        num *tmp = Pow(x, cal->power[i]);
        used = Mul(cal->a[i], tmp);
        free(tmp);
        temp[i] = *used;
        free(used);
    }
    num temp2;
    temp2 = temp[0];
    for (int i = 1; i < cal->sum; i++)
    {
        used = Add(&temp2, &temp[i]);
        temp2 = *used;
        free(used);
    }
    *ans = temp2;
    DestroyVar(cal);
    return ans;
}

//主函数
int main()
{
    int choose;
    char str[Max + n + 1];
    int N = 0;
    num *ans;
    while (1)
    {
        printf("功能选择：\n0.退出\n1.十进制转N进制\n2.二进制转N进制\n3.加减乘运算\n4.简单多项式求值\n\n");
        memset(str, 0, sizeof(str));
        N = 0;
        scanf("%d", &choose);
        if (choose == 0)
            break;
        if (choose == 1)
        {
            printf("十进制数 & N：");
            scanf("%s %d", str, &N);
            num *data = ReadStr(10, str);
            ans = BaseConversion(data, N);
            PrintNum(ans);
            printf("\n");
            free(data);
            free(ans);
        }
        else if (choose == 2)
        {
            printf("二进制数 & N：");
            scanf("%s %d", str, &N);
            num *data = ReadStr(2, str);
            ans = BaseConversion(data, N);
            PrintNum(ans);
            printf("\n");
            free(data);
            free(ans);
        }
        else if (choose == 3)
        {
            printf("数据1 & 数据2 & 运算符号(+-*)：");
            char str1[Max + n + 1], str2[Max + n + 1], sign;
            scanf("%s %s %c", str1, str2, &sign);
            printf("结果进制类型：");
            scanf("%d", &N);
            num *data1 = ReadStr(10, str1);
            num *data2 = ReadStr(10, str2);
            if (sign == '+')
                ans = Add(data1, data2);
            else if (sign == '-')
                ans = Sub(data1, data2);
            else
                ans = Mul(data1, data2);
            num *ans_ = BaseConversion(ans, N);
            PrintNum(ans);
            PrintNum(ans_);
            free(data1);
            free(data2);
            free(ans);
            free(ans_);
            printf("\n");
        }
        else if (choose == 4)
        {
            char expr[MX * 10], x[Max + n + 1];
            memset(expr, 0, sizeof(expr));
            memset(x, 0, sizeof(x));
            printf("选择输入格式：\n1.分数：形如3/7x^3-1/3x^2-6x+4\n2.小数：形如0.4286x^3-0.3333x^2-6x+4\n\n");
            printf("格式：");
            int type = 0;
            scanf("%d", &type);
            printf("多项式 & x：");
            scanf("%s %s", expr, x);
            num *x_ = ReadStr(10, x);
            ans = Calcu(expr, x_, type);
            PrintNum(ans);
            free(x_);
            free(ans);
            printf("\n");
        }
    }
    return 0;
}