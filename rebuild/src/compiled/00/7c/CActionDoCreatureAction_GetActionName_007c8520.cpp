#include "rebuild_abi.h"

// CActionDoCreatureAction::GetActionName @ 0x007c8520
// Returns a CCharString by value, constructed from a relocated string constant
// (0x0123d1d8) with the length sentinel -1. Retail shape:
//   push esi; mov esi,[esp+8]; push -1; push STR; mov ecx,esi; call ctor;
//   mov eax,esi; pop esi; ret 4
// i.e. the hidden CCharString return pointer arrives on the stack; this
// (CActionDoCreatureAction*) is passed in ecx but unused.

struct CCharString
{
    void* m_data;
    CCharString(const char* text, int length);
};

struct CActionDoCreatureAction
{
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x0123d1d8, -1);
}