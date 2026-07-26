// CDefinitionManager::ClearInstantiatedDefs @ 009ad950
struct CRef {                       // vtbl@+0, refcount@+4
    virtual void slot0();
    virtual void OnZeroRefs();       // vtable slot +4 -> called via thiscall
    int refcount;                    // +4
};
struct CDef { void* v[3]; CRef* ref; }; // ref at +0xc

struct Vec { char* first; char* last; char* end; };  // first@+0xc, last@+0x10, end@+0x14
struct Slot { char pad[0xc]; Vec v; };               // v@+0xc, Slot is 0x18

extern "C" char* engine_copy(char* dst, char* src, unsigned count);

// mimic inlined std::_Vector::erase(_First,_Last)->_Dest, returns new _Mylast
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
    Slot* s_first;   // +0x78
    Slot* s_last;    // +0x7c
    char pad2[0xa4 - 0x80];
    CDef** d_first;  // +0xa4
    CDef** d_last;   // +0xa8
};

extern "C" void engine_free(void* p);

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