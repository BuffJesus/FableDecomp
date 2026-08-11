#include <cstdio>

struct CQuestCardDef {
    char data[0x74];
    unsigned long GetSizeofClass();
};

unsigned long CQuestCardDef::GetSizeofClass()
{
    return sizeof(CQuestCardDef);
}

int main()
{
    CQuestCardDef d;
    unsigned long n = d.GetSizeofClass();
    if (n == 0x74) {
        printf("SIZEOF_OK_116\n");
        return 0;
    }
    printf("FAIL %lu\n", n);
    return 1;
}