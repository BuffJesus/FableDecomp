#include <cstdio>

struct Handle { unsigned char pad[0x14]; };
struct HandleVec { void* proxy; Handle* first; Handle* last; void* end; };
struct CMiniMapDisplay { HandleVec vecB; HandleVec vecA; };

static int g_elemDtor = 0;
static int g_vecDtor = 0;
static int g_objDtor = 0;
static int g_free = 0;

void HandleDtor(void* self) { (void)self; ++g_elemDtor; }
void HandleVecDtor(HandleVec* v) { (void)v; ++g_vecDtor; }
void ObjectDtor(CMiniMapDisplay* self) { (void)self; ++g_objDtor; }
void EngineFree(void* p) { (void)p; if (p) ++g_free; }

static void ClearOne(HandleVec* v)
{
    Handle* p = v->first;
    Handle* e = v->last;
    for (; p != e; ++p) HandleDtor(p);
    if (v->first) EngineFree(v->first);
    HandleVecDtor(v);
}

void ClearPrimitiveHandles(CMiniMapDisplay* self)
{
    ClearOne(&self->vecA);
    ClearOne(&self->vecB);
    ObjectDtor(self);
}

int main()
{
    Handle bufA[3]; Handle bufB[2];
    CMiniMapDisplay o;
    o.vecA.first = bufA; o.vecA.last = bufA + 3; o.vecA.proxy = 0; o.vecA.end = 0;
    o.vecB.first = bufB; o.vecB.last = bufB + 2; o.vecB.proxy = 0; o.vecB.end = 0;

    ClearPrimitiveHandles(&o);

    if (g_elemDtor == 5 && g_vecDtor == 2 && g_objDtor == 1 && g_free == 2)
        printf("PARITY_OK\n");
    else
        printf("FAIL e=%d v=%d o=%d f=%d\n", g_elemDtor, g_vecDtor, g_objDtor, g_free);

    g_elemDtor = g_vecDtor = g_objDtor = g_free = 0;
    CMiniMapDisplay o2;
    o2.vecA.first = 0; o2.vecA.last = 0; o2.vecA.proxy = 0; o2.vecA.end = 0;
    o2.vecB.first = 0; o2.vecB.last = 0; o2.vecB.proxy = 0; o2.vecB.end = 0;
    ClearPrimitiveHandles(&o2);
    if (g_elemDtor == 0 && g_vecDtor == 2 && g_objDtor == 1 && g_free == 0)
        printf("EMPTY_OK\n");
    else
        printf("EMPTY_FAIL\n");

    return 0;
}