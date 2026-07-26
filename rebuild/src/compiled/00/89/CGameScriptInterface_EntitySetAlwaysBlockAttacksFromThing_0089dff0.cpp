typedef unsigned char  u8;
typedef unsigned long  u32;
struct BlockSet { void* _node; void* _end; };
struct CThing { char _p0[0x24]; u32 flags; char _p1[0x44-0x28]; BlockSet blockSet; char _p2[0x91-0x4C]; u8 guardFlag; };
struct CEntityWrap;
typedef CThing* (__fastcall *ResolveFn)(CEntityWrap*);
struct BlockSet2 { void* _node; void* _end; void* LowerBound(const int* key); };
struct Mapped { void Add(CThing* thing); void Remove(CThing* thing); };
// Member modeled as __fastcall free fn: this in ecx (dead), unused edx, then 3 stack args (ret 0xc).
void __fastcall F(void* self, void* edx, CEntityWrap* entityA, CEntityWrap* entityB, u8 alwaysBlock)
{
    CThing* a = (*(ResolveFn**)entityA)[0x2c/4](entityA);
    CThing* b = (*(ResolveFn**)entityB)[0x2c/4](entityB);
    // Single short-circuit chain shifts liveness so VC7.1 parks the 2nd vtable in edx
    // (mov edx,[ecx]; mov esi,eax; call [edx+2c]) -- matches retail's 0x000f..0x0013.
    if (!a || (1 & a->guardFlag) || !b || (b->guardFlag & 1) || !(a->flags & 0x10000)) return;
    int key = 0x30;
    BlockSet2* bs = (BlockSet2*)((char*)a + 0x44);   // add esi,0x44 ; mov ecx,esi
    void* entry = bs->LowerBound(&key);              // __fastcall member -> no xor edx,edx
    void* end   = bs->_end;                          // mov esi,[esi+4]
    void* sel;
    if (entry == end) sel = end;
    else if (*(int*)entry <= 0x30) sel = entry;
    else sel = end;
    Mapped* mapped = *(Mapped**)((char*)sel + 4);    // mov ecx,[eax+4]
    if (alwaysBlock) mapped->Add(b); else mapped->Remove(b);   // __fastcall members
}