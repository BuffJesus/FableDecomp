#include "rebuild_abi.h"
// std::_Dest_val (variant) @ 0x008d6a30
// push esi; mov esi,ecx; call h1; mov ecx,[esi+4]; xor dl,dl; pop esi; jmp h2
struct FableDestValV_008d6a30_t
{
    void* field_0;   // +0x00
    void* field_4;   // +0x04
    void FABLE_FASTCALL h1();
};
struct FableDestValV_008d6a30_inner
{
    void FABLE_FASTCALL h2(char flag);
};
extern "C" void FABLE_FASTCALL FableDestValV_008d6a30(FableDestValV_008d6a30_t* self)
{
    self->h1();
    ((FableDestValV_008d6a30_inner*)self->field_4)->h2(0);
}