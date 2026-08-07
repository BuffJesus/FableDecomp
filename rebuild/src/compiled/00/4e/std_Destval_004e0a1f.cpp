#include "rebuild_abi.h"

// std::_Dest_val<...> @ 0x004e0a1f
// push esi; mov esi,ecx; lea ecx,[esi+0x08]; call inner;
// mov ecx,esi; pop esi; jmp outer  (esi-saved, tail-jmp forwarder)
struct FableDestVal_004e0a1f_t
{
    char pad[0x8];
    void FABLE_FASTCALL inner();
    void FABLE_FASTCALL outer();
};

extern "C" void FABLE_FASTCALL FableDestVal_004e0a1f(FableDestVal_004e0a1f_t* self)
{
    FableDestVal_004e0a1f_t* inner_this =
        (FableDestVal_004e0a1f_t*)((char*)self + 0x8);
    inner_this->inner();
    self->outer();
}