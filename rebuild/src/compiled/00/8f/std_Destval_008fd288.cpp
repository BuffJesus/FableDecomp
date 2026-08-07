#include "rebuild_abi.h"
struct FableDestVal_008fd288_t
{ char pad[0xc]; void FABLE_FASTCALL inner(); void FABLE_FASTCALL outer(); };
extern "C" void FABLE_FASTCALL FableDestVal_008fd288(FableDestVal_008fd288_t* self)
{
    FableDestVal_008fd288_t* inner_this = (FableDestVal_008fd288_t*)((char*)self + 0xc);
    inner_this->inner();
    self->outer();
}