#include "rebuild_abi.h"
// IsBuyable @ 0x005d9790
struct GData { char pad[0x5e]; unsigned char m_v; };
extern GData *g_invData_5d9790;
extern "C" char FABLE_CDECL FableCheck_5d9790();
extern "C" char FABLE_CDECL FableIsBuyable_5d9790(){
    return FableCheck_5d9790() ? (char)g_invData_5d9790->m_v : (char)0;
}