#include "engine/CText.h"
#include "rebuild_abi.h"

struct CText_Methods : CText {
    char FABLE_FASTCALL get();
};

char FABLE_FASTCALL CText_Methods::get()
{
    unsigned char v = this->flags;
    if (v & 4)
        return 1;
    v = (unsigned char)(v >> 3);
    v = (unsigned char)(v & 1);
    return (char)v;
}
