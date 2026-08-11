#include <cstdio>
#include <cstdlib>

void FreeBlock(void* p);

struct CThingComponentSet {
    void* m_data;
    char  m_flag;
    ~CThingComponentSet();
};

CThingComponentSet::~CThingComponentSet()
{
    FreeBlock(m_data);
    m_data = 0;
    m_flag = 0;
}

static int g_freed = 0;
static void* g_lastFreed = 0;
void FreeBlock(void* p) { g_freed++; g_lastFreed = p; }

int main()
{
    int dummy = 42;
    CThingComponentSet s;
    s.m_data = &dummy;
    s.m_flag = 7;
    s.~CThingComponentSet();
    if (g_freed == 1 && g_lastFreed == &dummy && s.m_data == 0 && s.m_flag == 0)
        printf("DTOR_OK_A7DAF0\n");
    else
        printf("DTOR_FAIL\n");
    return (g_freed == 1 && s.m_data == 0 && s.m_flag == 0) ? 0 : 1;
}