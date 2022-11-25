//Code Format "GB 2312"

#include <stdio.h>
#include <stdlib.h>

#define ElemType int //�궨����������

//����ṹ��
typedef struct LinkList
{
    ElemType data;         //�������
    struct LinkList *next; //��ź�̽ڵ��ַ
} List;

//��ʼ��������������ͷ���
List *InitList()
{
    List *p = (List *)malloc(sizeof(List)); //����ռ�
    if (!p)                                 //�жϿռ�����Ƿ�ɹ�
    {
        printf("��ʼ��ͷ������ռ�ʧ�ܣ�\n");
        return NULL;
    }
    p->next = NULL; //��̽ڵ��ַΪNULL
    return p;
}

//�ж������Ƿ�Ϊ�գ��Ƿ���1���񷵻�0
int ListEmpty(List *p)
{
    if (!p->next) //���ڳ�ͷ���֮��Ľڵ���Ϊ�ǿ�
        return 1;
    return 0;
}

//���������ȣ�ͷ��㲻����
int ListLength(List *p)
{
    int count = 0;
    while (p->next) //������������
    {
        count++;
        p = p->next;
    }
    return count;
}

//���������ض�λ�ò���ڵ㣬���ظýڵ��ַ
//iΪ����λ�ã�i=List.length + 1��Ϊ��ĩβ����
List *ListInsert(List *p, int i, ElemType value)
{
    if (i < 1 || i > ListLength(p) + 1) //�жϲ���λ���Ƿ�Ϸ�
    {
        printf("����λ�ò��Ϸ���\n");
        return NULL;
    }
    while (--i) //��������Ŀ�����λ�õ�ǰ�̽ڵ�
        p = p->next;
    List *q = (List *)malloc(sizeof(List)); //Ϊ�½ڵ����ռ�
    if (!q)
    {
        printf("����ڵ�ʱ����ռ�ʧ�ܣ�\n");
        return NULL;
    }
    //�����½ڵ㣬�����Ӻ�̽ڵ㣬Ȼ����ǰ������
    q->data = value;
    q->next = p->next;
    p->next = q;
    return q;
}

//ɾ���������ض�λ�õĽڵ㣬ͨ��value������ֵ
int ListDelete(List *p, int i, ElemType *value)
{
    if (ListEmpty(p))
    {
        printf("������Ϊ�գ��޷�ɾ���ڵ㣡\n");
        return 0;
    }
    if (i < 1 || i > ListLength(p)) //�ж�ɾ��λ���Ƿ�Ϸ�
    {
        printf("ɾ��λ�ò��Ϸ���\n");
        return 0;
    }
    while (--i) // ��������Ҫɾ���ڵ��ǰ���ڵ�
        p = p->next;
    //ͨ����ʱ����tempɾ��Ŀ��ڵ�
    List *temp = p->next;
    *value = temp->data;
    p->next = temp->next;
    free(temp);
    return 1;
}

//�����ض�λ�õ�Ԫ�ز���������������
ElemType GetElem(List *p, int i)
{
    if (i < 1 || i > ListLength(p)) //�жϲ���λ���Ƿ�Ϸ�
    {
        printf("����λ�ò��Ϸ���\n");
        return 0;
    }
    while (i--) //��������Ŀ��ڵ�
        p = p->next;
    return p->data;
}

//��ȡ�ض��������ڽڵ�ĵ�ַ
List *LocateElem(List *p, ElemType value)
{
    while (p = p->next) //������
    {
        if (p->data == value)   //�ҵ�Ŀ��ڵ㣬���ص�ַ
            return p;
    }
    return NULL;
}

//��ȡ�ض��ڵ��ǰ���ڵ�
List *PriorElem(List *p, List *cur_e)
{
    while (p = p->next) //���������ض��ڵ�ǰ��
    {
        if (p->next == cur_e)
            return p;   //����ǰ����ַ
    }
    return NULL;
}

//��ȡ�ض��ڵ�ĺ�̽ڵ�
List *NextElem(List *p, List *cur_e)
{
    return cur_e->next;
}

// ��ӡ�ض��ڵ�Ԫ�أ�ListTraverse����
void PrintElem(List *p)
{
    printf("%d -> ", p->data);
    return;
}

//��������ִ�в���
void ListTraverse(List *p, void (*func)(List *p))
{
    while (p = p->next) //������
        (*func)(p); //���ò�������
    printf("NULL\n");
    return;
}

//�������
void ClearList(List *p)
{
    while (p->next)
    {
        //ͨ����ʱ����tempɾ���ڵ㣬��ǰ����һ����ɾ
        List *temp = p->next;
        p->next = temp->next; //����Ҫɾ���ڵ��ǰ���ͺ��
        free(temp);
    }
    return;
}

//��������
void DestroyList(List *p)
{
    ClearList(p);   //�������
    free(p);    //freeͷ���
    return;
}

int main()
{
    List *p = InitList();   //��ʼ������
    ListInsert(p, 1, 2);    //�����������
    ListInsert(p, 1, 3);
    ListInsert(p, 2, 4);
    ListInsert(p, 2, 6);
    void (*Print)(List * p) = PrintElem;    //����ָ��ָ��print����
    ListTraverse(p, Print);    //������������
    printf("%d\n", PriorElem(p, LocateElem(p, 4))->data);   //��λ��������ȡǰ�����̺�������
    printf("%d\n", NextElem(p, LocateElem(p, 4))->data);
    ElemType *value = (ElemType*)malloc(sizeof(ElemType));
    ListDelete(p, 2, value);    //ɾ����������
    printf("%d\n", *value);
    ListTraverse(p, Print);
    ListDelete(p, 4, value);
    ListDelete(p, 1, value);
    ListTraverse(p, Print);
    printf("%d\n", GetElem(p, 1));  //���Һ�������
    printf("%d\n", GetElem(p, 2));
    printf("%d\n", GetElem(p, 3));
    printf("%d\n", LocateElem(p, 2)->data); //��λ��������
    ClearList(p);   //��պ�������
    ListTraverse(p, Print);
    DestroyList(p); //�����������
    printf("TRUE\n");   //���н��ȫ������Ԥ�����TRUE
    system("pause");
    return 0;
}