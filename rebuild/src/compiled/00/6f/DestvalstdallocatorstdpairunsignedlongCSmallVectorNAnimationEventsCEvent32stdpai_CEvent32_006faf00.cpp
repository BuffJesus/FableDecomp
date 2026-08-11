// _Dest_val for a small-vector: free the heap buffer, null the pointer+size.
extern "C" void __cdecl free_bfeb1c(void* p);

struct SmallVec {
    int   f0;
    void* f4;   // heap buffer
    int   f8;   // count/size
};

void __fastcall _Dest_val_pair_ulong_CSmallVector(SmallVec* self)
{
    free_bfeb1c(self->f4);
    self->f4 = 0;
    self->f8 = 0;
}