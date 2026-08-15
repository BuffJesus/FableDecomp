// CShaderRenderManager::ResetLight  0x00988640  __fastcall(self, ulong index)
// Light array: base ptr at +0x164, stride 0x38, count at +0x16c.
// Each light: enabled byte at +0x34, dirty byte at +0x35. Dirty flags at +0x3d8.

struct Light {
    unsigned char body[0x38];   // stride 0x38; enabled=+0x34, dirty=+0x35
};

struct CShaderRenderManager {
    char pad[0x164];
    Light* lights;              // +0x164
    char gap[0x16c - 0x168];
    unsigned long count;        // +0x16c
    char pad2[0x3d8 - 0x170];
    unsigned long dirtyFlags;   // +0x3d8

    void ResetLight(unsigned long index);
};

// Implicit thiscall member: this in ecx, index on stack, ret 4.
void CShaderRenderManager::ResetLight(unsigned long index)
{
    if (index >= this->count)
        return;
    Light* base = this->lights;
    unsigned char* pEnabled = &base[index].body[0x34];
    if (*pEnabled == 0)
        return;
    *pEnabled = 0;
    this->lights[index].body[0x35] = 1;
    this->dirtyFlags |= 0x700;
}