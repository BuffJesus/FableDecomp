#include "rebuild_abi.h"
// IsBuyable @ 0x005d9b30
struct GData { char pad[0x5c]; unsigned char m_v; };
extern GData *g_invData_5d9b30;
extern "C" char FABLE_CDECL FableCheck_5d9b30();
extern "C" char FABLE_CDECL FableIsBuyable_5d9b30(){
    return FableCheck_5d9b30() ? (char)g_invData_5d9b30->m_v : (char)0;
}