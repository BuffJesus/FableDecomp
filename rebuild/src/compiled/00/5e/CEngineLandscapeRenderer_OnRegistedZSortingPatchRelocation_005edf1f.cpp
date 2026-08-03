// CEngineLandscapeRenderer::OnRegistedZSortingPatchRelocation
// 0x005edf1f
// mov eax,[ecx+0x148]; mov ecx,[esp+8]; mov edx,[esp+4]; mov [eax+edx*4],ecx; ret 8
//
// __fastcall (this=ecx), two stack args: (index, value).
// this->m_zSortingPatchArray[index] = value;

struct CEngineLandscapeRenderer {
    char pad[0x148];
    int* m_zSortingPatchArray; // +0x148
};

void __fastcall OnRegistedZSortingPatchRelocation(CEngineLandscapeRenderer* self, void* /*edx_unused*/, int index, int value)
{
    self->m_zSortingPatchArray[index] = value;
}