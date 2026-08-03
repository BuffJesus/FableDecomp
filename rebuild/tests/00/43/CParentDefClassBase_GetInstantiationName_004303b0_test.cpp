#include <stdio.h>

#pragma pack(push,1)
struct CDefString {
    unsigned long value;
};
struct CParentDefClassBase {
    char pad[0x25];
    unsigned long instantiationName; // at +0x25
};
#pragma pack(pop)

CDefString GetInstantiationName(CParentDefClassBase *this_)
{
    CDefString r;
    r.value = this_->instantiationName;
    return r;
}

int main()
{
    CParentDefClassBase obj;
    for (int i = 0; i < (int)sizeof(obj); ++i) ((char*)&obj)[i] = 0;
    obj.instantiationName = 0xDEADBEEFu;

    CDefString got = GetInstantiationName(&obj);
    if (got.value != 0xDEADBEEFu) {
        printf("FAIL got=%08lx\n", got.value);
        return 1;
    }

    // field must be read from offset 0x25
    if ((char*)&obj.instantiationName - (char*)&obj != 0x25) {
        printf("FAIL offset\n");
        return 1;
    }

    printf("OK_0x004303b0\n");
    return 0;
}