#include <cstdio>

struct SubA { int a; __int64 Release(); };
struct SubB { int b; __int64 Release(); };
__declspec(noinline) __int64 SubA::Release() { a = -1; return a; }
__declspec(noinline) __int64 SubB::Release() { b = -1; return b; }

struct CInputProcessManager {
    int  m_flag;
    SubA m_a;
    SubB m_b;
    CInputProcessManager* Destroy();
};

__declspec(noinline) CInputProcessManager* CInputProcessManager::Destroy()
{
    m_flag = 0;
    m_a.Release();
    m_b.Release();
    return this;
}

int main()
{
    CInputProcessManager obj;
    obj.m_flag = 777;
    obj.m_a.a  = 111;
    obj.m_b.b  = 222;

    CInputProcessManager* ret = obj.Destroy();

    // return value must be the same object (this)
    if (ret != &obj) { printf("DTOR_PARITY_FAIL ret\n"); return 1; }
    // flag cleared to 0, both subs released to -1, in that order
    if (obj.m_flag == 0 && obj.m_a.a == -1 && obj.m_b.b == -1) {
        printf("DTOR_PARITY_OK\n");
        return 0;
    }
    printf("DTOR_PARITY_FAIL state\n");
    return 1;
}