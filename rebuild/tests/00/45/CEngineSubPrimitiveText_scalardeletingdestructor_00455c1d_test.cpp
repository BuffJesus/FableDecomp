#include <cstdio>
#include <cstdlib>

static int g_dtor = 0;
static void* g_deleted = 0;

struct CSubObj { ~CSubObj(); };
struct CEngineSubPrimitiveText {
    char pad[0x3c];
    CSubObj sub;
    ~CEngineSubPrimitiveText();
    void * scalar_deleting_destructor(unsigned int flags);
};
CSubObj::~CSubObj() {}
CEngineSubPrimitiveText::~CEngineSubPrimitiveText() { g_dtor++; }

void * CEngineSubPrimitiveText::scalar_deleting_destructor(unsigned int flags)
{
    this->sub.~CSubObj();
    this->~CEngineSubPrimitiveText();
    if (flags & 1)
        ::operator delete(this);
    return this;
}

int main()
{
    char buf1[sizeof(CEngineSubPrimitiveText)];
    CEngineSubPrimitiveText* p = (CEngineSubPrimitiveText*)buf1;
    void* r = p->scalar_deleting_destructor(0);
    if (r != p) { std::printf("FAIL ret\n"); return 1; }
    if (g_dtor != 1) { std::printf("FAIL dtor\n"); return 1; }
    CEngineSubPrimitiveText* q = (CEngineSubPrimitiveText*)::operator new(sizeof(CEngineSubPrimitiveText));
    void* qsave = q;
    void* r2 = q->scalar_deleting_destructor(1);
    if (r2 != qsave) { std::printf("FAIL ret2\n"); return 1; }
    if (g_dtor != 2) { std::printf("FAIL dtor2\n"); return 1; }
    std::printf("CEngineSubPrimitiveText_00455c1d_TEST PASS\n");
    return 0;
}