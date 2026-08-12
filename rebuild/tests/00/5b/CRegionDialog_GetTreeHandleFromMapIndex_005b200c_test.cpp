#include <stdio.h>

struct CGuiControlTree;

void __fastcall SubInit_5b1b20(CGuiControlTree* self, long index);

struct CGuiControlTree
{
    char pad[0xC];
};

struct TreeHandle
{
    char            head[0x10];
    CGuiControlTree tree;
};

struct CRegionDialog
{
    TreeHandle* GetTreeHandleFromMapIndex(long mapIndex);
};

// local stand-in for the engine callee: stamps the index into the sub-object.
static long g_lastIndex = 0;
static void* g_lastSelf = 0;
void __fastcall SubInit_5b1b20(CGuiControlTree* self, long index)
{
    g_lastSelf = self;
    g_lastIndex = index;
    *(long*)self = index;   // write something so we can observe it
}

TreeHandle* CRegionDialog::GetTreeHandleFromMapIndex(long mapIndex)
{
    TreeHandle* h = (TreeHandle*)::operator new(sizeof(TreeHandle));
    SubInit_5b1b20(&h->tree, mapIndex);
    return h;
}

int main()
{
    CRegionDialog d;
    long idx = 0x2A;
    TreeHandle* h = d.GetTreeHandleFromMapIndex(idx);
    bool ok = (h != 0)
        && (g_lastIndex == idx)
        && (g_lastSelf == (void*)&h->tree)
        && (*(long*)&h->tree == idx)
        && (sizeof(TreeHandle) == 0x1c);
    if (ok) printf("PASS_TREEHANDLE_2A\n");
    else    printf("FAIL idx=%ld self=%p size=%u\n", g_lastIndex, g_lastSelf, (unsigned)sizeof(TreeHandle));
    delete h;
    return ok ? 0 : 1;
}