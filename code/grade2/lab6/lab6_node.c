#ifndef _HUFFMANTREE_CPP_
#define _HUFFMANTREE_CPP_
//�������� ������������ʵ��

#include <stdio.h>
#include <stdlib.h>

//�������󳤶�
#define MCL 30
//����Ҷ�ڵ���,8bit��unsigned char
//��Ӧ256��ASCII��
#define n 256
//������������2n-1
// (m-n)*2 = m - 1
#define m 511

//��������㣬��������ָ�룺���ڵ㣬���Һ���
typedef struct HTNode
{
    int id;
    long w;
    struct HTNode *parent, *lchild, *rchild;
} HTNode, *HTree;

typedef struct
{
    unsigned long long code; //ǰ�涼��0�����lenλ�Ǳ���
    int len;                 //���볤��
} HCode;                     //�����������

//���ɲ���ʼ���½ڵ�
HTree InitNode()
{
    HTree temp = (HTree)malloc(sizeof(HTNode));
    temp->id = temp->w = 0;
    temp->parent = temp->lchild = temp->rchild = NULL;
}

//����Huffman����������������
HTree Destroy(HTree root)
{
    if (root->lchild)
    {
        Destroy(root->lchild);
        root->lchild = NULL;
    }
    if (root->rchild)
    {
        Destroy(root->rchild);
        root->rchild = NULL;
    }
    if (!root->lchild && !root->rchild)
        free(root);
}

/*** ���´�������������� ***/
//�����������/������,���볤��Ϊn=256��Ȩ����w��ÿ���ַ����ֵĴ����� �������������±�
int createHTree(HTree ptr[], long *w)
{
    int pos = 0;
    //����Ƶ�ʺͣ�δ���ֵ��ַ�����
    long allw = 0;
    int numZero = 0;
    //���� m �������ڵ㲢��ʼ�����ڵ��ַ�Ž�ָ��������
    for (int i = 0; i < m; ++i)
    {
        HTree temp = InitNode();
        temp->id = i;
        ptr[pos++] = temp;
        if (i < n)
        {
            temp->w = w[i];
            if (w[i] == 0)
                numZero++; //ͳ��û���ֵ��ַ�����
            allw += w[i];
        }
    }
    //����huffman�㷨���Ӹ��ڵ�
    int i = n, j;
    long min1, min2;
    while (i < m - numZero)
    {
        min1 = min2 = allw + 10; //��������wֵ
        unsigned pos1, pos2;
        for (j = 0; j < i; j++)
        {
            if (ptr[j]->w == 0) //����ȨֵΪ0�Ľ��
                continue;
            if (ptr[j]->parent == NULL) //�ų���ѡ����
                if (ptr[j]->w < min1)
                {
                    pos2 = pos1;
                    min2 = min1;
                    pos1 = j;
                    min1 = ptr[j]->w;
                } // min1<min2,�ҵ���min1��С��ptr[j]->w����ômin1���min2�� ptr[j]->w���min1
                else
                {
                    if (ptr[j]->w < min2)
                    { // min1 <= ptr[j]->w < min2
                        pos2 = j;
                        min2 = ptr[j]->w;
                    }
                }
        }                           //�ڲ�for�������ҵ�pos1,pos2��С������Ȩֵ��׼�������Ҷ�ڵ�
        ptr[i]->lchild = ptr[pos1]; //СȨֵΪ����
        ptr[i]->rchild = ptr[pos2];
        ptr[i]->w = min1 + min2;
        ptr[pos1]->parent = ptr[pos2]->parent = ptr[i];
        i++;
    }
    return m - numZero - 1; //���ظ����±�
}

//�����������������ӡ
void printHtree0(HTree root)
{
    if (root)
    { //�ݹ����
        if (root->w > 0)
            printf("%3d->%5d\n", root->id, root->w);    //��ӡ�ڵ�id��Ȩֵ
        printHtree0(root->lchild);
        printHtree0(root->rchild);
    }
}

/*** ���´�������ļ��ʹ��ļ������ɹ���������� ***/
//��filename�ж����ƶ�ȡ�ַ���ͳ��ÿ���ַ����ֵĴ�����д��w������
// w���鳤��Ϊ256������ַ�����256����ôû���ֵ��ַ�ȨֵΪ0
unsigned char *parseFile(const char filename[], long *w, long *fsize)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("�޷����ļ�!\n");
        exit(0);
    }
    fseek(fp, 0, SEEK_END);
    *fsize = ftell(fp);
    rewind(fp);
    unsigned char *dataArray;
    dataArray = (unsigned char *)malloc(sizeof(unsigned char) * (*fsize));
    if (!dataArray)
    {
        printf("�ļ�̫���ڴ治�����������!\n");
        exit(0);
    }
    fread(dataArray, sizeof(unsigned char), *fsize, fp); //��ȡ��ѹ���ļ�
    fclose(fp);

    for (int i = 0; i < n; ++i) //Ȩֵ���
        w[i] = 0;

    for (int i = 0; i < *fsize; ++i) //Ȩֵ+1������Ӧ�ַ�����
        w[dataArray[i]]++;

    return dataArray;
}

//�����κ��ַ�c���õ����Ĺ���������, ��genCodes()����
void getCode(HTree ptr[], int c, unsigned long long *code1, int *clen1)
{
    int clen = 0;                //���볤��
    unsigned long long code = 0; //����(unsigned int�����������Ϊunsigned long long)

    int size = sizeof(code);                          //��������bits������ʵsize������ֽ�����size * 8��bit��
    unsigned long long mask = 1ull << (size * 8 - 1); //���λΪ1����Ϊ0�������������λΪ1

    HTree parent = ptr[c]->parent;
    while (parent)
    {
        code = code >> 1;
        parent->lchild->id == c ? code : (code = code | mask);
        //������Һ��ӣ���ô���λ����Ϊ1���������0��mask���Ǹ����λΪ1������Ϊ0�Ĺ����ˣ�������code���λ��1
        clen++;
        c = parent->id;
        parent = ptr[c]->parent;
    }

    // code��ǰ��clenλ�ǹ��������룬�����ƶ������
    code = code >> (size * 8 - clen);
    // printf("0x%x-%d\n",code,clen);
    *code1 = code; //����ֵ
    *clen1 = clen; //����ֵ
}

//�ӹ�������ht���������ı����hc�����hcΪencode()���������
void genHCodes(HTree ptr[], HCode hc[])
{
    for (int i = 0; i < n; ++i)
        if (ptr[i]->w > 0)
            getCode(ptr, i, &(hc[i].code), &(hc[i].len));
}

/*** ����Ϊ�����ʵ�� ***/
//����,ԭ��olen������orgi��ѹ�������Ϊ��nlen������newc����Ҫ���ñ����hc
void encode(unsigned char *orgi, long olen, unsigned char *newc, long *nlen, HCode hc[])
{
    int sizec = sizeof(unsigned long long);
    //Ҫȷ���ַ��������󳤶�ҪС��sizec*8
    long i = 0; // orgi���±�
    long j = 0; // newc���±�
    newc[j] = 0;
    unsigned long long code;
    int len;
    int usedbits = 0; // newc[j]�б�ռ�õ�bits��
    while (i < olen)
    {                      //��ԭʼ��������ַ�����
        int idx = orgi[i]; //���hc�±�
        code = hc[idx].code;   //��ö�Ӧ����
        len = hc[idx].len;  //��ñ��볤��
        while (len + usedbits >= 8)
        { // code��Ҫ����һ�Σ�Ȼ���newc[j]λ�����
            unsigned long long code1 = code >> len + usedbits - 8;
            newc[j] = newc[j] | code1; //����ǰһ�������ַ�
            len = len + usedbits - 8;  // code������Ч����
            usedbits = 0;
            newc[++j] = 0;
            // printf("\n����ĵ� %d ���ַ�Ϊ��#%2x\n",j-1,newc[j-1]);
        } //ѭ����ʹ��len+usedbits <8
        //��code��ǰ���λ��0,��Ϊ��Чλ��len
        code = code & ((1 << len) - 1);    // code�ͺ���k��1������ȫΪ0�����ݽ����룬��������lenλ
        code = code << 8 - len - usedbits; //��ǰ�ƶ�����λ
        newc[j] = newc[j] | code;
        usedbits = len + usedbits;
        i++;
    }
    *nlen = j + 1;
}

//���ɺͱ���ѹ���ļ�,��ѹ���ļ�fin��ָ���ļ���fout�������õĹ������������ļ�
void zip(char fin[], char fout[])
{
    HCode hc[n];     //�ṹ�����飬��ʾ����������� :code,len�����ڱ����ļ�
    long wDist[256]; //�����ַ��ķֲ����ַ����ļ��г��ֵĴ�����
    long fsize;      //�ļ�����
    //��ȡ�ļ����ݣ�������ѹ���ļ�������Ȩֵ����wDist,�ļ�����content�ͳ���fsize
    unsigned char *content = parseFile(fin, wDist, &fsize);
    HTree ptr[m];
    // ����ѹ������H����
    int root = createHTree(ptr, wDist); // root�ǹ�������ht�ĸ������±�

    // printHtree0(ptr[root]);

    genHCodes(ptr, hc); //���������ı����,���ڱ����ļ�
    //����Ĵ������ڲ��Թ�������
    // printHtree1(ht); //�����������������ӡ����
    // printHtree0(ht,root); //���������������
    // saveTree(ht,root,"aa1.html"); //ͼ��չʾ�������� , ���ȵ���showgt.h
    // ��ʼѹ��
    unsigned char *zipContent;                                                     //����������
    long zipsize;                                                                  //ѹ�����ļ���С
    zipContent = (unsigned char *)malloc(sizeof(unsigned char) * (fsize + 10000)); //ѹ������ļ����ܸ��󣬿��ǽ�fsize����һ��
    if (!zipContent)
    {
        printf("�ļ�̫���ڴ治�����������!\n");
        exit(0);
    }
    encode(content, fsize, zipContent, &zipsize, hc); //����󷵻س���zipsize������zipContent

    FILE *fp = fopen(fout, "wb");
    if (fp == NULL)
    {
        printf("�޷���д���ļ�!\n");
        exit(0);
    }
    fwrite(&zipsize, sizeof(zipsize), 1, fp);               //����������ݵĴ�С
    fwrite(&fsize, sizeof(fsize), 1, fp);                   //����ԭʼ���ݵĴ�С
    fwrite(wDist, sizeof(wDist), 1, fp);                    // ����Ȩ��w������ʱ��createHtree�������������˳���Ŀ������ԣ�
    fwrite(zipContent, sizeof(unsigned char), zipsize, fp); //�������������

    fclose(fp);

    free(zipContent); //�ͷ��ļ�����
    free(content);
    Destroy(ptr[root]);
    printf("ѹ���ļ� %s �Ѿ����ɣ�\n", fout);
}

//��ȡѹ���ļ�����ѹ
void unzip(char zfile[], char ofile[])
{
    FILE *fp = fopen(zfile, "rb");
    if (fp == NULL)
    {
        printf("�޷���ѹ���ļ����ж�ȡ!\n");
        exit(0);
    }
    long ht_size1, zipsize1, fsize1;
    long wDist[n];                             //�����ַ��ķֲ����ַ����ļ��г��ֵĴ�����
    HTree ptr1[m];                             //�ṹ�����飬��ʾ�������� : id,w,parent,lchild,rchild���������ɱ����ͽ�ѹ
    fread(&zipsize1, sizeof(zipsize1), 1, fp); // ѹ�������ݵĴ�С
    fread(&fsize1, sizeof(fsize1), 1, fp);     // ��ѹ�����ݵĴ�С
    fread(wDist, sizeof(wDist), 1, fp);        // ��ȡȨ������
    int root = createHTree(ptr1, wDist);       // ע��ʹ��createHTree������

    // printHtree0(ptr1[root]);

    unsigned char *zcontent = (unsigned char *)malloc(zipsize1); //��Ŷ�ȡ�ı�������
    unsigned char *ocontent = (unsigned char *)malloc(fsize1);   //����������
    fread(zcontent, zipsize1, 1, fp);                            //��ȡ��������

    // for (int i = 0; i < 20; i++) {
    //     printf("%d\n", zcontent[i]);
    // }

    fclose(fp);

    fp = fopen(ofile, "wb");
    if (fp == NULL)
    {
        printf("�޷��򿪽�ѹ���ļ����н�ѹ!\n");
        exit(0);
    }

    HTree idx = ptr1[root]; //�Ӹ���ʼ
    int j, k = 0, i = 0;    // zcontent���±� k,ocontent���±�i
    while (i < fsize1)
    {            //����д���ѹ�����ݵ�ÿ���ֽ�
        j = 128; // 10000000
        while (j > 0)
        {
            if ((zcontent[k] & j) > 0)
            { //������ 1
                if (idx->rchild == NULL)
                {
                    ocontent[i++] = idx->id;
                    idx = ptr1[root]; //��������һ���ַ��ĵ�һ��bit
                    j = j << 1;
                }
                else
                    idx = idx->rchild;
            }
            else
            { //������ 0
                if (idx->lchild == NULL)
                {
                    ocontent[i++] = idx->id;
                    idx = ptr1[root]; //��������һ���ַ��ĵ�һ��bit
                    j = j << 1;
                }
                else
                    idx = idx->lchild;
            }
            j = j >> 1; // j����whileѭ��8�Σ����zcontent[k]��ÿһλ
        }
        k++; //׼����ȡ��һ���ַ�
    }
    fwrite(ocontent, fsize1, 1, fp); // ����ѹ����д���ļ�

    fclose(fp);
    free(ocontent); //�ͷſռ�
    free(zcontent);
    Destroy(ptr1[root]);
    printf("�ļ� %s �Ѿ��ɹ���ѹΪ %s ��\n", zfile, ofile);
}
int check(char file1[], char file2[])
{ // ����1˵���ļ�һ��������0˵���ļ���һ��
    FILE *fp1 = fopen(file1, "r"), *fp2 = fopen(file2, "r");
    unsigned fsize1, fsize2;
    fseek(fp1, 0, SEEK_END);
    fseek(fp2, 0, SEEK_END);
    fsize1 = ftell(fp1);
    fsize2 = ftell(fp2);
    rewind(fp1);
    rewind(fp2);
    if (fsize1 != fsize2)
        return 0;
    char c1, c2;
    for (unsigned i = 0; i < fsize1; ++i)
    {
        fread(&c1, 1, 1, fp1);
        fread(&c2, 1, 1, fp2);
        if (c1 != c2)
            return printf("at fsize = %d, c1 is %d, c2 is %d\n", i, c1, c2);
    }
    fclose(fp1);
    fclose(fp2);
    return 1;
}

int main()
{
    // ѹ��һ��ͼƬ
    zip("pic.png", "pic.png.myzip");
    unzip("pic.png.myzip", "myout_pic.png");
    printf("%d\n", check("pic.png", "myout_pic.png"));
    // ѹ��һ��pdf
    zip("lab6.pdf", "lab6.pdf.myzip");
    unzip("lab6.pdf.myzip", "myout_lab6.pdf");
    printf("%d\n", check("lab6.pdf", "myout_lab6.pdf"));
    // ѹ��һ����Ƶ
    zip("video.mp4", "video.mp4.myzip");
    unzip("video.mp4.myzip", "myout_video.mp4");
    printf("%d\n", check("video.mp4", "myout_video.mp4"));
    // ѹ��һ���ļ�
    zip("test", "test.myzip");
    unzip("test.myzip", "myout_test");
    printf("%d\n", check("test", "myout_test"));
    system("pause");
    return 1;
}
// ѹ���ɹ��������������֣��������ѹ��ʧ�ܣ���˼��ԭ��

#endif