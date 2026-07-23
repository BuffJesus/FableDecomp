#include <stdio.h>
#include <string.h>
#include "candidates/opaque_field_accessor_compiled.h"

int main()
{
    Getter_FieldE0_FC_InnerOverlay inner;
    Getter_FieldE0_FC_OuterOverlay outer;
    memset(&inner, 0, sizeof(inner));
    memset(&outer, 0, sizeof(outer));

    inner.ValueFC = 0x89ABCDEFu;
    outer.InnerE0 = &inner;
    if (Getter_FieldE0_FC(&outer) != 0x89ABCDEFu) return 1;

    inner.ValueFC = 0;
    if (Getter_FieldE0_FC(&outer) != 0) return 2;

    printf("GETTER_FIELD_E0_FC_TEST PASS\n");
    return 0;
}
