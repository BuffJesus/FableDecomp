#include "rebuild_abi.h"

// std::_Dest_val<...> @ 0x004e0ae8
// push esi; mov esi,ecx; lea ecx,[esi+0x0c]; call inner;
// mov ecx,esi; pop esi; jmp outer  (esi-saved, tail-jmp forwarder)
struct FableDestVal_004e0ae8_t
{
    char pad[0xc];
    void FABLE_FASTCALL inner();
    void FABLE_FASTCALL outer();
};

extern "C" void FABLE_FASTCALL FableDestVal_004e0ae8(FableDestVal_004e0ae8_t* self)
{
    FableDestVal_004e0ae8_t* inner_this =
        (FableDestVal_004e0ae8_t*)((char*)self + 0xc);
    inner_this->inner();
    self->outer();
}