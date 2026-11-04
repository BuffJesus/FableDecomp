// Behaviour fixture for CCountedPointer<CNavConnectorNode>::DeleteData (provider CCountedPointer_NavConnectorDeleteData_00a82fa0.cpp; it defines only the
// instantiation). DeleteData is private, so it is reached by its mangled name through /alternatename. The fixture
// supplies a recording destructor and global operator delete: non-null data must be destroyed and then freed (in that
// order, same pointer); null data must do neither.
#include <stdio.h>
#include <stdlib.h>
#include "fable_nav_connector_node.h"

static int g_fail, g_step, g_dtor_step, g_free_step;
static void* g_dtor_this;
static void* g_freed;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

CNavConnectorNode::~CNavConnectorNode() { g_dtor_this = this; g_dtor_step = ++g_step; }
void operator delete(void* p) { if (p) { g_freed = p; g_free_step = ++g_step; } }

extern "C" void __fastcall CallDeleteData(void*);
#pragma comment(linker, "/alternatename:@CallDeleteData@4=?DeleteData@?$CCountedPointer@VCNavConnectorNode@@@@CIXPAX@Z")

int main()
{
    void* object = malloc(0x40);
    CallDeleteData(object);
    CHECK(g_dtor_this == object && g_freed == object);
    CHECK(g_dtor_step == 1 && g_free_step == 2);
    g_step = g_dtor_step = g_free_step = 0; g_dtor_this = g_freed = 0;
    CallDeleteData(0);
    CHECK(g_step == 0 && g_dtor_this == 0 && g_freed == 0);
    free(object);
    if (g_fail) { printf("NAV_CONNECTOR_DELETEDATA_FAIL %d\n", g_fail); return 1; }
    printf("NAV_CONNECTOR_DELETEDATA_OK\n");
    return 0;
}
