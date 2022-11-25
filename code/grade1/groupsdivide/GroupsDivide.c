/*
问题描述：
    给定具有n（30<=n<=200)个学生的班级，男女比接近1:1, 大佬人数大约10%
    读入含有班级同学信息的txt文件，包括学号（3位数字），性别，是否是大佬。
    根据得到的信息进行随机分组。
    分组要求如下：
    1.每组人数为5-6人
    2.每组都含有男生和女生，且为使组员不孤单，男生至少两人，女生至少两人。
    3.为公平起见，大佬数目应尽量分配均匀
    4.若组内没有大佬，则小组人数为6
    5.为使更多人认识其他同学，每组内任意两人学号差需不小于5(可以不完全满足)
交互方式：
    输入：班级人数n，含班级学生信息的txt文件，命名为"information.txt"
    输出：含分组结果的txt文件，名称为"result.txt"
    格式：详见createdata.c
备注：
    由于分组要求对于n比较小的情况可能无解
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define N 200
#define swap(a, b) \
    {              \
        int t = a; \
        a = b;     \
        b = t;     \
    }
typedef struct Student
{
    int order;  //学号
    char gender;    //性别
    int abi;       //是否为大佬
} STU;
STU arr[N];     //存放所有学生信息
int groups[N / 5 + 1][6];   //存放分组信息
int groupsnum;  //总组数
int conflicts;  //冲突数
int heart[N], lao = 0;  //大佬的位置，个数
int mark[N] = {0};  //人员占用情况
int flag = 0;   //是否无解
FILE *fp1, *fp2;

void infor_read()
{
    //打开文件
    fp1 = fopen("information.txt", "r");
    fp2 = fopen("result.txt", "w");
    //读入信息
    memset(heart, 0, sizeof(int) * N);
    for (int i = 0; i < N; i++)
    {
        fscanf(fp1, "%d\t%c\t%d\n", &arr[i].order, &arr[i].gender, &arr[i].abi);
        if (arr[i].abi == 1)    //记录大佬的位置
            heart[lao++] = i;
    }
    fclose(fp1);
}

void random_set()
{
    flag = 0;
    srand((unsigned)time(NULL)); //保证随机性
    //确定5人组和6人组的个数
    int numsix = (N - lao * 5) / 6; //不含大佬的安排为6人组
    int laosix = (N - lao * 5) % 6; //剩下的不到6个人分配给含大佬的5人组
    if (laosix != 0)
        numsix += laosix;
    if (laosix > lao) //剩下的人放大佬所在组放不下
    {
        flag = 1;
    }
    groupsnum = numsix - laosix + lao; //总组数
    //随机生成初始解
    memset(groups, 0, sizeof(int) * groupsnum * 6);
    for (int i = 0; i < lao; i++) //先把大佬占用，防止后面随机找组员时找到大佬
        mark[heart[i]] = 1;
    for (int i = 0; i < lao; i++)
    {
        groups[i][0] = heart[i]; //安插大佬，heart[i]记录了第i+1个佬在结构体数组中的位置
        for (int j = 1; j < 5; j++)
        { // 5人小组前5个人安排好
            while (1)
            {
                int k = rand() % N;
                if (mark[k] == 0)
                {
                    groups[i][j] = k;
                    mark[k] = 1;
                    break;
                }
            }
        }
        if (i >= lao - laosix)
        { //考虑6人小组的第六个人
            while (1)
            {
                int k = rand() % N;
                if (mark[k] == 0)
                {
                    groups[i][5] = k;
                    mark[k] = 1;
                    break;
                }
            }
        }
        else
            groups[i][5] = N + 1; // 5人小组的第六个位置赋值为在正常分配时取不到的N+1，代表此处无人
    }
    for (int i = lao; i < groupsnum; i++)
    { //不含佬的6人组随机分配
        for (int j = 0; j < 6; j++)
        {
            while (1)
            {
                int k = rand() % N;
                if (mark[k] == 0)
                {
                    groups[i][j] = k;
                    mark[k] = 1;
                    break;
                }
            }
        }
    }
}

void conflict_sum()
{
    conflicts = 0;  //计数前先清零
    int girls = 0, boys = 0;    //男生女生个数
    for (int i = 0; i < groupsnum; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            if (groups[i][j] == N + 1)
                continue;
            for (int k = j + 1; k < 6; k++)
            {
                if (groups[i][k] == N + 1)
                    continue;
                //学号之差不符合
                if (arr[groups[i][j]].order - arr[groups[i][k]].order < 5 && arr[groups[i][j]].order - arr[groups[i][k]].order > -5)
                    conflicts++;
            }
            if (arr[groups[i][j]].gender == 'F')
                girls++;
            else
                boys++;
        }
        //男女生数目不符合
        if (girls < 2)
            conflicts += (2 - girls);
        else if (boys < 2)
            conflicts += (2 - boys);
        girls = 0;
        boys = 0;
    }
}

void result_print()
{
    for (int i = 0; i < groupsnum; i++)
    {
        fprintf(fp2, "Group%d:\n", i + 1);
        for (int j = 0; j < 6; j++)
        {
            if (groups[i][j] != N + 1)
                //根据数组中存放的位置信息，回到结构体数组中把学生信息打印出来
                fprintf(fp2, "%03d\t%c\t%d\n", arr[groups[i][j]].order, arr[groups[i][j]].gender, arr[groups[i][j]].abi);
        }
        fprintf(fp2, "\n");
    }
    fclose(fp2);
}

int main()
{
    //读入学生信息
    infor_read();
    //分组过程
    for (int count = 0;; count++)
    {                 //最外层循环，便于重启
        random_set(); //生成初始解
        if (flag == 1)
        { //不可能满足要求，退出
            printf("no solution!\n");
            fclose(fp2);
            system("pause");
            return 0;
        }
        conflict_sum(); //计算初始冲突
        if (conflicts == 0)
        { //冲突为0，返回
            printf("results are ready.\n");
            printf("conflictsnum:%d\n", conflicts);
            result_print();
            system("pause");
            return 0;
        }
        for (int i = 0; i < groupsnum; i++)
        {
            for (int j = 1; j < 6; j++)
            { //从每组的第二个人开始，不变动每组组长的位置
                if (groups[i][j] == N + 1)
                    continue;
                for (int m = 0; m < groupsnum; m++)
                {
                    if (m == i)
                        continue;
                    for (int n = 1; n < 6; n++)
                    {
                        if (groups[m][n] == N + 1)
                            continue;
                        swap(groups[i][j], groups[m][n]); //宏定义的交换
                        int temp = conflicts;
                        conflict_sum(); //重新计算冲突
                        if (conflicts >= temp)
                        { //没减少，换回去
                            swap(groups[i][j], groups[m][n]);
                            conflicts = temp;
                        }
                        if (conflicts == 0)
                        { //冲突为0，返回
                            printf("results are ready.\n");
                            printf("conflictsnum:%d\n", conflicts);
                            result_print();
                            system("pause");
                            return 0;
                        }
                    }
                }
            }
        }
        memset(mark, 0, sizeof(mark)); //重启前重置记录人员占用情况的数组
        if (count > 20)
        { //重启很多次都无法降到0，返回结果
            printf("results are ready.\n");
            printf("conflictsnum:%d\n", conflicts);
            result_print();
            system("pause");
            return 0;
        }
    }
}
