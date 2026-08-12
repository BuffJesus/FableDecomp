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