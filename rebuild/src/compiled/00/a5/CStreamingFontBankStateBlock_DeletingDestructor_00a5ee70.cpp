#include "fable_font_state_blocks.h"

// Retail compiler-generated deleting thunk. Both concrete state blocks have
// trivial payloads; destruction restores CBankStateBlock's vtable. The unused
// EDX parameter preserves the native ECX receiver and one stack flag word.
void* __fastcall CStreamingFontBankStateBlock_DeletingDestructor(CStreamingFontBankStateBlock* self, void*, unsigned flags)
{
    self->CBankStateBlock::~CBankStateBlock();
    if (flags & 1)
        operator delete(self);
    return self;
}
