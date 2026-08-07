#include "rebuild_abi.h"
struct FableDestVal_00765d20_t
{
    char pad[0x4];
    void FABLE_FASTCALL inner();
    void FABLE_FASTCALL outer();
};
extern "C" void FABLE_FASTCALL FableDestVal_00765d20(FableDestVal_00765d20_t* self)
{
    FableDestVal_00765d20_t* inner_this = (FableDestVal_00765d20_t*)((char*)self + 0x4);
    inner_this->inner();
    self->outer();
}