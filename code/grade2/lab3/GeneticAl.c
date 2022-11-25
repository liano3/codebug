//�Ŵ��㷨������Ż������ͨ���㷨
//������Ľ�x��ʾΪ0-1����Ȼ���������ŵĶ����ƴ���ʹ��Ŀ�꺯��ֵf(x)�ﵽ��С

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** �㷨�������� **/
//����0-1���ĳ��ȣ������ڱ�ʾ��ľ���
#define SLEN 200

//�������������
#define MAXGEN 100

//�������
#define mProb 1.0 / SLEN

//��Ⱥ������Ⱥ���С���Ӵ�������ż����ͨ������Ⱦɫ�����ɺ��ʱ��һ���������������
#define PSIZE 10
#define CSIZE 20
#define SIZE (PSIZE + CSIZE)

//һ����Ķ���
typedef struct
{
    int x[SLEN]; // x:����Ա�����0-1��
    double y;    // y=f(x),Ҫ�Ż������Ŀ�꺯��ֵ
} Solution;

//����һ���⼯/�����飺��֮ΪȺ��population
Solution pop[SIZE];               //�⼯���������Ӵ����洢������
Solution *parent = pop;           //��ǰ����Ҳ���Ǹ���
Solution *children = pop + PSIZE; //�Ӵ��⼯

//��������
void printPop(Solution *p, const char *str);

//��0-1��x����Ϊʵ��*xo ,�ٶ�����4bits��SLEN-4bitsΪС�����ֳ���
void decode(int *x, double *xo)
{
    //�������ֻ���10����
    int inte = 0, base = 1;
    for (int i = 3; i >= 0; i--)
    {
        inte += x[i] * base;
        base *= 2;
    }
    *xo = (double)inte;
    //С�����ֻ���10����
    double deci = 0, base1 = 0.5;
    for (int i = 4; i < SLEN; i++)
    {
        deci += x[i] * base1;
        base1 *= 0.5;
    }
    *xo = *xo + deci;
    return;
}

//����y=f(x) ,  0-1��x�ĳ��� SLEN
//���ӣ���y=x*x-3x+2����Сֵ ��������������4bits��С������SLEN-4bits
double func1(int *x)
{
    double xo;
    decode(x, &xo);              //��0-1��x����������Ľ�xo
    return xo * xo - 3 * xo + 2; //����Ŀ�꺯��ֵ
}

//����һ��Ⱥ������н��Ŀ�꺯��ֵy �������˺���ָ�룬֧�ָ��������Ż�
void evaluate(Solution *P, double ptrf(int *))
{
    for (int i = 0; i < SIZE; i++)
    {
        (P + i)->y = ptrf((P + i)->x);
    }
    return;
}

//�㷨��ʼ�������������⼯����Ŀռ䣬�������currentPop�еĽ⣬��������yֵ
void initialize()
{
    for (int i = 0; i < PSIZE; ++i)
    {                                  //��ʼ����һ������
        for (int j = 0; j < SLEN; ++j) //��ÿ�����0-1�����������
            parent[i].x[j] = rand() % 2;
    }
    evaluate(parent, func1);
}

//�Ӹ�����ѡ�������⣬ͨ���ӽ����������Ӵ�����
//����������ͨ��PKѡ�����������ѡ��
void crossover()
{ //��������
    int k = 0;
    while (k < CSIZE)
    { //�������Ӵ���һ������
        //���ѡ��������������
        int i = rand() % PSIZE, j = rand() % PSIZE;
        while (j == i)
            j = rand() % PSIZE;
        Solution *father = parent + i, *mother = parent + j;
        //���ȷ����������Ⱦɫ�彻��λ��
        int pos = rand() % SLEN;
        //��������λ��ǰ��������Ӵ�
        Solution child1 = *father;
        Solution child2 = *mother;
        for (int i = 0; i < pos; i++)
        {
            child1.x[i] = mother->x[i];
            child2.x[i] = father->x[i];
        }
        *(children + k) = child1;
        *(children + k + 1) = child2;
        k = k + 2;
    }
}

//���Ӵ��еĸ�����б��죺�������ΪmProb
//��ν�������x[j]��ȡֵ 0-1������ 0 <--> 1
void mutate()
{ //��������
    for (int i = 0; i < CSIZE; ++i)
        for (int j = 0; j < SLEN; ++j)
            if ((rand() % 100000) / 100000.0 < mProb)
                children[i].x[j] = 1 - children[i].x[j];
}

//��currentPop��offspring��ѡ����һ�����壬�ж���ѡ���㷨������ͨ�������Ȱ�����Ⱥ������õı������� ��Ȼ��
//����1��ѡ����õ�PSIZE��Ϊ��һ�����ض�ѡ��
//����2����ÿ������һ��ѡ����ʣ�yֵС���ã��ı�ѡ��ĸ��ʾ͸ߣ�Ȼ�����ݴ˸��ʷֲ��������PSIZE��
//����3������ѡ�����ѡ��k�����໥pk��������õķ�����һ��������ѡ��PSIZE�� ����ɾ����ѡ���˵ģ�
void select(int k)
{                 //ѡ������ �����ý���ѡ��
    double besty; //������ѡ�������Ӵ���yֵ
    int best;     //������ѡ��������Ӵ��±�
    Solution tmp[PSIZE];
    for (int count = 0; count < PSIZE; ++count)
    { //һ��һ���Ӵ�ѡ��
        //��pop[2*psize]���÷���1��2��3ѡ��һ����Ӧ��ֵ��yֵ���õ�
        //ѡ�񷽷��������PK
        int i = rand() % CSIZE, j = rand() % CSIZE;
        while (i == j)
            j = rand() % CSIZE;
        Solution *player1 = children + i, *player2 = children + j;
        best = (player1->y < player2->y ? i : j) + PSIZE;
        memcpy(&(tmp[count]), &(pop[best]), sizeof(Solution)); //ѡ������Ľ⣬���Ƶ���ʱ�⼯��
    }
    memcpy(parent, tmp, sizeof(Solution) * PSIZE);
}

//���Ⱥ�����Ϣ
void printPop(Solution *p, const char *str)
{
    printf("%s/�⼯��Ϣ����: \n", str);
    for (int i = 0; i < PSIZE; ++i)
    {
        printf("���� %3d : y=%10.6lf=f(", i, p[i].y);
        for (int j = 0; j < SLEN; ++j)
            printf("%d", p[i].x[j]);
        printf(")\n");
    }
}

int main()
{
    int seed = 991;
    srand(seed); //������������ӣ�ʹ���㷨�����������
    initialize();
    printf("�� %d �� ", 0);
    printPop(parent, "��ǰȺ��");

    for (int gen = 1; gen < MAXGEN; gen++)
    {
        crossover();
        mutate();
        evaluate(children, func1);
        select(2);
        printf("�� %d �� ", gen);
        printPop(parent, "��ǰȺ��");
    }
    printf("�� %d �� ", MAXGEN);
    printPop(parent, "����Ⱥ��");
    system("pause");
    return 1;
}
