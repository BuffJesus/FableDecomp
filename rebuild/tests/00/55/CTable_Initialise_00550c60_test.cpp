#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "../../../src/compiled/00/55/CTable_Initialise_00550c60.cpp"

static volatile int g_baseCalls;
static volatile int g_builderCalls;
static volatile int g_treeCalls;
static volatile int g_freeCalls;
static volatile int g_deleteCalls;
static volatile int g_childCalls;
static void* g_lastTreeOwner;
static void* g_lastTreeRoot;
static void* g_lastChildOwner;

extern "C" void __fastcall FableCTableInitialiseBase(void*)
{
    ++g_baseCalls;
}

extern "C" __declspec(naked) void __fastcall
FableCTableInitialiseDestroyTree(void*, void*)
{
    __asm
    {
        push eax
        inc dword ptr [g_treeCalls]
        mov dword ptr [g_lastTreeOwner],ecx
        mov eax,dword ptr [esp+8]
        mov dword ptr [g_lastTreeRoot],eax
        pop eax
        ret 4
    }
}

extern "C" void __cdecl FableCTableInitialiseFree(void* memory)
{
    ++g_freeCalls;
    std::free(memory);
}

extern "C" void __cdecl FableCTableInitialiseDelete(void* object)
{
    ++g_deleteCalls;
    std::free(object);
}

static void __stdcall ChildInitialise(void* self, void* owner)
{
    ++g_childCalls;
    g_lastChildOwner = owner;
    (void)self;
}

static void __fastcall Builder(void* self)
{
    ++g_builderCalls;
    (void)self;
}

static bool RunInitialise()
{
    unsigned char table[0x190];
    void* tableVtable[0x90];
    void* childVtable[0x40];
    unsigned char root[0x20];
    unsigned char treeNode[0x20];
    unsigned char child[0x20];
    void* children[2];

    std::memset(table, 0, sizeof(table));
    std::memset(tableVtable, 0, sizeof(tableVtable));
    std::memset(childVtable, 0, sizeof(childVtable));
    std::memset(root, 0, sizeof(root));
    std::memset(treeNode, 0, sizeof(treeNode));
    std::memset(child, 0, sizeof(child));
    tableVtable[0x238 / 4] = (void*)Builder;
    childVtable[0xcc / 4] = (void*)ChildInitialise;
    *(void**)table = tableVtable;
    *(void**)child = childVtable;
    *(void**)(table + 0x15c) = root;
    *(void**)(table + 0x160) = treeNode;
    *(void**)(root + 4) = treeNode;
    *(void**)(table + 0xb0) = children;
    *(void**)(table + 0xb4) = children;
    children[0] = child;

    *(void**)(table + 0x168) = std::malloc(8);
    *(void**)(table + 0x16c) = std::malloc(8);
    *(void**) *reinterpret_cast<void**>(table + 0x168) = std::malloc(4);
    *(void**) *reinterpret_cast<void**>(table + 0x16c) = 0;

    g_baseCalls = g_builderCalls = g_treeCalls = 0;
    g_freeCalls = g_deleteCalls = g_childCalls = 0;
    g_lastTreeOwner = g_lastTreeRoot = g_lastChildOwner = 0;

    CTable_Initialise_00550c60(table);
    const bool ok = g_baseCalls == 1 &&
        g_builderCalls == 1 &&
        g_treeCalls == 1 &&
        g_freeCalls == 1 &&
        g_deleteCalls == 2 &&
        g_childCalls == 0;
    return ok;
}

static bool RunGuard()
{
    unsigned char table[0x190];
    void* tableVtable[0x90];
    std::memset(table, 0, sizeof(table));
    std::memset(tableVtable, 0, sizeof(tableVtable));
    *(void**)table = tableVtable;
    *(void**)(table + 0x168) = (void*)1;
    CTable_Initialise_00550c60(table);
    return g_baseCalls == 1;
}

int main()
{
    const bool ok = RunInitialise() && RunGuard();
    std::puts(ok ? "CTable_Initialise_00550c60_test PASS"
                 : "CTable_Initialise_00550c60_test FAIL");
    return ok ? 0 : 1;
}
