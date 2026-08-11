// _Dest_val for std::allocator<std::pair<long,CMusicManager::CRegisteredMusicEntry>>
// retail 0x00739dd0: destroys two sub-objects (each dtor @0x99eae0) then stores a ptr at +0.

struct Sub {
    // dtor lives at 0x99eae0 (CCharString-like)
    void dtor();
    int pad0;
};

// external destructor thunk at 0x99eae0
void __fastcall Sub_dtor(Sub* self);

struct PairSlot {
    void*  head;   // +0  (vtable/const written last)
    Sub    a;      // +4
    Sub    b;      // +8
};

// __fastcall: self in ecx; harness rewrites __fastcall->__fastcall so model as free fn
void __fastcall Dest_val(PairSlot* self)
{
    Sub_dtor(&self->b);   // esi+8 first
    Sub_dtor(&self->a);   // esi+4 second
    self->head = (void*)0x122ffcc;
}