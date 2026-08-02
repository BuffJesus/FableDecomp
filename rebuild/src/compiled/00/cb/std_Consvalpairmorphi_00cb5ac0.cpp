
struct PairSlot
{
    void Release();
};

struct Allocator
{
    void Cons_val_pair_morph_i(PairSlot* p);
};

void Allocator::Cons_val_pair_morph_i(PairSlot* p)
{
    reinterpret_cast<PairSlot*>(&p)->Release();
}