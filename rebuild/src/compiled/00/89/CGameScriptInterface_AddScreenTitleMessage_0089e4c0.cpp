struct AstmMap { void* lb(int* pKey); };
struct AstmTarget { void d(void* a0, void* a1, void* a2, int z, int one); };
extern void* __fastcall ASTM_Helper_A(void* p_ecx);
extern void* __fastcall ASTM_Helper_B(void* p_ecx);

// The retail public prototype is:
//   void CGameScriptInterface::AddScreenTitleMessage(
//       CCharString const&, float, bool) const
//
// Keep the three stack arguments raw here.  Retail reads the complete third
// stack slot both as the fallback target pointer and as the bool forwarded to
// AstmTarget::d.  Giving that slot a C++ bool type makes VC7.1 normalize the
// load and changes the instruction stream.
void __fastcall CGameScriptInterface_AddScreenTitleMessage(
    void* self_ecx,
    unsigned long,
    void* areaName,
    void* fadeTimeBits,
    void* volatile rawMessageFlag)
{
    void* chain = *(void**)((char*)self_ecx + 0x14);
    void* a = ASTM_Helper_A(chain);
    unsigned char* obj = (unsigned char*)ASTM_Helper_B(a);
    if (obj == 0) return;
    if (*(unsigned char*)(obj + 0x91) & 0x01) return;
    AstmTarget* target;
    if (*(unsigned int*)(obj + 0x30) & 0x4000) {
        int key = 0x8e;
        AstmMap* map = (AstmMap*)(obj + 0x44);
        void* node = map->lb(&key);
        void* end = *(void**)((char*)map + 0x4);
        void* pick;
        if (node == end) pick = end;
        else if (*(int*)node > 0x8e) pick = end;
        else pick = node;
        target = *(AstmTarget**)((char*)pick + 0x4);
    } else target = (AstmTarget*)rawMessageFlag;
    if (target == 0) return;
    target->d(areaName, fadeTimeBits, rawMessageFlag, 0, 1);
}
