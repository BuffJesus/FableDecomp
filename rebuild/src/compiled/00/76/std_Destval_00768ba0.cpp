#include "rebuild_abi.h"
struct FableDestVal_00768ba0_t
{
    char pad[0x4];
    void FABLE_FASTCALL inner();
    void FABLE_FASTCALL outer();
};
extern "C" void FABLE_FASTCALL FableDestVal_00768ba0(FableDestVal_00768ba0_t* self)
{
    FableDestVal_00768ba0_t* inner_this = (FableDestVal_00768ba0_t*)((char*)self + 0x4);
    inner_this->inner();
    self->outer();
}