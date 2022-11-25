#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define MODE0
#define MODE1

#ifdef MODE0
//定长顺序结构存储

#define MAXSIZE 200

typedef struct
{
    char data[MAXSIZE];
    int length;
} String, *StringType;

//初始化串
StringType initString()
{
    // malloc空间
    StringType temp = (StringType)malloc(sizeof(String));
    //初始化空间
    memset(temp->data, 0, sizeof(temp->data));
    temp->length = 0;
    return temp;
}

//在原串的第k个字符前插入字符ch，k=0，1，……
void insert(StringType str, int k, char ch)
{
    //空间不足，报错并退出
    if (str->length >= MAXSIZE)
    {
        printf("\nerror! space not enough.\n");
        system("pause");
        exit(0);
    }
    //第k个字符往后整体右移
    for (int i = str->length - 1; i >= k; i--)
        str->data[i+1] = str->data[i];
    //插入字符ch
    str->data[k] = ch;
    str->length++;
}

//返回串的第k个字符，k=0，1，……
char StringAt(StringType str, int k)
{
    return str->data[k];
}

//返回串的长度
int length(StringType str)
{
    return str->length;
}

//清空字符串
void StringClear(StringType str)
{
    memset(str->data, 0, sizeof(str->data));
    str->length = 0;
}

//销毁字符串
void StringDestroy(StringType str)
{
    free(str);
}

#endif

#ifdef MODE1
//堆分配存储

#define INITSIZE 200
#define INCREASEMENT 10

typedef struct
{
    char *ch;
    int length;
} String, *StringType;

//初始化串
StringType initString() {
    StringType temp = (StringType)malloc(sizeof(String));
    //分配堆空间
    temp->ch = (char*)malloc(sizeof(char) * INITSIZE);
    //初始值均为0
    memset(temp->ch, 0, sizeof(char) * INITSIZE);
    //初始长度为0
    temp->length = 0;
    return temp;
}

//在原串的第k个字符前插入字符ch，k=0，1，……
void insert(StringType str, int k, char ch) {
    //扩展空间也用完了，报错并返回
    if (str->length + 1 > INITSIZE + INCREASEMENT)
    {
        printf("\nerror! space not enough.\n");
        system("pause");
        exit(0);
    }
    //空间不足，需要扩展空间
    if (str->length + 1 > INITSIZE)
        str->ch = realloc(str->ch, INITSIZE + INCREASEMENT);
    //第k个字符往后整体右移
    for (int i = str->length - 1; i >= k; i--)
        str->ch[i + 1] = str->ch[i];
    //插入字符ch
    str->ch[k] = ch;
    str->length++;
}

//返回串的第k个字符，k=0，1，……
char StringAt(StringType str, int k) {
    return str->ch[k];
}

//返回串的长度
int length(StringType str) {
    return str->length;
}

//清空字符串
void StringClear(StringType str) {
    memset(str->ch, 0, sizeof(char) * length(str));
    str->length = 0;
}

//销毁字符串
void StringDestroy(StringType str) {
    free(str->ch);
    free(str);
}

#endif
//以下操作仅需要使用上面封装好的函数，而不关注串的具体存储方式。

//打印串中所有元素
void StringPrint(StringType str)
{
    //依次获得所有字符并打印
    for (int i = 0; i < length(str); i++)
        printf("%c", StringAt(str, i));
    printf("\n");
}

//串的拼接，将串str拼接在串ans的末尾。
void StringConcat(StringType ans, StringType str)
{
    //一个字符一个字符地插入在ans的末尾
    for (int i = 0; i < length(str); i++)
        insert(ans, length(ans), StringAt(str, i));
}

// KMP算法中求解next数组
void Next(StringType pat, int *next)
{
    next[0] = -1;
    next[1] = 0;
    //初始下标j=2
    int j = 2, k;
    //遍历求出所有位置的next[j]
    while (j < length(pat))
    {
        //与next[j-1]位置的字符比较
        k = next[j - 1];
        //不等就继续比较next[next[j-1]]
        while (k > 0 && StringAt(pat, k) != StringAt(pat, j - 1))
            k = next[k];
        //如果是因为k<=0而退出循环，分两种情况讨论即可
        if (k <= 0)
        {
            if (StringAt(pat, 0) == StringAt(pat, j - 1))
                next[j] = 1;
            else
                next[j] = 0;
        }
        //否则就是找到了符合的两个子串，给next[j]赋值
        else
            next[j] = k + 1;
        j++;
    }
}

//串的模式匹配，txt为主串，pat为模式串，返回值为模式串在主串中第一次出现的位置。
/*
    StringPrint(txt);   //abaabcab
    StringPrint(pat);   //abc
    KMP(txt, pat);      //3
*/
int KMP(StringType txt, StringType pat, int begin)
{
    //计算next数组
    int next[length(pat)];
    memset(next, 0, sizeof(next));
    Next(pat, next);
    //分别为主串和子串的下标
    int i = begin, j = 0;
    while (i < length(txt) && j < length(pat))
    {
        // j==-1或者匹配成功，继续匹配下一个字符
        if (j == -1 || StringAt(txt, i) == StringAt(pat, j))
        {
            i++;
            j++;
        }
        //匹配不成功，主串指针不动，子串指针回溯
        else
            j = next[j];
    }
    //子串匹配成功，返回首位置
    if (j >= length(pat))
        return (i - length(pat));
    else
        return -1;
}

//求子串，结果为主串中从第begin个字符开始后的size个字符，begin=0，1，……
/*
    StringPrint(txt);       //abaabcab
    SubString(txt, 1, 2);   //ba
*/
StringType SubString(StringType str, int begin, int size)
{
    //储存结果
    StringType temp = initString();
    //插入目标子串
    for (int i = begin; i < begin + size; i++)
        insert(temp, length(temp), StringAt(str, i));
    return temp;
}

//字符串替换，将主串str中每个与串T相同的子串替换为串P
void StringReplace(StringType str, StringType T, StringType P)
{
    //存储替换结果
    StringType ans = initString();
    //pos指向当前KMP得到的子串T的位置，begin表示下一次KMP的开始位置
    int pos = 0, begin = 0;
    //遍历字符串找T
    while (pos != -1 && begin < length(str)) {
        pos = KMP(str, T, begin);
        //没找到T，退出
        if (pos == -1)
            break;
        // printf("\nbug=%d\n", pos);
        //两个字符串T之间的切片直接复制到ans中
        StringType temp = SubString(str, begin, pos - begin);
        // StringPrint(temp);
        begin = pos + length(T);
        StringConcat(ans, temp);
        //用完就扔，避免内存泄漏
        StringDestroy(temp);
        //把T换成P接到ans后面
        StringConcat(ans, P);
    }
    //把剩下的直接复制进ans
    StringType res = SubString(str, begin, length(str) - begin);
    StringConcat(ans, res);
    StringDestroy(res);
    //结果直接替换str
    StringClear(str);
    StringConcat(str, ans);
    StringDestroy(ans);
}

int main()
{
    //测试样例
    StringType txt = initString();
    insert(txt, 0, 'b');
    insert(txt, 0, 'c');
    insert(txt, 0, 'b');
    insert(txt, 0, 'a');
    insert(txt, 0, 'a');
    insert(txt, 0, 'c');
    insert(txt, 0, 'b');
    insert(txt, 0, 'a');
    insert(txt, 0, 'a');
    insert(txt, 0, 'b');
    insert(txt, 0, 'a');
    StringPrint(txt);
    // abaabcaabcb

    StringType pat = initString();
    insert(pat, 0, 'c');
    insert(pat, 0, 'b');
    printf("%d\n", KMP(txt, pat, 0));
    // 4

    StringConcat(txt, pat);
    StringPrint(txt);
    // abaabcaabcbbc

    StringClear(pat);
    insert(pat, 0, 'b');

    StringType str = initString();
    insert(str, 0, 'b');
    insert(str, 0, 'b');
    StringReplace(txt, pat, str);
    StringPrint(txt);
    // abbaabbcaabbcbbbbc
    StringDestroy(str);
    StringDestroy(txt);
    StringDestroy(pat);
    system("pause");
    return 0;
}