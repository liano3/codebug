#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXVERTEX 100
#define MAXEDGE 20000

//����ṹ��
typedef struct Vertex
{
    int id;                //������
    int w;                 //����Ȩֵ
    int degree;            //����Ķ�
    long edges[MAXVERTEX]; //�ߵı��
} Vertex;

//�߽ṹ��
typedef struct Edge
{
    long id; //�ߵı��
    int h;   //��ͷ������
    int t;   //��β������
    int w;   //Ȩֵ
} Edge;

//ͼ�ṹ��
typedef struct Graph
{
    Vertex v[MAXVERTEX]; //��������
    Edge e[MAXEDGE];     //������
    bool dirctional;     // t:����ͼ��f:����ͼ
    bool weighted;       // t:��Ȩͼ��f:��Ȩͼ
    unsigned nv;         //������
    unsigned ne;         //����
} Graph;

//ͼ�Ļ�������
int getVexIdx(Graph *g, int id);
long getEdgeIdx(Graph *g, long id);
//���Ҹ�����ŵĶ��㣬�����䶥�������±�
inline int getVexIdx(Graph *g, int id)
{
    for (int i = 0; i < g->nv; i++)
        if (g->v[i].id == id)
            return i;
    printf("input wrong vertex id in getVexIdx()!\n");
    system("pause");
    exit(0);
}
//���Ҹ�����ŵıߣ�������������±�
inline long getEdgeIdx(Graph *g, long id)
{
    for (long i = 0; i < g->ne; i++)
        if (g->e[i].id == id)
            return i;
    printf("input wrong edge id in getEdgeIdx()!\n");
    system("pause");
    exit(0);
}

//����һ�����ͼ������n�����㣬ÿ������������������ߵĸ���Ϊp
int randgengraph(int n, float p, bool directional, bool weighted, const char *filename, int seed)
{
    srand(seed);
    char *content = (char *)malloc(5000000);
    if (!content)
    {
        printf("����洢�ռ�ʧ�ܣ�\n");
        system("pause");
        exit(0);
    }
    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        printf("����ͼ�ļ�%sʱʧ�ܣ�\n", filename);
        system("pause");
        exit(0);
    }
    long offset = 0; // content��ǰ��д��λ��
    int ne = 0;      //���ɱ���
    for (int i = 0; i < n; i++)
        offset += sprintf(content + offset, "%3d %5d\n", i, rand() % 100); //д�붥��ı�źͶ������Ȩֵ
    int pn = (int)(10000 * p);
    if (directional)
    { //����ͼ
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                if (i != j && rand() % 10000 < pn)
                {
                    offset += sprintf(content + offset, "%3d %3d %4d\n", i, j, weighted ? rand() % 100 : 1);
                    ne++;
                }
    }
    else
    { //����ͼ
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
    fwrite(&ch, sz, 1, fp);         //д��ͼ�Ķ������ͱ���
    fwrite(content, offset, 1, fp); //д��ͼ�Ķ���ͱߵ���Ϣ
    fclose(fp);
    free(content);
}

//���ļ��ж���ͼ�����ڴ�������ͼ�ı�ʾ
void initgraph(Graph *g, const char *gfile)
{
    FILE *fp = fopen(gfile, "r");
    if (!fp)
    {
        printf("��ȡͼ�����ļ�����\n");
        system("pause");
        exit(0);
    }
    char bv[10];
    //��ȡͼ�Ļ�����Ϣ
    fscanf(fp, "%u%u%s", &(g->nv), &(g->ne), bv);
    printf("%d %d %s", g->ne, g->nv, bv);
    g->weighted = strcmp(bv, "true") == 0 ? true : false;
    fscanf(fp, "%s", bv);
    g->dirctional = strcmp(bv, "true") == 0 ? true : false;
    //��ȡ������Ϣ
    for (int i = 0; i < g->nv; i++)
    {
        fscanf(fp, "%d%d", &(g->v[i].id), &(g->v[i].w));
        g->v[i].degree = 0;
        // printf("����%d-Ȩ%d\n", g->v[i].id, g->v[i].w);
    }
    //��ȡ����Ϣ
    for (unsigned long i = 0; i < g->ne; i++)
    {
        fscanf(fp, "%u%u%d", &(g->e[i].h), &(g->e[i].t), &(g->e[i].w));
        // printf("��:%u-%u=>%d\n", g->e[i].h, g->e[i].t, g->e[i].w);
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

//��ӡ�ڽӱ����Ϣ
void printgraph(Graph *g)
{
    printf("ͼ�Ļ�����Ϣ:������(%u)-����(%lu)-%s-%s\n", g->nv, g->ne, g->weighted ? "��Ȩͼ" : "��Ȩͼ", g->dirctional ? "����ͼ" : "����ͼ");
    for (int i = 0; i < g->nv; i++)
    {
        printf("ID(%u)-��(%d)-Ȩ(%d)-�߱� t(eid|w):%u", g->v[i].id, g->v[i].degree, g->v[i].w, g->v[i].id);
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

//�����������
void BFS(Graph *g, int id, bool Visited[])
{
    //�洢�����߽磬Fringe[0]���Ԫ�ظ���
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

//�����������
void DFS(Graph *g, int id, bool Visited[])
{
    //�洢�����߽磬Fringe[0]���Ԫ�ظ���
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

// Path[i]�洢id��i�����·�����ȣ���·����i��ǰ��
void Dijkstra(Graph *g, int id, int Path[][2])
{
    // Visited�洢�Ѿ��ҵ����·���Ķ���
    bool Visited[g->nv];
    memset(Visited, 0, sizeof(Visited));
    //�����߽磬��·����������
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

//��ͨƬ����
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
    //����ͼ
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
    // ����DFS
    bool Visited[g->nv];
    memset(Visited, 0, sizeof(Visited));
    DFS(g, 0, Visited);
    printf("\n");
    //����BFS
    memset(Visited, 0, sizeof(Visited));
    BFS(g, 0, Visited);
    printf("\n");
    //������ͨƬ����
    printf("%d\n", Wnum(g));
    //�������·��
    int Path[g->nv][2];
    Dijkstra(g, 0, Path);
    for (int i = 0; i < g->nv; i++)
    {
        printf("%d %d\n", Path[i][0], Path[i][1]);
    }
    system("pause");
}