#include <cstdio>
#include <cstdlib>

__declspec(naked) void AddExclusiveObserver()
{
    __asm {
        mov eax, dword ptr [esp+4]
        mov dword ptr [ecx+0x70], eax
        ret 4
    }
}

struct Observable { char pad[0x70]; void* m_pExclusiveObserver; };

int main()
{
    Observable ob;
    ob.m_pExclusiveObserver = (void*)0xDEADBEEF;
    void* obs = (void*)0x12345678;
    void* thisptr = &ob;
    void* ret;
    __asm {
        mov ecx, thisptr
        push obs
        call AddExclusiveObserver
        mov ret, eax
    }
    if (ob.m_pExclusiveObserver != (void*)0x12345678) {
        printf("FAIL field=%p\n", ob.m_pExclusiveObserver);
        return 1;
    }
    if (ret != (void*)0x12345678) {
        printf("FAIL ret=%p\n", ret);
        return 1;
    }
    printf("OK_0x005bc4b6 field=%p ret=%p\n", ob.m_pExclusiveObserver, ret);
    return 0;
}