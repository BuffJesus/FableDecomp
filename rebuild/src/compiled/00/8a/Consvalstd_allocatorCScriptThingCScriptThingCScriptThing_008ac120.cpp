// _Cons_val<std::allocator<CScriptThing>,CScriptThing,CScriptThing> @ 008ac120
// __fastcall(self in ecx, src in edx, unused third stack param)

struct SelfNode {
    void*         vptr;   // +0
    int           f4;     // +4
    unsigned int* f8;     // +8 refcounted pointer
};

struct SrcNode {
    void*         vptr;   // +0 (unused)
    int           f4;     // +4
    unsigned int* f8;     // +8
};

void __fastcall Cons_val(SelfNode* self, SrcNode* src)
{
    if (self == 0)
        return;
    self->vptr = (void*)0x1238c8c;
    self->f4   = src->f4;
    unsigned int* p = src->f8;
    self->f8 = p;
    if (p != 0)
        ++(*p);
}