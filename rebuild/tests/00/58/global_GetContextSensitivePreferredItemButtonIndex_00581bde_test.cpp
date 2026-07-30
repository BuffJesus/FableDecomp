#include <stdio.h>

struct Entry { int key; int pad; int value; };

static Entry g_tbl[3] = {
    { 10, 0, 100 },
    { 20, 0, 200 },
    { 30, 0, 300 },
};

int __fastcall GetContextSensitivePreferredItemButtonIndex(int key)
{
    Entry* p = &g_tbl[0];
    Entry* e = &g_tbl[3];
    for (; p != e; ++p)
        if (p->key == key)
            return p->value;
    return -1;
}

int main()
{
    if (GetContextSensitivePreferredItemButtonIndex(10) != 100) { printf("FAIL a\n"); return 1; }
    if (GetContextSensitivePreferredItemButtonIndex(20) != 200) { printf("FAIL b\n"); return 1; }
    if (GetContextSensitivePreferredItemButtonIndex(30) != 300) { printf("FAIL c\n"); return 1; }
    if (GetContextSensitivePreferredItemButtonIndex(99) != -1)  { printf("FAIL miss\n"); return 1; }
    printf("OK_0x00581bde\n");
    return 0;
}