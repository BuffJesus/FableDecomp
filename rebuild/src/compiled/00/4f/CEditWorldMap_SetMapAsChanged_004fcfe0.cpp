#include "rebuild_abi.h"
// CEditWorldMap::SetMapAsChanged @ 0x004fcfe0
// entry stride = 0x48 (eax*9*8), field at +0x27
struct Entry72 { char raw[0x48]; };
struct CEditWorldMap { char pad[0x20]; Entry72 *m_arr; void Set(int idx, char v); };
void CEditWorldMap::Set(int idx, char v){ *((char*)&m_arr[idx] + 0x27) = v; }