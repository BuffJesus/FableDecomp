struct PairMorph
{
    void Construct();
};

extern "C" void __stdcall Cons_val_pair_morph_f(PairMorph* _Ptr)
{
    _Ptr->Construct();
}