#include "rebuild_abi.h"
// CEngineLandscapeRenderer::OnRegistedZSortingPatchRelocation @ 0x005ee01a
// mov eax,[ecx+0x160]; mov ecx,[esp+8]; mov edx,[esp+4]; mov [eax+edx*8],ecx; ret 8
// Store val into the 8-byte-stride relocation table at this->f160[index].
struct CEntry { void *a; void *b; };
struct CEngineLandscapeRenderer {
    char pad00[0x160];
    CEntry *m_table;   // +0x160
    void OnRegistedZSortingPatchRelocation(long index, void *val);
};
void CEngineLandscapeRenderer::OnRegistedZSortingPatchRelocation(long index, void *val)
{
    *(void**)((char*)&m_table[index]) = val;
}