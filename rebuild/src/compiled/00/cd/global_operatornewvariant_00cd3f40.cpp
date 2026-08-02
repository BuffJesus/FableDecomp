class CFixedAllocator
{
public:
    void* Allocate(unsigned int size);
};

#define g_FixedAllocator_0143e920 (*(CFixedAllocator*)0x0143e920)

void* __fastcall operator_new_variant(unsigned int size)
{
    return g_FixedAllocator_0143e920.Allocate(size);
}