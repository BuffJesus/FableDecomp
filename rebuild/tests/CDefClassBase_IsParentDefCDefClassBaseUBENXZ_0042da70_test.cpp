// Standalone behaviour test for CDefClassBase::IsParentDef @ 0x0042da70
#include <stdio.h>

struct CDefClassBase
{
    void* vftable;
};

extern "C" bool __fastcall CDefClassBase_IsParentDef(CDefClassBase* self)
{
    (void)self;
    return false;
}

int main(void)
{
    CDefClassBase obj;
    obj.vftable = 0;

    bool r = CDefClassBase_IsParentDef(&obj);

    if (r == false)
        printf("CDEFCLASSBASE_0042da70_TEST PASS\n");
    else
        printf("CDEFCLASSBASE_0042da70_TEST FAIL (got %d)\n", (int)r);

    return 0;
}