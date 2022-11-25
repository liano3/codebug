//Code Format "GB 2312"

#include <stdio.h>
#include <stdlib.h>

#define ElemType int //宏定义数据类型

//定义结构体
typedef struct LinkList
{
    ElemType data;         //存放数据
    struct LinkList *next; //存放后继节点地址
} List;

//初始化链表，返回链表头结点
List *InitList()
{
    List *p = (List *)malloc(sizeof(List)); //分配空间
    if (!p)                                 //判断空间分配是否成功
    {
        printf("初始化头结点分配空间失败！\n");
        return NULL;
    }
    p->next = NULL; //后继节点地址为NULL
    return p;
}

//判断链表是否为空，是返回1，否返回0
int ListEmpty(List *p)
{
    if (!p->next) //存在除头结点之外的节点视为非空
        return 1;
    return 0;
}

//计算链表长度，头结点不计入
int ListLength(List *p)
{
    int count = 0;
    while (p->next) //向后遍历并计数
    {
        count++;
        p = p->next;
    }
    return count;
}

//向链表中特定位置插入节点，返回该节点地址
//i为插入位置，i=List.length + 1视为在末尾插入
List *ListInsert(List *p, int i, ElemType value)
{
    if (i < 1 || i > ListLength(p) + 1) //判断插入位置是否合法
    {
        printf("插入位置不合法！\n");
        return NULL;
    }
    while (--i) //向后遍历到目标插入位置的前继节点
        p = p->next;
    List *q = (List *)malloc(sizeof(List)); //为新节点分配空间
    if (!q)
    {
        printf("插入节点时分配空间失败！\n");
        return NULL;
    }
    //插入新节点，先连接后继节点，然后与前驱连接
    q->data = value;
    q->next = p->next;
    p->next = q;
    return q;
}

//删除链表中特定位置的节点，通过value返回其值
int ListDelete(List *p, int i, ElemType *value)
{
    if (ListEmpty(p))
    {
        printf("链表已为空，无法删除节点！\n");
        return 0;
    }
    if (i < 1 || i > ListLength(p)) //判断删除位置是否合法
    {
        printf("删除位置不合法！\n");
        return 0;
    }
    while (--i) // 向后遍历至要删除节点的前驱节点
        p = p->next;
    //通过临时变量temp删除目标节点
    List *temp = p->next;
    *value = temp->data;
    p->next = temp->next;
    free(temp);
    return 1;
}

//查找特定位置的元素并返回其所存数据
ElemType GetElem(List *p, int i)
{
    if (i < 1 || i > ListLength(p)) //判断查找位置是否合法
    {
        printf("查找位置不合法！\n");
        return 0;
    }
    while (i--) //向后遍历至目标节点
        p = p->next;
    return p->data;
}

//获取特定数据所在节点的地址
List *LocateElem(List *p, ElemType value)
{
    while (p = p->next) //向后遍历
    {
        if (p->data == value)   //找到目标节点，返回地址
            return p;
    }
    return NULL;
}

//获取特定节点的前驱节点
List *PriorElem(List *p, List *cur_e)
{
    while (p = p->next) //向后遍历至特定节点前驱
    {
        if (p->next == cur_e)
            return p;   //返回前驱地址
    }
    return NULL;
}

//获取特定节点的后继节点
List *NextElem(List *p, List *cur_e)
{
    return cur_e->next;
}

// 打印特定节点元素，ListTraverse调用
void PrintElem(List *p)
{
    printf("%d -> ", p->data);
    return;
}

//遍历链表执行操作
void ListTraverse(List *p, void (*func)(List *p))
{
    while (p = p->next) //向后遍历
        (*func)(p); //调用操作函数
    printf("NULL\n");
    return;
}

//清空链表
void ClearList(List *p)
{
    while (p->next)
    {
        //通过临时变量temp删除节点，从前往后一个个删
        List *temp = p->next;
        p->next = temp->next; //连接要删除节点的前驱和后继
        free(temp);
    }
    return;
}

//销毁链表
void DestroyList(List *p)
{
    ClearList(p);   //清空链表
    free(p);    //free头结点
    return;
}

int main()
{
    List *p = InitList();   //初始化链表
    ListInsert(p, 1, 2);    //插入操作测试
    ListInsert(p, 1, 3);
    ListInsert(p, 2, 4);
    ListInsert(p, 2, 6);
    void (*Print)(List * p) = PrintElem;    //函数指针指向print函数
    ListTraverse(p, Print);    //遍历函数测试
    printf("%d\n", PriorElem(p, LocateElem(p, 4))->data);   //定位函数，获取前驱与后继函数测试
    printf("%d\n", NextElem(p, LocateElem(p, 4))->data);
    ElemType *value = (ElemType*)malloc(sizeof(ElemType));
    ListDelete(p, 2, value);    //删除操作测试
    printf("%d\n", *value);
    ListTraverse(p, Print);
    ListDelete(p, 4, value);
    ListDelete(p, 1, value);
    ListTraverse(p, Print);
    printf("%d\n", GetElem(p, 1));  //查找函数测试
    printf("%d\n", GetElem(p, 2));
    printf("%d\n", GetElem(p, 3));
    printf("%d\n", LocateElem(p, 2)->data); //定位函数测试
    ClearList(p);   //清空函数测试
    ListTraverse(p, Print);
    DestroyList(p); //销毁链表测试
    printf("TRUE\n");   //运行结果全部符合预期输出TRUE
    system("pause");
    return 0;
}