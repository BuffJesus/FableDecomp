#define Z_PREFIX
extern "C" {
#include "third_party/zlib121/deflate.h"
}
#include <stdio.h>
#include <string.h>

int main()
{
    for (int round = 0; round < 16; ++round)
    {
        deflate_state state;
        memset(&state, 0x30 + round, sizeof(state));
        _tr_init(&state);
        if (state.l_desc.dyn_tree != state.dyn_ltree || !state.l_desc.stat_desc ||
            state.d_desc.dyn_tree != state.dyn_dtree || !state.d_desc.stat_desc ||
            state.bl_desc.dyn_tree != state.bl_tree || !state.bl_desc.stat_desc ||
            state.bi_buf || state.bi_valid || state.last_eob_len != 8 ||
            state.dyn_ltree[256].Freq != 1 || state.opt_len || state.static_len ||
            state.last_lit || state.matches) return 1;
        for (int i = 0; i < 256; ++i) if (state.dyn_ltree[i].Freq) return 2;
    }
    puts("ZLIB_TREES_CORE_PASS cases=16");
    return 0;
}
