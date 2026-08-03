#include <string.h>
#include <stdio.h>

struct CAIStateGroup_CreatureUseCoverBase
{
    char pad0[0x1e];
    char flag1e;
    char pad1[0x34 - 0x1e - 1];
    char flag34;
    char pad2[0x38 - 0x34 - 1];
    unsigned long value38;

    void __fastcall Init(void* /*edx_unused*/);
};

void __fastcall CAIStateGroup_CreatureUseCoverBase::Init(void* /*edx_unused*/)
{
    flag1e = 0;
    flag34 = 0;
    value38 = 0;
}

int main()
{
    CAIStateGroup_CreatureUseCoverBase obj;
    memset(&obj, 0xAB, sizeof(obj));

    obj.Init(0);

    int ok = 1;
    if (obj.flag1e != 0) ok = 0;
    if (obj.flag34 != 0) ok = 0;
    if (obj.value38 != 0) ok = 0;

    if (ok)
        printf("INIT_OK_PASS\n");
    else
        printf("INIT_FAIL\n");

    return 0;
}