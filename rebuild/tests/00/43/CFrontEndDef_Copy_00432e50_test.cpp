#include "fable_frontend_def.h"
#include <stdio.h>
#include <string.h>

static CFrontEndDef* g_target;
static const CFrontEndDef* g_source;
static int g_calls;

CFrontEndDef& CFrontEndDef::operator=(const CFrontEndDef& other)
{
    g_target = this;
    g_source = &other;
    ++g_calls;
    ButtonABigGraphic = other.ButtonABigGraphic;
    return *this;
}

int main()
{
    unsigned char left[sizeof(CFrontEndDef)];
    unsigned char right[sizeof(CFrontEndDef)];
    CFrontEndDef* target = reinterpret_cast<CFrontEndDef*>(left);
    CFrontEndDef* source = reinterpret_cast<CFrontEndDef*>(right);
    for (int i = 0; i < 16; ++i) {
        memset(left, 0xa5, sizeof(left));
        memset(right, 0x5a, sizeof(right));
        source->ButtonABigGraphic = i * 17;
        g_calls = 0;
        target->CFrontEndDef::Copy(source);
        if (g_calls != 1 || g_target != target || g_source != source ||
            target->ButtonABigGraphic != i * 17) return 1;
        for (unsigned j = 0; j < sizeof(right); ++j)
            if (j < offsetof(CFrontEndDef, ButtonABigGraphic) ||
                j >= offsetof(CFrontEndDef, ButtonABigGraphic) + sizeof(long))
                if (right[j] != 0x5a) return 2;
    }
    puts("FRONTEND_DEF_COPY_PASS cases=16");
    return 0;
}
