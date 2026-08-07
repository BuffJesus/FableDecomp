#include "rebuild_abi.h"

// std::_Dest_val<...> @ 0x004e5c71
// push esi; mov esi,ecx; lea ecx,[esi+0x18]; call inner;
// mov ecx,esi; pop esi; jmp outer  (esi-saved, tail-jmp forwarder)
struct FableDestVal_004e5c71_t
{
    char pad[0x18];
    void FABLE_FASTCALL inner();
    void FABLE_FASTCALL outer();
};

extern "C" void FABLE_FASTCALL FableDestVal_004e5c71(FableDestVal_004e5c71_t* self)
{
    FableDestVal_004e5c71_t* inner_this =
        (FableDestVal_004e5c71_t*)((char*)self + 0x18);
    inner_this->inner();
    self->outer();
}