#include <cstdio>

static int g_baseCalls = 0;
static void* g_freed = 0;

void ExternFree(void* p)
{
    g_freed = p;
}

struct CActiveFileBase {
    void BaseOnReadFinished()
    {
        g_baseCalls++;
    }
};

struct CActiveFile : public CActiveFileBase {
    void* m_ptr;

    void OnReadFinished()
    {
        BaseOnReadFinished();
        void* p = m_ptr;
        if (p)
            ExternFree(p);
    }
};

int main()
{
    int dummy = 42;
    CActiveFile a;
    a.m_ptr = &dummy;
    a.OnReadFinished();
    bool ok1 = (g_baseCalls == 1) && (g_freed == &dummy);

    g_baseCalls = 0; g_freed = 0;
    CActiveFile b;
    b.m_ptr = 0;
    b.OnReadFinished();
    bool ok2 = (g_baseCalls == 1) && (g_freed == 0);

    if (ok1 && ok2) {
        printf("ONREADFINISHED_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}