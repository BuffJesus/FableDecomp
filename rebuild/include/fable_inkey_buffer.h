#pragma once
#include <stddef.h>
#include "fable_init_base_class.h"
// Ego_r layout and retail AddChar 00A5FA70 agree on all fields.
struct CCharEvent { wchar_t Char; bool Pressed; };
class CInkeyBuffer : public CInitBaseClass {
public:
    void AddChar(wchar_t character, bool pressed);
    CCharEvent Buffer[100];
    unsigned long FirstChar;
    unsigned long LastChar;
};
FABLE_STATIC_ASSERT(sizeof(CCharEvent)==4);
FABLE_STATIC_ASSERT(offsetof(CInkeyBuffer,FirstChar)==0x198);
FABLE_STATIC_ASSERT(sizeof(CInkeyBuffer)==0x1a0);
extern CInkeyBuffer GInkeyBuffer;
