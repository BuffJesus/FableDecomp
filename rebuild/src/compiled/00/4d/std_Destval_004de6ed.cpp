#include "rebuild_abi.h"

// std::_Dest_val<...> @ 0x004de6ed
// push esi; mov esi,ecx; lea ecx,[esi+0x0c]; call inner;
// mov ecx,esi; pop esi; jmp outer  (esi-saved, tail-jmp forwarder)
struct FableDestVal_004de6ed_t
{
    char pad[0xc];
    void FABLE_FASTCALL inner();
    void FABLE_FASTCALL outer();
};

extern "C" void FABLE_FASTCALL FableDestVal_004de6ed(FableDestVal_004de6ed_t* self)
{
    FableDestVal_004de6ed_t* inner_this =
        (FableDestVal_004de6ed_t*)((char*)self + 0xc);
    inner_this->inner();
    self->outer();
}