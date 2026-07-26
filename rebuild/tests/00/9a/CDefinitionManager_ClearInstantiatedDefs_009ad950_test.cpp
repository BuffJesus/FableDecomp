#include <cstdio>

static int g_freed = 0;
static int g_released = 0;

struct CRef {
    virtual void slot0() {}
    virtual void OnZeroRefs() { ++g_released; }
    int refcount;
};
struct CDef { void* v[3]; CRef* ref; };
struct Vec { char* first; char* last; char* end; };
struct Slot { char pad[0xc]; Vec v; };

extern "C" char* engine_copy(char* dst, char* src, unsigned count) {
    (void)src; (void)count; return dst;
}
extern "C" void engine_free(void* p) { (void)p; ++g_freed; }

static inline char* erase_range(char* _First, char* _Last, char* _Dest)
{
    char* _Res;
    if (_First == _Last)
        _Res = _Dest;
    else {
        unsigned count = (unsigned)(_Last - _First);
        _Res = engine_copy(_Dest, _First, count) + count;
    }
    return _Res;
}

struct CDefinitionManager {
    char pad78[0x78];
    Slot* s_first;
    Slot* s_last;
    char pad2[0xa4 - 0x80];
    CDef** d_first;
    CDef** d_last;
};

void __fastcall CDefinitionManager_ClearInstantiatedDefs(CDefinitionManager* self)
{
    CDef** p = self->d_first;
    if (p != self->d_last) {
        do {
            CDef* d = *p;
            if (d != 0) {
                CRef* r = d->ref;
                if (r != 0) {
                    if (--r->refcount == 0)
                        r->OnZeroRefs();
                    d->ref = 0;
                }
                engine_free(d);
            }
            *p = 0;
            ++p;
        } while (p != self->d_last);
    }

    Slot* s = self->s_first;
    if (s != self->s_last) {
        do {
            Vec* v = &s->v;
            v->last = erase_range(v->last, v->last, v->first);
            ++s;
        } while (s != self->s_last);
    }
}

int main() {
    CRef r0; r0.refcount = 1;   // -> 0 -> release
    CRef r1; r1.refcount = 3;   // stays

    CDef d0; for (int i=0;i<3;i++) d0.v[i]=0; d0.ref = &r0;
    CDef d1; for (int i=0;i<3;i++) d1.v[i]=0; d1.ref = &r1;
    CDef d2; for (int i=0;i<3;i++) d2.v[i]=0; d2.ref = 0;

    CDef* defs[4]; defs[0]=&d0; defs[1]=&d1; defs[2]=&d2; defs[3]=0;

    Slot slots[2];
    char buf0[8], buf1[8];
    slots[0].v.first = buf0; slots[0].v.last = buf0+8; slots[0].v.end = buf0+8;
    slots[1].v.first = buf1; slots[1].v.last = buf1+4; slots[1].v.end = buf1+8;

    CDefinitionManager m;
    m.d_first = &defs[0];
    m.d_last  = &defs[4];
    m.s_first = &slots[0];
    m.s_last  = &slots[2];

    CDefinitionManager_ClearInstantiatedDefs(&m);

    int ok = 1;
    if (r0.refcount != 0) ok = 0;
    if (g_released != 1) ok = 0;
    if (r1.refcount != 2) ok = 0;
    if (d0.ref != 0 || d1.ref != 0) ok = 0;
    if (g_freed != 3) ok = 0;
    for (int i=0;i<4;i++) if (defs[i] != 0) ok = 0;
    if (slots[0].v.last != slots[0].v.first) ok = 0;
    if (slots[1].v.last != slots[1].v.first) ok = 0;

    if (ok) { std::printf("CDefinitionManager_009ad950_TEST PASS\n"); return 0; }
    std::printf("CDefinitionManager_009ad950_TEST FAIL\n"); return 1;
}