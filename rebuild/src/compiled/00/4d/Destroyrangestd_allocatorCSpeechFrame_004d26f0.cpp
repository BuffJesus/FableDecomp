// Byte-exact reconstruction of _Destroy_range<allocator<CSpeechFrame> > at 0x004d26f0
// element size 0x44 = 68 bytes; destructor at 0x4d1db0 (thiscall); epilogue = ret (0)

struct CSpeechFrame {
    char pad[0x44];
};

extern "C" void __fastcall CSpeechFrame_dtor(CSpeechFrame* self);

// __fastcall: first in ecx, last in edx. No stack cleanup -> ret 0.
void __fastcall _Destroy_range_allocator_CSpeechFrame___(CSpeechFrame* first, CSpeechFrame* last)
{
    for (; first != last; ++first)
    {
        CSpeechFrame_dtor(first);
    }
}