#pragma once

#include "rebuild_abi.h"

namespace NTextLayoutConfig
{
    void FABLE_FASTCALL SetPrimaryLeftAlignment(bool enabled);
    void FABLE_FASTCALL SetSecondaryLeftAlignment(bool enabled);
}

// The two retail flags are adjacent, but their owning UI types are not yet
// proven. Keep their addresses visible until Phase 3 caller typing resolves
// whether they are separate channels or two fields of one settings object.
extern bool g_FablePrimaryLeftAlignment_013CA7EA;
extern bool g_FableSecondaryLeftAlignment_013CA7EB;
