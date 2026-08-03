#include <stdio.h>

class CAnimFilterBase
{
public:
    int filterData[1];
};

class CModeBlock
{
public:
    CAnimFilterBase* GetAnimFilter();
private:
    void* vfptr;
    void* field4;
    CAnimFilterBase animFilter;
};

CAnimFilterBase* CModeBlock::GetAnimFilter()
{
    return &animFilter;
}

int main()
{
    char buf[64];
    for (int i = 0; i < 64; i++) buf[i] = 0;
    CModeBlock* self = (CModeBlock*)buf;
    CAnimFilterBase* p = self->GetAnimFilter();
    void* expected = (void*)((char*)self + 8);
    if ((void*)p == expected)
        printf("GETANIMFILTER_OK\n");
    else
        printf("GETANIMFILTER_FAIL\n");
    return 0;
}