#pragma optimize("s",on)
struct BinarySearchTreeOverlay
{
    void* header;
    void* FindLessOrEqual_Copy(const unsigned long* key);
};

struct FID_conflict_max_size_Overlay
{
    unsigned char unused[0x144];
    BinarySearchTreeOverlay tree;
};

unsigned long __fastcall FID_conflict_max_size(
    FID_conflict_max_size_Overlay* self,
    void*,
    unsigned long key)
{
    BinarySearchTreeOverlay* tree = &self->tree;
    void* node = tree->FindLessOrEqual_Copy(&key);
    if (node != tree->header) {
        return *(unsigned long*)((unsigned char*)node + 0x18);
    }
    return 0xffffffffUL;
}