#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 12
void get_next(char t[], int next[])
{
    int j = 0, k = -1;
    next[0] = -1;
    while (j < N)
    {
        if (k == -1 || t[j] == t[k])
            next[++j] = ++k;
        else
            k = next[k];
    }
}
void next_val(char t[], int next[])
{
    int j = 0, k = -1;
    next[0] = -1;
    while (j < N)
    {
        if (k == -1 || t[j] == t[k])
        {
            ++j;
            ++k;
            if (t[j] != t[k])
                next[j] = k;
            else
                next[j] = next[k];
        }
        else
            k = next[k];
    }
}
int main() {
    char s[N+1] = "ababaabab";
    int next[N], nextval[N];
    // get_next(s, next);
    next_val(s, next);
    for (int i = 0; i < N; i++) {
        printf("%d ", next[i]);
    }
    system("pause");
    return 0;
}