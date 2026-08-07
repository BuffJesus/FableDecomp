#include "rebuild_abi.h"
// CAIStateGroup_MinionReactToNoise::Initialise @ 0x0091cc90
// xor al,al; mov [ecx+0x21],al; mov [ecx+0x20],al; mov [ecx+0x24],0xffffff88; jmp base
// Clear two flags, seed the state field, then tail-call the base Initialise.
struct CAIStateGroup_MinionReactToNoise;
extern "C" void FABLE_FASTCALL FableReactBaseInit_91cc90(CAIStateGroup_MinionReactToNoise *self);
struct CAIStateGroup_MinionReactToNoise {
    char pad00[0x20];
    unsigned char m_f20;  // +0x20
    unsigned char m_f21;  // +0x21
    char pad22[2];
    long m_f24;           // +0x24
    void Initialise();
};
void CAIStateGroup_MinionReactToNoise::Initialise()
{
    m_f21 = 0;
    m_f20 = 0;
    m_f24 = (long)0xffffff88;
    FableReactBaseInit_91cc90(this);
}