#ifndef _HUFFMANTREE_CPP_
#define _HUFFMANTREE_CPP_
//哈夫曼树 ：用三叉链表实现

#include <stdio.h>
#include <stdlib.h>

//编码的最大长度
#define MCL 30
//定义叶节点数,8bit的unsigned char
//对应256个ASCII码
#define n 256
//定义结点总数，2n-1
// (m-n)*2 = m - 1
#define m 511

//定义树结点，包括三个指针：父节点，左右孩子
typedef struct HTNode
{
    int id;
    long w;
    struct HTNode *parent, *lchild, *rchild;
} HTNode, *HTree;

typedef struct
{
    unsigned long long code; //前面都是0，最后len位是编码
    int len;                 //编码长度
} HCode;                     //哈夫曼编码表

//生成并初始化新节点
HTree InitNode()
{
    HTree temp = (HTree)malloc(sizeof(HTNode));
    temp->id = temp->w = 0;
    temp->parent = temp->lchild = temp->rchild = NULL;
}

//销毁Huffman树，后续遍历销毁
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

/*** 以下代码哈夫曼树生成 ***/
//构造哈夫曼树/二叉树,输入长度为n=256的权向量w（每个字符出现的次数） ，返回树根的下标
int createHTree(HTree ptr[], long *w)
{
    int pos = 0;
    //出现频率和，未出现的字符个数
    long allw = 0;
    int numZero = 0;
    //创建 m 个孤立节点并初始化，节点地址放进指针数组里
    for (int i = 0; i < m; ++i)
    {
        HTree temp = InitNode();
        temp->id = i;
        ptr[pos++] = temp;
        if (i < n)
        {
            temp->w = w[i];
            if (w[i] == 0)
                numZero++; //统计没出现的字符个数
            allw += w[i];
        }
    }
    //按照huffman算法链接各节点
    int i = n, j;
    long min1, min2;
    while (i < m - numZero)
    {
        min1 = min2 = allw + 10; //设置最大的w值
        unsigned pos1, pos2;
        for (j = 0; j < i; j++)
        {
            if (ptr[j]->w == 0) //忽略权值为0的结点
                continue;
            if (ptr[j]->parent == NULL) //排除已选过的
                if (ptr[j]->w < min1)
                {
                    pos2 = pos1;
                    min2 = min1;
                    pos1 = j;
                    min1 = ptr[j]->w;
                } // min1<min2,找到比min1更小的ptr[j]->w，那么min1替代min2， ptr[j]->w替代min1
                else
                {
                    if (ptr[j]->w < min2)
                    { // min1 <= ptr[j]->w < min2
                        pos2 = j;
                        min2 = ptr[j]->w;
                    }
                }
        }                           //内层for结束，找到pos1,pos2最小的两个权值，准备构造非叶节点
        ptr[i]->lchild = ptr[pos1]; //小权值为左孩子
        ptr[i]->rchild = ptr[pos2];
        ptr[i]->w = min1 + min2;
        ptr[pos1]->parent = ptr[pos2]->parent = ptr[i];
        i++;
    }
    return m - numZero - 1; //返回根的下标
}

//先序遍历二叉树并打印
void printHtree0(HTree root)
{
    if (root)
    { //递归出口
        if (root->w > 0)
            printf("%3d->%5d\n", root->id, root->w);    //打印节点id和权值
        printHtree0(root->lchild);
        printHtree0(root->rchild);
    }
}

/*** 以下代码分析文件和从文件中生成哈夫曼编码表 ***/
//从filename中二进制读取字符，统计每个字符出现的次数，写入w并返回
// w数组长度为256，如果字符不足256，那么没出现的字符权值为0
unsigned char *parseFile(const char filename[], long *w, long *fsize)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("无法打开文件!\n");
        exit(0);
    }
    fseek(fp, 0, SEEK_END);
    *fsize = ftell(fp);
    rewind(fp);
    unsigned char *dataArray;
    dataArray = (unsigned char *)malloc(sizeof(unsigned char) * (*fsize));
    if (!dataArray)
    {
        printf("文件太大，内存不够，读入错误!\n");
        exit(0);
    }
    fread(dataArray, sizeof(unsigned char), *fsize, fp); //读取待压缩文件
    fclose(fp);

    for (int i = 0; i < n; ++i) //权值清空
        w[i] = 0;

    for (int i = 0; i < *fsize; ++i) //权值+1，若对应字符出现
        w[dataArray[i]]++;

    return dataArray;
}

//输入任何字符c，得到它的哈夫曼编码, 被genCodes()调用
void getCode(HTree ptr[], int c, unsigned long long *code1, int *clen1)
{
    int clen = 0;                //编码长度
    unsigned long long code = 0; //编码(unsigned int不够大，这里改为unsigned long long)

    int size = sizeof(code);                          //编码的最大bits数，其实size存的是字节数，size * 8是bit数
    unsigned long long mask = 1ull << (size * 8 - 1); //最高位为1其它为0，用来设置最高位为1

    HTree parent = ptr[c]->parent;
    while (parent)
    {
        code = code >> 1;
        parent->lchild->id == c ? code : (code = code | mask);
        //如果是右孩子，那么最高位设置为1，否则就是0，mask就是个最高位为1，其他为0的工具人，用来给code最高位赋1
        clen++;
        c = parent->id;
        parent = ptr[c]->parent;
    }

    // code的前面clen位是哈夫曼编码，将其移动到最后
    code = code >> (size * 8 - clen);
    // printf("0x%x-%d\n",code,clen);
    *code1 = code; //返回值
    *clen1 = clen; //返回值
}

//从哈夫曼树ht生成完整的编码表hc，输出hc为encode()的输入参数
void genHCodes(HTree ptr[], HCode hc[])
{
    for (int i = 0; i < n; ++i)
        if (ptr[i]->w > 0)
            getCode(ptr, i, &(hc[i].code), &(hc[i].len));
}

/*** 以下为编解码实现 ***/
//编码,原长olen的内容orgi，压缩编码后为长nlen的内容newc，需要利用编码表hc
void encode(unsigned char *orgi, long olen, unsigned char *newc, long *nlen, HCode hc[])
{
    int sizec = sizeof(unsigned long long);
    //要确保字符编码的最大长度要小于sizec*8
    long i = 0; // orgi的下标
    long j = 0; // newc的下标
    newc[j] = 0;
    unsigned long long code;
    int len;
    int usedbits = 0; // newc[j]中被占用的bits数
    while (i < olen)
    {                      //对原始数据逐个字符处理
        int idx = orgi[i]; //获得hc下标
        code = hc[idx].code;   //获得对应编码
        len = hc[idx].len;  //获得编码长度
        while (len + usedbits >= 8)
        { // code需要右移一段，然后和newc[j]位或操作
            unsigned long long code1 = code >> len + usedbits - 8;
            newc[j] = newc[j] | code1; //填满前一个编码字符
            len = len + usedbits - 8;  // code现在有效长度
            usedbits = 0;
            newc[++j] = 0;
            // printf("\n编码的第 %d 个字符为：#%2x\n",j-1,newc[j-1]);
        } //循环，使得len+usedbits <8
        //将code的前面各位清0,因为有效位是len
        code = code & ((1 << len) - 1);    // code和后面k个1，其它全为0的数据进行与，保留后面len位
        code = code << 8 - len - usedbits; //向前移动若干位
        newc[j] = newc[j] | code;
        usedbits = len + usedbits;
        i++;
    }
    *nlen = j + 1;
}

//生成和保存压缩文件,被压缩文件fin，指定文件名fout，将所用的哈夫曼树存入文件
void zip(char fin[], char fout[])
{
    HCode hc[n];     //结构体数组，表示哈夫曼编码表 :code,len，用于编码文件
    long wDist[256]; //保存字符的分布（字符在文件中出现的次数）
    long fsize;      //文件长度
    //获取文件内容，分析待压缩文件，返回权值向量wDist,文件内容content和长度fsize
    unsigned char *content = parseFile(fin, wDist, &fsize);
    HTree ptr[m];
    // 生成压缩树（H树）
    int root = createHTree(ptr, wDist); // root是哈夫曼树ht的根结点的下标

    // printHtree0(ptr[root]);

    genHCodes(ptr, hc); //生成完整的编码表,用于编码文件
    //下面的代码用于测试哈夫曼树
    // printHtree1(ht); //将哈夫曼树按数组打印出来
    // printHtree0(ht,root); //先序遍历哈夫曼树
    // saveTree(ht,root,"aa1.html"); //图形展示哈夫曼树 , 请先导入showgt.h
    // 开始压缩
    unsigned char *zipContent;                                                     //编码后的内容
    long zipsize;                                                                  //压缩后文件大小
    zipContent = (unsigned char *)malloc(sizeof(unsigned char) * (fsize + 10000)); //压缩后的文件可能更大，考虑将fsize扩大一点
    if (!zipContent)
    {
        printf("文件太大，内存不够，读入错误!\n");
        exit(0);
    }
    encode(content, fsize, zipContent, &zipsize, hc); //编码后返回长度zipsize的内容zipContent

    FILE *fp = fopen(fout, "wb");
    if (fp == NULL)
    {
        printf("无法打开写入文件!\n");
        exit(0);
    }
    fwrite(&zipsize, sizeof(zipsize), 1, fp);               //保存编码内容的大小
    fwrite(&fsize, sizeof(fsize), 1, fp);                   //保存原始内容的大小
    fwrite(wDist, sizeof(wDist), 1, fp);                    // 保存权重w，解码时用createHtree生成树（利用了程序的可再现性）
    fwrite(zipContent, sizeof(unsigned char), zipsize, fp); //保存编码后的内容

    fclose(fp);

    free(zipContent); //释放文件内容
    free(content);
    Destroy(ptr[root]);
    printf("压缩文件 %s 已经生成！\n", fout);
}

//读取压缩文件，解压
void unzip(char zfile[], char ofile[])
{
    FILE *fp = fopen(zfile, "rb");
    if (fp == NULL)
    {
        printf("无法打开压缩文件进行读取!\n");
        exit(0);
    }
    long ht_size1, zipsize1, fsize1;
    long wDist[n];                             //保存字符的分布（字符在文件中出现的次数）
    HTree ptr1[m];                             //结构体数组，表示哈夫曼树 : id,w,parent,lchild,rchild，用于生成编码表和解压
    fread(&zipsize1, sizeof(zipsize1), 1, fp); // 压缩后内容的大小
    fread(&fsize1, sizeof(fsize1), 1, fp);     // 被压缩内容的大小
    fread(wDist, sizeof(wDist), 1, fp);        // 读取权重数组
    int root = createHTree(ptr1, wDist);       // 注意使用createHTree生成树

    // printHtree0(ptr1[root]);

    unsigned char *zcontent = (unsigned char *)malloc(zipsize1); //存放读取的编码数据
    unsigned char *ocontent = (unsigned char *)malloc(fsize1);   //解码后的数据
    fread(zcontent, zipsize1, 1, fp);                            //读取编码数据

    // for (int i = 0; i < 20; i++) {
    //     printf("%d\n", zcontent[i]);
    // }

    fclose(fp);

    fp = fopen(ofile, "wb");
    if (fp == NULL)
    {
        printf("无法打开解压后文件进行解压!\n");
        exit(0);
    }

    HTree idx = ptr1[root]; //从根开始
    int j, k = 0, i = 0;    // zcontent的下标 k,ocontent的下标i
    while (i < fsize1)
    {            //依次写入解压后数据的每个字节
        j = 128; // 10000000
        while (j > 0)
        {
            if ((zcontent[k] & j) > 0)
            { //向右走 1
                if (idx->rchild == NULL)
                {
                    ocontent[i++] = idx->id;
                    idx = ptr1[root]; //解码了下一个字符的第一个bit
                    j = j << 1;
                }
                else
                    idx = idx->rchild;
            }
            else
            { //向左走 0
                if (idx->lchild == NULL)
                {
                    ocontent[i++] = idx->id;
                    idx = ptr1[root]; //解码了下一个字符的第一个bit
                    j = j << 1;
                }
                else
                    idx = idx->lchild;
            }
            j = j >> 1; // j控制while循环8次，求出zcontent[k]的每一位
        }
        k++; //准备读取下一个字符
    }
    fwrite(ocontent, fsize1, 1, fp); // 将解压内容写入文件

    fclose(fp);
    free(ocontent); //释放空间
    free(zcontent);
    Destroy(ptr1[root]);
    printf("文件 %s 已经成功解压为 %s ！\n", zfile, ofile);
}
int check(char file1[], char file2[])
{ // 返回1说明文件一样，返回0说明文件不一样
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
    // 压缩一个图片
    zip("pic.png", "pic.png.myzip");
    unzip("pic.png.myzip", "myout_pic.png");
    printf("%d\n", check("pic.png", "myout_pic.png"));
    // 压缩一个pdf
    zip("lab6.pdf", "lab6.pdf.myzip");
    unzip("lab6.pdf.myzip", "myout_lab6.pdf");
    printf("%d\n", check("lab6.pdf", "myout_lab6.pdf"));
    // 压缩一个视频
    zip("video.mp4", "video.mp4.myzip");
    unzip("video.mp4.myzip", "myout_video.mp4");
    printf("%d\n", check("video.mp4", "myout_video.mp4"));
    // 压缩一个文件
    zip("test", "test.myzip");
    unzip("test.myzip", "myout_test");
    printf("%d\n", check("test", "myout_test"));
    system("pause");
    return 1;
}
// 压缩成功其中两个给满分，如果存在压缩失败，请思考原因

#endif