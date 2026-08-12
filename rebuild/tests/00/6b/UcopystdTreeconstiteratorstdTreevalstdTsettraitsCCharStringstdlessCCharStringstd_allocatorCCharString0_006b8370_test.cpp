#include <cstdio>

struct CCharString { int tag; };

struct TreeIter { void* _Ptr; };

struct CopyHelper {
    CCharString* Do(TreeIter first, CCharString* dest, TreeIter last);
};

struct VecObj {
    char _pad[0x90];
    CopyHelper helper;
    CCharString* _Ucopy(TreeIter first, TreeIter last, CCharString* dest);
};

static void* g_self;
static void* g_first;
static void* g_dest;
static void* g_last;
static CCharString g_ret;

CCharString* CopyHelper::Do(TreeIter first, CCharString* dest, TreeIter last)
{
    g_self  = this;
    g_first = first._Ptr;
    g_dest  = dest;
    g_last  = last._Ptr;
    g_ret.tag = 0x1234;
    return &g_ret;
}

CCharString* VecObj::_Ucopy(TreeIter first, TreeIter last, CCharString* dest)
{
    return helper.Do(first, dest, last);
}

int main()
{
    VecObj v;
    TreeIter first; first._Ptr = (void*)0x1111;
    TreeIter last;  last._Ptr  = (void*)0x2222;
    CCharString dest; dest.tag = 7;

    CCharString* r = v._Ucopy(first, last, &dest);

    // verify arg reordering and helper subobject offset
    void* expect_helper = (char*)&v + 0x90;
    if (r == &g_ret &&
        g_self  == expect_helper &&
        g_first == (void*)0x1111 &&
        g_dest  == &dest &&
        g_last  == (void*)0x2222)
    {
        printf("UCOPY_FWD_OK\n");
        return 0;
    }
    printf("FAIL self=%p first=%p dest=%p last=%p\n", g_self, g_first, g_dest, g_last);
    return 1;
}