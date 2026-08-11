// _Dest_val<std::pair<unsigned long, CSmallVector>>::... helper (0x006fb710)
// this in ECX. Reads this->buf (offset +4), frees it via a cdecl free func,
// then zeroes buf(+4) and end(+8).

extern "C" void __cdecl free_bfeb1b(void* p);

struct DestVal {
    void* pad0;   // +0
    void* buf;    // +4
    void* end;    // +8
};

extern "C" void __fastcall _Dest_val_pair_ulong_CSmallVector_v2(DestVal* self)
{
    free_bfeb1b(self->buf);
    self->buf = 0;
    self->end = 0;
}