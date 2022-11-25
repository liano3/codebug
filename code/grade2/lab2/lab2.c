// code Format GB2312
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define n 210   //��󾫶ȣ���С���������λ����
#define Max 200 //�����������λ��
#define MX 20   //����ʽ������

typedef struct
{
    int neg;           //����λ��0Ϊ����1Ϊ��
    int inte[Max + 1]; //�������֣���һλ��ʵ�ʳ���
    int deci[n + 1];   //С�����֣���һλ��ʵ�ʳ���
    int base;          //��������
} num;

//�ṹ���ʼ��
num *InitNum()
{
    //����ռ�
    num *tmp = (num *)malloc(sizeof(num));
    if (!tmp)
    {
        printf("����ռ�ʧ�ܣ�");
        system("pause");
        exit(0);
    }
    //��ʼ��
    tmp->base = 0;
    tmp->neg = 0;
    memset(tmp->inte, 0, sizeof(int) * (Max + 1));
    memset(tmp->deci, 0, sizeof(int) * (n + 1));
    //���ؽṹ��ָ��
    return tmp;
}

//��ӡ����
void PrintNum(num *data)
{
    printf("%d���ƣ�", data->base);
    //����0,���0����
    if (data->inte[0] == 0 && data->deci[0] == 0)
    {
        printf("0\n");
        return;
    }
    //��ӡ����
    if (data->neg == 1)
        printf("-");
    //��С��
    if (data->inte[0] == 0)
        printf("0");
    //��ӡ��������
    for (int i = 1; i <= data->inte[0]; i++)
    {
        //����9��Сд��ĸ����
        if (data->inte[i] >= 10)
            printf("%c", data->inte[i] - 10 + 'a');
        else
            printf("%d", data->inte[i]);
    }
    //��ӡС�����֣�ͬ����
    //ȥ��β�������0
    int temp = data->deci[0];
    while(data->deci[temp] == 0 && temp > 0)
        temp--;
    data->deci[0] = temp;
    //��ӡС����
    if (data->deci[0] > 0)
        printf(".");
    //��ӡ
    //����ľ���ֱ������
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

//��������ƻ�ʮ�������ݣ�תΪnum���ʹ洢
num *ReadStr(int N, char *data)
{
    num *ans = InitNum(); //���ת�����
    int pos;              //ָ��data
    int i;                //ָ��ans
    if (N != 2 && N != 10)
    {
        printf("ֻ�ܶ�������ƻ�ʮ������Ŷ��");
        system("pause");
        exit(0);
    }
    //����
    ans->base = N;
    //�жϸ���
    if (data[0] == '-')
    {
        ans->neg = 1;
        pos = 1; //ָ�����һλ
    }
    //�ж�����
    else if (data[0] == '+')
    {
        ans->neg = 0;
        pos = 1;
    }
    //�������ŵ�ȱʡ�ʹ�С�����������ֵ�ȱʡ
    else if ((data[0] < N + '0' && data[0] >= '0') || data[0] == '.')
    {
        ans->neg = 0;
        pos = 0;
    }
    //����Ϊ�Ƿ�����
    else
    {
        printf("��������ȷ�����ݣ�");
        system("pause");
        exit(0);
    }
    //����һ��������
    //��������
    i = 0;
    int flg = 0;
    while (pos < strlen(data) && data[pos] != '.')
    {
        //���ڶ���ʽ����������x��+-��ֹͣ����
        if (data[pos] == 'x' || data[pos] == '+' || data[pos] == '-')
        {
            //��¼����
            ans->inte[0] = i;
            ans->deci[0] = 0;
            return ans;
        }
        //�ж�����Ϸ���
        if (data[pos] >= N + '0' || data[pos] < '0')
        {
            printf("��������ȷ������");
            system("pause");
            exit(0);
        }
        //ȥ��ͷ�������0
        if (data[pos] == '0' && flg == 0)
        {
            pos++;
            continue;
        }
        flg = 1; //�����ж�0�Ƿ��ȥ
        ans->inte[++i] = data[pos++] - '0';
    }
    //��¼����
    ans->inte[0] = i;
    pos++; //С����ռλ��ָ�����

    //С������
    i = 0; // �ص���㣬����С������
    while (pos < strlen(data))
    {
        //���ڽ�������ʽ
        if (data[pos] == 'x' || data[pos] == '+' || data[pos] == '-')
        {
            //ȥ��β�������0
            while (ans->deci[i] == 0 && i >= 0)
                i--;
            ans->deci[0] = i;
            return ans;
        }
        //�жϺϷ���
        if (data[pos] >= N + '0' || data[pos] < '0')
        {
            printf("��������ȷ�����ݣ�");
            system("pause");
            exit(0);
        }
        ans->deci[++i] = data[pos++] - '0';
    }
    //ȥ��β�������0
    while (ans->deci[i] == 0 && i > 0)
        i--;
    ans->deci[0] = i;
    // PrintNum(ans);
    return ans;
}

//����ת��������ʵ��������ƻ�ת����dataתΪN����
num *BaseConversion(num *data, int N)
{
    // temp�洢ת�����
    num *temp = InitNum();
    int base = data->base;
    temp->base = N;
    temp->neg = data->neg; //����λ����

    //��������
    //ģ���ֹ�����ת��
    //��ѭ����inte[]/N��ȡ�࣬����/N��ȡ�ֱ࣬����Ϊ0��
    //��Ϊ���ֺܴ󣬲���ֱ�ӳ�������һλһλ�ĳ�����ƴ��ȥ��0����������̣�����*data->base+��һλ����Ϊ��һ��������������/N��ȡ���һ�ε�����Ϊ���������

    int div[data->inte[0] + 1]; //�洢��ѭ����ÿһ�εı�����
    //��һ�εı���������inte[]����
    for (int i = 0; i <= data->inte[0]; i++)
        div[i] = data->inte[i];
    //�洢��ѭ���е���
    int ans[data->inte[0] + 1];
    memset(ans, 0, sizeof(ans));
    //�洢��ѭ���е�����
    int res[Max];
    memset(res, 0, sizeof(res));
    //��ѭ��
    while (div[0] >= 1)
    {
        int y = 0; //��¼Сѭ���е��������᲻�ϸ���
        int i = 1; //ѭ������
        ans[0] = div[0];
        //Сѭ��
        while (i <= div[0])
        {
            y = y * base + div[i]; //��ǰ������
            ans[i++] = y / N;      //��¼��
            y %= N;                //��������
        }
        //��ѭ���е�һλ�������ǵ�ǰ���µ�y
        res[++res[0]] = y;
        i = 1; //�ظ����ã�ȥ���´α�����(���ϴε���)�Ķ����0
        while (i <= ans[0] && ans[i] == 0)
            i++;
        //��ձ��������������ϴε��̸�ֵ
        memset(div, 0, sizeof(div));
        for (int j = i; j <= ans[0]; j++)
            div[++div[0]] = ans[j];
        memset(ans, 0, sizeof(ans));
    }
    //����ȡ�������ɵõ�ת�������������
    temp->inte[0] = res[0];
    for (int i = 1; i <= temp->inte[0]; i++)
        temp->inte[i] = res[res[0]--];

    // С������
    //ģ���ֹ�����ת�����߼��Ϻ�������������
    //��ѭ����*Nȡ����ʣ�µļ���*Nȡ����ֱ��0����ȡ���������������м��ɵõ�ת�����
    //Сѭ����һλһλ��*N��/data->base��λ��%data->base���£�ƴ�����������ѭ���ļ���

    int multy[data->deci[0] + 1]; //�洢��ѭ����ÿһ�εı�����(���������ƺ�)
    //��һ�εı���������deci[]������Ҫ����洢
    multy[0] = data->deci[0];
    int pos = 0;
    for (int i = data->deci[0]; i >= 1; i--)
        multy[++pos] = data->deci[i];
    //�洢��ѭ���еĳ˻����
    int ans_[data->deci[0] + 1];
    memset(ans_, 0, sizeof(ans_));
    //�洢��ѭ���е�ȡ�����
    int res_[n + 1];
    memset(res_, 0, sizeof(res_));
    //��ѭ��
    while (multy[0] >= 1)
    {
        int y = 0; //��¼Сѭ���еĽ�λ�����᲻�ϸ���
        int i = 1; //����ƴ��Сѭ����ÿ�εĳ˻����
        //Сѭ��
        while (i <= multy[0])
        {
            y += (multy[i] * N);  //��ǰ�˻����
            ans_[i++] = y % base; //��¼��ǰ��������
            y /= base;            //���½�λ��
        }
        ans_[0] = i - 1;
        //ȡ������ȡ���һ�εĽ�λ��
        res_[0]++;
        //�ﵽ���Ⱥ��˳���ѭ��
        if (res_[0] > n)
        {
            res_[0]--;
            break;
        }
        res_[res_[0]] = y;
        //��ձ��������������ϴεĳ˻������ֵ
        int count = 0;
        memset(multy, 0, sizeof(multy));
        for (int j = 1; j <= ans_[0]; j++)
        {
            //��ⱻ�����Ƿ�Ϊ0
            if (ans_[j] == 0)
                count++;
            multy[++multy[0]] = ans_[j];
        }
        //������Ϊ 0���˳���ѭ��
        if (count == ans_[0])
            multy[0] = 0;
        count = 0;
        memset(ans_, 0, sizeof(ans_));
    }
    //����ȡ�������ɵõ�ת�����С������
    for (int i = 0; i <= res_[0]; i++)
        temp->deci[i] = res_[i];
    return temp;
}

//�߾��ȱȽ���������|x|>|Y|
int compare(num *x, num *y)
{
    //λ����ֱͬ�ӱȽ�
    if (x->inte[0] > y->inte[0])
        return 1;
    //λ����ͬ���Ƚ�
    else if (x->inte[0] == y->inte[0])
    {
        //�Ƚ���������
        for (int i = 1; i <= x->inte[0]; i++)
        {
            //���ֲ�ͬ�ͽ�����
            if (x->inte[i] > y->inte[i])
                return 1;
            else if (x->inte[i] < y->inte[i])
                return 0;
        }
        //�Ƚ�С������
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

//�߾��ȼӷ������ϸ�涨x��y��Ϊ������x>y
num *adding_box(num *x, num *y)
{
    //�洢���
    num *ans = InitNum();
    int base = x->base;
    //���Ʋ���
    ans->base = base;
    //����Ϊ+
    ans->neg = 0;

    //С������

    //ȷ�����һλС��
    int low = (x->deci[0] >= y->deci[0] ? x->deci[0] : y->deci[0]);
    int pos = low; //ָ��ǰ����λ��ָ��
    int temp = 0;  //�ݴ浱ǰ��λ��������
    for (int i = low; i >= 1; i--)
    {
        //��λ��ӣ����ǽ�λ
        temp += (x->deci[i] + y->deci[i]);
        ans->deci[pos--] = temp % base; //��ǰλ��������
        temp /= base;                   //��λֵ
    }
    ans->deci[0] = low; //�͵�С�����ֳ���

    //��������

    //�����������ֳ���֮�С������룬��λ��ӣ���С����������
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
    //������һλ�н�λ����Ҫ����������������
    if (temp > 0)
    {
        for (int i = ans->inte[0]; i >= 1; i--)
            ans->inte[i + 1] = ans->inte[i];
        ans->inte[1] = temp;           //�������Ľ�λ
        ans->inte[0] = x->inte[0] + 1; //���³���
    }
    else
        ans->inte[0] = x->inte[0];
    return ans;
}

//�߾��ȼ��������ϸ�涨x��y��Ϊ������x>y
num *sub_box(num *x, num *y)
{
    //�ͼӷ�������
    num *ans = InitNum();
    int base = x->base;
    ans->base = base;
    ans->neg = 0;
    int low = (x->deci[0] > y->deci[0] ? x->deci[0] : y->deci[0]);
    int pos = low;
    int temp = 0; //�洢��ǰ��λֵ
    int a;        //�洢��ǰλ��ֵ
    for (int i = low; i >= 1; i--)
    {
        a = x->deci[i] - temp;
        temp = 0;
        //��λ
        if (a < y->deci[i])
        {
            temp = 1;
            a += base;
        }
        ans->deci[pos--] = a - y->deci[i];
    }
    //ȥ�������0
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
    //ȥ�������0
    int count = 0;
    while (ans->inte[++pos] == 0 && pos <= x->inte[0])
        count++;
    for (int i = pos; i <= x->inte[0]; i++)
        ans->inte[i - pos + 1] = ans->inte[i];
    ans->inte[0] = x->inte[0] - count;
    return ans;
}

//�߾��ȼӷ����������üӷ����ͼ����������������������Ĳ�ͬ���ѡ����÷�ʽ
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

//�߾��ȼ������������üӷ���������
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

//�������˷�����ģ�����㣬ͨ��z[]���أ�������Чλ���(0��1)����һ����Чλλ�洢����
int multy_box(int base, int x[], int y[], int z[])
{
    //��λ���㼴��
    for (int i = y[0]; i >= 1; i--)
    {
        for (int j = x[0]; j >= 1; j--)
        {
            z[i + j] += x[j] * y[i];
            z[i + j - 1] += z[i + j] / base;
            z[i + j] %= base;
        }
    }
    //������Чλ������Ч����
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

//�߾��ȳ˷����������ô������˷������ں���λ�õ���С���㼴��
num *Mul(num *x, num *y)
{
    //��xתΪ������a
    int xlen = x->inte[0] + x->deci[0];
    int a[xlen + 1];
    a[0] = xlen;
    int pos = 0;
    for (int i = 1; i <= x->inte[0]; i++)
        a[++pos] = x->inte[i];
    for (int i = 1; i <= x->deci[0]; i++)
        a[++pos] = x->deci[i];
    //��yתΪ������b
    int ylen = y->inte[0] + y->deci[0];
    int b[ylen + 1];
    b[0] = ylen;
    pos = 0;
    for (int i = 1; i <= y->inte[0]; i++)
        b[++pos] = y->inte[i];
    for (int i = 1; i <= y->deci[0]; i++)
        b[++pos] = y->deci[i];
    //�洢�����z
    int z[xlen + ylen + 1];
    memset(z, 0, sizeof(z));
    //����������˷�������ȡz����Ч����
    int start = multy_box(x->base, a, b, z);
    int end = z[start] + start;
    //С����λ��
    int dot = x->deci[0] + y->deci[0];
    //�Ѵ�����zתΪ��Ҫ�Ľ��ans
    num *ans = InitNum(ans);
    ans->base = x->base;
    ans->neg = (x->neg + y->neg) % 2; //�жϷ���
    pos = 0;
    // printf("%d\n", z[start]);
    int temp = start + 1;
    while (z[temp] == 0) //ȥ����λ0
        temp++;
    for (int i = temp; i <= end - dot; i++) {
        // printf("%d %d\n", i, z[i]);
        ans->inte[++pos] = z[i];
    }
    ans->inte[0] = pos;
    //ָ����㣬����С������
    pos = 0;
    //��Чλ�����٣���Ҫ��0
    if (end - start < dot)
    {
        //z����Чλ���Ͳ���0��һ���㹻С��λ��Ϊֹ
        while (pos + end - start < dot)
        {
            if (pos >= n)
                break;
            ans->deci[++pos] = 0;
        }
        //�ٶ���z�е�ȫ������
        for (int i = start + 1; i <= end; i++)
        {
            ans->deci[++pos] = z[i];
            if (pos >= n)
                break;
        }
    }
    //����ֱ�Ӷ���
    else
    {
        for (int i = end - dot + 1; i <= end; i++)
        {
            ans->deci[++pos] = z[i];
            // printf("%d ", z[i]);
            //��Ϊ�������˷����õ��Ľ��������������������֮��
            //Ϊ�˷�ֹ��ans�������ﵽĿ�꾫�ȾͿ����˳���
            if (pos >= n)
                break;
        }
    }
    ans->deci[0] = pos;
    return ans;
}

//����ʽ�Ľṹ��
typedef struct
{
    num *a[MX];    //ϵ������λ�洢
    int power[MX]; //ָ��
    int sum;       //����
} var;

//��ʼ��
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

//����
var *DestroyVar(var *tmp)
{
    for (int i = 0; i < MX; i++)
    {
        if (tmp->a[i] != NULL)
            free(tmp->a[i]);
    }
    free(tmp);
}

//��ӡvar���ͱ��������ڵ���
void PrintVar(var *cal)
{
    for (int i = 0; i < cal->sum; i++)
    {
        PrintNum(cal->a[i]);
        printf("%d\n", cal->power[i]);
    }
}

//�����������;����Ե;������ظ߾�
num *divide(int x, int y)
{
    num *ans = InitNum();
    ans->base = 10;
    //ȷ������
    ans->neg = (x * y > 0 ? 0 : 1);
    //����תΪ��������������
    if (x < 0)
        x = -x;
    if (y < 0)
        y = -y;
    // printf("x=%d y=%d\n", x, y);
    //�ȵõ���������
    int z = x / y;
    //�洢z��ÿһλ������������������
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
    //��������ˣ�û��С������
    if (x % y == 0)
    {
        ans->deci[0] = 0;
        return ans;
    }
    //��������*10��������ֱ���������ߴﵽ����
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

//�������ʽ�����ȱ�׼����ʽ��Ȼ����ȡϵ����ָ����������ʽ��Ϣ��Var���ʹ洢
var *trans(char expr[], int type)
{
    //���Ȱ�ϵ��ȱʡ��1��ָ��ȱʡ��1��0��ȫ������ͳһ����
    char line[MX * 10];
    int pos = 0;
    for (int i = 0; i < strlen(expr); i++)
    {
        //ϵ��Ϊ1��ȱʡ
        if (i == 0 && expr[i] == 'x')
            line[pos++] = '1';
        if ((expr[i] == '+' || expr[i] == '-') && expr[i + 1] == 'x')
        {
            line[pos++] = expr[i];
            line[pos++] = '1';
            i++;
        }
        //ָ��Ϊ1��ȱʡ
        if (expr[i] == 'x' && expr[i + 1] != '^')
        {
            line[pos++] = 'x';
            line[pos++] = '^';
            line[pos++] = '1';
            i++;
        }
        //ָ��Ϊ0��ȱʡ
        if (expr[i] == '+' || expr[i] == '-' || (expr[i] >= '0' && expr[i] <= '9' && i == 0))
        {
            int j = i + 1;
            int flg = 0;
            while (expr[j] != '+' && expr[j] != '-' && j < strlen(expr))
            {
                //����Ƿ�ȱʡ
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
                //ȱʡ�ˣ�����
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

    //������ϵ�����Ը߾�����ʽ�洢������ȡָ��
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
        s_pos[pos2++] = 0; //��һ��ϵ����ȡλ��
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

//�˷��������������㣬���ó˷���������
num *Pow(num *x, int k)
{
    num *ans = InitNum();
    //����ָ��Ϊ0
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
        //��ֹ�ڴ�й©
        used = Mul(&temp, x);
        temp = *used;
        free(used);
    }
    *ans = temp;
    return ans;
}

//�������ʽ��ֵ����������ĺ�������
num *Calcu(char expr[], num *x, int type)
{
    //�Ƚ������ʽ
    var *cal = trans(expr, type);
    //���üӷ��ͳ˷�����
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

//������
int main()
{
    int choose;
    char str[Max + n + 1];
    int N = 0;
    num *ans;
    while (1)
    {
        printf("����ѡ��\n0.�˳�\n1.ʮ����תN����\n2.������תN����\n3.�Ӽ�������\n4.�򵥶���ʽ��ֵ\n\n");
        memset(str, 0, sizeof(str));
        N = 0;
        scanf("%d", &choose);
        if (choose == 0)
            break;
        if (choose == 1)
        {
            printf("ʮ������ & N��");
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
            printf("�������� & N��");
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
            printf("����1 & ����2 & �������(+-*)��");
            char str1[Max + n + 1], str2[Max + n + 1], sign;
            scanf("%s %s %c", str1, str2, &sign);
            printf("����������ͣ�");
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
            printf("ѡ�������ʽ��\n1.����������3/7x^3-1/3x^2-6x+4\n2.С��������0.4286x^3-0.3333x^2-6x+4\n\n");
            printf("��ʽ��");
            int type = 0;
            scanf("%d", &type);
            printf("����ʽ & x��");
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