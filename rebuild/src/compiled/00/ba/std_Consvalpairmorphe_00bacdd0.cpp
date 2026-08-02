// std::allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>>::construct
// Pure forwarding thunk: this->field_04 holds a pointer to a pointer to a
// pointer to the implementation object (triple indirection total: this,
// then field_04, then *field_04, then **field_04 = the resolved target's
// this). The thunk resolves all of it into ecx and tail-jumps into a 0-arg
// thiscall method (no ret/stack fixup of its own).

struct EgoPairMorphImpl
{
    void RealCtor();
};

struct AllocatorConsValPairMorphE
{
    void* field_00;
    EgoPairMorphImpl*** field_04; // this+4: pointer to pointer to pointer to target

    void Construct()
    {
        (**field_04)->RealCtor();
    }
};

void __fastcall Cons_val_pair_morph_e(AllocatorConsValPairMorphE* self, int /*edx*/)
{
    self->Construct();
}