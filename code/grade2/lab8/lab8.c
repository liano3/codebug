#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXVERTEX 100
#define MAXEDGE 20000

//顶点结构体
typedef struct Vertex
{
    int id;                //顶点编号
    int w;                 //顶点权值
    int degree;            //顶点的度
    long edges[MAXVERTEX]; //边的编号
} Vertex;

//边结构体
typedef struct Edge
{
    long id; //边的编号
    int h;   //边头顶点编号
    int t;   //边尾顶点编号
    int w;   //权值
} Edge;

//图结构体
typedef struct Graph
{
    Vertex v[MAXVERTEX]; //顶点数组
    Edge e[MAXEDGE];     //边数组
    bool dirctional;     // t:有向图，f:无向图
    bool weighted;       // t:带权图，f:等权图
    unsigned nv;         //顶点数
    unsigned ne;         //边数
} Graph;

//图的基本操作
int getVexIdx(Graph *g, int id);
long getEdgeIdx(Graph *g, long id);
//查找给定编号的顶点，返回其顶点数组下标
inline int getVexIdx(Graph *g, int id)
{
    for (int i = 0; i < g->nv; i++)
        if (g->v[i].id == id)
            return i;
    printf("input wrong vertex id in getVexIdx()!\n");
    system("pause");
    exit(0);
}
//查找给定编号的边，返回其边数组下标
inline long getEdgeIdx(Graph *g, long id)
{
    for (long i = 0; i < g->ne; i++)
        if (g->e[i].id == id)
            return i;
    printf("input wrong edge id in getEdgeIdx()!\n");
    system("pause");
    exit(0);
}

//生成一个随机图，包括n个顶点，每个顶点和其它顶点连边的概率为p
int randgengraph(int n, float p, bool directional, bool weighted, const char *filename, int seed)
{
    srand(seed);
    char *content = (char *)malloc(5000000);
    if (!content)
    {
        printf("分配存储空间失败！\n");
        system("pause");
        exit(0);
    }
    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        printf("生成图文件%s时失败！\n", filename);
        system("pause");
        exit(0);
    }
    long offset = 0; // content当前的写入位置
    int ne = 0;      //生成边数
    for (int i = 0; i < n; i++)
        offset += sprintf(content + offset, "%3d %5d\n", i, rand() % 100); //写入顶点的编号和顶点随机权值
    int pn = (int)(10000 * p);
    if (directional)
    { //有向图
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (i != j && rand() % 10000 < pn)
                {
                    offset += sprintf(content + offset, "%3d %3d %4d\n", i, j, weighted ? rand() % 100 : 1);
                    ne++;
                }
    }
    else
    { //无向图
        for (int i = 0; i < n; i++)
            for (int j = i; j < n; j++)
                if (i != j && rand() % 10000 < pn)
                {
                    offset += sprintf(content + offset, "%3d %3d %4d\n", i, j, weighted ? rand() % 100 : 1);
                    ne++;
                }
    }
    char ch[40];
    int sz = sprintf(ch, "%d %7d %5s %5s\n", n, ne, weighted ? "true" : "false", directional ? "true" : "false");
    fwrite(&ch, sz, 1, fp);         //写入图的顶点数和边数
    fwrite(content, offset, 1, fp); //写入图的顶点和边的信息
    fclose(fp);
    free(content);
}

//从文件中读入图，在内存中生成图的表示
void initgraph(Graph *g, const char *gfile)
{
    FILE *fp = fopen(gfile, "r");
    if (!fp)
    {
        printf("读取图数据文件出错！\n");
        system("pause");
        exit(0);
    }
    char bv[10];
    //读取图的基本信息
    fscanf(fp, "%u%u%s", &(g->nv), &(g->ne), bv);
    printf("%d %d %s", g->ne, g->nv, bv);
    g->weighted = strcmp(bv, "true") == 0 ? true : false;
    fscanf(fp, "%s", bv);
    g->dirctional = strcmp(bv, "true") == 0 ? true : false;
    //读取顶点信息
    for (int i = 0; i < g->nv; i++)
    {
        fscanf(fp, "%d%d", &(g->v[i].id), &(g->v[i].w));
        g->v[i].degree = 0;
        // printf("顶点%d-权%d\n", g->v[i].id, g->v[i].w);
    }
    //读取边信息
    for (unsigned long i = 0; i < g->ne; i++)
    {
        fscanf(fp, "%u%u%d", &(g->e[i].h), &(g->e[i].t), &(g->e[i].w));
        // printf("边:%u-%u=>%d\n", g->e[i].h, g->e[i].t, g->e[i].w);
        g->e[i].id = i;
        unsigned h, t;
        h = g->e[i].h;
        t = g->e[i].t;
        g->v[h].edges[g->v[h].degree++] = i;
        if (g->dirctional == false)
            g->v[t].edges[g->v[t].degree++] = i;
    }
    fclose(fp);
}

//打印邻接表的信息
void printgraph(Graph *g)
{
    printf("图的基本信息:顶点数(%u)-边数(%lu)-%s-%s\n", g->nv, g->ne, g->weighted ? "加权图" : "等权图", g->dirctional ? "有向图" : "无向图");
    for (int i = 0; i < g->nv; i++)
    {
        printf("ID(%u)-度(%d)-权(%d)-边表 t(eid|w):%u", g->v[i].id, g->v[i].degree, g->v[i].w, g->v[i].id);
        for (int j = 0; j < g->v[i].degree; j++)
        {
            unsigned long e = g->v[i].edges[j];
            int pos = getEdgeIdx(g, e);
            if (g->e[pos].h == g->v[i].id)
                printf("->(%d-%u(%d))", e, g->e[pos].t, g->e[pos].w);
            else
                printf("->(%d-%u(%d))", e, g->e[pos].h, g->e[pos].w);
        }
        printf("\n");
    }
}

//广度优先搜索
void BFS(Graph *g, int id, bool Visited[])
{
    //存储搜索边界，Fringe[0]存放元素个数
    int Fringe[MAXVERTEX];
    Fringe[0] = 1, Fringe[1] = id;
    while (Fringe[0])
    {
        // REMOVE
        int s = Fringe[1];
        for (int i = 2; i <= Fringe[0]; i++)
            Fringe[i - 1] = Fringe[i];
        Fringe[0]--;
        if (!Visited[s])
        {
            // UPDATE PATH
            Visited[s] = true;
            printf("%d->", s);
            int pos = getVexIdx(g, s);
            // UPDATE FRINGE
            for (int i = 0; i < g->v[pos].degree; i++)
            {
                int edge_id = g->v[pos].edges[i];
                int ptr = getEdgeIdx(g, edge_id);
                int next_id = (g->e[ptr].h == s ? g->e[ptr].t : g->e[ptr].h);
                if (!Visited[next_id])
                    Fringe[++Fringe[0]] = next_id;
            }
        }
    }
}

//深度优先搜索
void DFS(Graph *g, int id, bool Visited[])
{
    //存储搜索边界，Fringe[0]存放元素个数
    int Fringe[MAXVERTEX];
    Fringe[0] = 1, Fringe[1] = id;
    while (Fringe[0])
    {
        // REMOVE
        int s = Fringe[1];
        for (int i = 2; i <= Fringe[0]; i++)
            Fringe[i - 1] = Fringe[i];
        Fringe[0]--;
        if (!Visited[s])
        {
            // UPDATE PATH
            Visited[s] = true;
            printf("%d->", s);
            int pos = getVexIdx(g, s);
            // UPDATE FRINGE
            int degree = g->v[pos].degree;
            for (int i = 0; i < degree; i++)
            {
                int edge_id = g->v[pos].edges[i];
                int ptr = getEdgeIdx(g, edge_id);
                int next_id = (g->e[ptr].h == s ? g->e[ptr].t : g->e[ptr].h);
                if (!Visited[next_id])
                {
                    for (int i = Fringe[0]; i >= 1; i--)
                        Fringe[i + 1] = Fringe[i];
                    Fringe[1] = next_id;
                    Fringe[0]++;
                }
            }
        }
    }
}

// Path[i]存储id到i的最短路径长度，及路径中i的前驱
void Dijkstra(Graph *g, int id, int Path[][2])
{
    // Visited存储已经找到最短路径的顶点
    bool Visited[g->nv];
    memset(Visited, 0, sizeof(Visited));
    //搜索边界，按路径长度升序
    int Fringe[MAXVERTEX];
    Fringe[0] = 1, Fringe[1] = id;
    for (int i = 0; i < g->nv; i++)
    {
        Path[i][0] = 1000;
        Path[i][1] = -1;
    }
    Path[id][0] = 0;
    while (Fringe[0])
    {
        // REMOVE
        int s = Fringe[1];
        for (int i = 2; i <= Fringe[0]; i++)
            Fringe[i - 1] = Fringe[i];
        Fringe[0]--;
        if (!Visited[s])
        {
            Visited[s] = true;
            int pos = getVexIdx(g, s);
            for (int i = 0; i < g->v[pos].degree; i++)
            {
                int ptr = getEdgeIdx(g, g->v[pos].edges[i]);
                int next_id = (g->e[ptr].h == s ? g->e[ptr].t : g->e[ptr].h);
                int cur = Path[s][0] + g->e[ptr].w;
                if (cur < Path[next_id][0])
                {
                    // UPDATE PATH
                    Path[next_id][0] = cur;
                    Path[next_id][1] = s;
                    // UPDATE FRINGE
                    int insert_pos = 1;
                    for (int j = 1; j <= Fringe[0]; j++)
                    {
                        if (cur > Fringe[j])
                            insert_pos = j + 1;
                    }
                    for (int j = Fringe[0]; j >= insert_pos; j--)
                        Fringe[j + 1] = Fringe[j];
                    Fringe[insert_pos] = next_id;
                    Fringe[0]++;
                }
            }
        }
    }
}

//联通片数量
int Wnum(Graph *g)
{
    int count = 0;
    bool Visited[g->nv];
    memset(Visited, 0, sizeof(Visited));
    for (int i = 0; i < g->nv; i++)
    {
        if (!Visited[g->v[i].id])
        {
            count++;
            BFS(g, g->v[i].id, Visited);
            printf("\n");
        }
    }
    return count;
}

int main()
{
    //生成图
    Graph *g = (Graph *)malloc(sizeof(Graph));
    if (!g)
    {
        printf("error\n");
        system("pause");
        exit(0);
    }
    randgengraph(10, 0.5, 1, 1, "graph.txt", 5);
    initgraph(g, "graph.txt");
    printgraph(g);
    // 测试DFS
    bool Visited[g->nv];
    memset(Visited, 0, sizeof(Visited));
    DFS(g, 0, Visited);
    printf("\n");
    //测试BFS
    memset(Visited, 0, sizeof(Visited));
    BFS(g, 0, Visited);
    printf("\n");
    //测试联通片数量
    printf("%d\n", Wnum(g));
    //测试最短路径
    int Path[g->nv][2];
    Dijkstra(g, 0, Path);
    for (int i = 0; i < g->nv; i++)
    {
        printf("%d %d\n", Path[i][0], Path[i][1]);
    }
    system("pause");
}