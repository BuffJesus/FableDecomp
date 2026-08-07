#include "rebuild_abi.h"
// CThing::GetTypeName @ 0x004c7e30
// idx = byte[this+0x10]; table = (*g).[+4]; return byte[table + idx*20 + 9]
struct GTab { char pad[4]; char *m_data; };
extern GTab *g_typeTable_4c7e30;
struct CThing { char pad[0x10]; unsigned char m_type; char GetTypeName(); };
char CThing::GetTypeName(){
    unsigned idx = m_type;
    char *t = g_typeTable_4c7e30->m_data;
    return t[idx*20 + 9];
}