#ifndef _NETWORK_CPP_
#define _NETWORK_CPP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXVERTEX 100
#define MAXEDGE 20000

//顶点结构体
typedef struct Vertex
{
	int id;				   //顶点编号
	int w;				   //顶点权值
	int degree;			   //顶点的度
	long edges[MAXVERTEX]; //边的编号
} Vertex;

//边结构体
typedef struct Edge
{
	long id; //边的编号
	int h;	 //边头顶点编号
	int t;	 //边尾顶点编号
	int w;	 //权值
} Edge;

//图结构体
typedef struct Graph
{
	Vertex v[MAXVERTEX]; //顶点数组
	Edge e[MAXEDGE];	 //边数组
	bool dirctional;	 // t:有向图，f:无向图
	bool weighted;		 // t:带权图，f:等权图
	unsigned nv;		 //顶点数
	unsigned ne;		 //边数
} Graph;

#include "showgt.h"
long *getEdgesByNode(Graph *g, int id, int *ne);
int getVexIdx(Graph *g, int id);
void getVexByEdge(Graph *g, long id, int *h, int *t);
long getEdgeIdx(Graph *g, long id);

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
	int ne = 0;		 //生成边数
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
	fwrite(&ch, sz, 1, fp);			//写入图的顶点数和边数
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

//查找给定编号的顶点，返回其权值
inline int getVexW(Graph *g, int id)
{
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].id == id)
			return g->v[i].w;
	printf("input wrong vertex id in getVexW()!\n");
	system("pause");
	exit(0);
}

//查找给定编号的顶点，返回其度
inline int getVexDegree(Graph *g, int id)
{
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].id == id)
			return g->v[i].degree;
	printf("input wrong vertex id in getVexDegree()!\n");
	system("pause");
	exit(0);
}

//查找给定编号的顶点,返回其所有边
inline long *getEdgesByNode(Graph *g, int id, int *ne)
{
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].id == id)
		{
			*ne = g->v[i].degree;
			return g->v[i].edges;
		}
	printf("input wrong vertex id in getEdgesByNode()!\n");
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

//查找给定编号的边，返回其权值
inline int getEdgeW(Graph *g, long id)
{
	for (long i = 0; i < g->ne; i++)
		if (g->e[i].id == id)
			return g->e[i].w;
	printf("input wrong edge id in getEdgeW()!\n");
	system("pause");
	exit(0);
}

//查找给定编号的边，返回其两个顶点
inline void getVexByEdge(Graph *g, long id, int *h, int *t)
{
	for (long i = 0; i < g->ne; i++)
		if (g->e[i].id == id)
		{
			*h = g->e[i].h;
			*t = g->e[i].t;
			return;
		}
	printf("input wrong edge id in getVexByEdge()!\n");
	system("pause");
	exit(0);
}

//通过权值查找顶点编号
inline int locateVex(Graph *g, int w)
{
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].w == w)
			return g->v[i].id;
	return -1;
}

//通过权值查找边编号
inline int locateEdge(Graph *g, int w)
{
	for (int i = 0; i < g->ne; i++)
		if (g->e[i].w == w)
			return g->e[i].id;
	return -1;
}

//设置顶点权值
inline void setVexW(Graph *g, int id, int w)
{
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].id == id)
			g->v[i].w = w;
	printf("input wrong vertex id in setVexW()!\n");
	system("pause");
	exit(0);
}

//设置边权值
inline void setEdgeW(Graph *g, int id, int w)
{
	for (long i = 0; i < g->ne; i++)
		if (g->e[i].id == id)
			g->e[i].w = w;
	printf("input wrong edge id in setEdgeW()!\n");
	system("pause");
	exit(0);
}

//删除边
void deleteEdge(Graph *g, int id)
{
	//首先找到边两端顶点位置
	int h, t;
	getVexByEdge(g, id, &h, &t);
	int pos1 = getVexIdx(g, h);
	//删除顶点数组中关于此边的信息
	for (int i = 0; i < g->v[pos1].degree; i++)
	{ //左端顶点
		if (g->v[pos1].edges[i] == id)
		{
			for (int j = i + 1; j < g->v[pos1].degree; j++)
				g->v[pos1].edges[j - 1] = g->v[pos1].edges[j];
			g->v[pos1].degree--; //度数--
			break;
		}
	}
	if (!g->dirctional) {
		int pos2 = getVexIdx(g, t);
		//删除顶点数组中关于此边的信息
		for (int i = 0; i < g->v[pos2].degree; i++)
		{ //左端顶点
			if (g->v[pos2].edges[i] == id)
			{
				for (int j = i + 1; j < g->v[pos2].degree; j++)
					g->v[pos2].edges[j - 1] = g->v[pos2].edges[j];
				g->v[pos2].degree--; //度数--
				break;
			}
		}
	}
	//删除边数组中关于此边的信息
	int pos3 = getEdgeIdx(g, id);
	for (int i = pos3 + 1; i < g->ne; i++)
		g->e[i - 1] = g->e[i];
	//边数--
	g->ne--;
}

//删除顶点
void deleteVex(Graph *g, int id)
{
	//找到顶点所在位置
	int pos = getVexIdx(g, id);
	//删除所有与它关联的边
	for (int i = 0; i < g->ne; i++) {
		if (g->e[i].h == id || g->e[i].t == id) {
			deleteEdge(g, g->e[i].id);
			i--;
		}
	}
	//删除顶点
	for (int i = pos + 1; i < g->nv; i++)
		g->v[i - 1] = g->v[i];
	g->nv--; //顶点数--
}

//增加顶点
void addVex(Graph *g, int w)
{
	Vertex temp;
	//赋初值
	temp.id = g->v[g->nv - 1].id + 1, temp.degree = 0, temp.w = w;
	//加入顶点数组
	g->v[g->nv++] = temp;
}

//增加边
void addEdge(Graph *g, int w, int h, int t)
{
	Edge temp;
	//赋初值
	temp.h = h, temp.t = t, temp.w = w;
	temp.id = g->e[g->ne - 1].id + 1;
	//加入边数组
	g->e[g->ne++] = temp;
	//更新顶点信息
	int pos1 = getVexIdx(g, h);
	g->v[pos1].edges[g->v[pos1].degree++] = temp.id;
	if (!g->dirctional) {
		int pos2 = getVexIdx(g, t);
		g->v[pos2].edges[g->v[pos2].degree++] = temp.id;
	}
}

//以 id 为起点深度优先遍历
void dfs(Graph *g, int id, bool visited[])
{
	//标记已访问
	visited[id] = true;
	int pos = getVexIdx(g, id);
	//输出id与权重
	printf("->%d(%d)", g->v[pos].id, g->v[pos].w);
	for (int i = 0; i < g->v[pos].degree; i++)
	{
		int e = g->v[pos].edges[i];
		int pos1 = getEdgeIdx(g, e);
		if (!visited[g->e[pos1].t])
			dfs(g, g->e[pos1].t, visited);
		else if (!visited[g->e[pos1].h])
			dfs(g, g->e[pos1].h, visited);
	}
}

// dfs，遍历整张图，依次输出遍历时经过的节点
void dfs_traverse(Graph *g)
{
	//定义标记数组并初始化
	bool visited[MAXVERTEX];
	memset(visited, 0, sizeof(visited));
	//遍历每个联通片
	for (int i = 0; i < g->nv; i++)
	{
		if (!visited[g->v[i].id])
		{
			dfs(g, g->v[i].id, visited);
			printf("\n");
		}
	}
}

int main()
{
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

	addVex(g, 30);
	addEdge(g, 50, 1, 3);
	printgraph(g);

	int h;
	int t;
	getVexByEdge(g, 10, &h, &t);
	printf("%d %d\n", h, t);

	deleteEdge(g, 10);
	deleteVex(g, 5);
	printgraph(g);
	saveGraph(g, "net.html");

	dfs_traverse(g);
	printf("\n");
	
	bool visited1[MAXVERTEX];
	memset(visited1, 0, sizeof(visited1));
	dfs(g, 2, visited1);
	
	system("pause");
}
#endif