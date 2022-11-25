#ifndef _NETWORK_CPP_
#define _NETWORK_CPP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAXVERTEX 100
#define MAXEDGE 20000

//����ṹ��
typedef struct Vertex
{
	int id;				   //������
	int w;				   //����Ȩֵ
	int degree;			   //����Ķ�
	long edges[MAXVERTEX]; //�ߵı��
} Vertex;

//�߽ṹ��
typedef struct Edge
{
	long id; //�ߵı��
	int h;	 //��ͷ������
	int t;	 //��β������
	int w;	 //Ȩֵ
} Edge;

//ͼ�ṹ��
typedef struct Graph
{
	Vertex v[MAXVERTEX]; //��������
	Edge e[MAXEDGE];	 //������
	bool dirctional;	 // t:����ͼ��f:����ͼ
	bool weighted;		 // t:��Ȩͼ��f:��Ȩͼ
	unsigned nv;		 //������
	unsigned ne;		 //����
} Graph;

#include "showgt.h"
long *getEdgesByNode(Graph *g, int id, int *ne);
int getVexIdx(Graph *g, int id);
void getVexByEdge(Graph *g, long id, int *h, int *t);
long getEdgeIdx(Graph *g, long id);

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
	int ne = 0;		 //���ɱ���
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
	fwrite(&ch, sz, 1, fp);			//д��ͼ�Ķ������ͱ���
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

//���Ҹ�����ŵĶ��㣬������Ȩֵ
inline int getVexW(Graph *g, int id)
{
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].id == id)
			return g->v[i].w;
	printf("input wrong vertex id in getVexW()!\n");
	system("pause");
	exit(0);
}

//���Ҹ�����ŵĶ��㣬�������
inline int getVexDegree(Graph *g, int id)
{
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].id == id)
			return g->v[i].degree;
	printf("input wrong vertex id in getVexDegree()!\n");
	system("pause");
	exit(0);
}

//���Ҹ�����ŵĶ���,���������б�
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

//���Ҹ�����ŵıߣ�������Ȩֵ
inline int getEdgeW(Graph *g, long id)
{
	for (long i = 0; i < g->ne; i++)
		if (g->e[i].id == id)
			return g->e[i].w;
	printf("input wrong edge id in getEdgeW()!\n");
	system("pause");
	exit(0);
}

//���Ҹ�����ŵıߣ���������������
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

//ͨ��Ȩֵ���Ҷ�����
inline int locateVex(Graph *g, int w)
{
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].w == w)
			return g->v[i].id;
	return -1;
}

//ͨ��Ȩֵ���ұ߱��
inline int locateEdge(Graph *g, int w)
{
	for (int i = 0; i < g->ne; i++)
		if (g->e[i].w == w)
			return g->e[i].id;
	return -1;
}

//���ö���Ȩֵ
inline void setVexW(Graph *g, int id, int w)
{
	for (int i = 0; i < g->nv; i++)
		if (g->v[i].id == id)
			g->v[i].w = w;
	printf("input wrong vertex id in setVexW()!\n");
	system("pause");
	exit(0);
}

//���ñ�Ȩֵ
inline void setEdgeW(Graph *g, int id, int w)
{
	for (long i = 0; i < g->ne; i++)
		if (g->e[i].id == id)
			g->e[i].w = w;
	printf("input wrong edge id in setEdgeW()!\n");
	system("pause");
	exit(0);
}

//ɾ����
void deleteEdge(Graph *g, int id)
{
	//�����ҵ������˶���λ��
	int h, t;
	getVexByEdge(g, id, &h, &t);
	int pos1 = getVexIdx(g, h);
	//ɾ�����������й��ڴ˱ߵ���Ϣ
	for (int i = 0; i < g->v[pos1].degree; i++)
	{ //��˶���
		if (g->v[pos1].edges[i] == id)
		{
			for (int j = i + 1; j < g->v[pos1].degree; j++)
				g->v[pos1].edges[j - 1] = g->v[pos1].edges[j];
			g->v[pos1].degree--; //����--
			break;
		}
	}
	if (!g->dirctional) {
		int pos2 = getVexIdx(g, t);
		//ɾ�����������й��ڴ˱ߵ���Ϣ
		for (int i = 0; i < g->v[pos2].degree; i++)
		{ //��˶���
			if (g->v[pos2].edges[i] == id)
			{
				for (int j = i + 1; j < g->v[pos2].degree; j++)
					g->v[pos2].edges[j - 1] = g->v[pos2].edges[j];
				g->v[pos2].degree--; //����--
				break;
			}
		}
	}
	//ɾ���������й��ڴ˱ߵ���Ϣ
	int pos3 = getEdgeIdx(g, id);
	for (int i = pos3 + 1; i < g->ne; i++)
		g->e[i - 1] = g->e[i];
	//����--
	g->ne--;
}

//ɾ������
void deleteVex(Graph *g, int id)
{
	//�ҵ���������λ��
	int pos = getVexIdx(g, id);
	//ɾ���������������ı�
	for (int i = 0; i < g->ne; i++) {
		if (g->e[i].h == id || g->e[i].t == id) {
			deleteEdge(g, g->e[i].id);
			i--;
		}
	}
	//ɾ������
	for (int i = pos + 1; i < g->nv; i++)
		g->v[i - 1] = g->v[i];
	g->nv--; //������--
}

//���Ӷ���
void addVex(Graph *g, int w)
{
	Vertex temp;
	//����ֵ
	temp.id = g->v[g->nv - 1].id + 1, temp.degree = 0, temp.w = w;
	//���붥������
	g->v[g->nv++] = temp;
}

//���ӱ�
void addEdge(Graph *g, int w, int h, int t)
{
	Edge temp;
	//����ֵ
	temp.h = h, temp.t = t, temp.w = w;
	temp.id = g->e[g->ne - 1].id + 1;
	//���������
	g->e[g->ne++] = temp;
	//���¶�����Ϣ
	int pos1 = getVexIdx(g, h);
	g->v[pos1].edges[g->v[pos1].degree++] = temp.id;
	if (!g->dirctional) {
		int pos2 = getVexIdx(g, t);
		g->v[pos2].edges[g->v[pos2].degree++] = temp.id;
	}
}

//�� id Ϊ���������ȱ���
void dfs(Graph *g, int id, bool visited[])
{
	//����ѷ���
	visited[id] = true;
	int pos = getVexIdx(g, id);
	//���id��Ȩ��
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

// dfs����������ͼ�������������ʱ�����Ľڵ�
void dfs_traverse(Graph *g)
{
	//���������鲢��ʼ��
	bool visited[MAXVERTEX];
	memset(visited, 0, sizeof(visited));
	//����ÿ����ͨƬ
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