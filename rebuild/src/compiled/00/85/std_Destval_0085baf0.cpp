#include "rebuild_abi.h"
struct FableDestVal_0085baf0_t
{ char pad[0x8]; void FABLE_FASTCALL inner(); void FABLE_FASTCALL outer(); };
extern "C" void FABLE_FASTCALL FableDestVal_0085baf0(FableDestVal_0085baf0_t* self)
{
    FableDestVal_0085baf0_t* inner_this = (FableDestVal_0085baf0_t*)((char*)self + 0x8);
    inner_this->inner();
    self->outer();
}