#include <stdio.h>

struct CDefClassBase { int tag; };

struct CDefinitionManager
{
    void* vfptr;
    char pad0[0x18 - 4];
    const CDefClassBase** m_classArray;
};

long __stdcall ValidateClassIndex(CDefinitionManager* mgr, long classIndex)
{
    if (classIndex >= 0 && classIndex < 4)
        return classIndex;
    return -1;
}

const CDefClassBase* __fastcall GetPDefFromClassIndex(CDefinitionManager* self, long classIndex)
{
    long idx = ValidateClassIndex(self, classIndex);
    if (idx < 0)
        return (const CDefClassBase*)idx;
    return self->m_classArray[idx];
}

int main()
{
    CDefClassBase a, b, c, d;
    a.tag = 111; b.tag = 222; c.tag = 333; d.tag = 444;
    const CDefClassBase* arr[4];
    arr[0] = &a; arr[1] = &b; arr[2] = &c; arr[3] = &d;
    CDefinitionManager mgr;
    mgr.m_classArray = arr;

    const CDefClassBase* out = GetPDefFromClassIndex(&mgr, 2);
    const CDefClassBase* bad = GetPDefFromClassIndex(&mgr, -5);
    if (out == &c && bad == (const CDefClassBase*)-1) {
        printf("PDEF_CLASSIDX_OK\n");
        return 0;
    }
    printf("PDEF_CLASSIDX_BAD\n");
    return 1;
}