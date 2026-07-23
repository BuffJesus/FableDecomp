#ifndef FABLETLC_OPAQUE_FIELD_ACCESSOR_COMPILED_H
#define FABLETLC_OPAQUE_FIELD_ACCESSOR_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

struct Getter_FieldE0_FC_InnerOverlay
{
    fable_u8 Unknown00[0xFC];
    fable_u32 ValueFC;
};

struct Getter_FieldE0_FC_OuterOverlay
{
    fable_u8 Unknown00[0xE0];
    Getter_FieldE0_FC_InnerOverlay* InnerE0;
};

FABLE_STATIC_ASSERT(offsetof(Getter_FieldE0_FC_InnerOverlay, ValueFC) == 0xFC);
FABLE_STATIC_ASSERT(offsetof(Getter_FieldE0_FC_OuterOverlay, InnerE0) == 0xE0);

fable_u32 FABLE_FASTCALL Getter_FieldE0_FC(const Getter_FieldE0_FC_OuterOverlay* self);

#endif
