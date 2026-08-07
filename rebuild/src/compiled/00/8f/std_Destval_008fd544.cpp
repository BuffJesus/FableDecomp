#include "rebuild_abi.h"
struct FableDestVal_008fd544_t
{ char pad[0x14]; void FABLE_FASTCALL inner(); void FABLE_FASTCALL outer(); };
extern "C" void FABLE_FASTCALL FableDestVal_008fd544(FableDestVal_008fd544_t* self)
{
    FableDestVal_008fd544_t* inner_this = (FableDestVal_008fd544_t*)((char*)self + 0x14);
    inner_this->inner();
    self->outer();
}