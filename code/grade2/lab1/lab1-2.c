#include <stdio.h>
#include <stdlib.h>

#define STACK_INIT_SIZE 100 //栈预设空间大小
#define STACKINCREMENT 10   //栈扩容增量

//初始化栈
#define InitStack(stack)                                                      \
    /*分配空间*/                                                          \
    stack##_stackbase_ptr = (stack *)malloc(sizeof(stack) * STACK_INIT_SIZE); \
    /*分配成功，初始化值*/                                           \
    if (stack##_stackbase_ptr)                                                \
    {                                                                         \
        stack##_stacktop_ptr = stack##_stackbase_ptr;                         \
        stack##_stacksize = STACK_INIT_SIZE;                                  \
    }                                                                         \
    /*分配失败，退出*/                                                 \
    else                                                                      \
        exit(0);

//判断栈空，条件表达式实现
#define StackEempty(stack) ((stack##_stacktop_ptr == stack##_stackbase_ptr) ? 1 : 0)

//获取栈顶元素，通过e返回，条件表达式加逗号表达式实现
#define GetTop(stack, e) ((stack##_stacktop_ptr == stack##_stackbase_ptr) ? 0 : (e = *(stack##_stacktop_ptr - 1), 1))

//清空栈，令头指针等于尾指针，相当于将栈标记为空
#define ClearStack(stack) (stack##_stacktop_ptr = stack##_stackbase_ptr)

//销毁栈，free掉base即可
#define DestroyStack(stack) free(stack##_stackbase_ptr);

//获取栈的长度，头指针减去尾指针
#define StackLength(stack) (stack##_stacktop_ptr - stack##_stackbase_ptr)

//弹出栈顶元素，通过e获得其值
#define pop(stack, e) ((stack##_stacktop_ptr == stack##_stackbase_ptr) ? 0 : (e = *(--stack##_stacktop_ptr), 1))

//入栈操作
#define push(stack, e)                                                                                                         \
    /*栈已满，realloc*/                                                                                                    \
    if (stack##_stacktop_ptr - stack##_stackbase_ptr >= stack##_stacksize)                                                     \
    {                                                                                                                          \
        stack##_stackbase_ptr = (stack *)realloc(stack##_stackbase_ptr, (stack##_stacksize + STACKINCREMENT) * sizeof(stack)); \
        /*判断是否分配成功*/                                                                                           \
        if (!stack##_stackbase_ptr)                                                                                            \
            exit(0);                                                                                                           \
        stack##_stacktop_ptr = stack##_stackbase_ptr + stack##_stacksize;                                                      \
        stack##_stacksize += STACKINCREMENT;                                                                                   \
    }                                                                                                                          \
    /*栈未满，直接入栈*/                                                                                               \
    *(stack##_stacktop_ptr++) = e

//遍历操作，这里就把操作看做将栈的元素输出
#define StackTraverse(stack, fptr)                                                                              \
    /*栈为空，输出NULL*/                                                                                  \
    if (stack##_stacktop_ptr == stack##_stackbase_ptr)                                                          \
        printf("NULL");                                                                                         \
    /*因为宏定义是文本替换，所以不能直接改变top指针，通过临时变量temp获取栈值*/ \
    stack *stack##_temp = stack##_stacktop_ptr;                                                                 \
    /*这里根据栈类型决定不同输出格式*/                                                           \
    if (ElemType == 0)                                                                                          \
        while (--stack##_temp >= stack##_stackbase_ptr)                                                         \
            fptr("%d ", *stack##_temp);                                                                         \
    else if (ElemType == 1)                                                                                     \
        while (--stack##_temp >= stack##_stackbase_ptr)                                                         \
            fptr("%c ", *stack##_temp);                                                                         \
    else                                                                                                        \
        while (--stack##_temp >= stack##_stackbase_ptr)                                                         \
            fptr("%d=%.1f ", Lnode##_temp->data1, Lnode##_temp->data2);

//整型栈
unsigned int_stacksize; //栈大小
int *int_stacktop_ptr;  //栈顶指针
int *int_stackbase_ptr; //栈底指针

//字符栈
unsigned char_stacksize;  //栈大小
char *char_stacktop_ptr;  //栈顶指针
char *char_stackbase_ptr; //栈底指针

//结构体栈
typedef struct
{
    int data1;
    float data2;
} Lnode;
unsigned Lnode_stacksize;   //栈大小
Lnode *Lnode_stacktop_ptr;  //栈顶指针
Lnode *Lnode_stackbase_ptr; //栈底指针
Lnode *Lnode_temp;          //遍历指针
int ElemType = 0;

//测试
int main()
{
    //整数栈
    //初始化
    InitStack(int);
    //判断栈空
    printf("%d\n", StackEempty(int));
    //入栈
    push(int, 11);
    push(int, 45);
    push(int, 14);
    //判断栈空
    printf("%d\n", StackEempty(int));
    //获取栈顶元素
    int e;
    printf("%d %d\n", GetTop(int, e), e);
    //获取栈长度
    printf("%d\n", StackLength(int));
    //遍历栈
    StackTraverse(int, printf);
    printf("\n");
    //出栈
    pop(int, e);
    printf("%d %d\n", GetTop(int, e), e);
    ClearStack(int);
    printf("%d\n", StackLength(int));
    DestroyStack(int);
    printf("\n\n");

    //字符栈
    ElemType = 1;
    //初始化
    InitStack(char);
    //判断栈空
    printf("%d\n", StackEempty(char));
    //入栈
    push(char, 'Y');
    push(char, 'H');
    push(char, 'S');
    //判断栈空
    printf("%d\n", StackEempty(char));
    //获取栈顶元素
    char e1;
    int result = GetTop(char, e1);
    printf("%d %c\n", result, e1);
    //获取栈长度
    printf("%d\n", StackLength(char));
    //遍历栈
    StackTraverse(char, printf);
    printf("\n");
    //出栈
    pop(char, e1);
    result = GetTop(char, e1);
    printf("%d %c\n", result, e1);
    ClearStack(char);
    printf("%d\n", StackLength(char));
    DestroyStack(char);
    printf("\n\n");

    //结构体栈
    ElemType = 2;
    //初始化
    InitStack(Lnode);
    //判断栈空
    printf("%d\n", StackEempty(Lnode));
    //入栈
    Lnode a, b, c;
    a.data1 = 95;
    a.data2 = 4.3;
    b.data1 = 90;
    b.data2 = 4.0;
    c.data1 = 85;
    c.data2 = 3.7;
    push(Lnode, a);
    push(Lnode, b);
    push(Lnode, c);
    //判断栈空
    printf("%d\n", StackEempty(Lnode));
    //获取栈顶元素
    Lnode e2;
    result = GetTop(Lnode, e2);
    printf("%d %d=%.1f\n", result, e2.data1, e2.data2);
    //获取栈长度
    printf("%d\n", StackLength(Lnode));
    //遍历栈
    StackTraverse(Lnode, printf);
    printf("\n");
    //出栈
    pop(Lnode, e2);
    result = GetTop(Lnode, e2);
    printf("%d %d=%.1f\n", result, e2.data1, e2.data2);
    ClearStack(Lnode);
    printf("%d\n", StackLength(Lnode));
    DestroyStack(Lnode);
    system("pause");
    return 0;
}